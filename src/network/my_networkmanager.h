#ifndef MY_NETWORKMANAGER_H
#define MY_NETWORKMANAGER_H

#include <QObject>
#include <QUrlQuery>

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
     * @return 每日一词Json
     */
    QJsonObject getData();

    /**
     * @brief 查找关键字
     * @return 查找结果集
     */
    QJsonArray queryWord(const QString &keyword);

    /**
     * @brief 获取古诗详情
     * @param 古诗id
     * @return 古诗详情Json
     */
    QJsonObject getPoetry(const QString &id);

signals:

private:
    QString m_token;
    QNetworkAccessManager *m_manager = nullptr;

    QByteArray getSignature(QUrlQuery query);

};

#endif // MY_NETWORKMANAGER_H
