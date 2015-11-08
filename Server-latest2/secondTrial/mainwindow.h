#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QUdpSocket>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udp_socket;
    QHostAddress remot_address,my_address;
    quint16 remot_port,my_port;
    QList <QNetworkInterface>allInterfaces ;
    QList<QHostAddress> ips;

private slots:
    void send();
    void read();
    void logIn();
    void select();
};

#endif // MAINWINDOW_H
