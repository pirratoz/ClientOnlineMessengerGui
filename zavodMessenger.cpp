#include "zavodMessenger.h"
#include "./ui_zavodMessenger.h"
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>

ZavodMessenger::ZavodMessenger(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ZavodMessenger)
{
    ui->setupUi(this);
    this->auth = false;
    connect(ui->pb_connect, &QPushButton::clicked, this, &ZavodMessenger::connect_to_server);
    connect(ui->pb_auth, &QPushButton::clicked, this, &ZavodMessenger::auth_user);
    connect(ui->pb_create, &QPushButton::clicked, this, &ZavodMessenger::create_user);
    connect(ui->pb_logout, &QPushButton::clicked, this, &ZavodMessenger::logout_user);
    connect(&this->socket, &QTcpSocket::readyRead, this, &ZavodMessenger::readAnswer);
    connect(ui->pb_send_message, &QPushButton::clicked, this, &ZavodMessenger::sendMessage);
}

ZavodMessenger::~ZavodMessenger()
{
    delete ui;
}

void ZavodMessenger::connect_to_server()
{
    QString host = ui->le_ip->text();
    QString port = ui->le_port->text();

    if (host.isEmpty() || port.isEmpty())
        return;

    this->socket.connectToHost(host, port.toInt());


    ui->lbl_server_status_text->setText(this->socket.isOpen() ? "Connected!" : "Not Connected");
}

void ZavodMessenger::auth_user()
{
    if (!this->socket.isOpen())
        return;

    QJsonObject infoJson, dataJson;
    dataJson["login"] = ui->le_login->text();
    dataJson["password"] = ui->le_password->text();
    infoJson["type"] = "auth_user";
    infoJson["data"] = dataJson;

    if (ui->le_login->text().isEmpty() || ui->le_password->text().isEmpty())
        return;

    QJsonDocument jsonString(infoJson);
    this->socket.write(jsonString.toJson());
}

void ZavodMessenger::sendMessage()
{
    QString text = ui->pte_input_field->toPlainText();
    if (text.isEmpty())
        return;
    QJsonObject infoJson, dataJson;
    dataJson["text"] = text;
    dataJson["login"] = ui->lbl_server_auth_text->text();
    infoJson["type"] = "new_message";
    infoJson["data"] = dataJson;
    QJsonDocument jsonString(infoJson);
    this->socket.write(jsonString.toJson());
}

void ZavodMessenger::logout_user()
{
    this->auth = false;
    ui->lbl_server_auth_text->setText("Not Authorized!");
}

void ZavodMessenger::readAnswer()
{
    QByteArray jsonData = this->socket.readAll();
    QJsonDocument jsonText = QJsonDocument::fromJson(jsonData);
    QJsonObject json = jsonText.object();
    QJsonObject data = json["data"].toObject();

    qDebug() << jsonText;

    if (json["type"] == "create_user_status" || json["type"] == "auth_user_status")
    {
        if (data["status"].toBool())
        {
            ui->lbl_server_auth_text->setText(data["login"].toString());
            this->auth = true;
        }
    } else if (json["type"] == "new_message") {
        ui->pte_messenger->appendPlainText(data["text"].toString());
        return;
    }
}

void ZavodMessenger::create_user()
{
    if (!this->socket.isOpen())
        return;

    QJsonObject infoJson, dataJson;
    dataJson["login"] = ui->le_login->text();
    dataJson["password"] = ui->le_password->text();
    infoJson["type"] = "create_user";
    infoJson["data"] = dataJson;

    if (ui->le_login->text().isEmpty() || ui->le_password->text().isEmpty())
        return;

    QJsonDocument jsonString(infoJson);
    this->socket.write(jsonString.toJson());
}
