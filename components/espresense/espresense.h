#pragma once
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/mqtt/custom_mqtt_device.h"
#include "esphome/core/defines.h"
#include "BleFingerprint.h"
#include "BleFingerprintCollection.h"
#include "esphome/core/helpers.h"
#include "esphome/core/component.h"
#include "esphome/components/json/json_util.h"
#include "util.h"
#include "string_utils.h"
#include <array>
#include <string>
#include <tuple>
#include "cJSON.h"
#include "MessageQueue.h"

namespace esphome {
namespace espresense {

#define CHANNEL "espresense"

class BleFingerprint {
  public:
    BleFingerprint(const esp32_ble_tracker::ESPBTDevice &device);
    int get1mRssi();
    void fingerprintAddress();
    void fingerprint(const esp32_ble_tracker::ESPBTDevice &device);
    std::string address_str() const;
    std::tuple<std::string, std::string> fill() const;
    bool setId(const std::string &newId, short int newIdType, const std::string &newName = "");
    std::string fingerprintManufactureData(const esp32_ble_tracker::ESPBTDevice &device, bool haveTxPower, int8_t txPower);
  private:
    std::string id, name;
    uint32_t firstSeenMillis = 0;
    const uint8_t *address;
    esp_ble_addr_type_t addressRawType;
    int8_t calRssi = NO_RSSI, bcnRssi = NO_RSSI, mdRssi = NO_RSSI, asRssi = NO_RSSI;
    short int addressType = 0xFF, idType = NO_ID_TYPE;
    int rssi = NO_RSSI;
    unsigned long seenCount = 0;
    float raw = 0, dist = 0;
};

class ESPHomePresense : public Component, public mqtt::CustomMQTTDevice, public esp32_ble_tracker::ESPBTDeviceListener {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  bool reportDevice(BleFingerprint *f);
  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;
  void set_node_name(const char *s);
 private:
  char *nodeName;
  MessageQueue queue_;
};

}  // namespace espresense
}  // namespace esphome