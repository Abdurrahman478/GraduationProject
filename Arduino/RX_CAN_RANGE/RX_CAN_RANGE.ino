#include <Wire.h>
#include <SPI.h>

#define CAN_DATA_COUNT 0x03

#define CAN_2515

// Set SPI CS Pin according to your hardware
const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

int counts = 0;

void setup() {
    Serial.begin(115200);
    // Print a message 
    Serial.print("DATA COUNTS");

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {  // init can bus : baudrate = 500k
        Serial.println("CAN init fail, retry...");
        delay(100);
    }
    Serial.println("CAN init ok!");
}

void loop() {
    unsigned char len = 0;
    unsigned char rxBuf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {   // check if data coming
        CAN.readMsgBuf(&len, rxBuf);  // read data, len: data length, buf: data buffer

        unsigned long canId = CAN.getCanId();

        // Accept only messages with CAN IDs between 0x100 and 0x200
        if (canId >= 0x100 && canId <= 0x200) {
            Serial.println("-----------------------------");
            Serial.print("get data from ID: 0x");
            Serial.println(canId, HEX);

            if (len < 1) { 
                return; 
            }

            // check cmd
            uint8_t cmd = rxBuf[0];

            switch (cmd) {
              case CAN_DATA_COUNT: // 3 data bytes
                {
                    if (len < 4) { break; } // if the length is less than 4, break

                    int32_t data_count = (rxBuf[1] << 16) + (rxBuf[2] << 8) + rxBuf[3];
                    
                    // check if negative
                    if (data_count & (1 << 15)) {
                        data_count = -1 * ((~data_count + 1) & 0xFFFF);
                    }
                    
                    counts = data_count;
                    Serial.print("DATA COUNTS: ");
                    Serial.println(counts, DEC);
                }
              break;
            }
        } else {
            // Ignore messages outside the range of 0x100 to 0x200
            Serial.print("Message with ID outside range: 0x");
            Serial.println(canId, HEX);
        }
    }
}
