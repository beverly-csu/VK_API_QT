#include "vk_s.h"

VK::VK(QString access_token) : _access_token(access_token)
{
    qDebug() << "VK init";
}

QByteArray VK::getResponse(QUrl url)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest (url));
    QEventLoop wait;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &wait, SLOT(quit()));
    QTimer::singleShot(10000, &wait, SLOT(quit()));
    wait.exec();
    QByteArray answer = reply->readAll();
    reply->deleteLater();
    return answer;
}

QJsonDocument VK::getUser(quint64 id)
{
    QUrl current("https://api.vk.com/method/users.get");
    QUrlQuery param;
    param.addQueryItem("access_token", _access_token);
    param.addQueryItem("user_ids", QString::number(id));
    param.addQueryItem("fields", "photo_100");
    param.addQueryItem("v", _api_version);
    current.setQuery(param);
    QByteArray answer = getResponse(current);
    return QJsonDocument::fromJson(answer);
}

QVector<Message> VK::getVectorMessages(QString filter)
{
    QVector<Message> answer;
    QJsonValue doc = getMessages(filter)["response"];
    qint32 count = doc["count"].toInt();
    for (qint32 i = 0; i < count; i++) {
        quint64 user_id = doc["items"][i]["last_message"]["from_id"].toInt();
        QJsonValue user_data = getUser(user_id)["response"];
        QString first_name = user_data[0]["first_name"].toString();
        QString last_name = user_data[0]["last_name"].toString();
        QUrl photo_url = QUrl(user_data[0]["photo_100"].toString());
        QString text = doc["items"][i]["last_message"]["text"].toString();
        if (text.size() == 0)
            text = "Нераспознанный формат";
        quint64 id = doc["items"][i]["last_message"]["id"].toInt();
        answer.push_back(Message(id, text, user_id, first_name, last_name, photo_url));
    }
    return answer;
}

QJsonDocument VK::getMessages(QString filter)
{
    QUrl current("https://api.vk.com/method/messages.getConversations");
    QUrlQuery param;
    param.addQueryItem("access_token", _access_token);
    param.addQueryItem("filter", filter);
    param.addQueryItem("v",_api_version);
    current.setQuery(param);
    QByteArray answer = getResponse(current);
    return QJsonDocument::fromJson(answer);
}

Message::Message(quint64 msg_id, QString msg_text, quint64 user_id, QString first, QString last, QUrl photo)
{
    id = msg_id;
    text = msg_text;
    from_user.id = user_id;
    from_user.first_name = first;
    from_user.last_name = last;
    from_user.photo_url = photo;
}

QPixmap FileDownloader::img(QUrl url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(0);
    QNetworkReply *netReply = manager->get(QNetworkRequest(url));
    QEventLoop loop;
    connect(netReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray array = netReply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(array);
    return pixmap;
}
