#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <qstringview.h>
#include <qtmetamacros.h>
#include <unordered_map>
#include <cstdint>
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

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *socket;
    QByteArray buffer;
    std::unordered_map<uint8_t, size_t> packetSizes;
    bool listening;
    bool isServerStarted;

    // This stores the driver data for parsing
    DriverData driverData[22];

    // Session data stored
    PacketSessionData sessionDataBackup;

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
    void processParticipantData(const PacketParticipantData &data);
    void processFinalClassificationData(const PacketFinalClassificationData &data);
}; 
