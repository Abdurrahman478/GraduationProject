// demo : CAN-Bus Shield, send data
#include <SPI.h>

#define CAN_DATA_COUNT 0x03
#define CAN_2515
#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;
#endif

#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN);
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN);
#endif

int counts = 0;
void setup(){
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_500KBPS)){
    Serial.println("CAN init fail, retry...");
    delay(100);
  }
  Serial.println("CAN init ok !!");
}

void loop(){
  Serial.print("CAN SEND Count Data = ");
  counts = random(0,200);
  Serial.println(counts);
  uint8_t data_count[] = { CAN_DATA_COUNT, (counts >> 16) & 0xFF, (counts >> 8) & 0xFF, counts & 0xFF };
    // send data:  id = 0x70, standard frame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(0x50, 0, 4, data_count);
    delay(3000);                       // send data once per second
}