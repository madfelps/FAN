/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include <qmeter.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionOpen;
    QWidget *centralwidget;
    QMeter *meterWidget;
    QLabel *label_3;
    QFrame *frame;
    QLabel *label_12;
    QPushButton *sendButton;
    QLabel *label_6;
    QSlider *SpeedSlider;
    QProgressBar *inverterThermometer;
    QTextBrowser *textTempInverter;
    QTextBrowser *textTempMotor;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label;
    QPushButton *enable_motor_button;
    QPushButton *disable_motor_button;
    QLabel *speed_label;
    QLabel *label_13;
    QLabel *torque_label;
    QProgressBar *motorThermometer;
    QMenuBar *menubar;
    QMenu *menuAbout;
    QMenu *menuDataLog;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1020, 572);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(904, 0));
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(186, 189, 182);"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setCheckable(false);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        meterWidget = new QMeter(centralwidget);
        meterWidget->setObjectName(QString::fromUtf8("meterWidget"));
        meterWidget->setGeometry(QRect(20, 32, 487, 485));
        meterWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(136, 138, 133);"));
        label_3 = new QLabel(meterWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(412, 410, 81, 17));
        QFont font;
        font.setPointSize(14);
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(242, 40, 767, 469));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(136, 138, 133);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_12 = new QLabel(frame);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(206, 226, 563, 31));
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        label_12->setFont(font1);
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(186, 189, 182);"));
        label_12->setAlignment(Qt::AlignCenter);
        sendButton = new QPushButton(frame);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setGeometry(QRect(598, 344, 157, 57));
        QFont font2;
        font2.setPointSize(20);
        font2.setBold(true);
        font2.setWeight(75);
        sendButton->setFont(font2);
        sendButton->setStyleSheet(QString::fromUtf8("\n"
"\n"
"background-color: rgb(178, 255, 102);"));
        sendButton->setIconSize(QSize(16, 16));
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(286, 348, 91, 45));
        QFont font3;
        font3.setPointSize(20);
        label_6->setFont(font3);
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
""));
        label_6->setAlignment(Qt::AlignCenter);
        SpeedSlider = new QSlider(frame);
        SpeedSlider->setObjectName(QString::fromUtf8("SpeedSlider"));
        SpeedSlider->setGeometry(QRect(298, 410, 457, 47));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(SpeedSlider->sizePolicy().hasHeightForWidth());
        SpeedSlider->setSizePolicy(sizePolicy1);
        SpeedSlider->setFont(font3);
        SpeedSlider->setCursor(QCursor(Qt::ArrowCursor));
        SpeedSlider->setMouseTracking(false);
        SpeedSlider->setTabletTracking(false);
        SpeedSlider->setFocusPolicy(Qt::StrongFocus);
        SpeedSlider->setMaximum(5000);
        SpeedSlider->setSingleStep(100);
        SpeedSlider->setPageStep(100);
        SpeedSlider->setTracking(true);
        SpeedSlider->setOrientation(Qt::Horizontal);
        SpeedSlider->setTickPosition(QSlider::TicksBelow);
        SpeedSlider->setTickInterval(200);
        inverterThermometer = new QProgressBar(frame);
        inverterThermometer->setObjectName(QString::fromUtf8("inverterThermometer"));
        inverterThermometer->setGeometry(QRect(386, 140, 283, 71));
        inverterThermometer->setStyleSheet(QString::fromUtf8("background-color: rgb(211, 215, 207);"));
        inverterThermometer->setMaximum(100);
        inverterThermometer->setValue(0);
        inverterThermometer->setTextVisible(false);
        inverterThermometer->setOrientation(Qt::Horizontal);
        textTempInverter = new QTextBrowser(frame);
        textTempInverter->setObjectName(QString::fromUtf8("textTempInverter"));
        textTempInverter->setGeometry(QRect(674, 140, 83, 71));
        textTempInverter->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textTempInverter->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textTempInverter->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textTempMotor = new QTextBrowser(frame);
        textTempMotor->setObjectName(QString::fromUtf8("textTempMotor"));
        textTempMotor->setGeometry(QRect(674, 62, 83, 69));
        textTempMotor->setLayoutDirection(Qt::LeftToRight);
        textTempMotor->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textTempMotor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textTempMotor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label_9 = new QLabel(frame);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(256, 134, 129, 75));
        label_9->setFont(font3);
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_10 = new QLabel(frame);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(284, 64, 97, 61));
        label_10->setFont(font3);
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(286, 274, 285, 51));
        label->setFont(font3);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        enable_motor_button = new QPushButton(frame);
        enable_motor_button->setObjectName(QString::fromUtf8("enable_motor_button"));
        enable_motor_button->setGeometry(QRect(388, 270, 99, 61));
        QFont font4;
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        enable_motor_button->setFont(font4);
        enable_motor_button->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"color: rgb(46, 52, 54);\n"
"background-color: rgb(186, 189, 182);"));
        disable_motor_button = new QPushButton(frame);
        disable_motor_button->setObjectName(QString::fromUtf8("disable_motor_button"));
        disable_motor_button->setGeometry(QRect(492, 270, 97, 61));
        disable_motor_button->setFont(font4);
        disable_motor_button->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"color: rgb(46, 52, 54);\n"
"background-color: rgb(186, 189, 182);"));
        speed_label = new QLabel(frame);
        speed_label->setObjectName(QString::fromUtf8("speed_label"));
        speed_label->setGeometry(QRect(388, 346, 201, 55));
        QFont font5;
        font5.setPointSize(25);
        speed_label->setFont(font5);
        speed_label->setLayoutDirection(Qt::LeftToRight);
        speed_label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-color: rgb(0, 0, 0);\n"
"border-color: rgb(0, 0, 0);"));
        speed_label->setAlignment(Qt::AlignCenter);
        label_13 = new QLabel(frame);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(26, 4, 741, 31));
        label_13->setFont(font1);
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(186, 189, 182);"));
        label_13->setAlignment(Qt::AlignCenter);
        torque_label = new QLabel(frame);
        torque_label->setObjectName(QString::fromUtf8("torque_label"));
        torque_label->setGeometry(QRect(-18, 424, 289, 37));
        torque_label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-color: rgb(0, 0, 0);"));
        torque_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        motorThermometer = new QProgressBar(frame);
        motorThermometer->setObjectName(QString::fromUtf8("motorThermometer"));
        motorThermometer->setGeometry(QRect(386, 62, 283, 71));
        motorThermometer->setStyleSheet(QString::fromUtf8("background-color: rgb(211, 215, 207);"));
        motorThermometer->setMaximum(100);
        motorThermometer->setValue(0);
        motorThermometer->setTextVisible(false);
        motorThermometer->setOrientation(Qt::Horizontal);
        MainWindow->setCentralWidget(centralwidget);
        frame->raise();
        meterWidget->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1020, 21));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuDataLog = new QMenu(menubar);
        menuDataLog->setObjectName(QString::fromUtf8("menuDataLog"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuAbout->menuAction());
        menubar->addAction(menuDataLog->menuAction());
        menuAbout->addAction(actionAbout);
        menuDataLog->addSeparator();
        menuDataLog->addAction(actionOpen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
#if QT_CONFIG(tooltip)
        actionAbout->setToolTip(QCoreApplication::translate("MainWindow", "About", nullptr));
#endif // QT_CONFIG(tooltip)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "TORQUE", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "CONTROL COMMAND", nullptr));
        sendButton->setText(QCoreApplication::translate("MainWindow", "SEND", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "SPEED ", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "INVERTER", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "MOTOR", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "MOTOR ", nullptr));
        enable_motor_button->setText(QCoreApplication::translate("MainWindow", "ENABLE", nullptr));
        disable_motor_button->setText(QCoreApplication::translate("MainWindow", "DISABLE", nullptr));
        speed_label->setText(QString());
        label_13->setText(QCoreApplication::translate("MainWindow", "TEMPERATURES", nullptr));
        torque_label->setText(QString());
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuDataLog->setTitle(QCoreApplication::translate("MainWindow", "DataLog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
