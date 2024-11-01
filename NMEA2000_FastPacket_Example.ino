#include "mcp2515.h"
#include "NMEA2000.h"

#define CAN_CS_PIN 10
#define PGN_NMEA2000_ENGINE_PARAMETERS2 0x1F201

// Constructors for CAN module communication
MCP2515 CAN(CAN_CS_PIN);  // CAN class - 250kbps NMEA 2000

// CAN msg
struct can_frame rxMsg;  // Struct to hold received CAN messages

// Prepare structs and arrays to hold NMEA 2000 FastPacket data
N2KfastPacket fastPacket_1F201;
uint8_t payload_1F201[26];  // Make sure array is sam size as FastPacket payload data


void setup() {
  // Start serial for "debugging"
  Serial.begin(115200);
  // Setup CAN
  CAN.reset();
  CAN.setBitrate(CAN_250KBPS, MCP_16MHZ);
  CAN.setConfigMode();
  CAN.setFilterMask(MCP2515::MASK0, true, 0x1FFFF00);
  CAN.setFilterMask(MCP2515::MASK1, true, 0x1FFFF00);
  CAN.setFilter(MCP2515::RXF0, true, 0x1F20100);
  CAN.setNormalMode();
  // NMEA 2000 FastPacket, add payload array to struct
  fastPacket_1F201.payload = payload_1F201;
}


void loop() {
  rxCanMsg();
}


void rxCanMsg() {
  if (CAN.readMessage(&rxMsg) == MCP2515::ERROR_OK) {
    uint32_t pgn = (rxMsg.can_id >> 8) & 0x1FFFF;
    switch (pgn) {
      case PGN_NMEA2000_ENGINE_PARAMETERS2:
        {
          if (N2KfastPacketCollector(rxMsg.data, &fastPacket_1F201)) N2KfastPacketHandler(pgn);
          break;
        }
      default:
        break;
    }
  }
}


void N2KfastPacketHandler(uint32_t pgn) {
  switch (pgn) {
    case PGN_NMEA2000_ENGINE_PARAMETERS2:
      {
        // Call payload data handler
        for (int i = 0; i < 26; i++) {
          Serial.print(fastPacket_1F201.payload[i], HEX); Serial.print(" ");
        }
        Serial.println();
        break;
      }
    default:
      break;
  }
}
