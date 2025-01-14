#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set the window infomation.
    this->setMinimumSize(720, 480);
    this->setGeometry(400, 400, 720, 480);
    this->setWindowTitle("Assembly Pen v1.0.1");

    this->initMenuBar();

    this->mainTabBar = new FileManageTabBar(this, {0, 23, 720, 457});
}

MainWindow::~MainWindow()
{
}

void MainWindow::initMenuBar()
{
    this->mainMenuBar = new QMenuBar(this);
    this->mainMenuBar->setStyleSheet("QMenuBar{background-color:rgb(200, 200, 200);};");
    this->setMenuBar(this->mainMenuBar);

    this->fileMenu = this->mainMenuBar->addMenu("File");

    this->newAction = this->fileMenu->addAction("New");
    this->openAction = this->fileMenu->addAction("Open");
    this->fileMenu->addSeparator();
    this->saveAction = this->fileMenu->addAction("Save");
    this->saveAsAction = this->fileMenu->addAction("Save As");
    this->closeAction = this->fileMenu->addAction("Close");

    QObject::connect(this->newAction, &QAction::triggered, this, &MainWindow::newFile);
    QObject::connect(this->openAction, &QAction::triggered, this, &MainWindow::openFile);
    QObject::connect(this->saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    QObject::connect(this->saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    QObject::connect(this->closeAction, &QAction::triggered, this, &MainWindow::closeFile);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->mainTabBar->resize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

void MainWindow::newFile()
{
    QString filePath = QFileDialog::getSaveFileName(
        nullptr, "New file", "", "Assembly Source File. (*.asm *.s)");

    if (filePath.isEmpty())
    {
        return;
    }

    this->mainTabBar->addFile(filePath);
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(
        nullptr, "Open file", "", "Assembly Source File. (*.asm *.s)");

    if (filePath.isEmpty())
    {
        return;
    }

    this->mainTabBar->addFile(filePath);
}

void MainWindow::saveFile()
{
    CodeEditor* currentEditor = (CodeEditor*)this->mainTabBar->currentWidget();
    int currentTabIndex = this->mainTabBar->currentIndex();

    if (currentEditor == nullptr)
    {
        return;
    }

    // If the docment isn't saved.
    if (!currentEditor->getMaybeSave())
    {
        QString plainText = currentEditor->toPlainText();
        QString thisDocmentPath = this->mainTabBar->getCurrentDocmentFileInfo().filePath();

        // Write current text into the file.
        FileManage* writeManager = new FileManage(thisDocmentPath);
        writeManager->writeText(plainText);

        currentEditor->updateMaybeSave();

        QString tabTitle = this->mainTabBar->tabText(currentTabIndex);
        tabTitle.chop(1);     // Remove last character, *.

        this->mainTabBar->setTabText(currentTabIndex, tabTitle);
    }
}

void MainWindow::saveAsFile()
{
    CodeEditor* currentEditor = (CodeEditor*)this->mainTabBar->currentWidget();
    QString plainText = currentEditor->toPlainText();

    if (currentEditor == nullptr)
    {
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        nullptr, "Save As file", "", "Assembly Source File. (*.asm *.s)");

    if (filePath.isEmpty())
    {
        return;
    }

    FileManage* writeManager = new FileManage(filePath);
    writeManager->writeText(plainText);
}

void MainWindow::closeFile()
{
    int currentTabIndex = this->mainTabBar->currentIndex();
    this->mainTabBar->removeFileTab(currentTabIndex);
}
