#ifndef _BLEFINGERPRINT_
#define _BLEFINGERPRINT_
#include "esphome/core/defines.h"

#include <memory>

#include "QueryReport.h"
#include "rssi.h"

#define NO_RSSI int8_t(-128)

#define ID_TYPE_TX_POW short(1)

#define NO_ID_TYPE short(0)

#define ID_TYPE_ECHO_LOST short(-10)
#define ID_TYPE_MISC_APPLE short(-5)

#define ID_TYPE_RAND_MAC short(1)
#define ID_TYPE_RAND_STATIC_MAC short(5)
#define ID_TYPE_AD short(10)
#define ID_TYPE_SD short(15)
#define ID_TYPE_MD short(20)
#define ID_TYPE_MISC short(30)
#define ID_TYPE_FINDMY short(32)
#define ID_TYPE_NAME short(35)
#define ID_TYPE_MSFT short(40)
#define ID_TYPE_UNIQUE short(50)
#define ID_TYPE_PUBLIC_MAC short(55)
#define ID_TYPE_SONOS short(105)
#define ID_TYPE_GARMIN short(107)
#define ID_TYPE_MITHERM short(110)
#define ID_TYPE_MIFIT short(115)
#define ID_TYPE_EXPOSURE short(120)
#define ID_TYPE_SMARTTAG short(121)
#define ID_TYPE_ITAG short(125)
#define ID_TYPE_ITRACK short(127)
#define ID_TYPE_NUT short(128)
#define ID_TYPE_FLORA short(129)
#define ID_TYPE_TRACKR short(130)
#define ID_TYPE_TILE short(135)
#define ID_TYPE_MEATER short(140)
#define ID_TYPE_TRACTIVE short(142)
#define ID_TYPE_VANMOOF short(145)
#define ID_TYPE_APPLE_NEARBY short(150)
#define ID_TYPE_QUERY_MODEL short(155)
#define ID_TYPE_QUERY_NAME short(160)
#define ID_TYPE_RM_ASST short(165)
#define ID_TYPE_EBEACON short(170)
#define ID_TYPE_ABEACON short(175)
#define ID_TYPE_IBEACON short(180)
#define ID_TYPE_KNOWN_IRK short(200)
#define ID_TYPE_KNOWN_MAC short(210)
#define ID_TYPE_ALIAS short(250)
/*
class BleFingerprint {
   public:
    BleFingerprint(NimBLEAdvertisedDevice *advertisedDevice, float fcmin, float beta, float dcutoff);

    bool seen(BLEAdvertisedDevice *advertisedDevice);

    bool fill(JsonObject *doc);

    bool report(JsonObject *doc);

    bool query();

    const std::string getId() const { return id; }

    const std::string getName() const { return name; }

    void setName(const std::string &name) { this->name = name; }

    bool setId(const std::string &newId, short int newIdType, const std::string &newName = "");

    void setInitial(const BleFingerprint &other);

    const std::string getMac() const;

    const short getIdType() const { return idType; }

    const float getDistance() const { return dist; }

    const int getRssi() const { return rssi; }
    const int getRawRssi() const { return rssi; }

    const int get1mRssi() const;
    void set1mRssi(int8_t rssi) { calRssi = rssi; }

    const NimBLEAddress getAddress() const { return address; }

    const unsigned long getMsSinceLastSeen() const { return lastSeenMillis ? esphome::millis - lastSeenMillis : 4294967295; };

    const unsigned long getMsSinceFirstSeen() const { return esphome::millis - firstSeenMillis; };

    const bool getVisible() const { return !ignore && !hidden; }

    const bool getAdded() const { return added; };

    const bool getIgnore() const { return ignore; };

    const bool getAllowQuery() const { return allowQuery; };

    const bool hasReport() { return queryReport != nullptr; };
    const QueryReport getReport() { return *queryReport; };
    void setReport(const QueryReport &report) { queryReport = std::unique_ptr<QueryReport>(new QueryReport{report}); };
    void clearReport() { queryReport.reset(); };

    unsigned int getSeenCount() {
        auto sc = seenCount - lastSeenCount;
        lastSeenCount = seenCount;
        return sc;
    }

    bool shouldCount();
    void fingerprintAddress();
    void expire();

   private:

    bool added = false, close = false, reported = false, ignore = false, allowQuery = false, isQuerying = false, hidden = false, connectable = false, countable = false, counting = false;
    NimBLEAddress address;
    std::string id, name;
    short int idType = NO_ID_TYPE;
    int rssi = NO_RSSI;
    int8_t calRssi = NO_RSSI, bcnRssi = NO_RSSI, mdRssi = NO_RSSI, asRssi = NO_RSSI;
    unsigned int qryAttempts = 0, qryDelayMillis = 0;
    float raw = 0, dist = 0, vari = 0, lastReported = 0, temp = 0, humidity = 0;
    unsigned long firstSeenMillis, lastSeenMillis = 0, lastReportedMillis = 0, lastQryMillis = 0;
    unsigned long seenCount = 1, lastSeenCount = 0;
    uint16_t mv = 0;
    uint8_t battery = 0xFF, addressType = 0xFF;
    FilteredDistance filteredDistance;
    std::unique_ptr<QueryReport> queryReport = nullptr;

    static bool shouldHide(const std::string &s);
    void fingerprint(NimBLEAdvertisedDevice *advertisedDevice);
    void fingerprintServiceAdvertisements(NimBLEAdvertisedDevice *advertisedDevice, size_t serviceAdvCount, bool haveTxPower, int8_t txPower);
    void fingerprintServiceData(NimBLEAdvertisedDevice *advertisedDevice, size_t serviceDataCount, bool haveTxPower, int8_t txPower);
    void fingerprintManufactureData(NimBLEAdvertisedDevice *advertisedDevice, bool haveTxPower, int8_t txPower);
};
*/
#endif
