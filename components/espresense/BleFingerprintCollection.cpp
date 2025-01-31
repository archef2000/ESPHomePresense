#include "BleFingerprintCollection.h"

#include <sstream>

namespace BleFingerprintCollection {
// Public (externed)
std::string include{DEFAULT_INCLUDE},
            exclude{DEFAULT_EXCLUDE},
            query{DEFAULT_QUERY},
            knownMacs{DEFAULT_KNOWN_MACS},
            knownIrks{DEFAULT_KNOWN_IRKS},
            countIds{DEFAULT_COUNT_IDS};
float skipDistance = DEFAULT_SKIP_DISTANCE,
      maxDistance = DEFAULT_MAX_DISTANCE,
      absorption = DEFAULT_ABSORPTION,
      countEnter = DEFAULT_COUNT_ENTER,
      countExit = DEFAULT_COUNT_EXIT;
int8_t rxRefRssi = DEFAULT_RX_REF_RSSI,
       rxAdjRssi = DEFAULT_RX_ADJ_RSSI,
       txRefRssi = DEFAULT_TX_REF_RSSI;
int forgetMs = DEFAULT_FORGET_MS,
    skipMs = DEFAULT_SKIP_MS,
    countMs = DEFAULT_COUNT_MS,
    requeryMs = DEFAULT_REQUERY_MS;


unsigned long lastCleanup = 0;

}  // namespace BleFingerprintCollection
