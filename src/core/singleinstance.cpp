#include "singleinstance.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>
#include <QVariant>

SingleInstance::SingleInstance(const QString &serverName, QObject *parent) : QObject(parent), serverName_(serverName)
{
    server_ = new QLocalServer(this);
}

SingleInstance::~SingleInstance() = default;

QString SingleInstance::lastError() const
{
    return lastError_;
}

bool SingleInstance::sendMessage(const QByteArray &message, int timeoutMs)
{
    QLocalSocket socket;
    socket.connectToServer(serverName_);
    if (!socket.waitForConnected(timeoutMs))
        return false;

    socket.write(message);
    if (!socket.waitForBytesWritten(timeoutMs))
        return false;

    socket.disconnectFromServer();
    return true;
}

bool SingleInstance::listenServer()
{
    if (server_->isListening())
        return true;

    if (server_->listen(serverName_))
        return true;

    QLocalServer::removeServer(serverName_);
    if (server_->listen(serverName_))
        return true;

    lastError_ = server_->errorString();
    return false;
}

bool SingleInstance::tryRunAsPrimary(const QByteArray &secondaryMessage)
{
    lastError_.clear();

    if (sendMessage(secondaryMessage, 150))
        return false;

    if (!listenServer())
        return false;

    connect(server_, &QLocalServer::newConnection, this, [this]() {
        while (server_->hasPendingConnections()) {
            auto *socket = server_->nextPendingConnection();
            if (!socket)
                return;

            auto handleOnce = [this, socket]() {
                if (socket->property("_toolbox_handled").toBool())
                    return;
                socket->setProperty("_toolbox_handled", true);
                emit messageReceived(socket->readAll());
                socket->disconnectFromServer();
            };

            connect(socket, &QLocalSocket::readyRead, this, handleOnce);
            QTimer::singleShot(150, socket, handleOnce);
            connect(socket, &QLocalSocket::disconnected, socket, &QObject::deleteLater);
        }
    });

    return true;
}
