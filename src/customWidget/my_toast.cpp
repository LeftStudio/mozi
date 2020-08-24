#include "my_toast.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QPropertyAnimation>

My_Toast::My_Toast(QWidget *parent, int horizontalMargin, int verticalMargin,
                   const QString &style) :
    QWidget(parent),
    m_messageLabel(new QLabel(this)),
    m_layout(new QHBoxLayout(this)),
    m_animation(new QPropertyAnimation(this, "pos"))
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    m_layout->addWidget(m_messageLabel);
    m_messageLabel->setStyleSheet(style);
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setContentsMargins(horizontalMargin, verticalMargin, horizontalMargin, verticalMargin);

    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_animation, &QPropertyAnimation::finished,
            [this]{ QTimer::singleShot(1000, this, &My_Toast::close); });
}

/**
 * @brief 弹出提示
 */
void My_Toast::toast()
{
    if(m_isShowing)
        return;
    else
        m_isShowing = true;

    this->show();
    m_animation->start();
}

void My_Toast::closeEvent(QCloseEvent *event)
{
    m_isShowing = false;
    QWidget::closeEvent(event);
}

/**
 * @brief 设置文字
 */
void My_Toast::setText(const QString &text)
{
    m_messageLabel->setText(text);
    m_messageLabel->adjustSize();
    this->adjustSize();

    int startx = this->parentWidget()->x() + (this->parentWidget()->width() - width()) / 2 + (width() - m_messageLabel->width()) / 2;
    int starty = this->parentWidget()->y() + this->parentWidget()->height();
    int endy = this->parentWidget()->y() + this->parentWidget()->height() * 3 / 4;

    m_animation->setStartValue(QPoint(startx, starty));
    m_animation->setEndValue(QPoint(startx, endy));
}
