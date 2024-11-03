#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QTimer>
#include <qstringview.h>
#include <qtmetamacros.h>
#include <unordered_map>
#include <cstdint>
#include <array>
#include "../packets.h"
#include "DriverData.h"

class TelemetryReceiver : public QObject {
    
    Q_OBJECT

public:
    explicit TelemetryReceiver(QObject *parent = nullptr);
    ~TelemetryReceiver();

    void startListening();
    void stopListening();

    void setServerStarted(bool started);
    void startTelemetryUpdates();
    void stopTelemetryUpdates();

signals:
    void telemetryDataUpdated(const std::vector<DriverData>& driverData, const PacketSessionData& sessionData);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *socket;
    QByteArray buffer;
    std::unordered_map<uint8_t, size_t> packetSizes;
    bool listening;
    bool isServerStarted;

    QTimer *updateTimer;

    // This stores the driver data for parsing
    DriverData driverData[22];

    // Duplicates to store temporary data
    PacketSessionData sessionDataBackup;
    std::array<uint16_t, 3> sectorTimesBackup[22];
    std::array<float, 4> tyresWearBackup[22];
    float ersDeployed[22]; // ers used by a driver in a lap 
    uint8_t tyreCompound[22];
    uint8_t tyreAgeLaps[22];

    // Flags
    bool sessionStartFlag;
    bool sessionRunningFlag;

    // Temp variables to store and handle lap changes gracefully
    uint8_t lastLapNumber[22];

    // Temp variable to print finalClassification
    bool finalDataPacketArrived = false;

    template <typename T>
    void parsePacket(const QByteArray &buffer, T *packet);

    // All the packet types get processed in these functions
    void processMotionData(const PacketMotionData &data);
    void processSessionData(const PacketSessionData &data);
    void processLapTimeData(const PacketLapTimeData &data);
    void processEventData(const PacketEventData &data);
    void processCarStatusData(const PacketCarStatusData &data);
    void processParticipantData(const PacketParticipantData &data);
    void processFinalClassificationData(const PacketFinalClassificationData &data);
    void processCarDamageData(const PacketCarDamageData &data);

    void sendTelemetryData();
}; 
