/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "imageview.h"

#include <QDebug>
#include <QPainter>
#include <QVariantAnimation>

ImageView::ImageView(QWidget *parent)
    : QWidget(parent),
    m_animation(new QVariantAnimation(this))
{
    m_animation->setStartValue(255);
    m_animation->setEndValue(0);
    m_animation->setDuration(1000);

    QObject::connect(m_animation, &QVariantAnimation::valueChanged, this,
                     [this](const QVariant& value){
                         m_progress = value.toInt();
                         this->repaint();
                     });
}

ImageView::~ImageView()
{

}

void ImageView::setPixmapData(const QByteArray& pixmap)
{
    m_lastPixmap = m_currentPixmap;
    m_currentPixmap.loadFromData(pixmap);

    if(m_lastPixmap.isNull())
    {
        this->repaint();
        return;
    }

    if(m_animation->state() == QVariantAnimation::Running)
        m_animation->stop();

    m_progress = 255;

    m_animation->start();
}

QColor ImageView::majorColor() const
{
    return m_currentPixmap.scaled({3, 3}).toImage().pixelColor(1, 1);
}

void ImageView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);         // Painting based on QWidget::paintEvent

    if(m_currentPixmap.isNull())
        return;

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(this->rect(), m_currentPixmap);

    if(m_progress && !m_lastPixmap.isNull())
        painter.drawPixmap(this->rect(), transparentPixmap(m_lastPixmap, m_progress));
}

const QPixmap ImageView::transparentPixmap(const QPixmap &pixmap, int alpha)
{
    QPixmap temp(pixmap.size());
    temp.fill(Qt::transparent);

    QPainter painter(&temp);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(temp.rect(), {0, 0, 0, alpha});

    return temp;
}
