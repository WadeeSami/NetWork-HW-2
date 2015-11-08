#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString fileName;
    QString pathToDownload;
    QString downloadFile;
    QString downloadFileName;
    QString result;
    QByteArray fileData;
    QByteArray Data;
    QByteArray line;
    QByteArray fileNameInBytes;
    QTcpSocket *socket;
    void writeFile(QByteArray D);
    void ReadData(QByteArray D);
    char * s;
    int prog;

public slots:
    void on_ChooseFile_clicked();

    void on_Upload_clicked();

    void on_Connect_clicked();

    void readFile(QString Name);

    void updateComboBox(QStringList list);

    void receivData();

    bool sendData(QByteArray S);

    void on_comboBox_activated(const QString &arg1);

    void on_Download_clicked();

signals:

private slots:
    void on_UpdateList_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
