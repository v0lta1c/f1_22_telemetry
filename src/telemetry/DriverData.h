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

    // vectors storing the data for multiple laps 
    std::vector<uint32_t> lapTimes;
    std::vector<std::array<uint16_t, 3>> sectorTimes;
    std::vector<uint8_t> pitStatus;

    // Information from the Car Status Data
    uint8_t tyreCompound;
    uint8_t tyresAgeLaps;

    // Information from the Car Damage Packet
    // vectors storing the data for multiple laps
    std::vector<std::array<float, 4>> tyresWear;
    std::vector<uint8_t> engineDamage;

    // Constructor
    DriverData() = default;

    void addLapTimeData(uint8_t carPosition, uint8_t currentLapNum);

    void addParticipantData(uint8_t driverId, const std::string& driverName, uint8_t constructor);

    void addCarStatusData(uint8_t tyreCompound, uint8_t tyresAgeLaps);

    void clearData();
};
