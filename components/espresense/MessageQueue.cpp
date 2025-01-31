#include <queue>
#include <tuple>
#include <string>
#include <mutex>
#include "MessageQueue.h"

namespace esphome {
namespace espresense {
void MessageQueue::push(const MessageQueue::Message& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(msg);
}

bool MessageQueue::pop(MessageQueue::Message& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    msg = std::move(queue_.front());  // Use move to optimize performance
    queue_.pop();
    return true;
}

bool MessageQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

MessageQueue::MessageQueue() {
    ESP_LOGE("MessageQueue", "MessageQueue");
        xTaskCreate(
            task_runner,    // Function to run
            "MessageTask",  // Task name
            4096,           // Stack size
            this,           // Task argument
            1,              // Priority (1 = low, higher is more critical)
            nullptr         // Task handle (not used)
        );
}

void MessageQueue::message_task() {
    MessageQueue::Message msg;
    while (true) {
        if  (is_connected()) {
            ESP_LOGI("TAG1", "Memory Available heap: %d", esp_get_free_heap_size());
            if (pop(msg)) {
                std::string sensor = std::get<0>(msg);
                std::string value = std::get<1>(msg);
            ESP_LOGI("TAG2", "Memory Available heap: %d", esp_get_free_heap_size());
                publish(sensor, value);
                //ESP_LOGI("MessageQueue", "Received message: %s -> %s", sensor.c_str(), value.c_str());
            }
            ESP_LOGI("TAG3", "Memory Available heap: %d", esp_get_free_heap_size());
        } else {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}

}  // namespace espresense
}  // namespace esphome