/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#ifndef ANIMATIONSTACKEDWIDGET_H
#define ANIMATIONSTACKEDWIDGET_H

#include <QStackedWidget>

class QVariantAnimation;

class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit AnimationStackedWidget(QWidget *parent = nullptr);
    ~AnimationStackedWidget() Q_DECL_OVERRIDE;

    void rotate(int index, bool exec = true);

    QVariantAnimation* animation() const { return m_animation; }

signals:

private slots:
    void on_finished();

private:
    QVariantAnimation *m_animation = nullptr;

    int m_nextIndex;
    bool m_isAnimating = false;
    qreal m_rotateValue = 0;

    QPixmap m_currentPixmap;
    QPixmap m_nextPixmap;

    QWidget *m_currentWidget = nullptr;
    QWidget *m_nextWidget    = nullptr;

    void paintEvent(QPaintEvent *event)   Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
};

#endif // ANIMATIONSTACKEDWIDGET_H
