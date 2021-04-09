#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QDebug>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QByteArray SliceJson(QByteArray Buffer, QByteArray Campo){
        QByteArray Word;
        Pos1 = Buffer.indexOf(":", Buffer.indexOf(Campo));
        Pos1 = Pos1+1;
        Pos2 = Buffer.indexOf(",", Pos1);
        if(Pos2 == -1){
            Pos2 = Buffer.indexOf("}", Pos1);
        }
        return Buffer.mid(Pos1, Pos2-Pos1);
    }



signals:


private slots:
    void on_sendButton_clicked();
    void on_actionSobre_triggered();

    //void on_actionAbout_triggered();

    //void on_pushButton_clicked();





    void on_enable_motor_button_clicked();

    void on_disable_motor_button_clicked();


private:
    Ui::MainWindow *ui;
    QUdpSocket* socket;
    QString Message, FirstMsg;
    QString MessageSendTorque, MessageSendTorqueLimite;
    std::string MessageSendTorqueStd, MessageTorqueLimiteStd;
    int Pos1, Pos2;
    std::string style(double value, double med_threshold, double max_threshold);
    int getProperValue(float value, int singleStep, int maxSize);
    bool flag_motor = false;
    void sendJsonToUDP(const QJsonObject&);
};
#endif // MAINWINDOW_H
