/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QFrame *frame;
    QFrame *frame_3;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QFrame *frame_4;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(561, 371);
        frame = new QFrame(About);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 561, 371));
        frame->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(181, 181, 181);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(0, 10, 541, 221));
        frame_3->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(181, 181, 181);"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 531, 111));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        label->setWordWrap(true);
        label_3 = new QLabel(frame_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 120, 111, 101));
        label_3->setAutoFillBackground(false);
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/imagens/images/felipe.jpeg")));
        label_3->setScaledContents(true);
        label_2 = new QLabel(frame_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(130, 120, 121, 101));
        label_2->setTabletTracking(true);
        label_2->setFocusPolicy(Qt::NoFocus);
        label_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_2->setAcceptDrops(true);
        label_2->setAutoFillBackground(false);
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/imagens/images/gaspar.jpeg")));
        label_2->setScaledContents(true);
        label_2->setWordWrap(true);
        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(10, 230, 541, 146));
        frame_4->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(181, 181, 181);"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(frame_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(0, 10, 511, 41));
        label_4->setFont(font);
        label_5 = new QLabel(frame_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 40, 121, 91));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/imagens/images/greco.png")));
        label_5->setScaledContents(true);
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(130, 40, 111, 91));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/imagens/images/glauco.jpg")));
        label_6->setScaledContents(true);
        label_7 = new QLabel(frame_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(250, 40, 121, 91));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/imagens/images/henrique.jpg")));
        label_7->setScaledContents(true);

        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QCoreApplication::translate("About", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("About", "Graphical interface for using the wind tunnel in hangar 2 of the Aeronautical Engineering Department at S\303\243o Carlos School of Engineering - EESC USP, as part of the Scientific Initiation project \"Development of an embedded software for control via CAN communication of a wind tunnel \" between 2020 and 2021.\n"
"\n"
"Students:\n"
"Felipe Moura Madureira, Gaspar Henrique Martins de Oliveira", nullptr));
        label_3->setText(QString());
        label_2->setText(QString());
        label_4->setText(QCoreApplication::translate("About", "Advisors: \n"
"Prof.PhD.Paulo Celso Greco J\303\272nio, Prof.PhD.Glauco, PhD. Student Henrique Borges Garcia\n"
"\n"
"", nullptr));
        label_5->setText(QString());
        label_6->setText(QString());
        label_7->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
