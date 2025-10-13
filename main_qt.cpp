#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFile>
#include <unordered_map>
#include <string>
#include <iostream>    
#include <utility>    
#include "/home/juku/Desktop/Code/c++/JustPay_Console/Include/MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
