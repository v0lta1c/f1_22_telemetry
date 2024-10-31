#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QCheckBox>
#include <QThread>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <qwindowdefs.h>
#include "../telemetry/telemetry.h"
#include "../network/server.h"
#include "../network/discord_notifier.h"

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleTelemetry();   // Slot for toggling telemetry on/off

signals:
    void startServer();
    void stopServer();

    void sendDiscordNotification(const std::string &eventType, const nlohmann::json &eventData);

private:
    QPushButton *toggleButton;  // Button to toggle telemetry
    QCheckBox* startServerCheckBox; // Checkbox to know whether server enabled or not
    TelemetryReceiver *telemetryReceiver;
    bool isRunning; 
    Server* server;
    QThread* serverThread;
    bool isServerStarted; // bool tracking server

    DiscordNotifier* discordNotifier;
    QThread* notifierThread;
};
