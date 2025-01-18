#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set the window infomation.
    this->setMinimumSize(720, 480);
    this->setGeometry(400, 400, 720, 480);
    this->setWindowTitle("Assembly Pen v1.0.1");
    this->setWindowIcon(QIcon(":image/icon.ico"));

    this->mainTabBar = new FileManageTabBar(this, {0, 23, 720, 457});

    this->initMenuBar();
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
    this->buildMenu = this->mainMenuBar->addMenu("Build");
    this->helpMenu = this->mainMenuBar->addMenu("Help");

    this->newAction = this->fileMenu->addAction("New");
    this->openAction = this->fileMenu->addAction("Open");
    this->fileMenu->addSeparator();
    this->saveAction = this->fileMenu->addAction("Save");
    this->saveAsAction = this->fileMenu->addAction("Save As");
    this->closeAction = this->fileMenu->addAction("Close");

    this->buildAction = this->buildMenu->addAction("Build");
    this->runAction = this->buildMenu->addAction("Run");
    this->sdkSettingsAction = this->helpMenu->addAction("SDK settings");

    QObject::connect(this->newAction, &QAction::triggered, this, &MainWindow::newFile);
    QObject::connect(this->openAction, &QAction::triggered, this, &MainWindow::openFile);
    QObject::connect(this->saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    QObject::connect(this->saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    QObject::connect(this->closeAction, &QAction::triggered, this, &MainWindow::closeFile);

    QObject::connect(this->buildAction, &QAction::triggered, this, &MainWindow::buildProgram);
    QObject::connect(this->runAction, &QAction::triggered, this, &MainWindow::runProgram);
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
        QString thisDocmentPath = this->mainTabBar->getCurrentDocmentFileInfo().filePath();

        // Write current text into the file.
        currentEditor->saveFileText(thisDocmentPath);

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

void MainWindow::buildProgram()
{
    CodeEditor* currentEditor = (CodeEditor*)this->mainTabBar->currentWidget();
    if (currentEditor == nullptr)
    {
        return;
    }
    currentEditor->saveFileText(
        this->mainTabBar->getCurrentDocmentFileInfo().filePath());

    QString configFilePath = QApplication::applicationDirPath() + "/config.ini";
    QSettings* settings = new QSettings(configFilePath, QSettings::IniFormat);

    QString assemblerPath = settings->value("SDK/ASSEMBLER_PATH").toString();
    QString assemblerArgs = settings->value("SDK/ASSEMBLER_ARGS").toString();
    QString linkerPath = settings->value("SDK/LINKER_PATH").toString();
    QString linkerArgs = settings->value("SDK/LINKER_ARGS").toString();

    QStringList assemblerArgsList = assemblerArgs.split(
        QRegularExpression("\\s+"), QString::SkipEmptyParts);
    QProcess assemblerProcess;
    assemblerProcess.start(assemblerPath, assemblerArgsList);
    assemblerProcess.waitForFinished();
    qDebug() << assemblerProcess.readAllStandardOutput();
    qDebug() << assemblerProcess.readAllStandardError();

    QStringList linkerArgsList = linkerArgs.split(
        QRegularExpression("\\s+"), QString::SkipEmptyParts);
    QProcess linkerProcess;
    linkerProcess.start(assemblerPath, assemblerArgsList);
    linkerProcess.waitForFinished();
    qDebug() << linkerProcess.readAllStandardOutput();
    qDebug() << linkerProcess.readAllStandardError();
}

void MainWindow::runProgram()
{
    bool isOk;
    QString command = QInputDialog::getText(
        nullptr,
        "Run Program",
        "Command: ",
        QLineEdit::Normal,
        "Example: run.exe arg1 arg2...",
        &isOk
    );

    if (isOk && !command.isEmpty())
    {
        QStringList commandList = command.split(
            QRegularExpression("\\s+"), QString::SkipEmptyParts);

        QString program = commandList[0];
        commandList.removeFirst();

        QProcess runProcess;
        runProcess.start(program, commandList);
        runProcess.waitForFinished();

        qDebug() << runProcess.readAllStandardOutput();
        qDebug() << runProcess.readAllStandardError();
    }
    else
    {
        return;
    }
}

void MainWindow::sdkSettingsUI()
{
    //this->sdkOptionUI = new SDKSettings(nullptr);
}

void MainWindow::aboutSoftware()
{
    QMessageBox::information(nullptr, tr("About Assembly Pen"),
        tr("Current version: v1.0.1\n") +
        tr("Assembly Pen is written based on Qt.\n"));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->mainTabBar->resize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}
