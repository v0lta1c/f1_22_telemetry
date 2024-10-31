#pragma once

#include <nlohmann/json.hpp>

class FinalRaceData {
public:
    static FinalRaceData& instance() {
        static FinalRaceData instance;
        return instance;
    }

    nlohmann::json getRaceData() const {
        return raceData_;
    }

    void updateRaceData(const nlohmann::json& newRaceData) {
        raceData_ = newRaceData;
    }


private:
    FinalRaceData() {}
    nlohmann::json raceData_;
};
