#include "header.h"

const uint8_t buzz[2] = {PD5, PD6};

void playTone(uint8_t buzzerpin, int frequency, int duration)
{
    long period = 1000000L / frequency;
    long cycles = (long)duration * 1000L / period;

    for (long i = 0; i < cycles; i++)
    {
        SET(PORTD, buzzerpin);
        delayMicroseconds(period / 2);
        CLR(PORTD, buzzerpin);
        delayMicroseconds(period / 2);
    }
}

void playBothTone(int frequency, int duration)
{
    long period = 1000000L / frequency;
    long cycles = (long)duration * 1000L / period;

    for (long i = 0; i < cycles; i++)
    {
        SET(PORTD, buzz[0]);
        SET(PORTD, buzz[1]);
        delayMicroseconds(period / 2);
        CLR(PORTD, buzz[0]);
        CLR(PORTD, buzz[1]);
        delayMicroseconds(period / 2);
    }
}

void wrongSelection(uint8_t buzzerPin)
{
    playTone(buzzerPin, NOTE_C4, 200);
}

void gameStart()
{
    playBothTone(NOTE_C4, 200);
    playBothTone(NOTE_D4, 200);
    playBothTone(NOTE_E4, 200);
}

void miss(uint8_t buzzerPin)
{
    playTone(buzzerPin, NOTE_G4, 500);
}

void shipHit(uint8_t buzzerPin)
{
    playTone(buzzerPin, NOTE_C5, 200);
}

void shipSunken(uint8_t buzzerPin)
{
    playTone(buzzerPin, NOTE_C5, 200);
    playTone(buzzerPin, NOTE_B4, 200);
    playTone(buzzerPin, NOTE_A4, 200);
}

void victory(uint8_t buzzerPin)
{
    playTone(buzzerPin, NOTE_C4, 300);
    playTone(buzzerPin, NOTE_E4, 300);
    playTone(buzzerPin, NOTE_G4, 300);
    playTone(buzzerPin, NOTE_C5, 600);
}
