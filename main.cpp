// #include <QApplication>
// #include "ui/splashscreen.h"

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);

//     SplashScreen splash;
//     splash.show();

//     return a.exec();
// }

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
