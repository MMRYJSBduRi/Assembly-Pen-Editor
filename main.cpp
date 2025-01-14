#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication programApplication(argc, argv);

    MainWindow EditorMainWindow;
    EditorMainWindow.show();

    return programApplication.exec();
}
