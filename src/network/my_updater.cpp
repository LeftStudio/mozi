/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "my_updater.h"

#include <QUrl>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>

My_Updater::My_Updater(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{

}

void My_Updater::checkUpdate()
{
    const QUrl url("https://gitee.com/api/v5/repos/left-studio/mozi/releases/latest");

    QNetworkReply *reply = m_manager->get(QNetworkRequest(url));

    QEventLoop loop;
    connect(m_manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();

    QByteArray data = reply->readAll();
    QJsonObject resultJson = QJsonDocument::fromJson(data).object();

    m_latestVersion = resultJson["tag_name"].toString();
    m_downloadUrl = resultJson["assets"].toArray()[0].toObject()["browser_download_url"].toString();
}
