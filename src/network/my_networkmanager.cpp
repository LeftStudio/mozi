/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#include "my_networkmanager.h"

#include <QUrl>
#include <QPointer>
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
    qDebug() << "OpenSSL Version:" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL Support:" << QSslSocket::supportsSsl();
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

        reply->deleteLater();
    }
    else
        m_token = token;
}

const Poetry My_NetworkManager::dailyPoetry()
{
    if(m_token.isEmpty())
        return {};

    const QUrl url("https://v2.jinrishici.com/sentence");

    QNetworkRequest request(url);
    request.setRawHeader("X-User-Token",m_token.toUtf8());

    QNetworkReply *reply = m_manager->get(request);

    QEventLoop loop;
    connect(m_manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();

    const QJsonObject resultJson = QJsonDocument::fromJson(
                reply->readAll()).object()["data"].toObject();
    const QJsonObject origin(resultJson["origin"].toObject());

    reply->deleteLater();

    Poetry result;
    result.title = origin["title"].toString();
    result.dynasty = origin["dynasty"].toString();
    result.author = origin["author"].toString();
    result.poetry = resultJson["content"].toString();

    return result;
}

const PoetryList My_NetworkManager::search(const QString &keyword)
{
    m_keyword = keyword;
    m_currentPage = 1;

    return this->buildSearchList(this->searchInfo());
}

const Poetry My_NetworkManager::detail(const Poetry &oldPoetry)
{
    QUrl url("https://hanyu.baidu.com/shici/detail");

    QUrlQuery query;
    query.addQueryItem("pid", oldPoetry.pid);

    url.setQuery(query);

    QPointer<QNetworkReply> reply = m_manager->get(QNetworkRequest(url));

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    return this->buildPoetry(reply->readAll(), oldPoetry);
}

const PoetryList My_NetworkManager::loadMore()
{
    if(!this->hasMore())
        return {};

    ++m_currentPage;

    return this->buildSearchList(this->searchInfo());
}

const QByteArray My_NetworkManager::searchInfo() const
{
    QUrl url("https://hanyu.baidu.com/hanyu/ajax/search_list");

    QUrlQuery query;
    query.addQueryItem("wd", m_keyword);
    query.addQueryItem("from", "poem");
    query.addQueryItem("pn", QString::number(m_currentPage));
    query.addQueryItem("_", QString::number(QDateTime::currentMSecsSinceEpoch()));

    url.setQuery(query);

    QPointer<QNetworkReply> reply = m_manager->get(QNetworkRequest(url));

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    return reply->readAll();
}

const PoetryList My_NetworkManager::buildSearchList(const QByteArray &data)
{
    PoetryList list;

    const QJsonObject root(QJsonDocument::fromJson(data).object());
    QJsonArray poemArray;

    if(root["ret_type"].toString() == "author")
    {
        QJsonObject poems(QJsonDocument::fromJson(data).object()["ret_array"]
                              .toArray().first().toObject()["poems"].toObject());

        m_totalPages = poems["extra"].toObject()["total-page"].toInt();

        poemArray = poems["ret_array"].toArray();
    }
    else
    {
        m_totalPages = root["extra"].toObject()["total-page"].toInt();

        poemArray = root["ret_array"].toArray();
    }

    const QJsonArray& constlist = poemArray;
    for(const QJsonValue &poem : constlist)
    {
        QString author;
        if((author = poem["literature_author"].toArray().first().toString()).isEmpty())
            author = poem["literature_author"].toArray().first().toObject()["basic_name"]
                         .toArray().first().toString();

        Poetry poetry;
        poetry.title = poem["display_name"].toArray().first().toString();
        poetry.dynasty = poem["dynasty"].toArray().first().toString();
        poetry.author = author;
        poetry.poetry = poem["body"].toArray().first().toString();
        poetry.pid = poem["sid"].toArray().first().toString();

        if(poetry.isEmpty() || poetry.pid.isEmpty())
            continue;

        list.push_back(poetry);
    }

    return list;
}

const Poetry My_NetworkManager::buildPoetry(const QByteArray &data,
                                            const Poetry &oldPoetry) const
{
    QRegularExpression transReg("means-fold\">(?<trans>(.*?))<");
    const QString translate = transReg.match(data).captured("trans");

    QRegularExpression descrReg("poem-detail-item-content\">\\s+(?<info>(.*))\\s");
    QRegularExpressionMatchIterator it = descrReg.globalMatch(data);

    QString notes, description;
    for(int i = 0; it.hasNext() && i <= 2; i++)
    {
        if(i == 1)          // Notes
        {
            notes = it.next().captured("info");

            if(notes.left(4) == "<div") // Ignore "<div class="poem-author-container">"
            {
                notes.clear();
                continue;
            }

            notes = notes.mid(3, notes.size() - 7)      // Ignore "<p>...</p>"
                        .replace("</br>", "<br>");      // To rich text wraps format
        }
        else if(i == 2)     // Description
            description = it.next().captured("info").replace("</br>", "<br>");
        else
            it.next();
    }

    Poetry poetry = oldPoetry;
    poetry.notes = notes;
    poetry.translate = translate;
    poetry.description = description;

    return poetry;
}
