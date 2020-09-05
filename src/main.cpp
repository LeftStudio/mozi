/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "mainwidget.h"

#include <QIcon>
#include <QTextCodec>
#include <QTranslator>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(
                QTextCodec::codecForName("UTF-8"));

    QTranslator qtGloble;
    qtGloble.load(":/tr/tr/qtbase_zh_CN.qm");
    a.installTranslator(&qtGloble);

    a.setWindowIcon(QIcon(":/images/images/Mozi.ico"));
    a.setApplicationVersion("1.0.0.7");

#ifdef Q_OS_WIN
    a.setFont(QFont("Microsoft YaHei",9));
#endif

    MainWidget w;
    w.show();
    return a.exec();
}
