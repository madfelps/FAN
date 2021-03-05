#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sobre.h"
#include <QPalette>
#include "qmeter.h"
#include <QDebug>
#include <QString>
#include <string>
#include <QProgressBar>
#include <QStyle>
int MainWindow::getProperValue(float value, int singleStep, int maxSize)
{
    for (int iterator = 0; iterator != maxSize; iterator += singleStep)
    {
        auto diff = std::abs(iterator - value);
        if (diff <= singleStep/2)
        {
            return iterator;
        }
    }
}

std::string MainWindow::style(int value, int med_threshold, int max_threshold)
{

    std::string string_style;
    value = ui->motorTermometer->value();

    if (value < med_threshold)
    {
      std::string s{"QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
      string_style = s;
    }
    else if (value >= med_threshold && value < max_threshold)
    {
        std::string s{"QProgressBar::chunk{""background-color: yellow;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
         string_style = s;
    }
    else
    {
        std::string s{"QProgressBar::chunk{""background-color: red;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
        string_style = s;
    }
return string_style;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    using json = nlohmann::json;
    ui->setupUi(this);

    /*TERMOMETROS*/

    /*motor */
    ui->motorTermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->textTempMotor->setStyleSheet("QTextEdit{ " "background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");
    ui->motorTermometer->setMaximum(90);
    connect(ui->setSpeedSlider,SIGNAL(sliderMoved(int)),ui->motorTermometer,SLOT(setValue(int)));
    connect(ui->motorTermometer, &QProgressBar::valueChanged,[=](){ui->motorTermometer->setStyleSheet(QString::fromStdString(style(ui->motorTermometer->value(),30,60)));});

    /*inversor*/
    ui->inverterTermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->inverterTermometer->setMaximum(80);
    ui->textTempInverter->setStyleSheet("QTextEdit{ " "background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");
    connect(ui->setSpeedSlider,SIGNAL(sliderMoved(int)),ui->inverterTermometer,SLOT(setValue(int)));
    connect(ui->inverterTermometer, &QProgressBar::valueChanged,[=](){ui->inverterTermometer->setStyleSheet(QString::fromStdString(style(ui->inverterTermometer->value(),30,60)));});


    /*VELOCIMETRO CONFIG*/
    ui->meterWidget->setForeground(Qt::white);
    ui->meterWidget->setMaxValue(5000);
    ui->meterWidget->setThresholdMedium(2500);
    ui->meterWidget->setThreshold(4000);
    ui->meterWidget->setUnits("RPM");
    ui->meterWidget->setLabel("Speed");


    ui->setSpeedSlider->setMaximum(5000);
    ui->setSpeedSlider->setSingleStep(200);
    connect(ui->setSpeedSlider, &QSlider::valueChanged, [=](){ui->setSpeedSlider->setValue(
                        getProperValue(ui->setSpeedSlider->value(), ui->setSpeedSlider->singleStep(), ui->setSpeedSlider->maximum())
                        );});
    connect(ui->setSpeedSlider, &QSlider::valueChanged,
                [&](){ui->speed_label->setText(QString::fromStdString(std::to_string(ui->setSpeedSlider->value())));});


    /*BOTOES*/
    //ui->sendButton->setStyleSheet("QPushButton{ ""border: 2px solid gray;" "border-radius: 3px;""padding: 0 8px;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");

    ui->setSpeedSlider->setStyleSheet("QSlider::groove:vertical {""border: 5px white;""width: 50px;" "border-radius: 5px;""height:140px;""margin: 2px;""}""QSlider::handle:horizontal {""background-color: blue;""border-color:black;""border: 3px black;""border-radius: 3px;""height: 50px;""width: 30px;""margin: -8px 0px;""}");
    ui->setSpeedSlider->setStyleSheet("QSlider::groove:horizontal{height: 20px; margin: 0 0;}""QSlider::handle:horizontal {background-color: black; border: 1px; height: 40px; width: 40px; margin: 0 0;}" "");




    json UDP_Package_Send;

    UDP_Package_Send["Torque"]          						= "X";
    UDP_Package_Send["TorqueLimit"] 							= "X";



    socket = new QUdpSocket(this);
    using json = nlohmann::json;
    connect(socket, &QUdpSocket::readyRead, [&](){
        if(socket->hasPendingDatagrams()){
            QByteArray Buffer;
            QByteArray Word;
            QByteArray Angle,Speed,Torque,TemperatureModuleA,TemperatureModuleB,TemperatureModuleC,TemperatureMotor,
                       TemperatureInverter;
            QString Mensagem;
            Buffer.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
            //qDebug() << Buffer;

           /*Recebe na janela de recebimento de dados*/
            //ui->messageReceived->setText(Buffer);

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
            qDebug() << "Word: " << Word;

            if(Word == "MOTOR_POSITION")
            {
                Word = SliceJson(Buffer,"Speed");
                Speed = Word;
                /*Passa a Speed para o velocimetro*/
                ui->meterWidget->setValue(Speed.toInt());

                Word = SliceJson(Buffer,"Angle");
                Angle = Word;
               // qDebug() << "Angle: " << Angle;
                //ui->textAngle->setText(Angle);

            }
            if(Word == "MOTOR_TORQUE")
            {
                Word = SliceJson(Buffer,"Torque");
                Torque = Word;
                qDebug() << "Torque: " << Torque;
                /*Passa o Torque para o velocimetro*/
                //ui->meterWidget_2->setValue(Torque.toInt());
            }
            //Verifica o ID
            if(Word == "TEMPERATURES_1"){
                Word = SliceJson(Buffer,"TemperatureModuleA");
                TemperatureModuleA = Word;
               // qDebug() << "TemperatureModuleA: " << Word;
                //ui->textTempModuleA->setText(TemperatureModuleA);
               // ui->module_a_Termometer->setValue(TemperatureModuleA.toInt());
                Word = SliceJson(Buffer,"TemperatureModuleB");
                TemperatureModuleB = Word;
               // qDebug() << "TemperatureModuleB: " << Word;
                //ui->textTempModuleB->setText(TemperatureModuleB);
                //ui->module_b_Termometer->setValue(TemperatureModuleB.toInt());
                Word = SliceJson(Buffer,"TemperatureModuleC");
                TemperatureModuleC = Word;
                //qDebug() << "TemperatureModuleC: " << Word;
                //ui->textTempModuleC->setText(TemperatureModuleC);
                //ui->module_c_Termometer->setValue(TemperatureModuleC.toInt());

                Word = SliceJson(Buffer,"TemperatureMotor");
                TemperatureMotor = Word;
                qDebug() << "TemperatureMotor: " << Word;
                ui->textTempMotor->setText(TemperatureMotor);
                //ui->motorTermometer->setValue(TemperatureMotor.toInt());


                Word = SliceJson(Buffer,"TemperatureInverter");
                TemperatureInverter = Word;
                ui->textTempInverter->setText(TemperatureInverter);
                ui->inverterTermometer->setValue(TemperatureInverter.toInt());

            }

            if(Word ==  "FAULT_ERRORS"){
                std::array<int, 8>Error_Bytes;
                Word = SliceJson(Buffer, "FaultCode_0");
                Error_Bytes[0] = Word.toInt();
                Word = SliceJson(Buffer, "FaultCode_1");
                Error_Bytes[1] = Word.toInt();
                Word = SliceJson(Buffer, "FaultCode_2");
                Error_Bytes[2] = Word.toInt();
                Word = SliceJson(Buffer, "FaultCode_3");
                Error_Bytes[3] = Word.toInt();
                Word = SliceJson(Buffer, "FaultCode_4");
                Error_Bytes[4] = Word.toInt();
                Word = SliceJson(Buffer, "FaultCode_5");
                Error_Bytes[5] = Word.toInt();
                Word = SliceJson(Buffer, "FaultCode_6");
                Error_Bytes[6] = Word.toInt();
                Word = SliceJson(Buffer, "FaultCode_7");
                Error_Bytes[7] = Word.toInt();

                int j = 0;
                for(int i = 0; i < 64; i++){
                    //Error_Pair.at(i).first = (Error_Bytes[j] >> i) & 0x01;
                    j = i%8;
                }

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
    Data = ui->speed_label->text().toLatin1();
    socket->writeDatagram(Data, QHostAddress("10.0.0.106"), 8080);

}

void MainWindow::on_actionSobre_triggered()
{
    Sobre sobre;
    sobre.exec();
}


