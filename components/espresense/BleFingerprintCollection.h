#pragma once
#include "defaults.h"
#include "esphome/core/defines.h"
#include <string>
#include <cstdint>

#define ONE_EURO_FCMIN 1e-1f
#define ONE_EURO_BETA 1e-3f
#define ONE_EURO_DCUTOFF 5e-3f

#ifndef ALLOW_BLE_CONTROLLER_RESTART_AFTER_SECS
#define ALLOW_BLE_CONTROLLER_RESTART_AFTER_SECS 1800
#endif
/*
struct DeviceConfig {
    std::string id;
    std::string alias;
    std::string name;
    int8_t calRssi = NO_RSSI;
};
*/
namespace BleFingerprintCollection {

extern std::string include, exclude, query, knownMacs, knownIrks, countIds;
extern float skipDistance, maxDistance, absorption, countEnter, countExit;
extern int8_t rxRefRssi, rxAdjRssi, txRefRssi;
extern int forgetMs, skipMs, countMs, requeryMs;
}  // namespace BleFingerprintCollection
