#include "MainWindow.h"
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent), 
      server(new Server()),
      discordNotifier(new DiscordNotifier()),
      serverThread(new QThread(this)), 
      notifierThread(new QThread(this)),
      isServerStarted(false), 
      isRunning(false) {
    
    // Initialize the TelemetryReceiver but do not start it yet
    telemetryReceiver = new TelemetryReceiver(this);

    // Create toggle button
    toggleButton = new QPushButton("Start telemetry", this);
    connect(toggleButton, &QPushButton::clicked, this, &MainWindow::toggleTelemetry);

    startServerCheckBox = new QCheckBox("Start Server?", this);

    // Set up a simple layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(toggleButton);
    layout->addWidget(startServerCheckBox);
    setCentralWidget(centralWidget);

    server->moveToThread(serverThread);
    discordNotifier->moveToThread(notifierThread);

    connect(this, &MainWindow::sendDiscordNotification, discordNotifier, &DiscordNotifier::sendNotification);

    connect(this, &MainWindow::startServer, server, &Server::start);
    connect(this, &MainWindow::stopServer, server, &Server::stop);

    connect(serverThread, &QThread::finished, server, &QObject::deleteLater);
    connect(notifierThread, &QThread::finished, discordNotifier, &QObject::deleteLater);

    serverThread->start();
    notifierThread->start();
}

MainWindow::~MainWindow() {

    serverThread->quit();
    serverThread->wait();

    notifierThread->quit();
    notifierThread->wait();

    delete telemetryReceiver;
    delete server;
    delete serverThread;
    delete discordNotifier;
    delete notifierThread;
}

void MainWindow::toggleTelemetry() {

    if(isRunning) {
        telemetryReceiver->stopListening();
        toggleButton->setText("Start Telemetry");

        if(isServerStarted) {
            server->stop();
            isServerStarted = false;
            discordNotifier->disconnect();
            telemetryReceiver->setServerStarted(isServerStarted);
        }
        startServerCheckBox->setVisible(true);

        isRunning = !isRunning;
    }
    else {
        telemetryReceiver->startListening();
        toggleButton->setText("Stop Telemetry");

        telemetryWindow = new TelemetryWindow();
        connect(telemetryWindow, &TelemetryWindow::captureStopped, this, &MainWindow::onCaptureStopped);
        //telemetryWindow->setGeometry(100, 100, 800, 600);
        telemetryWindow->raise();
        telemetryWindow->activateWindow();
        telemetryWindow->show();
        telemetryWindow->setVisible(true);
        this->hide();

        telemetryReceiver->startTelemetryUpdates();
        connect(telemetryReceiver, &TelemetryReceiver::telemetryDataUpdated, telemetryWindow, &TelemetryWindow::updateTelemetryWindow);

        if(!isServerStarted && startServerCheckBox->isChecked()) {
            server->start();
            isServerStarted = true;
            //discordNotifier->sendNotification("Server Started", nlohmann::json({{"debug", "The server has started and this message was sent using CPP!"}}).dump());
            telemetryReceiver->setServerStarted(isServerStarted);
        }
        startServerCheckBox->setVisible(false);
        
        isRunning = !isRunning;
    }
}

void MainWindow::onCaptureStopped() {

    if (telemetryWindow) {
        telemetryWindow->deleteLater();
        telemetryWindow = nullptr;
    }
    if (isServerStarted) {
        server->stop();
        isServerStarted = true;
        discordNotifier->disconnect();
        telemetryReceiver->setServerStarted(isServerStarted);
    }
    telemetryReceiver->stopListening();
    toggleButton->setText("Start Telemetry");
    telemetryReceiver->stopTelemetryUpdates();
    startServerCheckBox->setVisible(true);
    isRunning = !isRunning;
    this->show();
}
