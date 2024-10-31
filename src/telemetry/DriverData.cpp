#include "DriverData.h"

void DriverData::addLapTimeData(uint8_t carPosition, uint8_t currentLapNum) {

    this->carPosition = carPosition;
    this->currentLapNum = currentLapNum;
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

void DriverData::clearData() {

    *this = DriverData(); // Reset everything to default values
}
