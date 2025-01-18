#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent, csize size)
    : QPlainTextEdit(parent), zoom(DEFAULT_ZOOM)
{
    // Set the code editor geometry.
    this->setGeometry(size.x, size.y, size.width, size.height);

    // Init the default font.
    this->font = QFont("Courier New", this->zoom / 10);  // zoom * 1% * 10 (Default font size).

    this->lineNumberArea = new LineNumberArea(this);
    this->lineNumberArea->setFont(this->font);
    this->setFont(this->font);

    // 4 space width.
    this->setTabWidth(4);

    // Connect the signals and the slot functions.
    QObject::connect(this, &CodeEditor::ctrlWheelScrolled, &CodeEditor::zoomAdjustment);
    QObject::connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    QObject::connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);

    // Init the line number area.
    this->updateLineNumberAreaWidth(0);

    this->lineNumberArea->show();
    this->show();
}

CodeEditor::~CodeEditor()
{
    delete this->lineNumberArea;
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = LINE_LABEL_DEFAULT_WIDTH +
        fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::setHighlighter(Highlighter *hPtr)
{

}

bool CodeEditor::getMaybeSave()
{
    return this->maybeSave;
}

void CodeEditor::updateMaybeSave()
{
    this->maybeSave = !this->maybeSave;
}

void CodeEditor::saveFileText(QString filePath)
{
    if (!this->maybeSave)
    {
        FileManage* manager = new FileManage(filePath);
        manager->writeText(this->toPlainText());

        this->updateMaybeSave();
    }
}

void CodeEditor::setTabWidth(int unitWidth)
{
    QFontMetrics fontMetrics(this->font);

    int spaceWidth = fontMetrics.horizontalAdvance(' ');
    int tabWidth = spaceWidth * unitWidth;

    QTextOption option = this->document()->defaultTextOption();
    option.setTabStopDistance(tabWidth);
    this->document()->setDefaultTextOption(option);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

    QPlainTextEdit::resizeEvent(event);
}

void CodeEditor::wheelEvent(QWheelEvent *event)
{
    // If user click control key and scroll wheel.
    if (event->modifiers() & Qt::ControlModifier)
    {
        // Obtain the rotation angle of the roller and broadcast Ctrl signal.
        int delta = event->angleDelta().y();
        emit ctrlWheelScrolled(delta, Qt::Vertical);
        event->accept();
    }
    else
    {
        // Use member functions of the base class for processing other event.
        QPlainTextEdit::wheelEvent(event);
    }
}

void CodeEditor::zoomAdjustment(int delta, Qt::Orientation orientation)
{
    // If the orientation is vertical.
    // No need to handle horizontal mouse wheel scrolling.
    if (orientation == Qt::Vertical)
    {
        if (delta > 0 && (zoom + 5) <= MAX_ZOOM)
        {
            this->zoom += 5;
        }
        else if (delta < 0 && (zoom - 5) >= MIN_ZOOM)
        {
            this->zoom -= 5;
        }
    }

    // Update the font point size.
    this->font.setPointSize(this->zoom / 10);

    // Apply the new font.
    this->setFont(this->font);
    this->lineNumberArea->setFont(this->font);
}

LineNumberArea::LineNumberArea(CodeEditor *editor)
    : QWidget(editor), codeEditor(editor)
{

}

QSize LineNumberArea::sizeHint() const
{
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEditor->lineNumberAreaPaintEvent(event);
}
