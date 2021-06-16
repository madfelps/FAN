#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sobre.h"
/**  Maximum temperature value showed in motor thermometer */
constexpr int MAX_MOTOR_TEMP  = 90;
/**  Limit range value of motor thermometer low temperature */
constexpr double LOW_LIMIT_MOTOR_TEMP = 30.0;
/**  Limit range value of motor thermometer medium temperature */
constexpr double MED_LIMIT_MOTOR_TEMP  = 60.0;
/**  Maximum temperature value showed in inverter thermometer*/
constexpr int MAX_INVERTER_TEMP = 80;
/**  Limit range value of inverter thermometer low temperature */
constexpr double  LOW_LIMIT_INVERTER_TEMP = 30.0;
/**  Limit range value of inverter thermometer medium temperature */
constexpr double  MED_LIMIT_INVERTER_TEMP = 60.0;
/**  Maximum speed value showed in motor speedometer */
constexpr int MAX_SPEED = 5000;
/** Limit range value of speedometer low speed */
constexpr int MEDIUM_THRESHOLD_SPEED = 2500;
/** Limit range value of speedometer medium speed */
constexpr int MAX_THRESHOLD_SPEED = 3000;
/** maximum speed value supported by the slider */
constexpr int SLIDER_MAX_SPEED = 5000;
/** speed slider steps value */
constexpr int SLIDER_STEPS = 200;
/** Socket UDP port*/
constexpr int PORT = 8080;

void MainWindow::clear_thermometer()
{
    ui->frame_motor_1->setStyleSheet("background-color: gray;");
    ui->frame_motor_2->setStyleSheet("background-color: gray;");
    ui->frame_motor_3->setStyleSheet("background-color: gray;");
    ui->frame_motor_4->setStyleSheet("background-color: gray;");
    ui->frame_motor_5->setStyleSheet("background-color: gray;");
    ui->frame_motor_6->setStyleSheet("background-color: gray;");
    ui->frame_motor_7->setStyleSheet("background-color: gray;");
    ui->frame_motor_8->setStyleSheet("background-color: gray;");
}
void MainWindow::inverter_thermometer_style(double value)
{
    if(value < 12)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
    }
    else if(value >= 12 && value < 24)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
        ui->frame_motor_2->setStyleSheet("background-color: rgb(102, 185, 0);");
    }
    else if(value >= 24 && value < 36)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
        ui->frame_motor_2->setStyleSheet("background-color: rgb(102, 185, 0);");
        ui->frame_motor_3->setStyleSheet("background-color: rgb(141, 169, 0);");
    }
    else if(value >= 36 && value < 48)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
        ui->frame_motor_2->setStyleSheet("background-color: rgb(102, 185, 0);");
        ui->frame_motor_3->setStyleSheet("background-color: rgb(141, 169, 0);");
    }
    else if(value >= 48 && value < 60)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
        ui->frame_motor_2->setStyleSheet("background-color: rgb(102, 185, 0);");
        ui->frame_motor_3->setStyleSheet("background-color: rgb(141, 169, 0);");
        ui->frame_motor_4->setStyleSheet("background-color: rgb(172, 152, 0);");
        ui->frame_motor_5->setStyleSheet("background-color: rgb(198, 131, 0);");
    }
    else if(value >= 60 && value < 72)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
        ui->frame_motor_2->setStyleSheet("background-color: rgb(102, 185, 0);");
        ui->frame_motor_3->setStyleSheet("background-color: rgb(141, 169, 0);");
        ui->frame_motor_4->setStyleSheet("background-color: rgb(172, 152, 0);");
        ui->frame_motor_5->setStyleSheet("background-color: rgb(198, 131, 0);");
        ui->frame_motor_6->setStyleSheet("background-color: rgb(220, 106, 0);");
    }
    else if(value >= 72 && value < 84)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
        ui->frame_motor_2->setStyleSheet("background-color: rgb(102, 185, 0);");
        ui->frame_motor_3->setStyleSheet("background-color: rgb(141, 169, 0);");
        ui->frame_motor_4->setStyleSheet("background-color: rgb(172, 152, 0);");
        ui->frame_motor_5->setStyleSheet("background-color: rgb(198, 131, 0);");
        ui->frame_motor_6->setStyleSheet("background-color: rgb(220, 106, 0);");
        ui->frame_motor_7->setStyleSheet("background-color: rgb(237, 74, 0);");
    }
    else if(value >= 84)
    {
        clear_thermometer();
        ui->frame_motor_1->setStyleSheet("background-color: rgb(23, 198, 10);");
        ui->frame_motor_2->setStyleSheet("background-color: rgb(102, 185, 0);");
        ui->frame_motor_3->setStyleSheet("background-color: rgb(141, 169, 0);");
        ui->frame_motor_4->setStyleSheet("background-color: rgb(172, 152, 0);");
        ui->frame_motor_5->setStyleSheet("background-color: rgb(198, 131, 0);");
        ui->frame_motor_6->setStyleSheet("background-color: rgb(220, 106, 0);");
        ui->frame_motor_7->setStyleSheet("background-color: rgb(237, 74, 0);");
        ui->frame_motor_8->setStyleSheet("background-color: rgb(249, 15, 15);");
    }
}
int MainWindow::getProperValue(int value, int singleStep)
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
std::string MainWindow::style(double value, double med_threshold, double max_threshold)
{

    std::string string_style;

    /*If the progress bar value is bigger than the highest temperature value considered low and smaller than the highest considered high.*/
    if (value < med_threshold)
    {
      std::string s{"QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
      string_style = s;
    }

    /*If the progress bar value is bigger than the highest temperature value considered low and smaller,i.e the medium value, than the highest considered high.*/
    else if (value >= med_threshold && value < max_threshold)
    {
        std::string s{"QProgressBar::chunk{""background-color: yellow;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
         string_style = s;
    }

    /*If the progress bar value is bigger than the highest temperature value considered medium.*/
    else
    {
        std::string s{"QProgressBar::chunk{""background-color: red;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
        string_style = s;
    }

return string_style;
}
void MainWindow::speedometer_config()
{
    ui->meterWidget->setForeground(Qt::white);
    ui->meterWidget->setMaxValue(MAX_SPEED);
    ui->meterWidget->setThresholdMedium(MEDIUM_THRESHOLD_SPEED);
    ui->meterWidget->setThreshold(MAX_THRESHOLD_SPEED);
    ui->meterWidget->setUnits("RPM");
    ui->meterWidget->setLabel("Speed");
}
void MainWindow::speedslider_config()
{
    ui->SpeedSlider->setMaximum(SLIDER_MAX_SPEED);
    ui->SpeedSlider->setSingleStep(SLIDER_STEPS);
    connect(ui->SpeedSlider, &QSlider::valueChanged, [&](){ui->SpeedSlider->setValue(
                        getProperValue(ui->SpeedSlider->value(), ui->SpeedSlider->singleStep())
                        );});

    connect(ui->SpeedSlider, &QSlider::valueChanged,
                [&](){ui->speed_label->setText(QString::fromStdString(std::to_string(ui->SpeedSlider->value())));});



    ui->SpeedSlider->setStyleSheet("QSlider::groove:vertical {border: 5px white;width: 50px;border-radius: 5px;height:140px;margin: 2px;}QSlider::handle:horizontal {background-color: blue;border-color:black;border: 3px black;border-radius: 3px;height: 50px;width: 30px;margin: -8px 0px;}");
    ui->SpeedSlider->setStyleSheet("QSlider::groove:horizontal{height: 20px; margin: 0 0;}QSlider::handle:horizontal {background-color: black; border: 1px; height: 40px; width: 40px; margin: 0 0;}");

}
void MainWindow::thermometers_config()
{
    /*Motor thermometer*/
    //ui->motorTermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->textTempMotor->setStyleSheet("QTextEdit{ " "background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");
    //ui->motorTermometer->setMaximum(MAX_MOTOR_TEMP);

    /*Inverter thermometer*/
    ui->inverterTermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->inverterTermometer->setMaximum(MAX_INVERTER_TEMP);
    ui->textTempInverter->setStyleSheet("QTextEdit{ ""background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 16px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");

}

void MainWindow::receiveMessages()
{
    /*Creation of UDP socket for communication*/
    socket = new QUdpSocket(this);
    /*Receiving, serializing and interpreting received messages*/
    connect(socket, &QUdpSocket::readyRead, [&](){
        if(socket->hasPendingDatagrams())
        {      
            QString ID;/*ID receives the Json message identification; */

            /* Speed, Torque, MotorTemperature receive the speed, toque and motor temperature, respectively.
             * TemperatureInverter receives the inverter temperature value*/
            double Speed,Torque,MotorTemperature,InverterTemperature,ModuleATemperature,ModuleBTemperature,ModuleCTemperature;

            /* Buffer is a UDP reception buffer. It stores the received data.*/
            QByteArray Buffer;
            /* The buffer size is set with resize() according to the size of the first pending UDP
             * datagram using pendingDatagramSize().*/
            Buffer.resize(socket->pendingDatagramSize());

            QHostAddress sender;
            quint16 senderPort;
            socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
            /*Jsonbuffer is the buffer that will receive the Json formatted buffer datagram*/
            QJsonDocument JsonBuffer = QJsonDocument::fromJson(Buffer);

            ID = JsonBuffer["ID"].toString();

            /*If the ID is of the MOTOR_POSITION class, pass the Speed ​​to the speedometer*/
            if(ID == "MOTOR_POSITION")
            {
                Speed = JsonBuffer["Speed"].toDouble();
                ui->meterWidget->setValue(Speed);
            }
            /*If the ID is of the TORQUE_INFORMATION class, pass the Speed ​​to the speedometer*/
            else if(ID == "TORQUE_TIMER_INFO")
            {
                Torque = JsonBuffer["TorqueFeedback"].toDouble();
                /*Passa o Torque para a label*/
                //ui->torque_label->setText(QString::fromStdString(std::to_string(Torque)));
            }

            /*If the ID is of the TEMPERATURES_3 class, pass the temperatures ​​to the thermometers*/
            else if(ID == "TEMPERATURES_3"){

                MotorTemperature = JsonBuffer["MotorTemperature"].toDouble();
                /*Pass the TemperatureMotor to the thermometer and text field.*/
                //ui->textTempMotor->setText(QString::fromStdString(std::to_string(MotorTemperature)));
                //ui->motorTermometer->setStyleSheet((QString::fromStdString(style(TemperatureMotor,LOW_LIMIT_MOTOR_TEMP,MED_LIMIT_MOTOR_TEMP))));
                //ui->motorTermometer->setValue(TemperatureMotor);
                inverter_thermometer_style(MotorTemperature);

                /**/
                ModuleATemperature = JsonBuffer["ModuleATemperature"].toDouble();
                ModuleBTemperature = JsonBuffer["ModuleBTemperature"].toDouble();
                ModuleCTemperature = JsonBuffer["ModuleCTemperature"].toDouble();
                double temps[] = {ModuleATemperature,ModuleBTemperature,ModuleCTemperature};
                qDebug()<< std::max_element(temps, temps+3);
                /*Pass the TemperatureInverter to the thermometer and to the text field*/
                InverterTemperature = JsonBuffer["TemperatureInverter"].toDouble();
                //ui->textTempInverter->setText(QString::fromStdString(std::to_string(InverterTemperature)));
                ui->inverterTermometer->setStyleSheet((QString::fromStdString(style(InverterTemperature,LOW_LIMIT_INVERTER_TEMP,MED_LIMIT_INVERTER_TEMP))));
                ui->inverterTermometer->setValue(InverterTemperature);

                /*Add TemperatureModuleA,B,C*/
            }

        }
    });

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Configure the thermometer*/
    thermometers_config();

    /*Configure the speedometer*/
    speedometer_config();

    /*Configure the SpeedSlider*/
    speedslider_config();

    /*Create a socket and receive the messages sended by embedded board via UDP*/
    receiveMessages();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::sendJsonToUDP(const QJsonObject& qJsonObject) {
    socket->writeDatagram(QJsonDocument{qJsonObject}.toJson(), QHostAddress{"192.168.1.3"}, PORT);
}

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

    flag_motor = true;
    /*You can to change the button style, just change the parameters: color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n\nbackground-color: rgb(13, 94, 4);") */
    ui->enable_motor_button->setStyleSheet("color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n\nbackground-color: rgb(13, 94, 4);");
    ui->disable_motor_button->setStyleSheet("color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);\nbackground-color: rgb(186, 189, 182);");

    QJsonObject UDP_Packet_Send;
    UDP_Packet_Send["ID"] = "enable_id";
    UDP_Packet_Send["Enable_Command"] = true;

    sendJsonToUDP(UDP_Packet_Send);

}

void MainWindow::on_disable_motor_button_clicked()
{

    flag_motor = false;
    ui->disable_motor_button->setStyleSheet("color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n\nbackground-color: rgb(13, 94, 4);");
    ui->enable_motor_button->setStyleSheet("color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);\nbackground-color: rgb(186, 189, 182);");

    QJsonObject UDP_Packet_Send;
    UDP_Packet_Send["ID"] = "disable_id";
    UDP_Packet_Send["Enable_Command"] = false;

    sendJsonToUDP(UDP_Packet_Send);

}
