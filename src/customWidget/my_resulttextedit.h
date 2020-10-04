#ifndef MY_RESULTTEXTEDIT_H
#define MY_RESULTTEXTEDIT_H

#include <QTextEdit>
#include <QJsonObject>

class My_ResultTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit My_ResultTextEdit(QWidget *parent = nullptr);

    void printPoetry(QJsonObject &resultObj);

signals:

private:
    QImage defaultBackground;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // MY_RESULTTEXTEDIT_H
