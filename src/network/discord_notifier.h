#pragma once

#include <QObject>
#include <QTcpSocket>
#include <nlohmann/json.hpp>
#include <qtcpsocket.h>
#include <qtmetamacros.h>

class DiscordNotifier : public QObject {
    Q_OBJECT

public:
    DiscordNotifier(QObject *parent = nullptr);
    ~DiscordNotifier();
    void sendNotification(const std::string &eventType, const std::string &eventData);
    void sendToDiscord(const std::string& payload);

private slots:
    void onConnected();
    void onBytesWritten(qint64 bytes);
    void onErrorOccured(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket_;
    std::string payload_;
};
