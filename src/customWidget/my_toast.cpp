/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "my_toast.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

My_Toast::My_Toast(QWidget *parent, int horizontalMargin, int verticalMargin, int waitMsecs,
                   const QString &style) :
    QWidget(parent),
    m_messageLabel(new QLabel(this)),
    m_layout(new QHBoxLayout(this)),
    m_animation(new QSequentialAnimationGroup(this)),
    m_posAnimation(new QPropertyAnimation(this, "pos")),
    m_opacityAnimation(new QPropertyAnimation(this, "windowOpacity"))
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    m_layout->addWidget(m_messageLabel);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_messageLabel->setStyleSheet(style);
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setContentsMargins(horizontalMargin, verticalMargin, horizontalMargin, verticalMargin);

    /* 显示动画 */
    m_posAnimation->setDuration(300);
    m_posAnimation->setEasingCurve(QEasingCurve::OutCubic);

    /* 消失动画 */
    m_opacityAnimation->setDuration(250);
    m_opacityAnimation->setStartValue(1);
    m_opacityAnimation->setEndValue(0);

    m_animation->addAnimation(m_posAnimation);
    m_animation->addPause(waitMsecs);
    m_animation->addAnimation(m_opacityAnimation);

    connect(m_animation, &QSequentialAnimationGroup::finished, this, &My_Toast::close);
}

/**
 * @brief 弹出提示
 */
void My_Toast::toast()
{
    if(m_animation->state() == QSequentialAnimationGroup::Running)
        m_animation->stop();

    this->setWindowOpacity(1);          // 透明度复位
    this->show();
    m_animation->start();
}

/**
 * @brief 设置文字
 */
void My_Toast::setText(const QString &text)
{
    // 自适应大小
    m_messageLabel->setText(text);
    m_messageLabel->adjustSize();
    this->adjustSize();

    // 计算动画起止坐标
    int startx = this->parentWidget()->x() + (this->parentWidget()->width() - this->width())
            / 2 + (this->width() - m_messageLabel->width()) / 2;
    int starty = this->parentWidget()->y() + this->parentWidget()->height();
    int endy = this->parentWidget()->y() + this->parentWidget()->height() * 3 / 4;

    m_posAnimation->setStartValue(QPoint(startx, starty));
    m_posAnimation->setEndValue(QPoint(startx, endy));
}
