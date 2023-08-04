#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.setWindowIcon(QIcon( ":/favicon.ico"));
    w.show();
    w.setWindowTitle("OCR Recognize Text from Image");

    return a.exec();
}
