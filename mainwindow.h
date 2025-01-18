#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
#include "filemanagetabbar.h"

#include <QMainWindow>
#include <QSettings>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QRegularExpression>
#include <QApplication>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initMenuBar();

    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void closeFile();

    void buildProgram();
    void runProgram();

    void sdkSettingsUI();
    void aboutSoftware();

    // Sorry, when I try to use the layout manager, my program will have various bugs.
    void resizeEvent(QResizeEvent* event) override;

private:
    FileManageTabBar* mainTabBar;
    QMenuBar* mainMenuBar;

    QMenu* fileMenu;
    QMenu* buildMenu;
    QMenu* settingsMenu;
    QMenu* helpMenu;

    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* closeAction;

    QAction* buildAction;
    QAction* runAction;

    // QAction* editorSettingsAction;
    QAction* sdkSettingsAction;
};

#endif // MAINWINDOW_H
