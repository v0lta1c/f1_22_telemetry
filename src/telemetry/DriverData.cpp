#include "DriverData.h"

void DriverData::addLapTimeData(uint8_t carPosition, uint8_t currentLapNum, uint32_t lastLapTime,
                                uint8_t pitStatus, uint8_t numPitStops, uint8_t penalties) {

    this->carPosition = carPosition;
    this->currentLapNum = currentLapNum;
    this->lastLapTime = lastLapTime;
    this->pitStatus = pitStatus;
    this->numPitStops = numPitStops;
    this->penalties = penalties;
}

void DriverData::addParticipantData(uint8_t driverId, const std::string& driverName, uint8_t constructor) {
    
    this->driverId = driverId;
    this->driverName = driverName;
    this->constructor = constructor;
}

void DriverData::addCarStatusData(uint8_t tyreCompound, uint8_t tyresAgeLaps) {

    this->tyreCompound = tyreCompound;
    this->tyresAgeLaps = tyresAgeLaps;
}

void DriverData::onLapChangeDataUpdate(uint32_t prevLapTime, std::array<uint16_t, 3> sectorTimes, 
                                       uint32_t bestLapTime, std::array<float, 4> tyresWear,
                                       float ersUsage) {

    this->lapTimes.push_back(prevLapTime);
    this->sectorTimes.push_back(sectorTimes);
    this->bestLapTime = bestLapTime;
    this->tyresWear.push_back(tyresWear);
    this->ersUsage.push_back(ersUsage);
}

void DriverData::clearData() {

    *this = DriverData(); // Reset everything to default values
}
