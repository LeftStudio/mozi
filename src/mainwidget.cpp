/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <QJsonArray>
#include <QJsonObject>
#include <QDesktopServices>
#include <QGraphicsDropShadowEffect>

#ifdef Q_OS_WIN
# if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
# endif
#endif

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    m_updater(new My_Updater(this)),
    m_networkManager(new My_NetworkManager(this)),
    m_toast(new My_Toast(this))
{
    ui->setupUi(this);
    this->initUI();
    this->loadSettings();
    this->initSignalSlots();

    ui->aboutLabel->installEventFilter(this);
    ui->aboutLabel_2->installEventFilter(this);

    this->refesh();

    if(m_updater->checkUpdate())
    {
        if(m_updater->isNeedUpdate())
            m_toast->toast(QString("发现新版本:%1，点击Mozi 2021跳转到链接")
                           .arg(m_updater->latestVersion()));
    }
    else
        m_toast->toast(QString("检查更新失败，当前版本:%1")
                       .arg(QApplication::applicationVersion()));
}

MainWidget::~MainWidget()
{
    delete ui;
}

/**
 * @brief 初始化UI
 */
void MainWidget::initUI()
{
    /* 按钮阴影 */
    QGraphicsDropShadowEffect *newShadow = nullptr;
    for(int i = 0 ; i < 6 ; i++)
    {
        newShadow = new QGraphicsDropShadowEffect(this);

        newShadow->setColor(QColor(200,200,200,180));
        newShadow->setBlurRadius(6);
        newShadow->setOffset(3, 3);

        m_shadowList.push_back(newShadow);
    }

    ui->searcbLineEdit->setGraphicsEffect(m_shadowList.at(0));
    ui->searchBtn->setGraphicsEffect(m_shadowList.at(1));
    ui->collectionBtn->setGraphicsEffect(m_shadowList.at(2));
    ui->returnBtn->setGraphicsEffect(m_shadowList.at(3));
    ui->returnBtn_2->setGraphicsEffect(m_shadowList.at(4));
    ui->cancelBtn->setGraphicsEffect(m_shadowList.at(5));

    QFont font = ui->resultList->font();
    font.setPointSize(14);
    ui->resultList->setFont(font);
    ui->collectionList->setFont(font);

    ui->loadMoreBtn->hide();
}

/**
 * @brief 初始化信号槽
 */
void MainWidget::initSignalSlots()
{
    connect(ui->refreshBtn, &QPushButton::clicked, this,           // 刷新冷却
            [this]{
        QTimer::singleShot(1000, this, [this]{ ui->refreshBtn->setEnabled(true); });

        ui->refreshBtn->setEnabled(false);
        this->refesh();
    });
    connect(ui->searchPageBtn, &QPushButton::clicked, this,
            [this]{
        ui->stackedWidget->rotate(3);
        this->setWindowTitle("Mozi - 搜索");
    });
    connect(ui->collectionPageBtn, &QPushButton::clicked, this,
            [this]{
        ui->stackedWidget->rotate(2);
        this->setWindowTitle("Mozi - 收藏");
    });

    connect(ui->returnBtn, &QPushButton::clicked, this,
            [this]{
        ui->stackedWidget->rotate(0);
        this->setWindowTitle("Mozi");
    });
    connect(ui->returnBtn_2, &QPushButton::clicked, this,
            [this]{
        ui->stackedWidget->rotate(0);
        this->setWindowTitle("Mozi");
    });
}

/**
 * @brief 加载设置
 */
void MainWidget::loadSettings()
{
#ifdef Q_OS_WIN
    m_settings = new QSettings("./Mozi_Settings.ini", QSettings::IniFormat, this);
#else
    m_settings = new QSettings("left-studio", "mozi", this);
#endif
    m_settings->setIniCodec("UTF-8");

    if(m_settings->contains("Window/WindowSize"))
        this->resize(m_settings->value("Window/WindowSize").toSize());

    if(m_settings->contains("Window/WindowPos"))
        this->move(m_settings->value("Window/WindowPos").toPoint());

    this->m_networkManager->setToken(m_settings->value("Mozi/Token").toString());

    QListWidgetItem *newItem = nullptr;
    int size = m_settings->beginReadArray("Collection");
    for(int i = 0;i < size ;i++)
    {
        m_settings->setArrayIndex(i);

        Poetry poetry;
        poetry.pid = m_settings->value("PID").toString();
        poetry.title = m_settings->value("Title").toString();
        poetry.author = m_settings->value("Author").toString();
        poetry.poetry = m_settings->value("Poetry").toString();
        poetry.dynasty = m_settings->value("Dynasty").toString();

        newItem = new QListWidgetItem("《" + poetry.title + "》", ui->collectionList);
        newItem->setData(Qt::StatusTipRole, QVariant::fromValue<Poetry>(poetry));
    }
    m_settings->endArray();
}

/**
 * @brief 保存设置
 */
void MainWidget::saveSettings()
{
    m_settings->setValue("Window/WindowSize", QVariant(this->size()));
    m_settings->setValue("Window/WindowPos", QVariant(this->pos()));

    m_settings->setValue("Mozi/Token", QVariant(m_networkManager->token()));

    QListWidgetItem *item = nullptr;
    m_settings->beginWriteArray("Collection");
    for(int i = 0;i < ui->collectionList->count();i++)
    {
        m_settings->setArrayIndex(i);
        item = ui->collectionList->item(i);

        const Poetry poetry = item->data(Qt::StatusTipRole).value<Poetry>();
        m_settings->setValue("PID", poetry.pid);
        m_settings->setValue("Title", poetry.title);
        m_settings->setValue("Author", poetry.author);
        m_settings->setValue("Poetry", poetry.poetry);
        m_settings->setValue("Dynasty", poetry.dynasty);
    }
    m_settings->endArray();
}

/**
 * @brief 关于
 */
bool MainWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->aboutLabel)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            if(m_updater->checkUpdate() && m_updater->isNeedUpdate())
                QDesktopServices::openUrl(m_updater->downloadUrl());
            else
            {
                ui->stackedWidget->rotate(1);
                this->setWindowTitle("Mozi - 关于");
            }
        }
    }
    else if(obj == ui->aboutLabel_2)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            ui->stackedWidget->rotate(0);
            this->setWindowTitle("Mozi");
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    this->saveSettings();
    QWidget::closeEvent(event);
}

/**
 * @brief 刷新每日一词
 */
void MainWidget::refesh()
{
    const Poetry result = m_networkManager->dailyPoetry();

    if(result.isEmpty())
    {
        m_toast->toast("数据获取失败 （　ﾟ Дﾟ）");
        return;
    }

    ui->poetryLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt;\">"
    + result.poetry +"</span></p><p align=\"center\"><span style=\" font-size:14pt;\">"
    "——" + result.dynasty + "  " + result.author + "  《" + result.title +"》</span></p></body></html>");
}

/**
 * @brief 查找
 */
void MainWidget::on_searchBtn_clicked()
{
    if(ui->searcbLineEdit->text().isEmpty())
    {
        m_toast->toast("搜索内容不能为空哦 (;-_-)ᴇᴍᴍᴍ");
        return;
    }

    ui->searchProgressBar->setMaximum(0);

    /* 获取数据 */
    const PoetryList resultList = m_networkManager->search(ui->searcbLineEdit->text());
    if(resultList.isEmpty())
    {
        m_toast->toast("什么都没找到 ㄟ( ▔, ▔ )ㄏ");
        ui->searchProgressBar->setMaximum(100);
        return;
    }

    /* 清空列表 */
    ui->resultList->disconnect(this);
    ui->resultList->clear();
    connect(ui->resultList, &QListWidget::currentItemChanged, this,
            &MainWidget::on_resultList_currentItemChanged);

    /* 将结果添加到列表 */
    QListWidgetItem *newItem = nullptr;
    for(const Poetry &result : resultList)
    {
        newItem = new QListWidgetItem(QString("《%1》").arg(result.title), ui->resultList);
        newItem->setData(Qt::StatusTipRole, QVariant::fromValue<Poetry>(result));
    }

    ui->loadMoreBtn->setHidden(!m_networkManager->hasMore());

    ui->searchProgressBar->setMaximum(100);
}

/**
 * @brief 打印查找古诗详情
 */
void MainWidget::on_resultList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    ui->searchProgressBar->setMaximum(0);
    const Poetry result = m_networkManager->detail(
                current->data(Qt::StatusTipRole).value<Poetry>());

    if(result.isEmpty())
    {
        m_toast->toast("数据获取失败 （　ﾟ Дﾟ）");
        ui->searchProgressBar->setMaximum(100);
        return;
    }
    ui->searchProgressBar->setMaximum(100);

    ui->resultTextEdit->printPoetry(result);
}

/**
 * @brief 打印收藏古诗详情
 */
void MainWidget::on_collectionList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    ui->collectionProgressBar->setMaximum(0);
    const Poetry result = m_networkManager->detail(
                current->data(Qt::StatusTipRole).value<Poetry>());

    if(result.isEmpty())
    {
        m_toast->toast("数据获取失败 （　ﾟ Дﾟ）");
        ui->collectionProgressBar->setMaximum(100);
        return;
    }
    ui->collectionProgressBar->setMaximum(100);

    ui->collectionTextEdit->printPoetry(result);
}

/**
 * @brief 收藏
 */
void MainWidget::on_collectionBtn_clicked()
{
    QListWidgetItem *currentItem = ui->resultList->currentItem();
    if(currentItem == nullptr)
        return;

    for(int i = 0;i < ui->collectionList->count();i++)
    {
        if(ui->collectionList->item(i)->text() == currentItem->text())
        {
            m_toast->toast("不要重复收藏哦 (* \"･∀･)ﾉ――◎");
            return;
        }
    }

    QListWidgetItem *newItem = new QListWidgetItem(currentItem->text(), ui->collectionList);
    newItem->setData(Qt::StatusTipRole, currentItem->data(Qt::StatusTipRole));

    m_toast->toast("已收藏 (///ᴗ///)");
}

/**
 * @brief 取消收藏
 */
void MainWidget::on_cancelBtn_clicked()
{
    QListWidgetItem *currentItem = ui->collectionList->currentItem();
    if(currentItem == nullptr)
        return;

    if(ui->collectionList->count() == 1)
    {
        ui->collectionList->disconnect(this);
        delete ui->collectionList->takeItem(ui->collectionList->row(currentItem));
        connect(ui->collectionList, &QListWidget::currentItemChanged,this,
                &MainWidget::on_collectionList_currentItemChanged);
    }
    else
        delete ui->collectionList->takeItem(ui->collectionList->row(currentItem));

    m_toast->toast("收藏已取消 (*￣m￣)");
}

/**
 * @brief Load more result
 */
void MainWidget::on_loadMoreBtn_clicked()
{
    ui->searchProgressBar->setMaximum(0);

    const PoetryList resultList = m_networkManager->loadMore();

    /* 将结果添加到列表 */
    QListWidgetItem *newItem = nullptr;
    for(const Poetry &result : resultList)
    {
        newItem = new QListWidgetItem(QString("《%1》").arg(result.title), ui->resultList);
        newItem->setData(Qt::StatusTipRole, QVariant::fromValue<Poetry>(result));
    }

    ui->loadMoreBtn->setHidden(!m_networkManager->hasMore());

    ui->searchProgressBar->setMaximum(100);
}

