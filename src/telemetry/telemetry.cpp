#include "telemetry.h"
#include "FinalRaceData.h"
#include "../network/discord_notifier.h"
#include "../constants.h"
#include "../utils.h"
#include <iostream>
#include <cstring>
#include <memory>
#include <qudpsocket.h>

std::unique_ptr<DiscordNotifier> discordNotifier = std::make_unique<DiscordNotifier>();

TelemetryReceiver::TelemetryReceiver(QObject *parent) : QObject(parent), listening(false), isServerStarted(false) {

    socket = new QUdpSocket(this);
    
    packetSizes = {
        {0, sizeof(PacketMotionData)},
        {1, sizeof(PacketSessionData)},
        {2, sizeof(PacketLapTimeData)},
        {3, sizeof(PacketEventData)},
        {4, sizeof(PacketParticipantData)},
        {5, sizeof(PacketCarSetupData)},
        {6, sizeof(PacketCarTelemetryData)},
        {7, sizeof(PacketCarStatusData)},
        {8, sizeof(PacketFinalClassificationData)},
        {9, sizeof(PacketLobbyInfoData)},
        {10, sizeof(PacketCarDamageData)},
        {11, sizeof(PacketSessionHistoryData)}
    };

    connect(socket, &QUdpSocket::readyRead, this, &TelemetryReceiver::processPendingDatagrams);

    // Initialize the lastLapNumber to 0
    for (int i=0; i<22; i++) {
        lastLapNumber[i] = 1;
    }
}

void TelemetryReceiver::setServerStarted(bool started) {

    isServerStarted = started;
}

void TelemetryReceiver::processPendingDatagrams() {

    while(socket->hasPendingDatagrams()) {

        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        buffer.append(datagram);

        while(buffer.size() >= static_cast<int>(sizeof(PacketHeader))) {
            PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer.data());
            uint8_t packetId = header->m_packetId;

            auto it = packetSizes.find(packetId);
            if(it != packetSizes.end() && buffer.size() >= static_cast<int>(it->second)) {
                size_t packetSize = it->second;

                // Check if buffer has enough data for the expected packet size
                if(buffer.size() >= packetSize) {

                    if(packetId == 0) {
                        PacketMotionData motionData;
                        parsePacket(buffer, &motionData);
                        //processMotionData(motionData);
                    } 
                    else if(packetId == 1) {
                        PacketSessionData sessionData;
                        parsePacket(buffer, &sessionData);
                        processSessionData(sessionData);
                    }
                    else if(packetId == 2) {
                        PacketLapTimeData lapTimeData;
                        parsePacket(buffer, &lapTimeData);
                        processLapTimeData(lapTimeData);
                    }
                    else if(packetId == 3) {
                        PacketEventData eventData;
                        parsePacket(buffer, &eventData);
                        processEventData(eventData);
                    }
                    else if(packetId == 4) {
                        PacketParticipantData participantData;
                        parsePacket(buffer, &participantData);
                        processParticipantData(participantData);
                    }
                    else if(packetId == 8) {
                        PacketFinalClassificationData finalClassificationData;
                        parsePacket(buffer, &finalClassificationData);
                        processFinalClassificationData(finalClassificationData);
                    }
                    // Additional packet types can be added here

                    // Remove the processed packet from the buffer 
                    buffer.remove(0, packetSize);
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
}

template <typename T>
void TelemetryReceiver::parsePacket(const QByteArray &buffer, T *packet) {

    std::memcpy(packet, buffer.data(), sizeof(T));
}

void TelemetryReceiver::processMotionData(const PacketMotionData &data) {
    // Motion data processing here
}

void TelemetryReceiver::processSessionData(const PacketSessionData &data) {

    sessionDataBackup = data;
    //std::cout << "Track ID : " << getTrackName(static_cast<int>(data.m_trackId)) << std::endl;
}

void TelemetryReceiver::processLapTimeData(const PacketLapTimeData &data) {
    
    // Store information in a central struct

    for(uint8_t i = 0; i < 22; i++) {

        const LapData& lapData = data.m_lapData[i];
        
        std::array<uint16_t, 3> sectorTimes = {
            lapData.m_sector1TimeInMS,
            lapData.m_sector2TimeInMS,
            static_cast<uint16_t>(lapData.m_currentLapTimeInMS - lapData.m_sector1TimeInMS - lapData.m_sector2TimeInMS)
        };

        driverData[i].addLapTimeData(lapData.m_carPosition, lapData.m_currentLapNum);
        
        // Detect if the lap changed and perform necessary operations
        if (driverData[i].currentLapNum != lastLapNumber[i]) {
            
            std::cout << "CURRENT LAP NUM:" << (int)driverData[i].currentLapNum << "LAST LAP NUM: " << (int)lastLapNumber[i] << std::endl;
            std::cout << "Lap Changed for driver " << driverData[i].driverName << "on index " << (int)i << std::endl;
            lastLapNumber[i] = driverData[i].currentLapNum;
        }
    }
}

void TelemetryReceiver::processEventData(const PacketEventData &data) {
    
    //std::cout << "EVENT STRING CODE: " << std::string(reinterpret_cast<const char*>(data.m_eventStringCode), 4) << std::endl;
    
    // Event handling for when Session Starts
    if( std::string(reinterpret_cast<const char*>(data.m_eventStringCode)) == "SSTA") {

        // Clear the driverData struct when a new session starts
        for ( int i = 0; i < 22; i++) {
            driverData[i].clearData();
        }
    }
}

void TelemetryReceiver::processParticipantData(const PacketParticipantData &data) {
    
    // Store information in a central struct

    for(uint8_t i = 0; i < 22; i++) {

        const ParticipantData& participant = data.m_participants[i];

        driverData[i].driverId = participant.m_driverId;
        driverData[i].driverName = (participant.m_driverId == 255) ? std::string(participant.m_name) : getPlayerName(participant.m_driverId);
        driverData[i].constructor = participant.m_teamId;
    }
}

void TelemetryReceiver::processFinalClassificationData(const PacketFinalClassificationData &data) {

    // We use a flag to keep track of final classification data's transmission
    // because the packet is transmitted twice at the end of the session
    if(!finalDataPacketArrived) {
        std::cout << "Final Classification Data was transmitted!" << std::endl;

        // Toggle the value of the flag
        finalDataPacketArrived = true;

        nlohmann::json jArray = nlohmann::json::array();

        // Add track and session info as the first element
        nlohmann::json trackInfo = nlohmann::json::array();
        trackInfo.push_back({ {"track_name", getTrackName(sessionDataBackup.m_trackId)} });
        trackInfo.push_back({ {"session_type", getSessionType(sessionDataBackup.m_sessionType)} });
        jArray.push_back(trackInfo);
    
        // Create a vector for sorting
        std::vector<FinalClassificationData> classificationEntries;

        // Use the numCars to check how many entries need to be processed
        for (uint8_t i = 0; i < data.m_numCars; i++) {
            classificationEntries.push_back(data.m_classficationData[i]);
        }

        // Sort the data by position
        std::sort(classificationEntries.begin(), classificationEntries.end(), [](const FinalClassificationData& a, const FinalClassificationData& b) {
            return a.m_position < b.m_position; // Sort in ascending order
        });

        // Populate the json array with sorted data
        for (size_t i = 0; i < classificationEntries.size(); i++) {

            const auto& classificationData = classificationEntries[i];
            nlohmann::json jObject;
            jObject["position"] = classificationData.m_position;
            jObject["name"] = driverData[i].driverName;
            jObject["total_time"] = formatTimeFromSeconds(classificationData.m_totalRaceTime);
            jObject["best_lap_time"] = formatTimeFromMilliseconds_32(classificationData.m_bestLapTimeInMS);
            jObject["penalties_time"] = classificationData.m_penaltiesTime;
            jObject["total_laps"] = classificationData.m_numLaps;
            jObject["num_of_pitstops"] = classificationData.m_numPitStops;
            jObject["result_status"] = getResultStatus(classificationData.m_resultStatus);

            jArray.push_back(jObject);
        }

        // If the server is running then send a notification to discord
        if (isServerStarted) {

            FinalRaceData::instance().updateRaceData(jArray.dump());
            discordNotifier->sendNotification("raceData", jArray.dump());
        }
    }
    else {
        finalDataPacketArrived = false;
    }

   //nlohmann::json raceData = {{"event", "final_classification"}, {"position", 1}};
    //discordNotifier->sendNotification("RaceComplete", raceData);
}

void TelemetryReceiver::startListening() {

    if(!listening) {
        socket->bind(QHostAddress::LocalHost, 20777);
        listening = true;
        std::cout << "Telemetry listening started on host " 
                  << socket->localAddress().toString().toStdString()
                  << " and port 20777" << std::endl;
    }
}

void TelemetryReceiver::stopListening() {

    if(listening) {
        socket->close();
        listening = false;
    }
}

TelemetryReceiver::~TelemetryReceiver() {

}
