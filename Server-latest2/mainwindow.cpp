#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDir>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*
     * If you need to handle incoming TCP connections (e.g., in a server application), use the QTcpServer class. Call QTcpServer::listen() to set up the server, and connect to the QTcpServer::newConnection() signal, which is emitted once for every client that connects. In your slot, call QTcpServer::nextPendingConnection() to accept the connection and use the returned QTcpSocket to communicate with the client.
     */
    ui->setupUi(this);
    server = new QTcpServer(this);
    path = "C:/Users/Wadee Sami/Desktop/Files2";
   connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any , 1234)){
        qDebug() <<"Error Has Occured"<<endl;
    }else{
        qDebug()<<"Connected"<<endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnection(){

   // qDebug()<<"In the newConnection Slot"<<endl;
    socket = server->nextPendingConnection();//accept the connection and use the socket for comm.
    //qDebug() <<socket<<endl;
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));//to recieve data from the client


}
void MainWindow::readyRead()
{

    QByteArray Data;
    Data.resize(socket->bytesAvailable());
    Data = socket->readAll();
    //qDebug()<<Data<<endl;

    if(Data.contains("Download:") || Data.contains("Upload:") || Data.contains("GetList:"))
        fileName = Data.mid(Data.indexOf(":") + 1);


    if(Data.contains("Download:") || Downloading)
    {


        qDebug() << " we Are at download !";

//        //it must download
       QString toBeDownloaded = Data.mid(Data.indexOf(":") + 1);

//       qDebug()<<"File name is " << toBeDownloaded<<endl;

           readFile(fileName);
//       QFile file(path+"/"+toBeDownloaded);
//           if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//           {
//               qDebug()<<"Can't Open File";
//               return;
//           }

//            fileData = file.readAll();
//            socket->write(fileData);

////       QString pathToDownloaded = path;
////       pathToDownloaded.append(toBeDownloaded);
////       QFile down(pathToDownloaded);
//       return;
    }



    if(Data.contains("Upload:")  || Uploading){
        qDebug() << " we Are at upload !";

        if(Data.contains("Upload:")){
            Uploading = true;
            return;}



        //qDebug() << Data << "  lol "<< endl;
        //qDebug() << fileName;
        if(Data.contains("::ENDMSG::"))
            Data.chop(10);

        if(!Data.contains("::ENDMSG::"))
        {
             Uploading = true;

             //qDebug() << Data;


             writeFile(Data);

        }
        else
            Uploading=false;





    }

    if(Data.contains("GetList:"))
    {

        qDebug() << " we Are at File List !";

                sendData("FileList:");

                 QDir dir(path);
                 QStringList listOfDirs = dir.entryList();
                   // qDebug()<<listOfDirs<<endl;
                 for(int i = 0 ; i < listOfDirs.size() ; i++)
                 {
                     if(listOfDirs[i].indexOf(".") == 0 || listOfDirs[i].indexOf("..") == 0)continue;
                     sendData(listOfDirs[i].append(",").toUtf8());

                 }



    }




}

void MainWindow::writeFile(QByteArray D)
{

    QFile file(path+"/"+fileName);
                if (!file.open(QIODevice::Append))
                {
                    qDebug()<<"Can't Open File-->the write file method";
                    return;
                }


                file.write(D);
                // fileData = file.readAll();


}

void MainWindow::sendData(QByteArray M)
{

    socket->write(M);

}

void MainWindow::readFile(QString Name)
{

    QFile file(path+"/"+Name);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"Can't Open File--the read file method";
            return;
        }

//         fileData = file.readAll();
//         fileNameInBytes = Name.prepend("Upload:").toUtf8();
//         qDebug() << fileNameInBytes;
        int n = file.size();
        line.resize(file.size());
        Data.resize(file.size());

        QDataStream in(&file);
        in.readRawData(Data.data(),n);
        line = Data;
        int packetSize=1;
        for (int i=0;i<file.size();i++){

            //qDebug() << line.mid(i*packetSize,packetSize);
            socket->write(line.mid(i*packetSize,packetSize));
            socket->waitForBytesWritten(3000);
        }

//        while (!file.atEnd()) {
////            QByteArray fileData = file.readAll();
//            //n-=file.pos();
//            prog = file.pos()/n *100;
//            line = file.readLine();
            for (int i=0 ; i<1000000 ; i++){}
            //sendData("::ENDMSG::");
//            sendData(line);
//            ui->progressBar->setValue(prog);
//        }

        //qDebug()<<fileData;




}
