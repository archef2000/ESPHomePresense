#pragma once
#include <string>

class QueryReport {
public:
    QueryReport(const std::string& id, const std::string& payload) : id(id), payload(payload) {}

    std::string getId() const { return id; }
    std::string getPayload() const { return payload; }

    void setId(const std::string& id) { this->id = id; }
    void setPayload(const std::string& payload) { this->payload = payload; }

private:
    std::string id;
    std::string payload;
};
