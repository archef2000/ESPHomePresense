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

namespace esphome {
namespace espresense {
class MessageQueue : public mqtt::CustomMQTTDevice {
public:
    MessageQueue();
    using Message = std::tuple<std::string, std::string>;
    void push(const Message& msg);
    bool pop(Message& msg);
    bool empty() const;
    void message_task();
private:
    mutable std::mutex mutex_;
    std::queue<Message> queue_;
    static void task_runner(void* param) {
        static_cast<MessageQueue*>(param)->message_task();
    }
};
}  // namespace espresense
}  // namespace esphome