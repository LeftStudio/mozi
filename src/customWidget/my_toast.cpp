/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "my_toast.h"

#include <QTimer>
#include <QEventLoop>
#include <QHBoxLayout>
#include <QPropertyAnimation>

My_Toast::My_Toast(QWidget *parent, int horizontalMargin, int verticalMargin,
                   const QString &style) :
    QWidget(parent),
    m_timer(new QTimer(this)),
    m_messageLabel(new QLabel(this)),
    m_layout(new QHBoxLayout(this)),
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

    m_posAnimation->setDuration(300);
    m_posAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_opacityAnimation->setDuration(300);
    m_opacityAnimation->setStartValue(1);
    m_opacityAnimation->setEndValue(0);

    connect(m_timer, &QTimer::timeout,this,&My_Toast::on_toastClose);
    connect(m_posAnimation, &QPropertyAnimation::finished,
            [this]{ m_timer->start(1100); });
}

/**
 * @brief 弹出提示
 */
void My_Toast::toast()
{
    if(m_isShowing)
    {
        m_posAnimation->stop();
        m_opacityAnimation->stop();
        this->setWindowOpacity(1);
        m_timer->stop();
    }
    else
        m_isShowing = true;

    this->show();
    m_posAnimation->start();
}

void My_Toast::on_toastClose()
{
    m_opacityAnimation->start();

    QEventLoop loop;
    connect(m_opacityAnimation,&QPropertyAnimation::finished,&loop,&QEventLoop::quit);
    loop.exec();

    this->close();
    m_timer->stop();
}

void My_Toast::closeEvent(QCloseEvent *event)
{
    m_isShowing = false;
    this->setWindowOpacity(1);
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

    m_posAnimation->setStartValue(QPoint(startx, starty));
    m_posAnimation->setEndValue(QPoint(startx, endy));
}
