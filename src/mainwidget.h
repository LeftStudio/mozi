/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "my_networkmanager.h"
#include "my_updater.h"

#include "my_toast.h"

#define SHADOW_NUM 6
#define LINE_SPACING 50

class QGraphicsDropShadowEffect;
class QListWidgetItem;
class QProgressBar;
class QListWidget;
class QTextEdit;
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

    void on_collectionBtn_clicked();

    void on_cancelBtn_clicked();

    void on_resultList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_collectionList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_loadMoreBtn_clicked();

private:
    Ui::MainWidget *ui;

    bool m_isCheckSuccess = false;

    QSettings *m_settings = nullptr;
    My_Updater *m_updater = nullptr;
    My_NetworkManager *m_networkManager = nullptr;

    My_Toast *m_toast = nullptr;

    /* 控件阴影 */
    QVector<QGraphicsDropShadowEffect *> m_shadowList;

    Poetry m_dailyPoetry;

    /* 初始化 */
    inline void initUI();
    inline void initSignalSlots();

    inline void refresh();       // Refresh the daily poetry

    /* 设置操作函数 */
    void loadSettings();    // 加载设置
    void saveSettings();    // 保存设置

    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    /* 事件重写 */
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

    inline void updateForeground(const QColor& color);

    static inline void createItem(const QString& text,
                                              const QVariant &data,
                                              QListWidget *parent);
};

#endif // MAINWIDGET_H
