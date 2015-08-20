#include "mainwindow.h"
#include <QApplication>
#include "windialog.h"
#include "helpdialog.h"
WinDialog *winbox;
HelpDialog *helpbox;
MainWindow *mainwindow;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    mainwindow = &w;
    WinDialog wb;
    winbox = &wb;
    HelpDialog hb;
    helpbox = &hb;
    w.show();
    return a.exec();
}
