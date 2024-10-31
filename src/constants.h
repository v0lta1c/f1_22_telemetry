#pragma once

#include <string>
#include <cstdint>

// IP's AND PORT's

// IP and Port for the Discord Webhook server
const std::string Webhook_URL = "84.249.17.176";
const int Webhook_PORT = 20003;

const std::string Webhook_ProjektServer = "https://discord.com/api/webhooks/1266629985620197426/DvWC9zKP3O0k7T1KxE2AEpBC95wgZnoUcxc6PZ7cypny7HPu0VNHUo2skZHYiKaEJ9jY";
const std::string Webhook_Interstellar = "https://discord.com/api/webhooks/1263968867290452009/SUpbGVLM1Rnb7wJwfxbOsuxW3wE_i4fdgWhlvCCSzKW6ZPrzsqzmAnEO-nz8_ZPFI7Ic";

// IP and Port for the API server
const int API_PORT = 8000;

std::string getTeamName(int teamId);
std::string getPlayerName(int playerId);
std::string getTrackName(int trackId);
std::string getNationalityName(int nationalityId);
std::string getGameModeName(int gameModeId);
std::string getRulesetName(int rulesetId);
std::string getSurfaceType(int surfaceId);
std::string getPenaltyType(int penaltyTypeId);
std::string getInfringementType(int infringementId);
std::string getInfringementType(int infringementId);

std::string getResultStatus(uint8_t status);
std::string getSessionType(uint8_t sessionType);
