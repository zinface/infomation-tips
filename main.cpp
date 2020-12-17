#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


// fedora 不正常 疑似没有Qt环境
// deepin 非窗口特效异常
