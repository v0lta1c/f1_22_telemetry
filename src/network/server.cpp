#include "server.h"
#include <iostream>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include "../constants.h"
#include "../telemetry/FinalRaceData.h"

Server::Server() {
    
    // Register default routes
    addEndpoint("/api/raceData", [this]() {
        nlohmann::json raceData = FinalRaceData::instance().getRaceData();

        return raceData;
    });
}

Server::~Server() {

    if (tcpServer_) {
        tcpServer_->close();
        delete tcpServer_;
    }
}

void Server::start() {

    std::cout << "Starting server...." << std::endl;

    // Create a TCP server
    tcpServer_ = new QTcpServer();        

    // Listen on port 8080
    if(!tcpServer_->listen(QHostAddress::Any, API_PORT)) {
        std::cerr << "Error starting server: " << tcpServer_->errorString().toStdString() << std::endl;
        return;
    }

    std::cout << "Server started, listening on port 8080.... " << std::endl;

    // Connect the newConnetion signal to a slot to handle incoming connections
    connect(tcpServer_, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

void Server::stop() {

    if(tcpServer_) {
        tcpServer_->close();    // Close da server socket
    }
}

void Server::addEndpoint(const QString& route, RouteHandler handler) {
    endpoints_[route] = handler;    
}

void Server::onNewConnection() {
    QTcpSocket* socket = tcpServer_->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
}

void Server::onReadyRead() {
        
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    // Read the incoming request
    QByteArray request = socket->readAll();

    // Extract the route from the request
    QString route = parseRoute(request);

    // Handle the request
    nlohmann::json response = handleRequest(route);

    // Send the response back to the client
    QByteArray responseData = QString::fromStdString(response.dump()).toUtf8(); // Convert JSON to string and then to ByteArray
    socket->write("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + QByteArray::number(responseData.size()) + "\r\n\r\n" + responseData);
    socket->flush();
    socket->close();
}

nlohmann::json Server::handleRequest(const QString& route) {

    if (endpoints_.find(route) != endpoints_.end()) {

        return endpoints_[route]();
    }
    else {
        return {{"error", "Route not found"}};
    }
}

QString Server::parseRoute(const QByteArray& request) {

    // Basic parsing: assuming requrest format is "GET /route HTTP/1.1"
    QStringList lines = QString(request).split("\r\n");
    if(!lines.isEmpty()) {
        QString firstLine = lines[0];
        QStringList parts = firstLine.split(" ");
        if(parts.size() >= 2) {
            return parts[1];    // Return the route (second part of the request)
        }
    }
    return "";  // Return empty if parsing fails
}
