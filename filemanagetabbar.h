#ifndef FILEMANAGETABBAR_H
#define FILEMANAGETABBAR_H

#include "codeeditor.h"
#include "filemanage.h"

#include <QObject>
#include <QTabWidget>
#include <QTabBar>
#include <QStackedWidget>
#include <QVector>

class FileManageTabBar : public QTabWidget
{
    Q_OBJECT

public:
    FileManageTabBar(QWidget* parent, csize size);
    ~FileManageTabBar();

    void addFile(QString filePath);
    void removeFileTab(int index);

    QFileInfo getCurrentDocmentFileInfo();

private:
    QStackedWidget* container;
    QFont font;

    QVector<QFileInfo> fileInfoList;
};

#endif // FILEMANAGETABBAR_H
