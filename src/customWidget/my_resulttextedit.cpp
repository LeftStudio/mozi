/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "my_resulttextedit.h"

#include <QPainter>

#ifdef Q_OS_WIN
# if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
# endif
#endif

My_ResultTextEdit::My_ResultTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    defaultBackground = QImage(":/images/images/default_background.png")
            .scaled(300, 300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void My_ResultTextEdit::printPoetry(const Poetry& poem)
{
    this->clear();

    QTextCharFormat fmt = this->currentCharFormat();

    this->setAlignment(Qt::AlignCenter);

    /* Print title */
    fmt.setFontPointSize(22);
    fmt.setFontWeight(QFont::Bold);
    this->setCurrentCharFormat(fmt);
    this->append(poem.title);

    /* Print author */
    fmt.setFontPointSize(18);
    fmt.setFontWeight(QFont::Normal);
    this->setCurrentCharFormat(fmt);
    this->append(poem.author + "\n");

    this->setAlignment(Qt::AlignLeft);

    QTextCursor cursor = this->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();

    blockFmt.setLeftMargin(50);                         // 间隔
    cursor.setBlockFormat(blockFmt);

    fmt.setFontPointSize(16);
    this->setCurrentCharFormat(fmt);

    /* Print poetry */
    this->append(poem.poetry);

    /* Print notes */
    if(!poem.notes.isEmpty())
    {
        fmt.setFontPointSize(16);
        this->setCurrentCharFormat(fmt);
        this->append("\n注释");

        fmt.setFontPointSize(12);
        this->setCurrentCharFormat(fmt);
        this->append(poem.notes);
    }

    /* Print translate */
    if(!poem.translate.isEmpty())
    {
        fmt.setFontPointSize(16);
        this->setCurrentCharFormat(fmt);
        this->append("\n翻译");

        fmt.setFontPointSize(12);
        this->setCurrentCharFormat(fmt);
        this->append(poem.translate);
    }

    /* Print description */
    if(!poem.description.isEmpty())
    {
        fmt.setFontPointSize(16);
        this->setCurrentCharFormat(fmt);
        this->append("\n赏析");

        fmt.setFontPointSize(12);
        this->setCurrentCharFormat(fmt);
        this->append(poem.description);
    }

    this->moveCursor(QTextCursor::Start);
}

void My_ResultTextEdit::paintEvent(QPaintEvent *event)
{
    if(this->toPlainText().isEmpty())
    {
        QPainter painter(this->viewport());
        QRect drawRect = defaultBackground.rect();

        drawRect.moveCenter(this->rect().center());

        painter.drawImage(drawRect, defaultBackground);
    }

    return QTextEdit::paintEvent(event);
}
