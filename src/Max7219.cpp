#include "header.h"

const uint8_t csPin = PB2;

void initMax7219()
{
    sendToMaxAll(0x0C, 0x01, 0x01); // Normal operation mode
    sendToMaxAll(0x09, 0x00, 0x00); // No decoding
    sendToMaxAll(0x0B, 0x07, 0x07); // Scan limit = 7 (all rows)
    sendToMaxAll(0x0A, 0x08, 0x08); // Intensity (range is 0x00 to 0x0F)
    sendToMaxAll(0x0F, 0x00, 0x00); // Display test: off
}

void sendToMaxAll(byte address, byte data1, byte data2)
{
    CLR(PORTB, csPin);
    SPI.transfer(address); // Send the address
    SPI.transfer(data1);
    SPI.transfer(address); // Send the address
    SPI.transfer(data2);   // Followed by the data
    SET(PORTB, csPin);
}