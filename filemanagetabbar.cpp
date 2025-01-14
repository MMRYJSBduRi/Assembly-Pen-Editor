#include "filemanagetabbar.h"

FileManageTabBar::FileManageTabBar(QWidget* parent, csize size)
    : QTabWidget(parent)
{
    this->setGeometry(size.x, size.y, size.width, size.height);

    this->container = this->findChild<QStackedWidget*>();
    this->font = QFont("Courier New", 10);
    this->setFont(this->font);
    this->setTabsClosable(true);

    QObject::connect(this, &QTabWidget::tabCloseRequested, this, &FileManageTabBar::removeFileTab);
    QObject::connect(this, &QTabWidget::tabBarDoubleClicked, this, &FileManageTabBar::removeFileTab);

    this->show();
}

FileManageTabBar::~FileManageTabBar()
{

}

void FileManageTabBar::addFile(QString filePath)
{
    csize size;
    size.x = 0;
    size.y = 0;
    size.width = this->container->width();
    size.height = this->container->height();

    FileManage* manager = new FileManage(filePath);
    CodeEditor* editor = new CodeEditor(this->container, size);
    editor->setPlainText(manager->readText());

    QObject::connect(editor, &CodeEditor::textChanged, this, [this, editor](){
        // If current docment is saved.
        if(editor->getMaybeSave() == true)
        {
            editor->updateMaybeSave();
            int currentTabIndex = this->currentIndex();
            QString currentTitle = this->tabText(currentTabIndex);
            this->setTabText(currentTabIndex, currentTitle.append('*'));
        }
    });

    this->addTab(editor, manager->fileInfo().fileName());
    this->fileInfoList.push_back(manager->fileInfo());
}

void FileManageTabBar::removeFileTab(int index)
{
    this->removeTab(index);
    this->fileInfoList.remove(index);
}

QFileInfo FileManageTabBar::getCurrentDocmentFileInfo()
{
    int currentTabIndex = this->currentIndex();
    return this->fileInfoList[currentTabIndex];
}
