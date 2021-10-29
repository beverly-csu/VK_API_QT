#ifndef VK_S_H
#define VK_S_H

#include <QString>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSize>
#include <QListWidgetItem>
#include <QListWidget>
#include <QPixmap>
#include <QByteArray>
#include <QVector>
#include <QTimer>


class FileDownloader : public QObject {
    Q_OBJECT
public:
    static QPixmap img(QUrl);
};


class User {
public:
    quint64 id;
    QString first_name;
    QString last_name;
    QUrl photo_url;

};


class Message {
public:
    Message(quint64, QString, quint64, QString, QString, QUrl);
    User from_user;
    QString text;
    quint64 id;
};


class VK : public QObject{
    Q_OBJECT
public:
    VK(QString access_token);
    QJsonDocument getMessages(QString);
    QJsonDocument getUser(quint64);
    QVector<Message> getVectorMessages(QString);


private:
    QByteArray getResponse(QUrl);

    const QString _access_token;
    const QString _api_version = "5.131";
};

#endif // VK_S_H
