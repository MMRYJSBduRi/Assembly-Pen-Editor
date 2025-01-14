#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QTextDocument>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument* parent);
    ~Highlighter();
};

#endif // HIGHLIGHTER_H
