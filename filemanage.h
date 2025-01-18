#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include "common.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

class FileManage : public QFile
{
    Q_OBJECT

public:
    FileManage(QString fileName);
    ~FileManage();

    void writeText(QString text);
    void appendText(QString text);
    QString readText();

    QFileInfo fileInfo();

private:
    QFile* targetFile;
    QString fileName;
};

#endif // FILEMANAGE_H
