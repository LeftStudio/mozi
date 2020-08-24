#ifndef MY_TOAST_H
#define MY_TOAST_H

#include <QLabel>

class QHBoxLayout;
class QPropertyAnimation;

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
                      const QString &style = DEFULT_TOAST_STYLE);

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
    QPropertyAnimation *m_animation = nullptr;

    bool m_isShowing = false;

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
};

#endif // MY_TOAST_H
