#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
#include "filemanagetabbar.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QEvent>

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

    // Sorry, when I try to use the layout manager, my program will have various bugs.
    void resizeEvent(QResizeEvent* event) override;

private:
    FileManageTabBar* mainTabBar;

    QMenuBar* mainMenuBar;
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* buildMenu;
    QMenu* settingsMenu;
    QMenu* helpMenu;

    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* closeAction;
};

#endif // MAINWINDOW_H
