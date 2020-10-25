#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QUdpSocket(this);
    //socket->bind(QHostAddress("192.168.15.16"), 8080);
    connect(socket, &QUdpSocket::readyRead, [&](){
        if(socket->hasPendingDatagrams()){
            QByteArray Buffer;
            Buffer.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

            QByteArray IdWord;
            QByteArray Code;
            QByteArray ValueString;
            int beginCut = 0, endCut = 0;

            for(int i = 0; i < Buffer.length(); i++){
                if(Buffer[i] == 'X'){
                  endCut = i;
                }
            }
            IdWord = Buffer.mid(beginCut, endCut-beginCut);
            beginCut = endCut+1;

            for(int i = beginCut; i < Buffer.length(); i++){
                if(Buffer[i] == 'Y'){
                  endCut = i;
                }
            }

            Code = Buffer.mid(beginCut, endCut-beginCut);
            beginCut = endCut+1;
            ValueString = Buffer.mid(beginCut, Buffer.length()-beginCut);

            qDebug() << IdWord;
            qDebug() << Code;
            qDebug() << ValueString;

            qDebug() << "chega aqui1";

            if(!qstrcmp(IdWord, QByteArray("AC"))){
                qDebug() << "chega aqui2";
                if(!qstrcmp(Code, QByteArray("0"))){
                    qDebug() << "chega aqui3";
                    ui->textTempModuleA->setText(ValueString);
                }
                if(!strcmp(Code, QByteArray("1"))){
                    ui->textTempModuleB->setText(ValueString);
                }
                if(!strcmp(Code, QByteArray("2"))){
                    ui->textTempModuleC->setText(ValueString);
                }
            }

            qDebug() << "Message from: " << sender.toString();
            qDebug() << "Message port: " << senderPort;
            qDebug() << "Message: " << Buffer;

            ui->messageReceived->setText(Buffer);
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendButton_clicked()
{
    QByteArray Data;
    Data.append("Hello from UDP!");
    //auto datagrama = ui->msj->text().toLatin1();
    Data = ui->message->text().toLatin1();
    socket->writeDatagram(Data, QHostAddress("192.168.15.16"), 8080);
}
