#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sobre.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    using json = nlohmann::json;
    ui->setupUi(this);

    json UDP_Package_Send;

    UDP_Package_Send["Torque"]          						= "X";
    UDP_Package_Send["TorqueLimit"] 							= "X";

    socket = new QUdpSocket(this);
    using json = nlohmann::json;
    connect(socket, &QUdpSocket::readyRead, [&](){
        if(socket->hasPendingDatagrams()){
            QByteArray Buffer;
            QByteArray Word;
            QByteArray Angle,Speed;
            QString Mensagem;
            Buffer.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
            //qDebug() << Buffer;

            ui->messageReceived->setText(Buffer);


            //"Função" para pegar o valor de um campo do json
            Pos1 = Buffer.indexOf(":", Buffer.indexOf("ID"));
            //qDebug() << Pos1;
            Pos1 = Pos1+1;
            Pos2 = Buffer.indexOf(",", Pos1);
            if(Pos2 == -1){
                Pos2 = Buffer.indexOf("}", Pos1);
            }
            //qDebug() << Pos2;
            Word = Buffer.mid(Pos1, Pos2-Pos1);

            Word = Word.mid(1,Word.length()-2);
            //qDebug() << "Alooooo" << Word;

            if(Word == "MOTOR_POSITION"){
                Word = SliceJson(Buffer,"Speed");
                Speed = Word;
                //qDebug() << "Speed: " << Speed;
                ui->textSpeed->setText(Speed);

                Word = SliceJson(Buffer,"Angle");
                Angle = Word;
               // qDebug() << "Angle: " << Angle;
                ui->textAngle->setText(Angle);

            }

            //Verifica o ID
            if(Word == "TEMPERATURES_1"){
                Word = SliceJson(Buffer,"TemperatureModuleA");
               // qDebug() << "TemperatureModuleA: " << Word;
                ui->textTempModuleA->setText(Word);

                Word = SliceJson(Buffer,"TemperatureModuleB");
               // qDebug() << "TemperatureModuleB: " << Word;
                ui->textTempModuleB->setText(Word);

                Word = SliceJson(Buffer,"TemperatureModuleC");
                //qDebug() << "TemperatureModuleC: " << Word;
                ui->textTempModuleC->setText(Word);
            }

            //auto MessageJSON = json::parse(Buffer.toStdString());

//            if(MessageJSON["ID"] == "MOTOR_POSITION"){
//                Message = QString::fromStdString(MessageJSON["Speed"]);
//                qDebug() << "ALOOOOOOOOOOOO";
//                qDebug() << Message;
//                ui->textSpeed->setText(Message);
//                Message = QString::fromStdString(MessageJSON["Angle"]);
//                ui->textAngle->setText(Message);
//            }



            /*for(int i = 0; i < Buffer.length(); i++){
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
            }*/


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
    socket->writeDatagram(Data, QHostAddress("10.0.0.105"), 8080);
}



void MainWindow::on_ButtonSendTorque_clicked()
{
    MessageSendTorque = ui->textTorqueSend->toPlainText();

    QByteArray Data = MessageSendTorque.toUtf8();
    socket->writeDatagram(Data, QHostAddress("10.0.0.105"), 8080);


}

void MainWindow::on_ButtonSendTorqueLimite_clicked()
{
    MessageSendTorqueLimite = ui->textTorqueLimit->toPlainText();

    QByteArray Data = MessageSendTorqueLimite.toUtf8();
    socket->writeDatagram(Data, QHostAddress("10.0.0.105"), 8080);

}



void MainWindow::on_actionSobre_triggered()
{
    Sobre sobre;
    sobre.exec();
}
