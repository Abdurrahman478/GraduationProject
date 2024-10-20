// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13

#include <Wire.h>


#include <SPI.h>

#define CAN_DATA_COUNT    0x03

#define CAN_2515
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

int counts = 0;

void setup() {
    Serial.begin(115200);
    // Print a message 
    Serial.print("DATA COUNTS");

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
        Serial.println("CAN init fail, retry...");
        delay(100);
    }
    Serial.println("CAN init ok!");
}


void loop() {
  
    unsigned char len = 0;
    unsigned char rxBuf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, rxBuf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        Serial.println("-----------------------------");
        Serial.print("get data from ID: 0x");
        Serial.println(canId, HEX);

         if (len < 1) { return; }

    // check cmd
    uint8_t cmd = rxBuf[0];

    switch (cmd) {
      case CAN_DATA_COUNT: // 3 data bytes
        {
          if (len < 4) { break; } // pb

          int32_t data_count = (rxBuf[1] << 16) + (rxBuf[2] << 8) + rxBuf[3];
        
          // check if negative
          if (data_count & (1 << 15)) {
            data_count = -1 * ((~data_count + 1) & 0xFFFF);
          }
        
          counts = data_count;
          Serial.print("DATA COUNTS : ");
          Serial.println(counts,DEC);  
        }
      break;
       
    }
    }
}