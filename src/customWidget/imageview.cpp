/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "imageview.h"

#include <QPainter>
#include <QVariantAnimation>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage,
                                          qreal radius, bool quality,
                                          bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

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

    if(!blurPixmap(m_currentPixmap))
        return;

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
    return m_currentPixmap.isNull() ? Qt::white :
               m_currentPixmap.scaled({3, 3}).toImage().pixelColor(1, 1);
}

void ImageView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);         // Painting based on QWidget::paintEvent

    if(m_currentPixmap.isNull())
        return;

    QPainter painter(this);
    painter.drawPixmap(fitRect(this->rect(), m_currentPixmap.size()), m_currentPixmap);

    if(m_progress && !m_lastPixmap.isNull())
        painter.drawPixmap(fitRect(this->rect(), m_lastPixmap.size()),
                           transparentPixmap(m_lastPixmap, m_progress));
}

QRect ImageView::fitRect(const QRect &screen, QSize size)
{
    if(!screen.isValid())
        return {};

    QRect ret;

    ret.setSize(size.scaled(screen.size(), Qt::KeepAspectRatioByExpanding));
    ret.moveCenter(screen.center());

    return ret;
}

bool ImageView::blurPixmap(QPixmap &pixmap)
{
    if(pixmap.isNull())
        return false;

    QImage image(pixmap.toImage());

    QPainter painter(&pixmap);
    qt_blurImage(&painter, image, 50, true, false);     //blur radius: 60px

    return true;
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
