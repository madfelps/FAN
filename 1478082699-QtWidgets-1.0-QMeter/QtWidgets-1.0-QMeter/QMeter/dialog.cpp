/*
 *   Copyright (C) 2016 by P. Sereno
 *   http://www.qt-italia.net
 */

#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->meterWidget->setForeground(Qt::white);
    connect(ui->pushButton,SIGNAL(released()),this,SLOT(close()));
    //connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),ui->meterWidget,SLOT(setValue(int)));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_send_clicked()
{
    qDebug()<< ui->lineEdit->text().toInt();
    ui->meterWidget->setValue(ui->lineEdit->text().toInt()) ;
    qDebug()<< ui->meterWidget->value();
}
