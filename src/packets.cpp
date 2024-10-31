#include "packets.h"
#include <Qstring>
#include <stdexcept>

PacketType getPacketType(const std::vector<uint8_t>& data) {
  
    if (data.size() < sizeof(PacketHeader)) {
      throw std::runtime_error("Packet data is incomplete!");
    }

    // Get the PacketHeader from the Packet to get information about the packet
    PacketHeader m_header;
    std::memcpy(&m_header, data.data(), sizeof(PacketHeader));

    switch(m_header.m_packetId) {
        case 0:
            return PacketType::MotionData;
        case 1:
            return PacketType::SessionData;
        case 2:
            return PacketType::LapData;
        case 3: 
            return PacketType::EventData;
        case 4: 
            return PacketType::ParticipantData;
        case 5: 
            return PacketType::CarSetupData;
        case 6: 
            return PacketType::CarTelemetryData;
        case 7:
            return PacketType::CarStatusData;
        case 8: 
            return PacketType::FinalClassificationData;
        case 9:
            return PacketType::LobbyInfoData;
        case 10:
            return PacketType::CarDamageData;
        case 11:
            return PacketType::SessionHistoryData;
        default:
            return PacketType::Unknown;
    }
}

void parsePacket(const std::vector<uint8_t>& data, void* packet) {

    PacketType packetType = getPacketType(data);
    
    switch(packetType) {
        case PacketType::MotionData: {
            PacketMotionData* motionDataPacket = reinterpret_cast<PacketMotionData*>(packet);
            std::memcpy(motionDataPacket, data.data(), sizeof(PacketMotionData));
            break;
        }
        case PacketType::SessionData: {
            PacketSessionData* sessionDataPacket = reinterpret_cast<PacketSessionData*>(packet);
            std::memcpy(sessionDataPacket, data.data(), sizeof(PacketSessionData));
            break;
        }
        case PacketType::LapData: {
            PacketLapTimeData* lapTimeDataPacket = reinterpret_cast<PacketLapTimeData*>(packet);
            std::memcpy(lapTimeDataPacket, data.data(), sizeof(PacketLapTimeData));
            break;
        }
        case PacketType::EventData: {
            PacketEventData* eventDataPacket = reinterpret_cast<PacketEventData*>(packet);
            std::memcpy(eventDataPacket, data.data(), sizeof(PacketEventData));
            break;
        } 
        case PacketType::ParticipantData: {
            PacketParticipantData* participantDataPacket = reinterpret_cast<PacketParticipantData*>(packet);
            std::memcpy(participantDataPacket, data.data(), sizeof(PacketParticipantData));
            break;
        }
        case PacketType::CarSetupData: {
            PacketCarSetupData* carSetupsDataPacket = reinterpret_cast<PacketCarSetupData*>(packet);
            std::memcpy(carSetupsDataPacket, data.data(), sizeof(PacketCarSetupData));
            break;
        }
        case PacketType::CarTelemetryData: {
            PacketCarTelemetryData* carTelemetryDataPacket = reinterpret_cast<PacketCarTelemetryData*>(packet);
            std::memcpy(carTelemetryDataPacket, data.data(), sizeof(PacketCarTelemetryData));
            break;
        }
        case PacketType::CarStatusData: {
            PacketCarStatusData* carStatusDataPacket = reinterpret_cast<PacketCarStatusData*>(packet);
            std::memcpy(carStatusDataPacket, data.data(), sizeof(PacketCarStatusData));
            break;
        }
        case PacketType::FinalClassificationData: {
            PacketFinalClassificationData* finalClassificationDataPacket = reinterpret_cast<PacketFinalClassificationData*>(packet);
            std::memcpy(finalClassificationDataPacket, data.data(), sizeof(PacketFinalClassificationData));
            break;
        }
        case PacketType::LobbyInfoData: {
            PacketLobbyInfoData* lobbyInfoDataPacket = reinterpret_cast<PacketLobbyInfoData*>(packet);
            std::memcpy(lobbyInfoDataPacket, data.data(), sizeof(PacketLobbyInfoData));
            break;
        }
        case PacketType::CarDamageData: {
            PacketCarDamageData* carDamageDataPacket = reinterpret_cast<PacketCarDamageData*>(packet);
            std::memcpy(carDamageDataPacket, data.data(), sizeof(PacketCarDamageData));
            break;
        }
        case PacketType::SessionHistoryData: {
            PacketSessionHistoryData* sessionHistoryDataPacket = reinterpret_cast<PacketSessionHistoryData*>(packet);
            std::memcpy(sessionHistoryDataPacket, data.data(), sizeof(PacketSessionHistoryData));
            break;
        }
        default: {
            throw std::runtime_error("Unknown packet type.");
        }
    }
}
