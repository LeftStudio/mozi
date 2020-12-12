/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "animationstackedwidget.h"

#include <QPainter>
#include <QEventLoop>
#include <QVariantAnimation>

AnimationStackedWidget::AnimationStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    m_animation(new QVariantAnimation(this))
{
    m_animation->setStartValue(0);
    m_animation->setEndValue(180);
    m_animation->setDuration(500);
    m_animation->setEasingCurve(QEasingCurve::Linear);

    connect(m_animation, &QVariantAnimation::valueChanged,
            [this](const QVariant value){
        m_rotateValue = value.toInt();
        this->repaint();
    });
    connect(m_animation, &QVariantAnimation::finished, this,
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
        QPainter painter(this);

        if(m_rotateValue > 90)
        {
            QTransform transform;
            transform.translate(this->width() / 2, 0);
            transform.rotate(m_rotateValue + 180, Qt::YAxis);

            painter.setTransform(transform);
            painter.drawPixmap(-1 * this->width() / 2, 0, m_nextPixmap);
        }
        else
        {
            QTransform transform;
            transform.translate(this->width() / 2, 0);
            transform.rotate(m_rotateValue, Qt::YAxis);

            painter.setTransform(transform);
            painter.drawPixmap(-1 * this->width() / 2, 0, m_currentPixmap);
        }
    }
    else
        QStackedWidget::paintEvent(event);
}

/**
 * @brief 大小调整时更新动画帧
 */
void AnimationStackedWidget::resizeEvent(QResizeEvent *event)
{
    if(m_isAnimating)
    {
        m_nextWidget->setGeometry(0, 0, this->width(), this->height());
        m_currentWidget->setGeometry(0, 0, this->width(), this->height());

        m_currentPixmap = QPixmap(m_currentWidget->size());
        m_currentWidget->render(&m_currentPixmap);

        m_nextPixmap = QPixmap(m_nextWidget->size());
        m_nextWidget->render(&m_nextPixmap);
    }

    QStackedWidget::resizeEvent(event);
}

/**
 * @brief 启动动画
 */
void AnimationStackedWidget::rotate(int index, bool exec)
{
    if(m_isAnimating)
        return;

    m_nextIndex = index;

    m_currentWidget = this->currentWidget();
    m_nextWidget = this->widget(m_nextIndex);

    if(m_nextWidget == nullptr)
        return;

    m_nextWidget->setGeometry(0, 0, this->width(), this->height());

    m_currentPixmap = QPixmap(m_currentWidget->size());
    m_currentWidget->render(&m_currentPixmap);

    m_nextPixmap = QPixmap(m_nextWidget->size());
    m_nextWidget->render(&m_nextPixmap);

    m_currentWidget->hide();
    m_isAnimating = true;
    m_animation->start();

    if(exec)
    {
        QEventLoop loop;
        connect(m_animation, &QVariantAnimation::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }
}

/**
 * @brief 动画完成
 */
void AnimationStackedWidget::on_finished()
{
    m_rotateValue = 0;
    m_isAnimating = false;

    m_nextWidget->show();
    m_nextWidget->raise();

    this->setCurrentIndex(m_nextIndex);
    this->repaint();
}
