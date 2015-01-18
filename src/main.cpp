#include "mainwindow.h"
#include "fileIO.hh"
#include <QApplication>

int main(int argc, char *argv[])
{  
    //loadhistory();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
