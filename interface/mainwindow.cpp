//17/05/2021

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
#include <QThread>

//TROCAR OS DEFINES
constexpr int MAX_MOTOR_TEMP  = 90;
constexpr double LOW_LIMIT_MOTOR_TEMP = 30.0;
constexpr double MED_LIMIT_MOTOR_TEMP  = 60.0;
constexpr int MAX_INVERTER_TEMP = 80;
constexpr double  LOW_LIMIT_INVERTER_TEMP = 30.0;
constexpr double  MED_LIMIT_INVERTER_TEMP = 60.0;
constexpr int MAX_SPEED = 5000;
constexpr int MEDIUM_THRESHOLD_SPEED = 2500;
constexpr int MAX_THRESHOLD_SPEED = 3000;
constexpr int SLIDER_MAX_SPEED = 5000;
constexpr int SLIDER_STEPS = 200;
constexpr int PORT = 8080;
/**/
/*Função para discretizar o slider de velocidade*/
int MainWindow::getProperValue(int value, int singleStep, int maxSize)
{

    int remainder = value % singleStep;

    if ( remainder == 0 ) {
        return value;
    }

    if ( singleStep - remainder < singleStep/(2.f) ) {
        return (1 + (value / singleStep)) * singleStep;
    }
    else {
        return (value / singleStep) * singleStep;
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
    ui->motorTermometer->setMaximum(MAX_MOTOR_TEMP);
    //connect(ui->setSpeedSlider,SIGNAL(sliderMoved(int)),ui->motorTermometer,SLOT(setValue(int)));
    //connect(ui->motorTermometer, &QProgressBar::valueChanged,[=](){ui->motorTermometer->setStyleSheet(QString::fromStdString(style(ui->motorTermometer->value(),30,60)));});

    /*Termômetro do inversor*/
    ui->inverterTermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->inverterTermometer->setMaximum(MAX_INVERTER_TEMP);
    ui->textTempInverter->setStyleSheet("QTextEdit{ " "background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");


    /*VELOCIMETRO CONFIG*/
    ui->meterWidget->setForeground(Qt::white);
    ui->meterWidget->setMaxValue(MAX_SPEED);
    ui->meterWidget->setThresholdMedium(MEDIUM_THRESHOLD_SPEED);
    ui->meterWidget->setThreshold(MAX_THRESHOLD_SPEED);
    ui->meterWidget->setUnits("RPM");
    ui->meterWidget->setLabel("Speed");

    /*SLIDER CONFIG*/
    ui->setSpeedSlider->setMaximum(SLIDER_MAX_SPEED);
    ui->setSpeedSlider->setSingleStep(SLIDER_STEPS);
    connect(ui->setSpeedSlider, &QSlider::valueChanged, [&](){ui->setSpeedSlider->setValue(
                        getProperValue(ui->setSpeedSlider->value(), ui->setSpeedSlider->singleStep(), ui->setSpeedSlider->maximum())
                        );});

    connect(ui->setSpeedSlider, &QSlider::valueChanged,
                [&](){ui->speed_label->setText(QString::fromStdString(std::to_string(ui->setSpeedSlider->value())));});



    ui->setSpeedSlider->setStyleSheet("QSlider::groove:vertical {border: 5px white;width: 50px;border-radius: 5px;height:140px;margin: 2px;}QSlider::handle:horizontal {background-color: blue;border-color:black;border: 3px black;border-radius: 3px;height: 50px;width: 30px;margin: -8px 0px;}");
    ui->setSpeedSlider->setStyleSheet("QSlider::groove:horizontal{height: 20px; margin: 0 0;}QSlider::handle:horizontal {background-color: black; border: 1px; height: 40px; width: 40px; margin: 0 0;}");

    /*Criação do socket UDP para comunicação*/
    socket = new QUdpSocket(this);
    connect(socket, &QUdpSocket::readyRead, [&](){
        if(socket->hasPendingDatagrams()){
            QString ID,Torque_text,TemperatureMotor_text,TemperatureInverter_text;
            double Speed,TemperatureMotor,TemperatureInverter;
            //double Torque;
            QByteArray Buffer;
            QByteArray TemperatureModuleA,TemperatureModuleB,TemperatureModuleC;

            Buffer.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

            QJsonDocument JsonBuffer = QJsonDocument::fromJson(Buffer);
            qDebug() << JsonBuffer.toJson();
            //ID = JsonBuffer.object().value("ID").toString();
            ID = JsonBuffer["ID"].toString();

            if(ID == "MOTOR_POSITION")
            {
                //Speed = JsonBuffer.object().value("Speed").toDouble();
                Speed = JsonBuffer["Speed"].toDouble();
                qDebug() << Speed;
                /*Passa a Speed para o velocimetro*/
                ui->meterWidget->setValue(Speed);
            }
            else if(ID == "MOTOR_TORQUE")
            {
                Torque_text = JsonBuffer["Torque"].toString();
                //Torque_text = JsonBuffer.object().value("Torque").toString();
                qDebug() << Torque_text;
                //Torque_text = QString::number(Torque);
                /*Passa o Torque para o velocimetro*/
                ui->torque_label->setText(Torque_text);
            }
            else if(ID == "TEMPERATURES_3"){

                TemperatureMotor = JsonBuffer["TemperatureMotor"].toDouble();
                TemperatureMotor_text = QString::number(TemperatureMotor);
                /*Passa a TemperatureMotor para o termometro e para o campo de texto*/
                ui->textTempMotor->setText(TemperatureMotor_text);
                //COLOCAR GRADUAÇÕES NOS TERMOMETROS
                ui->motorTermometer->setStyleSheet((QString::fromStdString(style(TemperatureMotor,LOW_LIMIT_MOTOR_TEMP,MED_LIMIT_MOTOR_TEMP))));
                ui->motorTermometer->setValue(TemperatureMotor);

                /*Passa a TemperatureInverter para o termometro e para o campo de texto*/
                TemperatureInverter = JsonBuffer["TemperatureInverter"].toDouble();
                TemperatureInverter_text = QString::number(TemperatureInverter);

                ui->textTempInverter->setText(TemperatureInverter_text);
                ui->inverterTermometer->setStyleSheet((QString::fromStdString(style(TemperatureInverter,LOW_LIMIT_INVERTER_TEMP,MED_LIMIT_INVERTER_TEMP))));
                ui->inverterTermometer->setValue(TemperatureInverter);

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
    socket->writeDatagram(QJsonDocument{qJsonObject}.toJson(), QHostAddress{"10.0.0.106"}, PORT);
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
