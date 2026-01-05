#pragma once

#include <QByteArray>
#include <QObject>
#include <QString>

class QLocalServer;

class SingleInstance final : public QObject
{
    Q_OBJECT

public:
    explicit SingleInstance(const QString &serverName, QObject *parent = nullptr);
    ~SingleInstance() override;

    bool tryRunAsPrimary(const QByteArray &secondaryMessage = "activate");
    QString lastError() const;

signals:
    void messageReceived(const QByteArray &message);

private:
    bool sendMessage(const QByteArray &message, int timeoutMs);
    bool listenServer();

    QString serverName_;
    QLocalServer *server_ = nullptr;
    QString lastError_;
};
