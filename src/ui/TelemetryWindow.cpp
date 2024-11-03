#include "TelemetryWindow.h"
#include "../constants.h"
#include "../utils.h"
#include <QFrame>
#include <QPixmap>
#include <QHeaderView>
#include <nlohmann/detail/meta/type_traits.hpp>
#include <qboxlayout.h>
#include <qgridlayout.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qscrollarea.h>
#include <qtablewidget.h>
#include <qwidget.h>
#include <QDir>
#include <QCoreApplication>

TelemetryWindow::TelemetryWindow(QWidget *parent) : QWidget(parent) {

    mainLayout = new QVBoxLayout(this);

    setWindowTitle("Telemetry Data");
    resize(1280, 760);

    // Track and session information
    trackLabel = new QLabel(this);
    lapLabel = new QLabel(this);

    QHBoxLayout *infoLayout = new QHBoxLayout;
    infoLayout->addWidget(trackLabel, 0, Qt::AlignCenter);
    infoLayout->addWidget(lapLabel, 0, Qt::AlignRight);

    mainLayout->addLayout(infoLayout);

    // Table for driver data n stuff
    // Contains black magic trickery, i would not mess with it if i were you =)

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scoreBoardTable = new QTableWidget(this);
    scoreBoardTable->setColumnCount(9);
    // Add the headers
    scoreBoardTable->setHorizontalHeaderLabels({"Position", "Constructor", "Driver", "Interval", "Last Lap", "Best Lap","Tyre", "Pit Stops", "Penalties"});
    scoreBoardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    scoreBoardTable->verticalHeader()->setVisible(false);
    scoreBoardTable->setShowGrid(false);
    scoreBoardTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scrollArea->setWidget(scoreBoardTable);

    mainLayout->addWidget(scrollArea);
    
    // Add the track conditions
    conditionsLabel = new QLabel("Conditions", this);
    mainLayout->addWidget(conditionsLabel, 0, Qt::AlignCenter);

    // Buttons at the very bottom to display more or klose
    additionalInfoButton = new QPushButton("Display Additional Info", this);
    stopCaptureButton = new QPushButton("Stop Capture", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(additionalInfoButton);
    buttonLayout->addWidget(stopCaptureButton);

    mainLayout->addLayout(buttonLayout);

    // Connect Stop Capture button to the handleStopCapture slot
    connect(stopCaptureButton, &QPushButton::clicked, this, &TelemetryWindow::onStopCapture);
    connect(additionalInfoButton,&QPushButton::clicked, this, &TelemetryWindow::onDisplayAdditionalInfo);

    // Set up context menu
    contextMenu = new QMenu(this);
    QAction *lapHistoryAction = new QAction("Lap History", this);
    QAction *carInfoAction = new QAction("Car History", this);
    contextMenu->addAction(lapHistoryAction);
    contextMenu->addAction(carInfoAction);

    // Set up the context menu for this row
    scoreBoardTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(scoreBoardTable, &QTableWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        int row = scoreBoardTable->rowAt(pos.y());
        if (row != -1 ) {
            showDriverContextMenu(row, pos);
        }
    });

    // Connect the menu actions to the respective slots
    connect(lapHistoryAction, &QAction::triggered, this, &TelemetryWindow::showLapHistory);
    connect(carInfoAction, &QAction::triggered, this, &TelemetryWindow::showCarInfoHistory);

}

// Gets triggered when the Stop Capture button is hit
// MAYDAY MAYDAY WE'RE GOING DOWNNNN
void TelemetryWindow::onStopCapture() {
    
    emit captureStopped();
}

// Context menu handling
void TelemetryWindow::showDriverContextMenu(int driverIndex, const QPoint &pos) {

    contextMenu->exec(scoreBoardTable->viewport()->mapToGlobal(pos));
}

void TelemetryWindow::showLapHistory() {

}

void TelemetryWindow::showCarInfoHistory() {

}

void TelemetryWindow::onDisplayAdditionalInfo() {

}

// Populate the UI with the driver data
void TelemetryWindow::updateTelemetryWindow(const std::vector<DriverData>& driverData, const PacketSessionData& sessionData) {

    QTableWidget *scoreBoardTable = qobject_cast<QTableWidget*>(scrollArea->widget());
    if(!scoreBoardTable) return;

    // Clear existing rows to prevent a duplication catastrophe (probably wont happen though)
    scoreBoardTable->setRowCount(0);

    // Filter active drivers
    std::vector<DriverData> activeDrivers;
    for(const auto& driver: driverData) {
        if(!driver.driverName.empty()) {
            activeDrivers.push_back(driver);
        }
    }

    // Sort the driver data by position
    std::sort(activeDrivers.begin(), activeDrivers.end(), [](const DriverData& a, const DriverData& b) {
        return a.carPosition < b.carPosition;
    });

    // Update track and session labels
    trackLabel->setText(QString::fromStdString(getTrackName(sessionData.m_trackId)) + " - " + QString::fromStdString((getSessionType(sessionData.m_sessionType))));
    lapLabel->setText(QString("Lap %1/%2").arg(activeDrivers[0].currentLapNum).arg(sessionData.m_totalLaps));

    // Populate das table with driver data

    for (int i = 0; i < activeDrivers.size(); i++) {

        const auto& driver = activeDrivers[i];
        scoreBoardTable->insertRow(i);

        // Positions
        QTableWidgetItem *positionItem = new QTableWidgetItem(QString::number(driver.carPosition));
        positionItem->setTextAlignment(Qt::AlignCenter);
        scoreBoardTable->setItem(i, 0, positionItem);

        // Constructors

        QLabel *constructorLabel = new QLabel;
        QString logoPathString = QString::fromStdString(getConstructorLogoPath(driver.constructor));
        QString logoPath = QDir(QCoreApplication::applicationDirPath()).filePath(logoPathString);

        if(!logoPath.isEmpty()) {
            QPixmap pixmap(logoPath);
            if (!pixmap.isNull()) {
                constructorLabel->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            else {
                qDebug() << "Failed to load pixmap from " << logoPath;
                constructorLabel->setText(QString::fromStdString(getTeamName(driver.constructor)));
                constructorLabel->setAlignment(Qt::AlignCenter);
            }
        }
        else {
            constructorLabel->setText(QString::fromStdString(getTeamName(driver.constructor)));
            constructorLabel->setAlignment(Qt::AlignCenter);
        }

        constructorLabel->setAlignment(Qt::AlignCenter);
        scoreBoardTable->setCellWidget(i, 1, constructorLabel);

        // Name
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(driver.driverName));
        //nameItem->setTextAlignment(Qt::AlignCenter);
        scoreBoardTable->setItem(i, 2, nameItem);

        // TODO Interval
        QTableWidgetItem *intervalItem = new QTableWidgetItem("-");
        intervalItem->setTextAlignment(Qt::AlignCenter);
        scoreBoardTable->setItem(i, 3, intervalItem);

        // Last Lap
        QTableWidgetItem *lastLapItem = new QTableWidgetItem(QString::fromStdString(formatTimeFromMilliseconds_32(driver.lastLapTime)));
        lastLapItem->setTextAlignment(Qt::AlignCenter);
        scoreBoardTable->setItem(i, 4, lastLapItem);

        // Best Lap
        QTableWidgetItem *bestLapItem = new QTableWidgetItem(QString::fromStdString(formatTimeFromMilliseconds_32(driver.bestLapTime)));
        bestLapItem->setTextAlignment(Qt::AlignCenter);
        scoreBoardTable->setItem(i, 5, bestLapItem);
        
        // Tyres
        QLabel *tyreLabel = new QLabel;
        QString tyrePathString = QString::fromStdString(getTyreImagePath(driver.tyreCompound));
        QString tyreImagePath = QDir(QCoreApplication::applicationDirPath()).filePath(tyrePathString);
        if (!tyreImagePath.isEmpty()) {
            QPixmap pixmap(tyreImagePath);
            if (!pixmap.isNull()) {
            
                tyreLabel->setPixmap(pixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            else {
                tyreLabel->setText(QString::number(driver.tyreCompound));
                tyreLabel->setAlignment(Qt::AlignCenter);
            }
        }
        else {
            tyreLabel->setText(QString::number(driver.tyreCompound));
            tyreLabel->setAlignment(Qt::AlignCenter);
        }
        tyreLabel->setAlignment(Qt::AlignCenter);
        scoreBoardTable->setCellWidget(i, 6, tyreLabel);

        // Pit stop num 
        QTableWidgetItem *pitStopItem = new QTableWidgetItem(QString::number(driver.numPitStops));
        pitStopItem->setTextAlignment(Qt::AlignCenter);
        scoreBoardTable->setItem(i, 7, pitStopItem);

        // Penalties
        QTableWidgetItem *penaltiesItem = new QTableWidgetItem(QString::number(driver.penalties));
        penaltiesItem->setTextAlignment(Qt::AlignCenter);
        scoreBoardTable->setItem(i, 8, penaltiesItem);
        
    }

    // Conditions at das bottome
    conditionsLabel->setText(QString("Air Temperature: %1°C Track Temperature: %2°C")
                             .arg(sessionData.m_airTemperature)
                             .arg(sessionData.m_trackTemperature)
                             );

    //this->resize(scrollArea->sizeHint().width(), 600);
}
