﻿#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "my_networkmanager.h"
#include "my_updater.h"

#include "my_toast.h"

class QGraphicsDropShadowEffect;
class QListWidgetItem;
class QSettings;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() Q_DECL_OVERRIDE;

private:

private slots:

    void on_searchBtn_clicked();

    void on_resultList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_collectionBtn_clicked();

    void on_collectionList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_cancelBtn_clicked();

private:
    Ui::MainWidget *ui;

    QSettings *m_settings = nullptr;
    My_Updater *m_updater = nullptr;
    My_NetworkManager *m_networkManager = nullptr;

    My_Toast *m_toast = nullptr;

    QGraphicsDropShadowEffect *m_shadow_1 = nullptr;
    QGraphicsDropShadowEffect *m_shadow_2 = nullptr;
    QGraphicsDropShadowEffect *m_shadow_3 = nullptr;
    QGraphicsDropShadowEffect *m_shadow_4 = nullptr;
    QGraphicsDropShadowEffect *m_shadow_5 = nullptr;
    QGraphicsDropShadowEffect *m_shadow_6 = nullptr;

    /* 初始化 */
    inline void initUI();
    inline void initSignalSlots();

    inline void refesh();

    /* 设置操作函数 */
    void loadSettings();    // 加载设置
    void saveSettings();    // 保存设置

    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    /* 事件重写 */
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;
};

#endif // MAINWIDGET_H
