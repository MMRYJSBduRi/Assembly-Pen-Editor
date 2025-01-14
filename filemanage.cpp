#include "filemanage.h"

FileManage::FileManage(QString fileName)
    : QFile(fileName)
{
    if (fileName.isEmpty())
    {
        return;
    }

    this->fileName = fileName;
}

FileManage::~FileManage()
{
    this->close();
}

void FileManage::writeText(QString text)
{
    if (!this->open(QIODevice::WriteOnly))
    {
        return;
    }

    this->write(text.toUtf8());
    this->close();
}

void FileManage::appendText(QString text)
{
    if (!this->open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream append(this);
    append.setCodec("UTF-8");
    append << text;

    append.device()->close();
    this->close();
}

QString FileManage::readText()
{
    if (!this->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }

    QTextStream read(this);
    read.setCodec("UTF-8");
    QString fileText = read.readAll();

    read.device()->close();
    this->close();

    return  fileText;
}

QFileInfo FileManage::fileInfo()
{
    return QFileInfo(*this);
}
