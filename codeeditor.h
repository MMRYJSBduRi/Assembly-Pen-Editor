#ifndef CODEEDITORPAGE_H
#define CODEEDITORPAGE_H

#include "common.h"
#include "highlighter.h"

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <QFontMetrics>
#include <QTextOption>
#include <QFont>
#include <QWheelEvent>
#include <QTextBlock>
#include <QPainter>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent, csize size);
    ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setHighlighter(Highlighter* hPtr);

    bool getMaybeSave();
    void updateMaybeSave();

    void setTabWidth(int unitWidth);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void zoomAdjustment(int delta, Qt::Orientation orientation);

signals:
    void ctrlWheelScrolled(int delta, Qt::Orientation orientation);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    enum
    {
        LINE_LABEL_DEFAULT_WIDTH = 30,   // 30px
        MAX_ZOOM = 200,                  // 200%
        MIN_ZOOM = 10,                   //  10%
        DEFAULT_ZOOM = 100,              // 100%
    };

    csize size;
    quint8 zoom;    // For example, when zoom=65, it represents 65%
    bool maybeSave = true;

    QFont font;
    QWidget* lineNumberArea;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITORPAGE_H
