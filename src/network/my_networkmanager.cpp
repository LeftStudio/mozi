/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "my_networkmanager.h"

#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>

My_NetworkManager::My_NetworkManager(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{
    //qDebug()<<"QSslSocket = " << QSslSocket::sslLibraryBuildVersionString();
}

void My_NetworkManager::setToken(const QString &token)
{
    if(token.isEmpty())
    {
        const QUrl url("https://v2.jinrishici.com/token");

        QNetworkReply *reply = m_manager->get(QNetworkRequest(url));

        QEventLoop loop;
        connect(m_manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
        loop.exec();

        QByteArray data = reply->readAll();
        QJsonObject resultJson = QJsonDocument::fromJson(data).object();

        m_token = resultJson["data"].toString();
    }
    else
        m_token = token;
}

QJsonObject My_NetworkManager::getData()
{
    if(m_token.isEmpty())
        return QJsonObject();

    const QUrl url("https://v2.jinrishici.com/sentence");

    QNetworkRequest request(url);
    request.setRawHeader("X-User-Token",m_token.toUtf8());

    QNetworkReply *reply = m_manager->get(request);

    QEventLoop loop;
    connect(m_manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();

    QByteArray data = reply->readAll();
    QJsonObject resultJson = QJsonDocument::fromJson(data).object();

    return resultJson["data"].toObject();
}

QJsonArray My_NetworkManager::queryWord(const QString &keyword)
{
    QUrl url("http://api.avatardata.cn/TangShiSongCi/Search");

    QUrlQuery query;
    query.addQueryItem("key","dbe213f6b9ff4cc0ad641070b8f61c0c");
    query.addQueryItem("keyWord",keyword);
    query.addQueryItem("rows","50");

    url.setQuery(query);

    QNetworkReply *reply = m_manager->get(QNetworkRequest(url));

    QEventLoop loop;
    connect(m_manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();

    QByteArray data = reply->readAll();
    return QJsonDocument::fromJson(data).object()["result"].toArray();
}

QJsonObject My_NetworkManager::getPoetry(const QString &id)
{
    QUrl url("http://api.avatardata.cn/TangShiSongCi/LookUp");

    QUrlQuery query;
    query.addQueryItem("key","dbe213f6b9ff4cc0ad641070b8f61c0c");
    query.addQueryItem("id",id);

    url.setQuery(query);

    QNetworkReply *reply = m_manager->get(QNetworkRequest(url));

    QEventLoop loop;
    connect(m_manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();

    QByteArray data = reply->readAll();
    return QJsonDocument::fromJson(data).object()["result"].toObject();
}
