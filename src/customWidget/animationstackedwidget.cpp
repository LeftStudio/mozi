/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "animationstackedwidget.h"

#include <QPixmap>
#include <QPainter>
#include <QTransform>
#include <QPropertyAnimation>

AnimationStackedWidget::AnimationStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    m_animation(new QPropertyAnimation(this,"rotateValue"))
{
    m_animation->setDuration(500);
    m_animation->setEasingCurve(QEasingCurve::Linear);

    connect(m_animation, &QPropertyAnimation::valueChanged,
            [this]{ this->repaint(); });
    connect(m_animation, &QPropertyAnimation::finished, this,
            &AnimationStackedWidget::on_finished);
}

AnimationStackedWidget::~AnimationStackedWidget()
{

}

/**
 * @brief 绘制旋转画面
 */
void AnimationStackedWidget::paintEvent(QPaintEvent *event)
{
    if(m_isAnimating)
    {
        if(m_rotateValue > 90)
        {
            QWidget *nextWidget = this->widget(m_nextIndex);

            QPixmap pixmap(nextWidget->size());
            nextWidget->render(&pixmap);

            QTransform transform;
            transform.translate(this->width() / 2, 0);
            transform.rotate(m_rotateValue + 180, Qt::YAxis);

            QPainter painter(this);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * this->width() / 2, 0, pixmap);
        }
        else
        {
            QWidget *currentWidget = this->currentWidget();

            QPixmap pixmap(currentWidget->size());
            currentWidget->render(&pixmap);

            QTransform transform;
            transform.translate(this->width() / 2, 0);
            transform.rotate(m_rotateValue, Qt::YAxis);

            QPainter painter(this);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * this->width() / 2, 0, pixmap);
        }
    }
    else
        QWidget::paintEvent(event);
}

/**
 * @brief 启动动画
 */
void AnimationStackedWidget::rotate(int index)
{
    if(m_isAnimating)
        return;

    m_nextIndex = index;

    this->widget(index)->setGeometry(0, 0, this->width(), this->height());

    m_animation->setStartValue(0);
    m_animation->setEndValue(180);

    this->currentWidget()->hide();

    m_isAnimating = true;
    m_animation->start();
}

/**
 * @brief 动画完成
 */
void AnimationStackedWidget::on_finished()
{
    m_rotateValue = 0;
    m_isAnimating = false;

    this->widget(m_nextIndex)->show();
    this->widget(m_nextIndex)->raise();

    this->setCurrentWidget(this->widget(m_nextIndex));
    this->repaint();
}
