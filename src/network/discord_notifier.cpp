#include "discord_notifier.h"
#include <QHostAddress>
#include <qdebug.h>
#include <qoverload.h>
#include <iostream>

#include "../constants.h"

DiscordNotifier::DiscordNotifier(QObject *parent)
    :   QObject(parent), socket_(new QTcpSocket()) {

    //connect(socket_, &QTcpSocket::connected, this, &DiscordNotifier::onConnected);
    //connect(socket_, &QTcpSocket::bytesWritten, this, &DiscordNotifier::onBytesWritten);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &DiscordNotifier::onErrorOccured);
}

DiscordNotifier::~DiscordNotifier() {

    if(socket_) {

        socket_->close();
        delete socket_;
    }
}

void DiscordNotifier::sendNotification(const std::string &eventType, const std::string &eventData) {

    if (socket_->state() == QAbstractSocket::ConnectedState ||
        socket_->state() == QAbstractSocket::ConnectingState) {
        return;
    }

    if(eventType == "raceData") {

        nlohmann::json raceData = nlohmann::json::parse(eventData);

        nlohmann::json payload;
        std::string raceDataString = raceData.dump();
        payload["raceData"] = raceDataString;
        sendToDiscord(payload.dump());
    }
    else {
        sendToDiscord(eventData);
    }

    connect(socket_, &QTcpSocket::errorOccurred, this, &DiscordNotifier::onErrorOccured);
}

void DiscordNotifier::sendToDiscord(const std::string& payload) {

    // Create the HTTP POST request 
    QString host = QString::fromStdString(Webhook_URL);
    QString request = QString("POST / HTTP/1.1\r\n") + 
                      "Host: " + host + ":" + QString::number(Webhook_PORT) + "\r\n" +
                      "Content-Type: application/json\r\n" +
                      "Content-Length: " + QString::number(payload.size()) + "\r\n" +
                      "Connection: close\r\n\r\n" +
                      QString::fromStdString(payload);

    socket_->connectToHost(host, Webhook_PORT);

    // Connect to discord's webhook server
    if (socket_->waitForConnected()) {
        socket_->write(request.toUtf8());
        socket_->flush();

        // Wait for the server to respond
        if (socket_->waitForReadyRead()) {

            // Read the response
            while(socket_->bytesAvailable()) {
                QByteArray response = socket_->readAll();
                //qDebug() << response;
            }
        }

        socket_->disconnectFromHost();
    }
    else {
        qDebug() << "Connection failed: " << socket_->errorString();
    }
}

void DiscordNotifier::onConnected() {

    //
}

void DiscordNotifier::onBytesWritten(qint64 bytes) {
    //
}

void DiscordNotifier::onErrorOccured(QAbstractSocket::SocketError socketError) {
    //std::cerr << "Socket error: " << socketError << " - " << socket_->errorString().toStdString() << std::endl;
}
