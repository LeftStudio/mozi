#ifndef MY_NETWORKMANAGER_H
#define MY_NETWORKMANAGER_H

#include <QObject>

class QNetworkAccessManager;

class My_NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit My_NetworkManager(QObject *parent = nullptr);

    void setToken(const QString &token);
    QString token() const { return m_token; }

    QJsonObject getData();                          // 获取每日一词

    QJsonObject queryWord(const QString &keyword);  // 查询

    QJsonObject getPoetry(const QString &id);       // 获取诗词详情

signals:

private:
    QString m_token;
    QNetworkAccessManager *m_manager = nullptr;

};

#endif // MY_NETWORKMANAGER_H
