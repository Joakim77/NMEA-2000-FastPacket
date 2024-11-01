#include "Nmea2000.h"


bool N2KfastPacketCollector(const uint8_t data[8], N2KfastPacket *fp) {
  // Get sequece Id and frame counter
  uint8_t sequenceId = (data[0] >> 5) & 0x3;
  uint8_t frameCounter = data[0] & 0x1F;
  // First frame
  if (frameCounter == 0) {
    // Max allowed bytes = 223
    if (data[1] > 223) return false;
    // Set frame info
    fp->frameCounter = 0;
    fp->sequenceId = sequenceId;
    fp->bytesTotal = data[1];
    fp->bytesReceived = 6;
    // Copy payload data
    memcpy(fp->payload, &data[2], 6);
  }
  // Consecutive frames
  else if (frameCounter == fp->frameCounter + 1 && sequenceId == fp->sequenceId) {
    // Remaning bytes
    uint8_t remaningBytes = 7;  // Default to 7 bytes since this is max for consecutive frames
    // Check if last frame
    if (fp->bytesReceived + remaningBytes >= fp->bytesTotal) {
      // Update remaning bytes
      remaningBytes = fp->bytesTotal - fp->bytesReceived;
    }
    // Add payload data from this frame
    memcpy(fp->payload + fp->bytesReceived, &data[1], remaningBytes);
    // Update received byte count
    fp->bytesReceived += remaningBytes;
    // Update frameCounter
    fp->frameCounter = frameCounter;
    // Check if all bytes received
    if (fp->bytesTotal == fp->bytesReceived) {
      fp->lastReceived = millis();
      return true;
    }
  }
// All frames not received
return false;
}
