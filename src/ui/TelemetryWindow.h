#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QMenu>
#include <QTableWidget>
#include <QScrollArea>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include "../telemetry/telemetry.h"
#include "../packets.h"

class TelemetryWindow : public QWidget {
    Q_OBJECT

public:
    explicit TelemetryWindow(QWidget* parent = nullptr);
    void updateTelemetryWindow(const std::vector<DriverData>& driverData, const PacketSessionData& sessionData);  // Update driver data 

signals:
    void captureStopped();

private slots:
    void onStopCapture();   // Slot to handle when capture is stopped
    void onDisplayAdditionalInfo();

    // Signals to open the new guis
    void showDriverContextMenu(int driverIndex, const QPoint &pos);
    void showLapHistory();
    void showCarInfoHistory();

private:
    QLabel *trackLabel;
    QLabel *lapLabel;
    QScrollArea *scrollArea;
    QTableWidget *scoreBoardTable;
    QVBoxLayout *mainLayout;
    QLabel *conditionsLabel;
    QLabel *airTempLabel;
    QLabel *humidityLabel;
    QLabel *trackTempLabel;
    QLabel *windLabel;
    QPushButton* stopCaptureButton;
    QPushButton* additionalInfoButton;

    QMenu *contextMenu;

    void setupLayout();
    void populateDriverRow(int row, const DriverData& driver);
};

