/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "my_resulttextedit.h"

#include <QPainter>

My_ResultTextEdit::My_ResultTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    defaultBackground = QImage(":/images/images/default_background.png")
            .scaled(300, 300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void My_ResultTextEdit::printPoetry(QJsonObject &resultObj)
{
    this->clear();

    QTextCharFormat fmt = this->currentCharFormat();

    this->setAlignment(Qt::AlignCenter);

    /* 打印标题 */
    fmt.setFontPointSize(22);
    fmt.setFontWeight(QFont::Bold);
    this->setCurrentCharFormat(fmt);
    this->append(resultObj["biaoti"].toString());

    /* 打印作者 */
    fmt.setFontPointSize(18);
    fmt.setFontWeight(QFont::Normal);
    this->setCurrentCharFormat(fmt);
    this->append(resultObj["zuozhe"].toString() + "\n");

    /* 打印内容 */
    fmt.setFontPointSize(16);
    this->setCurrentCharFormat(fmt);
    this->append(resultObj["neirong"].toString() + "\n");

    this->setAlignment(Qt::AlignLeft);

    QTextBlockFormat blockFmt;
    blockFmt.setLeftMargin(50);                                 // 间隔
    this->textCursor().setBlockFormat(blockFmt);

    /* 打印介绍 */
    fmt.setFontPointSize(12);
    this->setCurrentCharFormat(fmt);
    this->append(resultObj["jieshao"].toString());

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
