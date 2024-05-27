#include "header.h"

void lcd_send(uint8_t value, uint8_t mode)
{
    uint8_t highNib = value & 0xF0;
    uint8_t lowNib = (value << 4) & 0xF0;

    // Send high nibble
    twi_start();
    twi_write(LCD_ADDRESS << 1);
    twi_write(mode | highNib | 0x0C); // En=1, Rs/Rw as per mode
    twi_write(mode | highNib | 0x08); // En=0, Rs/Rw as per mode
    twi_stop();

    // Send low nibble
    twi_start();
    twi_write(LCD_ADDRESS << 1);
    twi_write(mode | lowNib | 0x0C); // En=1, Rs/Rw as per mode
    twi_write(mode | lowNib | 0x08); // En=0, Rs/Rw as per mode
    twi_stop();
}

void lcd_send_command(uint8_t command)
{
    lcd_send(command, 0x00);
}

void lcd_send_data(uint8_t data)
{
    lcd_send(data, 0x01); // Rs=1 for data
}

void lcd_init()
{
    _delay_ms(50); // Wait for more than 40 ms after VCC rises to 2.7V

    lcd_send_command(0x03);
    _delay_ms(5);
    lcd_send_command(0x03);
    _delay_us(150);
    lcd_send_command(0x03);
    _delay_ms(2);
    lcd_send_command(0x02); // Set to 4-bit mode
    _delay_ms(2);

    lcd_send_command(0x28); // Function set: 2 line, 5x8 dot
    _delay_ms(2);
    lcd_send_command(0x08); // Display off
    _delay_ms(2);
    lcd_send_command(0x01); // Display clear
    _delay_ms(2);
    lcd_send_command(0x06); // Entry mode set: increment cursor, no shift
    _delay_ms(2);
    lcd_send_command(0x0C); // Display on, cursor off, blink off
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    address += col;
    lcd_send_command(address);
}

void lcd_print(char *str)
{
    while (*str)
    {
        lcd_send_data(*str++);
    }
}