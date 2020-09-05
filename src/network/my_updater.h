#ifndef MY_UPDATER_H
#define MY_UPDATER_H

#include <QObject>
#include <QApplication>

class QNetworkAccessManager;

class My_Updater : public QObject
{
    Q_OBJECT
public:
    explicit My_Updater(QObject *parent = nullptr);

    bool checkUpdate();

    bool isNeedUpdate(){ return m_latestVersion != QApplication::applicationVersion(); }

    QString latestVersion(){ return m_latestVersion; }
    QString downloadUrl(){ return m_downloadUrl; }

signals:

private:
    QNetworkAccessManager *m_manager = nullptr;

    QString m_latestVersion;
    QString m_downloadUrl;
};

#endif // MY_UPDATER_H
