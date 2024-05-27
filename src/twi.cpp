/* twi.c - TWI library for ATMega328P */

#include "header.h"

void twi_init(void)
{
    /* Set prescaler bits to zero (prescaler = 1) */
    TWSR = 0x00;
    /* Set bit rate register */
    TWBR = (uint8_t)TWBR_VAL;
    /* Enable TWI */
    TWCR = (1 << TWEN);
}

void twi_start(void)
{
    // Send START condition (using TWCR)
    TWCR = 0; /* reset it again, just to be sure */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    /* Wait for TWINT Flag set. This indicates that the START condition has been transmitted */
    while (!(TWCR & (1 << TWINT)))
        ;
}

void twi_write(uint8_t data)
{
    // Send a byte of data (TWCR + TWDR)
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN) /* | (1 << TODO_ACKNOWLEDGE) */;
    while (!(TWCR & (1 << TWINT)))
        ;
}

void twi_stop(void)
{
    /* Send STOP condition */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    /* Wait for STOP condition to be executed and bus released */
    while (TWCR & (1 << TWSTO))
        ;
}

uint8_t twi_discover(void)
{
    for (uint8_t i = 0x00; i < 0x7F; i++)
    {
        twi_start();
        twi_write((i << 1) | TW_WRITE);
        if ((TWSR & 0xF8) == TW_MT_SLA_ACK)
        {
            return i;
        }
        twi_stop();
    }
    return 0x00;
}