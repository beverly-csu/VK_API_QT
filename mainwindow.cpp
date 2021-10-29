#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->setIconSize(QSize(64, 64));

    client = new VK("token");

    timer = new QTimer(this);
    timer->setInterval(3000);
    updateMessages();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMessages()));
    timer->start();
}

void MainWindow::updateMessages()
{
    QVector<Message> msges = client->getVectorMessages("unread");
    ui->listWidget->clear();
    for(qint32 i = 0; i < msges.size(); i++) {
        QListWidgetItem* listItem = new QListWidgetItem(msges[i].from_user.first_name + " " + msges[i].from_user.last_name + "\n" +msges[i].text);
        listItem->setIcon(QPixmap(FileDownloader::img(msges[i].from_user.photo_url)));
        ui->listWidget->addItem(listItem);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
