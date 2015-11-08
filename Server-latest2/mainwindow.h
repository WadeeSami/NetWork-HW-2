#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTcpServer *server;
    QTcpSocket *socket;
    QString path;
    QByteArray fileData;
    QString fileName;
    QByteArray Data;
    bool Uploading = false;
    bool Downloading = false;
    bool sendFileList = false;
    QByteArray line;

    void writeFile(QByteArray D);
    void sendData(QByteArray M);
    void readFile(QString Name);

public slots:
void newConnection();
void readyRead();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
