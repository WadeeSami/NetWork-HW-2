#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ChooseFile_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "C:/Users/Wadee Sami/Desktop/", tr(""));
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    qDebug()<<fileName<< endl;
    ui->FileName->setText(fileName);

}

void MainWindow::on_Upload_clicked()
{

    if(socket->waitForConnected(3000))
    {


        //fileNameInBytes = fileName.prepend("Upload:").toUtf8();
        // Remeber To Chamge The Nimber Dynmiclly;
        fileNameInBytes = fileName.mid(fileName.lastIndexOf("/") + 1).toUtf8();
        qDebug() << fileName;
        qDebug() << fileNameInBytes;


        sendData(fileNameInBytes.prepend("Upload:"));
        for (int i=0 ; i<1000000 ; i++){}
        readFile(fileName);

        for (int i=0 ; i<1000000 ; i++){}

    }
}

bool MainWindow::sendData(QByteArray S)
{

    socket->write(S);


    return socket->waitForBytesWritten(3000);


}

void MainWindow::on_Connect_clicked()
{
    MainWindow::socket = new QTcpSocket(this);
  //  socket = new QTcpSocket(this);
    QString address;
    int port;
    if(ui->RemoteIP->text().toInt())
        address= ui->RemoteIP->text();
    else
        address = "127.0.0.1";
    if(ui->RemotePort->text().toInt())
        port = ui->RemotePort->text().toInt();
    else
        port = 1234;
    MainWindow::socket->connectToHost(address,port);
    connect(socket,SIGNAL(readyRead()),this,SLOT(receivData()));

}

void MainWindow::readFile(QString Name)
{

    QFile file(Name);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"Can't Open File-->in the read file method";
            return;
        }


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
        }

//        while (!file.atEnd()) {
////            QByteArray fileData = file.readAll();

            for (int i=0 ; i<1000000 ; i++){}
            sendData("::ENDMSG::");


}

void MainWindow::updateComboBox(QStringList list)
{
    ui->comboBox->clear();
    for(int i = 0 ; i < list.size() ; i++){
        if(list[i].contains(""))ui->comboBox->addItem(list[i]);
    }
}

void MainWindow::receivData(){
    fileData = socket->readAll();


    if(fileData.contains("FileList")){
            //,eans it's about updating the list
        qDebug()<<"recieved some data"<<endl;//
        QString files = fileData.mid(fileData.indexOf(":") + 1);
        qDebug()<<files<<endl;
        updateComboBox(files.split(","));

        }
        else{ writeFile(fileData);
        //counter++;

        }
    


}

void MainWindow::on_comboBox_activated(const QString &a)
{
    //a is the file name
    //now shall send the name to the server
    //a.prepend("Download:");
    downloadFile = a;
    downloadFileName = a;
    downloadFile.prepend("Download:");

}

void MainWindow::on_Download_clicked()
{

   pathToDownload = QFileDialog::getSaveFileName(this,tr("Save File"),"C:/Users/Wadee Sami/Desktop/Downs/"+downloadFileName,tr(""));
   QString dataa = "Download:" + downloadFileName;
   socket->write(dataa.toUtf8());

}

void MainWindow::on_UpdateList_clicked()
{

    sendData("GetList:");
}

void MainWindow::ReadData(QByteArray D)
{
    writeFile(D);
    while(!D.contains("::ENDMSG::")){
        D = socket->readAll();
        writeFile(D);

    }
    if(D.contains("::ENDMSG::"))
    {
        D.chop(10);
        writeFile(D);

    }


}

void MainWindow::writeFile(QByteArray D)
{

    QFile file(pathToDownload);
                if (!file.open(QIODevice::Append))
                {
                    qDebug()<<"Can't Open File--in the write file method";
                    return;
                }


                file.write(D);
                // fileData = file.readAll();


}
