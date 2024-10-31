#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <qobject.h>
#include <qtcpserver.h>
#include <qtmetamacros.h>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>

class Server : public QObject {
    Q_OBJECT

public:
    using RouteHandler = std::function<nlohmann::json()>;

    Server();
    ~Server();

    void start();
    void stop();
    void addEndpoint(const QString& route, RouteHandler handler);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer* tcpServer_;
    std::unordered_map<QString, RouteHandler> endpoints_;
    nlohmann::json handleRequest(const QString& route);
    QString parseRoute(const QByteArray& request);
};
