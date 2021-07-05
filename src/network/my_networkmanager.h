/********************/
/*  By Left Studio  */
/*     @Ho 229      */
/********************/

#ifndef MY_NETWORKMANAGER_H
#define MY_NETWORKMANAGER_H

#include "poetry.h"

#include <QObject>
#include <QUrlQuery>
#include <QNetworkReply>

typedef QList<Poetry> PoetryList;

class QNetworkAccessManager;

class My_NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit My_NetworkManager(QObject *parent = nullptr);

    /**
     * @brief 获取每日一词Token
     * @param 本地记录的Token
     */
    void setToken(const QString &token);
    QString token() const { return m_token; }

    /**
     * @brief 获取每日一词
     * @return Poetry
     */
    const Poetry dailyPoetry();

    /**
     * @brief 查找关键字
     * @return 查找结果集
     */
    const PoetryList search(const QString &keyword);

    /**
     * @brief 获取古诗详情
     * @param pid Baidu Wenxue PID
     * @return Poetry
     */
    const Poetry detail(const Poetry &oldPoetry);

    bool hasMore() const { return m_currentPage < m_totalPages; }

    /**
     * @brief Load more result
     */
    const PoetryList loadMore();

private:
    QString m_token;
    QNetworkAccessManager *m_manager = nullptr;

    QString m_keyword;
    int m_currentPage = 0;
    int m_totalPages = 0;

    inline const QByteArray searchInfo() const;
    inline const PoetryList buildSearchList(const QByteArray &data);
    inline const Poetry buildPoetry(const QByteArray& data, const Poetry& oldPoetry) const;
};

#endif // MY_NETWORKMANAGER_H
