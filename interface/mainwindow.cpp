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

/*Função para discretizar o slider de velocidade*/
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

/*Função que seta o estilo dos termometros baseado no valor das temperaturas*/
std::string MainWindow::style(double value, double med_threshold, double max_threshold)
{

    std::string string_style;

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

    ui->setupUi(this);

    /*TERMOMETROS*/
    /*Termômetro do motor */
    ui->motorTermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->textTempMotor->setStyleSheet("QTextEdit{ " "background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");
    ui->motorTermometer->setMaximum(90);
    //connect(ui->setSpeedSlider,SIGNAL(sliderMoved(int)),ui->motorTermometer,SLOT(setValue(int)));
    //connect(ui->motorTermometer, &QProgressBar::valueChanged,[=](){ui->motorTermometer->setStyleSheet(QString::fromStdString(style(ui->motorTermometer->value(),30,60)));});

    /*Termômetro do inversor*/
    ui->inverterTermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->inverterTermometer->setMaximum(80);
    ui->textTempInverter->setStyleSheet("QTextEdit{ " "background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");


    /*VELOCIMETRO CONFIG*/
    ui->meterWidget->setForeground(Qt::white);
    ui->meterWidget->setMaxValue(5000);
    ui->meterWidget->setThresholdMedium(2500);
    ui->meterWidget->setThreshold(3000);
    ui->meterWidget->setUnits("RPM");
    ui->meterWidget->setLabel("Speed");

    /*SLIDER CONFIG*/
    ui->setSpeedSlider->setMaximum(5000);
    ui->setSpeedSlider->setSingleStep(200);
    connect(ui->setSpeedSlider, &QSlider::valueChanged, [=](){ui->setSpeedSlider->setValue(
                        getProperValue(ui->setSpeedSlider->value(), ui->setSpeedSlider->singleStep(), ui->setSpeedSlider->maximum())
                        );});
    connect(ui->setSpeedSlider, &QSlider::valueChanged,
                [&](){ui->speed_label->setText(QString::fromStdString(std::to_string(ui->setSpeedSlider->value())));});
    ui->setSpeedSlider->setStyleSheet("QSlider::groove:vertical {""border: 5px white;""width: 50px;" "border-radius: 5px;""height:140px;""margin: 2px;""}""QSlider::handle:horizontal {""background-color: blue;""border-color:black;""border: 3px black;""border-radius: 3px;""height: 50px;""width: 30px;""margin: -8px 0px;""}");
    ui->setSpeedSlider->setStyleSheet("QSlider::groove:horizontal{height: 20px; margin: 0 0;}""QSlider::handle:horizontal {background-color: black; border: 1px; height: 40px; width: 40px; margin: 0 0;}" "");

    /*Criação do socket UDP para comunicação*/
    socket = new QUdpSocket(this);
    connect(socket, &QUdpSocket::readyRead, [&](){
        if(socket->hasPendingDatagrams()){

            QByteArray Buffer;
            QByteArray Word;
            QByteArray Angle,Speed,Torque,TemperatureModuleA,TemperatureModuleB,TemperatureModuleC,TemperatureMotor,
                       TemperatureInverter;
            Buffer.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

            /*"Função" para pegar o valor de um campo do json*/
            Pos1 = Buffer.indexOf(":", Buffer.indexOf("ID"));
            Pos1 = Pos1+1;
            Pos2 = Buffer.indexOf(",", Pos1);
            if(Pos2 == -1){
                Pos2 = Buffer.indexOf("}", Pos1);
            }
            Word = Buffer.mid(Pos1, Pos2-Pos1);
            Word = Word.mid(1,Word.length()-2);

            if(Word == "MOTOR_POSITION")
            {
                Word = SliceJson(Buffer,"Speed");
                Speed = Word;
                /*Passa a Speed para o velocimetro*/
                ui->meterWidget->setValue(Speed.toInt());
            }
            if(Word == "MOTOR_TORQUE")
            {
                Word = SliceJson(Buffer,"Torque");
                Torque = Word;
                qDebug() << "Torque: " << Torque;
                /*Passa o Torque para o velocimetro*/
                ui->torque_label->setText(Torque);
            }
            if(Word == "TEMPERATURES_3"){

                Word = SliceJson(Buffer,"TemperatureMotor");
                TemperatureMotor = Word;
                /*Passa a TemperatureMotor para o termometro e para o campo de texto*/
                ui->textTempMotor->setText(TemperatureMotor);
                ui->motorTermometer->setStyleSheet((QString::fromStdString(style(TemperatureMotor.toDouble(),50.0,70.0))));
                ui->motorTermometer->setValue(TemperatureMotor.toDouble());

                /*Passa a TemperatureInverter para o termometro e para o campo de texto*/
                Word = SliceJson(Buffer,"TemperatureInverter");
                ui->textTempInverter->setText(TemperatureInverter);
                ui->inverterTermometer->setStyleSheet((QString::fromStdString(style(TemperatureInverter.toDouble(),30.0,60.0))));
                ui->inverterTermometer->setValue(TemperatureInverter.toDouble());

            }

        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*Função que envia o comando formatado em JSON para a BeagleBone.*/
void MainWindow::sendJsonToUDP(const QJsonObject& qJsonObject) {
    socket->writeDatagram(QJsonDocument{qJsonObject}.toJson(), QHostAddress{"10.0.0.106"}, 8080);
}

/*Função com operações referentes ao botão de envio de velocidade do motor.
*Habilitado apenas quando o botão enable é ativado*/
void MainWindow::on_sendButton_clicked()
{

    if(flag_motor)
    {
        QJsonObject UDP_Packet_Send;
        UDP_Packet_Send["ID"] = "speed_id";
        UDP_Packet_Send["Speed_Command"] = ui->speed_label->text();

        sendJsonToUDP(UDP_Packet_Send);
    }
}

void MainWindow::on_actionSobre_triggered()
{
    Sobre sobre;
    sobre.exec();
}



void MainWindow::on_enable_motor_button_clicked()
{

    flag_motor = true; //habilita o envio de comandos de velocidade
    ui->enable_motor_button->setStyleSheet("color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n\nbackground-color: rgb(13, 94, 4);");
    ui->disable_motor_button->setStyleSheet("color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);\nbackground-color: rgb(186, 189, 182);");

    QJsonObject UDP_Packet_Send;
    UDP_Packet_Send["ID"] = "enable_id";
    UDP_Packet_Send["Enable_Command"] = true;

    sendJsonToUDP(UDP_Packet_Send);

}

void MainWindow::on_disable_motor_button_clicked()
{

    flag_motor = false; //desabilita o envio de comandos de velocidade
    ui->disable_motor_button->setStyleSheet("color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n\nbackground-color: rgb(13, 94, 4);");
    ui->enable_motor_button->setStyleSheet("color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);\nbackground-color: rgb(186, 189, 182);");

    QJsonObject UDP_Packet_Send;
    UDP_Packet_Send["ID"] = "disable_id";
    UDP_Packet_Send["Enable_Command"] = false;

    sendJsonToUDP(UDP_Packet_Send);

}
