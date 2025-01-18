#ifndef COMMON_H
#define COMMON_H

#include <qglobal.h>

#include <QString>
#include <QStringList>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QVector>

typedef struct CommonSize
{
    quint32 x;
    quint32 y;
    quint32 width;
    quint32 height;
} csize;

typedef struct AssemblyLexicon
{
    //QStringList
} asmlex;

typedef struct CurrentSettings
{
    QString assemblerPath;
    QString assemblerArgs;
    QString linkerPath;
    QString linkerArgs;
} csettings;

#endif // COMMON_H
