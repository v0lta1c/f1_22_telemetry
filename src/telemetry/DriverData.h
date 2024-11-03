#include <cstdint>
#include <string>
#include <vector>
#include <array>

struct DriverData {

    // Information about the driver
    uint8_t driverId;
    std::string driverName;
    uint8_t constructor;

    // Information regarding the lap data
    uint8_t carPosition;
    uint8_t currentLapNum;
    uint32_t lastLapTime;
    uint8_t pitStatus;
    uint8_t numPitStops;
    uint8_t penalties;
    uint32_t bestLapTime;

    // vectors storing the data for multiple laps 
    std::vector<uint32_t> lapTimes;
    std::vector<std::array<uint16_t, 3>> sectorTimes;

    // Information from the Car Status Data
    uint8_t tyreCompound;
    uint8_t tyresAgeLaps;
    std::vector<float> ersUsage; 

    // Information from the Car Damage Packet
    // vectors storing the data for multiple laps
    std::vector<std::array<float, 4>> tyresWear;
    std::vector<uint8_t> engineDamage;

    // Constructor
    DriverData() = default;

    void addLapTimeData(uint8_t carPosition, uint8_t currentLapNum, uint32_t lastLapTime, uint8_t pitStatus, uint8_t numPitStops, uint8_t penalties);

    void addParticipantData(uint8_t driverId, const std::string& driverName, uint8_t constructor);

    void addCarStatusData(uint8_t tyreCompound, uint8_t tyresAgeLaps);

    void onLapChangeDataUpdate(uint32_t prevLapTime, std::array<uint16_t, 3> sectorTimes, uint32_t bestLapTime, 
                               std::array<float, 4> tyresWear, float ersUsage);

    void clearData();
};
