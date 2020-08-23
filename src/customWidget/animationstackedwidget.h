#ifndef ANIMATIONSTACKEDWIDGET_H
#define ANIMATIONSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QVariant>

class QPropertyAnimation;

class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal rotateValue MEMBER m_rotateValue)

public:
    explicit AnimationStackedWidget(QWidget *parent = nullptr);
    ~AnimationStackedWidget() Q_DECL_OVERRIDE;

    void rotate(int);

signals:

private slots:
    void on_finished();

private:
    QPropertyAnimation *m_animation = nullptr;

    int m_nextIndex;
    bool m_isAnimating = false;
    qreal m_rotateValue = 0;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // ANIMATIONSTACKEDWIDGET_H
