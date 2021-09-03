#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
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
constexpr int UDP_PORT = 8080;
/** Socket TCP port*/
constexpr int TCP_PORT = 8053;

QString text;


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

    datalog_init();

    /*Fill txt file with FAN datas*/
    //fill_datalog_file();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::datalog_init()
{

    //File manipulation/
    //QString local = "/home/gaspar/Documentos/IC/codigos/Git_2/FAN/interface/fan_ui_qt_creator_code/datalog";
    QString filename = "PM100_CAN_datalog.txt";
    //QDir dir(local);
    //CAN_datalog_file.setFileName(dir.absoluteFilePath(filename));
    CAN_datalog_file.setFileName(filename);
    if(!CAN_datalog_file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::warning(this,"ERROR","Error opening log file!");
        return;
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
      std::string s{"QProgressBar::chunk{""background-color: green"";""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
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
    ui->meterWidget->setSteps(10);
    ui->meterWidget->setMaxValue(MAX_SPEED);
    ui->meterWidget->setThresholdMedium(MEDIUM_THRESHOLD_SPEED);
    ui->meterWidget->setThreshold(MAX_THRESHOLD_SPEED);
    ui->meterWidget->setUnits("RPM");
    ui->meterWidget->setLabel("Speed");
}
void MainWindow::speedslider_config()
{
    ui->SpeedSlider->setStyleSheet("QSlider::groove:vertical {border: 5px white;width: 50px;border-radius: 5px;height:140px;margin: 2px;}QSlider::handle:horizontal {background-color: blue;border-color:black;border: 3px black;border-radius: 3px;height: 50px;width: 30px;margin: -8px 0px;}");
    ui->SpeedSlider->setStyleSheet("QSlider::groove:horizontal{height: 20px; margin: 0 0;}QSlider::handle:horizontal {background-color: black; border: 1px; height: 40px; width: 40px; margin: 0 0;}");
    ui->SpeedSlider->setMaximum(SLIDER_MAX_SPEED);
    ui->SpeedSlider->setSingleStep(SLIDER_STEPS);
    /*This connection links the slider to the getProperValue() function that discretizes its values*/
    connect(ui->SpeedSlider, &QSlider::valueChanged, [&](){ui->SpeedSlider->setValue(
                        getProperValue(ui->SpeedSlider->value(), ui->SpeedSlider->singleStep())
                        );});
    /*This connection links the slider to the speed_label to update and show the speed value selected*/
    connect(ui->SpeedSlider, &QSlider::valueChanged,
                [&](){ui->speed_label->setText(QString::fromStdString(std::to_string(ui->SpeedSlider->value())));});

}
void MainWindow::thermometers_config()
{
    /*Motor thermometer*/
    ui->motorThermometer->setMaximum(MAX_MOTOR_TEMP);
    ui->motorThermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->textTempMotor->setStyleSheet("QTextEdit{ ""text-align: center""; ""background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 25px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");


    /*Inverter thermometer*/
    ui->inverterThermometer->setMaximum(MAX_INVERTER_TEMP);
    ui->inverterThermometer->setStyleSheet("QProgressBar::chunk{""background-color: green;""border-radius: 7px""}QProgressBar{""background-color: rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}");
    ui->textTempInverter->setStyleSheet("QTextEdit{ ""background-color:rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px;""padding: 0 8px;""selection-background-color: darkgray;""font-size: 25px;}""QTextEdit:focus { ""background-color:rgb(192, 192, 255);}");


}

void MainWindow::receiveMessages()
{
    /*Creation of TCP socket for datalog*/
    mSocket = new QTcpSocket(this);
    mSocket ->connectToHost("127.0.0.1",TCP_PORT);

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
                ui->torque_label->setText(QString::number(Torque,'f', 1));

            }

            /*If the ID is of the TEMPERATURES_3 class, pass the temperature ​​to the motor thermometer*/
            else if(ID == "TEMPERATURES_3"){

                MotorTemperature = JsonBuffer["MotorTemperature"].toDouble();
                /*Pass the TemperatureMotor to the thermometer and text field.*/
                ui->textTempMotor->setText(QString::number(MotorTemperature,'f', 1));

                //ui->textTempInverter->setAlignment(Qt::AlignCenter);
                ui->motorThermometer->setStyleSheet((QString::fromStdString(style(MotorTemperature,LOW_LIMIT_MOTOR_TEMP,MED_LIMIT_MOTOR_TEMP))));
                ui->motorThermometer->setValue(MotorTemperature);
            }

            else if(ID == "TEMPERATURES_1")
            {
                ModuleATemperature = JsonBuffer["ModuleATemperature"].toDouble();
                ModuleBTemperature = JsonBuffer["ModuleBTemperature"].toDouble();
                ModuleCTemperature = JsonBuffer["ModuleCTemperature"].toDouble();

                double temps[] = {ModuleATemperature,ModuleBTemperature,ModuleCTemperature};
                qDebug() << "Maior temperatura:" << *std::max_element(temps,temps+3);
                InverterTemperature = *std::max_element(temps,temps+3);

                /*Pass the TemperatureInverter to the thermometer and to the text field*/
                ui->textTempInverter->setText(QString::number(ModuleATemperature, 'f', 1));
                ui->inverterThermometer->setStyleSheet((QString::fromStdString(style(InverterTemperature,LOW_LIMIT_INVERTER_TEMP,MED_LIMIT_INVERTER_TEMP))));
                ui->inverterThermometer->setValue(InverterTemperature);
            }


        }
    });

}

void MainWindow::fill_datalog_file()
{
    volatile int buffer_size = 0;

    //Fill txt file with FAN system data/
    connect(mSocket, &QTcpSocket::readyRead, [&](){
        QByteArray TCP_Buffer;
        QTextStream out(&CAN_datalog_file);
        TCP_Buffer = mSocket->readAll();
        buffer_size = TCP_Buffer.size();

        qDebug() << buffer_size << "\n";

        for(int i = 0; i < buffer_size; i++)
        {
           if(TCP_Buffer[i] == '/')
           {
              TCP_Buffer[i] = '\n';
           }

        }
        text  = QString(TCP_Buffer);
        out << text;

        CAN_datalog_file.flush();
        for(int i = 0; i < buffer_size; i++)
        {
           if(TCP_Buffer[i] == '*')
           {
                qDebug() << "Finish Datalog" << "\n";
                /*Close datalog and tcp socket */
                mSocket->close();
                CAN_datalog_file.close();

            }
        }

    });
}
void MainWindow::sendJsonToUDP(const QJsonObject& qJsonObject) {
    socket->writeDatagram(QJsonDocument{qJsonObject}.toJson(), QHostAddress{"127.0.0.1"}, UDP_PORT);
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

void MainWindow::on_actionAbout_triggered()
{
    About about;
    about.exec();
}



void MainWindow::on_enable_motor_button_clicked()
{
    ui->inverterThermometer->setValue(35);
    ui->textTempInverter->setText("35");
    ui->motorThermometer->setValue(30);
    ui->textTempMotor->setText("30");

    ui->torque_label->setText("0.1");

    flag_motor = true;
    /*You can to change the button style, just change the parameters: color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n\nbackground-color: rgb(13, 94, 4);") */
    ui->enable_motor_button->setStyleSheet("background-color: rgb(178, 255, 102);");
    ui->disable_motor_button->setStyleSheet("color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);\nbackground-color: rgb(186, 189, 182);");

    QJsonObject UDP_Packet_Send;
    UDP_Packet_Send["ID"] = "enable_id";
    UDP_Packet_Send["Enable_Command"] = "true";

    sendJsonToUDP(UDP_Packet_Send);



}

void MainWindow::on_disable_motor_button_clicked()
{


    flag_motor = false;
    ui->disable_motor_button->setStyleSheet("background-color: rgb(255, 30, 30);");
    ui->enable_motor_button->setStyleSheet("color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);\nbackground-color: rgb(186, 189, 182);");

    QJsonObject UDP_Packet_Send;
    UDP_Packet_Send["ID"] = "enable_id";
    UDP_Packet_Send["Enable_Command"] = "false";

    /*Initialize the datalog file*/
    fill_datalog_file();
    /*Send UDP message*/
    sendJsonToUDP(UDP_Packet_Send);




}


