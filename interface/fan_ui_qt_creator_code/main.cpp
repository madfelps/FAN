#include "mainwindow.h"

#include <QApplication>
/**  Maximum temperature value showed in motor thermometer */
int main(int argc, char *argv[])
{

    //! SEND button function
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
