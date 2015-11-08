#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QList>
#include <QtNetwork/QUdpSocket>
#include <QNetworkInterface>
#include <qfile.h>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udp_socket = new QUdpSocket(this);

    connect(udp_socket,SIGNAL(readyRead()),this,SLOT(read()));
    connect(ui->login,SIGNAL(clicked()),this,SLOT(logIn()));
    connect(ui->send,SIGNAL(clicked()),this,SLOT(send()));
    connect(ui->t1,SIGNAL(returnPressed()),this,SLOT(send()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(select()));

    allInterfaces = QNetworkInterface::allInterfaces();
    ips=QNetworkInterface::allAddresses();

    for(int i=0;i<allInterfaces.size();i++){
    QNetworkInterface n = allInterfaces[i];
   // qDebug()<< "n  " << n<<endl;
    //QHostAddress ip = ips[i*2+1];
    ui->comboBox->addItem(n.humanReadableName());
    //ui->comboBox->addItem(n.humanReadableName()+"  "+ip.toString());
    }


    ui->localAddress->setText("127.0.0.1");
    ui->lport->setText("1111");
    ui->remotAddress->setText("127.0.0.1");
    ui->rport->setText("2222");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send(){
    QString  message = ui->t1->text();
    ui->t1->setText("");
    ui->t2->setText(ui->t2->toPlainText()+"\nMe : "+message);

    remot_address = QHostAddress(ui->remotAddress->text());
    remot_port = ui->rport->text().toInt();
    QByteArray out = message.toUtf8();
    udp_socket->writeDatagram(out,remot_address,remot_port);
    remot_address = QHostAddress(ui->remotAddress->text());
    remot_port = ui->rport->text().toInt();
    qDebug()<<remot_address<<endl;


}

void MainWindow::read(){

    while(udp_socket->hasPendingDatagrams()){


    QByteArray in;
    in.resize(udp_socket->pendingDatagramSize());

    udp_socket->readDatagram(in.data(),in.size(),&remot_address,&remot_port);
    ui->t2->setText(ui->t2->toPlainText()+"\nYou : "+in.data());
    }



}

void MainWindow::logIn(){
    my_address=QHostAddress(ui->localAddress->text());
    udp_socket->bind(my_address,ui->lport->text().toInt());


}
void MainWindow::select(){
    int index = ui->comboBox->currentIndex();

    QString ip =ips[index*2+1].toString();
    ui->localAddress->setText(ip);
}

