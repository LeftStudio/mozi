#ifndef MY_UPDATER_H
#define MY_UPDATER_H

#define MOZI_CURRENT_VERSION "1.0.0.5"

#include <QObject>

class QNetworkAccessManager;

class My_Updater : public QObject
{
    Q_OBJECT
public:
    explicit My_Updater(QObject *parent = nullptr);

    void checkUpdate();

    bool isNeedUpdate(){ return m_latestVersion != MOZI_CURRENT_VERSION; }

    QString latestVersion(){ return m_latestVersion; }
    QString downloadUrl(){ return m_downloadUrl; }

signals:

private:
    QNetworkAccessManager *m_manager = nullptr;

    QString m_latestVersion;
    QString m_downloadUrl;
};

#endif // MY_UPDATER_H
