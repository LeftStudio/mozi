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
#include <QCloseEvent>
#include <QJsonObject>
#include <QGraphicsDropShadowEffect>

#ifdef Q_OS_WIN
# if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
# endif
#endif

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    m_timer(new QTimer(this)),
    m_networkManager(new My_NetworkManager(this)),
    m_shadow_1(new QGraphicsDropShadowEffect(this)),
    m_shadow_2(new QGraphicsDropShadowEffect(this)),
    m_shadow_3(new QGraphicsDropShadowEffect(this)),
    m_shadow_4(new QGraphicsDropShadowEffect(this)),
    m_shadow_5(new QGraphicsDropShadowEffect(this)),
    m_shadow_6(new QGraphicsDropShadowEffect(this))
{
    ui->setupUi(this);
    this->initUI();
    this->loadSettings();
    this->initSignalSlots();

    ui->aboutLabel->installEventFilter(this);
    ui->aboutLabel_2->installEventFilter(this);

    this->refesh();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initUI()
{
    /* 按钮阴影 */
    m_shadow_1->setColor(QColor(200,200,200,180));
    m_shadow_1->setBlurRadius(6);
    m_shadow_1->setOffset(4,4);

    m_shadow_2->setColor(QColor(200,200,200,180));
    m_shadow_2->setBlurRadius(6);
    m_shadow_2->setOffset(4,4);

    m_shadow_3->setColor(QColor(200,200,200,180));
    m_shadow_3->setBlurRadius(6);
    m_shadow_3->setOffset(4,4);

    m_shadow_4->setColor(QColor(200,200,200,180));
    m_shadow_4->setBlurRadius(6);
    m_shadow_4->setOffset(4,4);

    m_shadow_5->setColor(QColor(200,200,200,180));
    m_shadow_5->setBlurRadius(6);
    m_shadow_5->setOffset(4,4);

    m_shadow_6->setColor(QColor(200,200,200,180));
    m_shadow_6->setBlurRadius(6);
    m_shadow_6->setOffset(4,4);

    ui->searcbLineEdit->setGraphicsEffect(m_shadow_1);
    ui->searchBtn->setGraphicsEffect(m_shadow_2);
    ui->collectionBtn->setGraphicsEffect(m_shadow_3);
    ui->returnBtn->setGraphicsEffect(m_shadow_4);
    ui->returnBtn_2->setGraphicsEffect(m_shadow_5);
    ui->cancelBtn->setGraphicsEffect(m_shadow_6);

    QFont font = ui->resultList->font();
    font.setPointSize(14);
    ui->resultList->setFont(font);
    ui->collectionList->setFont(font);
}

void MainWidget::initSignalSlots()
{
    connect(ui->refreshBtn,&QPushButton::clicked,
            [this]{
        ui->refreshBtn->setEnabled(false);
        m_timer->start(1000);
        this->refesh();
    });
    connect(m_timer,&QTimer::timeout,
            [this]{
        ui->refreshBtn->setEnabled(true);
    });
    connect(ui->searchPageBtn,&QPushButton::clicked,
            [this]{
        ui->stackedWidget->rotate(3);
        this->setWindowTitle("Mozi - 搜索");
    });
    connect(ui->collectionPageBtn,&QPushButton::clicked,
            [this]{
        ui->stackedWidget->rotate(2);
        this->setWindowTitle("Mozi - 收藏");
    });
    connect(ui->returnBtn,&QPushButton::clicked,
            [this]{
        ui->stackedWidget->rotate(0);
        this->setWindowTitle("Mozi");
    });
    connect(ui->returnBtn_2,&QPushButton::clicked,
            [this]{
        ui->stackedWidget->rotate(0);
        this->setWindowTitle("Mozi");
    });
}

void MainWidget::refesh()
{
    QJsonObject data = m_networkManager->getData();

    if(data.isEmpty())
    {
        ui->poetryLabel->setText("数据获取失败");
        return;
    }

    QJsonObject origin = data["origin"].toObject();
    ui->poetryLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:24pt;\">"
    +data["content"].toString()+"</span></p><p align=\"center\"><span style=\" font-size:14pt;\">"                                      // 诗句
    "——"+origin["dynasty"].toString()+"  "+origin["author"].toString()+"  《"+origin["title"].toString()+"》</span></p></body></html>"); // ————[朝代] [诗人] 《[标题]》
}

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
        newItem = new QListWidgetItem(m_settings->value("Title").toString(),
                                      ui->collectionList);
        newItem->setData(Qt::StatusTipRole, m_settings->value("ID"));
    }
    m_settings->endArray();
}

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
        m_settings->setValue("Title", item->text());
        m_settings->setValue("ID", item->data(Qt::StatusTipRole));
    }
    m_settings->endArray();
}

bool MainWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->aboutLabel)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            ui->stackedWidget->rotate(1);
            this->setWindowTitle("Mozi - 关于");
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

void MainWidget::on_searchBtn_clicked()
{
    if(ui->searcbLineEdit->text().isEmpty())
        return;

    /* 清空列表 */
    ui->resultList->disconnect(this);
    ui->resultList->clear();
    connect(ui->resultList,&QListWidget::currentItemChanged,
            this,&MainWidget::on_resultList_currentItemChanged);

    /* 获取数据 */
    QJsonObject resultObj = m_networkManager->queryWord(ui->searcbLineEdit->text());
    QJsonArray resultArray;

    if(resultObj["reason"].toString() == "Succes")
        resultArray = resultObj["result"].toArray();

    /* 将结果添加到列表 */
    QListWidgetItem *newItem = nullptr;
    for(QJsonValue result : resultArray)
    {
        newItem = new QListWidgetItem(QString("《%1》").arg(result["name"].toString()), ui->resultList);
        newItem->setData(Qt::StatusTipRole, result["id"]);
    }
}

void MainWidget::on_resultList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    ui->resultTextEdit->clear();

    QJsonObject resultObj;
    QJsonObject reasonObj = m_networkManager->getPoetry(current->data(Qt::StatusTipRole).toString());
    if(reasonObj["reason"].toString().contains("Succes"))
        resultObj = reasonObj["result"].toObject();
    else
    {
        ui->resultTextEdit->setText("数据获取失败");
        return;
    }

    QTextBlockFormat blockFmt;
    blockFmt.setLeftMargin(50);                                 // 间隔
    ui->resultTextEdit->textCursor().setBlockFormat(blockFmt);

    QTextCharFormat fmt = ui->resultTextEdit->currentCharFormat();

    /* 打印标题 */
    fmt.setFontPointSize(22);
    fmt.setFontWeight(QFont::Bold);
    ui->resultTextEdit->setCurrentCharFormat(fmt);
    ui->resultTextEdit->append(resultObj["biaoti"].toString());

    /* 打印作者 */
    fmt.setFontPointSize(18);
    fmt.setFontWeight(QFont::Normal);
    ui->resultTextEdit->setCurrentCharFormat(fmt);
    ui->resultTextEdit->append(resultObj["zuozhe"].toString() + "\n");

    /* 打印内容 */
    fmt.setFontPointSize(16);
    ui->resultTextEdit->setCurrentCharFormat(fmt);
    ui->resultTextEdit->append(resultObj["neirong"].toString() + "\n");

    /* 打印介绍 */
    fmt.setFontPointSize(12);
    ui->resultTextEdit->setCurrentCharFormat(fmt);
    ui->resultTextEdit->append(resultObj["jieshao"].toString());

    ui->resultTextEdit->moveCursor(QTextCursor::Start);
}

void MainWidget::on_collectionList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    ui->collectionTextEdit->clear();

    QJsonObject resultObj;
    QJsonObject reasonObj = m_networkManager->getPoetry(current->data(Qt::StatusTipRole).toString());
    if(reasonObj["reason"].toString().contains("Succes"))
        resultObj = reasonObj["result"].toObject();
    else
    {
        ui->collectionTextEdit->setText("数据获取失败");
        return;
    }

    QTextBlockFormat blockFmt;
    blockFmt.setLeftMargin(50);                                 // 间隔
    ui->collectionTextEdit->textCursor().setBlockFormat(blockFmt);

    QTextCharFormat fmt = ui->collectionTextEdit->currentCharFormat();

    /* 打印标题 */
    fmt.setFontPointSize(22);
    fmt.setFontWeight(QFont::Bold);
    ui->collectionTextEdit->setCurrentCharFormat(fmt);
    ui->collectionTextEdit->append(resultObj["biaoti"].toString());

    /* 打印作者 */
    fmt.setFontPointSize(18);
    fmt.setFontWeight(QFont::Normal);
    ui->collectionTextEdit->setCurrentCharFormat(fmt);
    ui->collectionTextEdit->append(resultObj["zuozhe"].toString() + "\n");

    /* 打印内容 */
    fmt.setFontPointSize(16);
    ui->collectionTextEdit->setCurrentCharFormat(fmt);
    ui->collectionTextEdit->append(resultObj["neirong"].toString() + "\n");

    /* 打印介绍 */
    fmt.setFontPointSize(12);
    ui->collectionTextEdit->setCurrentCharFormat(fmt);
    ui->collectionTextEdit->append(resultObj["jieshao"].toString());

    ui->collectionTextEdit->moveCursor(QTextCursor::Start);
}

void MainWidget::on_collectionBtn_clicked()
{
    QListWidgetItem *currentItem = ui->resultList->currentItem();
    if(currentItem == nullptr)
        return;

    for(int i = 0;i < ui->collectionList->count();i++)
    {
        if(ui->collectionList->item(i)->text() == currentItem->text())
            return;
    }

    QListWidgetItem *newItem = new QListWidgetItem(currentItem->text(), ui->collectionList);
    newItem->setData(Qt::StatusTipRole, currentItem->data(Qt::StatusTipRole));
}

void MainWidget::on_cancelBtn_clicked()
{
    QListWidgetItem *currentItem = ui->collectionList->currentItem();
    if(currentItem == nullptr)
        return;

    delete ui->collectionList->takeItem(ui->collectionList->row(currentItem));
}
