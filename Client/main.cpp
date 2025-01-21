//
// Created by Danyil Pozniakov on /18/2025.
//

#include <QApplication>

#include "MainWindow/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    return app.exec();
}


