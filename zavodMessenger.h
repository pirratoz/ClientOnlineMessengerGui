#ifndef ZAVODMESSENGER_H
#define ZAVODMESSENGER_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class ZavodMessenger; }
QT_END_NAMESPACE

class ZavodMessenger : public QMainWindow
{
    Q_OBJECT

public:
    ZavodMessenger(QWidget *parent = nullptr);
    ~ZavodMessenger();

private:
    Ui::ZavodMessenger *ui;
    QTcpSocket socket;
    bool auth;
    void connect_to_server();
    void auth_user();
    void create_user();
    void logout_user();
    void readAnswer();
    void sendMessage();
};
#endif // ZAVODMESSENGER_H
