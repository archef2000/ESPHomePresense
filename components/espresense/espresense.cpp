#include "espresense.h"
#include "esphome/core/log.h"
#include "BleFingerprintCollection.h"
#include "esphome/core/application.h" // Required for MQTT
#include "esphome/core/component.h"
#include "MessageQueue.h"

namespace esphome {
namespace espresense {

static const char *TAG = "espresense";
SemaphoreHandle_t mqttMutex;
int test = 0;
const TickType_t MAX_WAIT = portTICK_PERIOD_MS * 100;
bool ESPHomePresense::reportDevice(BleFingerprint *f) {
    return true;
}

void ESPHomePresense::setup() {
    ESP_LOGCONFIG(TAG, "Empty component");
    mqttMutex = xSemaphoreCreateMutex();
}

void ESPHomePresense::loop() {
}

void ESPHomePresense::dump_config() {
    ESP_LOGCONFIG(TAG, "Empty component");
}

bool ESPHomePresense::parse_device(const esp32_ble_tracker::ESPBTDevice &device) {
    ESP_LOGI(TAG, "BLE device: %s, RSSI: %d", device.address_str().c_str(), device.get_rssi());
    ESP_LOGI("TAG", "Memory Available heap: %d", esp_get_free_heap_size());
    if (is_connected()) {
        BleFingerprint created(device);
        std::string topic;
        std::string json;
        tie(topic, json) = created.fill();
        topic = Sprintf("espresense/devices/%s/%s", nodeName, topic.c_str());
        queue_.push({topic, json});
    }
    return true;
}

void ESPHomePresense::set_node_name(const char *s) {
    if (s=="") {
        nodeName = (char *)App.get_name().c_str();
    } else {
        nodeName = (char *)s;
    }
}

std::string BleFingerprint::address_str() const {
  char mac[19];
  snprintf(mac, sizeof(mac), "%02x%02x%02x%02x%02x%02x", this->address[0], this->address[1], this->address[2],
           this->address[3], this->address[4], this->address[5]);
  return mac;
}

void BleFingerprint::fingerprint(const esp32_ble_tracker::ESPBTDevice &device) {
    size_t serviceAdvCount = device.get_service_uuids().size();
    size_t serviceDataCount = device.get_service_datas().size();
    bool haveTxPower = device.get_tx_powers().size() > 0;
    int8_t txPower = haveTxPower ? device.get_tx_powers()[0] : NO_RSSI; 
    if (device.get_manufacturer_datas().size() > 0) fingerprintManufactureData(device, haveTxPower, txPower);
}

std::tuple<std::string, std::string> BleFingerprint::fill() const {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "mac", address_str().c_str());
    cJSON_AddStringToObject(root, "id", id.c_str());
    char *json_cstr = cJSON_PrintUnformatted(root);
    if (json_cstr == nullptr) {
        ESP_LOGE("BleFingerprint", "Failed to create JSON string");
        return std::make_tuple("", "");
    }
    std::string json_str(json_cstr);
    cJSON_free(json_cstr);
    return std::make_tuple(id, json_str);
}

BleFingerprint::BleFingerprint(const esp32_ble_tracker::ESPBTDevice &device) {
    firstSeenMillis = esphome::millis();
    address = device.address();
    addressRawType = device.get_address_type();
    rssi = device.get_rssi();
    name = device.get_name();
    fingerprint(device);
    raw = dist = pow(10, float(get1mRssi() - rssi) / (10.0f * BleFingerprintCollection::absorption));
    seenCount = 1;
    fingerprintAddress();
}

// https://github.com/ESPresense/ESPresense/blob/f6803ab8a4e8a5a83f3f52cb24953993973d2c54/src/BleFingerprint.cpp#L115
// https://github.com/ESPresense/ESPresense/blob/f6803ab8a4e8a5a83f3f52cb24953993973d2c54/src/BleFingerprint.cpp#L351

std::string BleFingerprint::fingerprintManufactureData(const esp32_ble_tracker::ESPBTDevice &device, bool haveTxPower, int8_t txPower) {
    auto manuf = device.get_manufacturer_datas();
    for (const auto &data : manuf) {
        auto uuid = static_cast<int>(data.uuid.get_uuid().uuid.uuid16);
        ESP_LOGE(TAG,"uuid:%04x len:%i",uuid,data.data.size());
        //printf("\n");
        //for (int i = 0; i < data.data.size(); i++) {
        //    printf("%02x ",data.data[i]);
        //}
        //printf("\n");
        ESP_LOGI(TAG, "Memory Available heap: %d", esp_get_free_heap_size());
        if (uuid == 0x004c) {
            //printf("iBeacon\n");
            if (data.data.size() == 23 && data.data[0] == 0x02 && data.data[1] == 0x15) {
                auto ibeacon = device.get_ibeacon();
                if (ibeacon.has_value()) {
                    bcnRssi = ibeacon.value().get_signal_power();
                    setId(Sprintf("iBeacon:%s-%u-%u", ibeacon.value().get_uuid().to_string().c_str(), ibeacon.value().get_major(), ibeacon.value().get_minor()), (bcnRssi != 3 ? ID_TYPE_IBEACON : ID_TYPE_ECHO_LOST));
                }
            }
            else if (data.data.size() >= 2) {
                mdRssi = BleFingerprintCollection::rxRefRssi + APPLE_TX;
                if (data.data[0] == 0x10) {
                    std::string pid = Sprintf("apple:%02x%02x:%u", data.data[0], data.data[1], data.data.size()+2);
                    if (haveTxPower) pid += std::to_string(-txPower);
                    setId(pid, ID_TYPE_APPLE_NEARBY);
                } else if (data.data[0] == 0x12 && data.data.size() == 27) {
                    std::string pid = "apple:findmy";
                    setId(pid, ID_TYPE_FINDMY);
                } else {
                    std::string pid = Sprintf("apple:%02x%02x:%u", data.data[0], data.data[1], data.data.size()+2);
                    if (haveTxPower) pid += std::to_string(-txPower);
                    setId(pid, ID_TYPE_MISC_APPLE);
                }
            }
        } else if (uuid == 0x05a7)  // Sonos
        {
            mdRssi = haveTxPower ? BleFingerprintCollection::rxRefRssi + txPower : NO_RSSI;
            setId("sonos:" + address_str(), ID_TYPE_SONOS);
        } else if (uuid == 0x0087)  // Garmin
        {
            mdRssi = haveTxPower ? BleFingerprintCollection::rxRefRssi + txPower : NO_RSSI;
            setId("garmin:" + address_str(), ID_TYPE_GARMIN);
        } else if (uuid == 0x4d4b)  // iTrack
        {
            mdRssi = haveTxPower ? BleFingerprintCollection::rxRefRssi + txPower : NO_RSSI;
            setId("iTrack:" + address_str(), ID_TYPE_ITRACK);
        } else if (uuid == 0x0157)  // Mi-fit
        {
            mdRssi = haveTxPower ? BleFingerprintCollection::rxRefRssi + txPower : NO_RSSI;
            setId("mifit:" + address_str(), ID_TYPE_MIFIT);
        } else if (uuid == 0x0006 && data.data.size() == 27)  // microsoft
        {
            mdRssi = haveTxPower ? BleFingerprintCollection::rxRefRssi + txPower : NO_RSSI;
            setId(Sprintf("msft:cdp:%02x%02x", data.data[1], data.data[3]), ID_TYPE_MSFT);
        } else if (uuid == 0x0075)  // samsung
        {
            mdRssi = haveTxPower ? BleFingerprintCollection::rxRefRssi + txPower : NO_RSSI;
            setId("samsung:" + address_str(), ID_TYPE_MISC);
        } else if (uuid == 0xbeac && data.data.size() == 24) {
            auto ibeacon = device.get_ibeacon();
            if (ibeacon.has_value()) {
                bcnRssi = ibeacon.value().get_signal_power();
                setId(Sprintf("altBeacon:%s-%u-%u", ibeacon.value().get_uuid().to_string().c_str(), ibeacon.value().get_major(), ibeacon.value().get_minor()), ID_TYPE_ABEACON);
            }
        } else if (uuid != 0x0000) {
            mdRssi = haveTxPower ? BleFingerprintCollection::rxRefRssi + txPower : NO_RSSI;
            std::string fingerprint = Sprintf("md:%04x:%u", uuid, data.data.size()+2);
            if (haveTxPower) fingerprint += std::to_string(-txPower);
            setId(fingerprint, ID_TYPE_MD);
        }
        
    }
    ESP_LOGI(TAG, "Memory Available heap: %d", esp_get_free_heap_size());
    //ESP_LOGE("test","fingerprintManufactureData %s",id.c_str());
    return id;
}

bool BleFingerprint::setId(const std::string &newId, short newIdType, const std::string &newName) {
    ESP_LOGD(TAG, "setId %s", newId.c_str());
    idType = newIdType;
    id = newId;
    return true;
}

void BleFingerprint::fingerprintAddress() {
    std::string mac = address_str();
    switch (addressRawType) {
        case BLE_ADDR_TYPE_PUBLIC:
        case BLE_ADDR_TYPE_RPA_PUBLIC:
            addressType = ID_TYPE_PUBLIC_MAC;
            break;
        case BLE_ADDR_TYPE_RANDOM:
        case BLE_ADDR_TYPE_RPA_RANDOM:
            if ((address[5] & 0xc0) == 0xc0)
                addressType = ID_TYPE_RAND_STATIC_MAC;
            else {
                // TODO
                addressType = ID_TYPE_RAND_MAC;
            }
            break;
        default:
            addressType = ID_TYPE_RAND_MAC;
            break;
    }  
}

int BleFingerprint::get1mRssi() {
    if (calRssi != NO_RSSI) return calRssi + BleFingerprintCollection::rxAdjRssi;
    if (bcnRssi != NO_RSSI) return bcnRssi + BleFingerprintCollection::rxAdjRssi;
    if (mdRssi != NO_RSSI) return mdRssi + BleFingerprintCollection::rxAdjRssi;
    if (asRssi != NO_RSSI) return asRssi + BleFingerprintCollection::rxAdjRssi;
    return BleFingerprintCollection::rxRefRssi + DEFAULT_TX + BleFingerprintCollection::rxAdjRssi;
}

}  // namespace espresense
}  // namespace esphome