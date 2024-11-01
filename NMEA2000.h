#ifndef NMEA2000_H_
#define NMEA2000_H_

#include <Arduino.h>


typedef struct {
    uint32_t lastReceived = 0;
    uint8_t frameCounter;
    uint8_t sequenceId;
    uint8_t bytesTotal;
    uint8_t bytesReceived;
    uint8_t *payload;
} N2KfastPacket;


bool N2KfastPacketCollector(const uint8_t data[8], N2KfastPacket *packet);


#endif // NMEA2000_H_