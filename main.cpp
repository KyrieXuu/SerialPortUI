//#include "mainwindow.h"
#include "loginwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginwidget w;
    w.showMaximized();
    return a.exec();
}
