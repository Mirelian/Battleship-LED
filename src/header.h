#ifndef HEADER_H_
#define HEADER_H_

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <SPI.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#define CLR(x, y) (x &= (~(1 << y)))
#define SET(x, y) (x |= (1 << y))
#define SETT(x, y1, y2) (x |= (1 << y1) | (1 << y2))
#define View_Enemy(x, y) ((x & ~y) | (x & f1))
#define View_Self(x, y) ((y & f1) | x)
#define View_Placement(x, y) ((~x & y) | (x & ~y) | (y & f1))

#define F_SCL 100000UL
#define PRESCALER 1
#define TWBR_VAL ((((16000000UL / F_SCL) / PRESCALER) - 16) / (2 * PRESCALER))

#define LCD_ADDRESS 0x27

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

#define BTN1 980
#define BTN2 903
#define BTN3 853
#define BTN4 696
#define BTN5 512
#define BTN6 179

#define VALUE_IN_RANGE(read_value, threshold_value) \
    ((read_value - threshold_value) <= 15 || ((threshold_value - read_value) <= 15))

extern unsigned char f1;
extern unsigned char f2;

extern const uint8_t csPin;
extern const uint8_t buzz[2];

extern uint8_t nr_ships[2];
extern bool ver[2];
extern bool currentPlayer;
extern int winner;

enum GamePhase
{
    PLACE_PHASE,
    BATTLE_PHASE,
};
extern enum GamePhase gamePhase;

struct cursor
{
    uint8_t r;
    uint8_t c;
    bool viewSelf;
    bool active;
};
extern cursor P_cursor[2];

struct Ship
{
    uint8_t row;
    uint8_t col;
    uint8_t length;
    bool vertical;
    uint8_t hits;
};
extern Ship P_ships[2][5];

extern unsigned char LEDS[2][8];
extern unsigned char Ships[2][8];

void adc_init(void);
uint16_t myAnalogRead(uint8_t channel);
uint16_t buttonPress(uint8_t channel);

void playTone(uint8_t buzzerpin, int frequency, int duration);

void playBothTone(int frequency, int duration);

void wrongSelection(uint8_t buzzerPin);

void gameStart();

void miss(uint8_t buzzerPin);

void shipHit(uint8_t buzzerPin);

void shipSunken(uint8_t buzzerPin);

void victory(uint8_t buzzerPin);

void inputP1();
void inputP2();

void action(uint8_t player, uint16_t command);

bool checkMovement(uint8_t player, uint8_t r_dist, uint8_t c_dist);
bool checkShipPlacement(uint8_t player, uint8_t row, uint8_t col, uint8_t length);

void placeShip(uint8_t player);
void rotateShip(uint8_t player, uint8_t length);
void checkHit(uint8_t player);

void updatePlayerTurn();

void displayPlacePhase();
void displayBattlePhase();

void lcd_send_command(uint8_t command);

void lcd_send_data(uint8_t data);

void lcd_init(void);

void lcd_set_cursor(uint8_t row, uint8_t col);

void lcd_print(char *str);

void initMax7219();
void sendToMaxAll(byte address, byte data1, byte data2);

void timer1_init();
void timer2_init();

void twi_init(void);
void twi_start(void);
void twi_write(uint8_t data);
void twi_stop(void);
uint8_t twi_discover(void);

#endif
