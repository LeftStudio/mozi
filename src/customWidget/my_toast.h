/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#ifndef MY_TOAST_H
#define MY_TOAST_H

#include <QLabel>

class QTimer;
class QHBoxLayout;
class QPropertyAnimation;
class QSequentialAnimationGroup;

#define DEFULT_TOAST_STYLE "\
QLabel{\
    color:#FFFFFF;\
    font:15px;\
    font-weight:500;\
    background-color:rgba(0,0,0,150);\
    padding:3px;\
    border-radius:9;\
}"\

class My_Toast : public QWidget
{
    Q_OBJECT
public:
    explicit My_Toast(QWidget *parent = nullptr, int horizontalMargin = 12, int verticalMargin = 12,
                      int waitMsecs = 1200, const QString &style = DEFULT_TOAST_STYLE);

    void setText(const QString& text);

    void toast();
    void toast(const QString& text)
    {
        this->setText(text);
        this->toast();
    }

private:
    QLabel *m_messageLabel = nullptr;
    QHBoxLayout *m_layout = nullptr;

    QSequentialAnimationGroup *m_animation = nullptr;
    QPropertyAnimation *m_posAnimation     = nullptr;
    QPropertyAnimation *m_opacityAnimation = nullptr;    
};

#endif // MY_TOAST_H
