#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QDebug>
#include "nlohmann/json.hpp"
#include <iostream>



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



private:
    Ui::MainWindow *ui;
    QUdpSocket* socket;
    QString Message, FirstMsg;
    QString MessageSendTorque, MessageSendTorqueLimite;
    std::string MessageSendTorqueStd, MessageTorqueLimiteStd;
    int Pos1, Pos2;
    std::string style(int value, int med_threshold, int max_threshold);
    int getProperValue(float value, int singleStep, int maxSize);

};
#endif // MAINWINDOW_H
