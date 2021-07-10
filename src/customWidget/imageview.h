/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>

class QVariantAnimation;

class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);
    virtual ~ImageView() Q_DECL_OVERRIDE;

    void setPixmapData(const QByteArray &pixmap);
    QPixmap pixmap() const { return m_currentPixmap; }

    QColor majorColor() const;

signals:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int m_progress = 0;

    QPixmap m_currentPixmap;
    QPixmap m_lastPixmap;

    QVariantAnimation *m_animation = nullptr;

    static bool blurPixmap(QPixmap& pixmap);

    static const QPixmap transparentPixmap(const QPixmap& pixmap, int alpha);
};

#endif // IMAGEVIEW_H
