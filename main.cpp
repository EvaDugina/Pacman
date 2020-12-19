#include <iostream>
#include <QApplication>
#include <QVector>

#include "mainwindow.h"
#include "game.h"


int main(int argc, char* argv []){
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
