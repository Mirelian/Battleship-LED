#include "header.h"

unsigned char f1 = 0xFF; // ship flicker
unsigned char f2 = 1;    // cursor flicker

void timer1_init()
{
    // Clear control registers
    TCCR1A = 0;
    TCCR1B = 0;

    // Set up timer with prescaler = 64
    TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
    TCCR1B |= (1 << CS12);  // Prescaler set to 256

    // Set OCR1A value for a 50 ms interval
    OCR1A = 3124;

    // Enable Timer1 Compare Match A Interrupt
    TIMSK1 |= (1 << OCIE1A);
}

void timer2_init()
{
    // Clear control registers
    TCCR2A = 0;
    TCCR2B = 0;

    // Set up timer with prescaler = 1024
    TCCR2A |= (1 << WGM21);                            // Configure timer 2 for CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler set to 1024

    // Set OCR2A value for a 1000 ms interval
    OCR2A = 155; // (16000000 / (1024 * 1)) / 256 - 1, approximate calculation

    // Enable Timer2 Compare Match A Interrupt
    TIMSK2 |= (1 << OCIE2A);
}

ISR(TIMER2_COMPA_vect)
{
    f1 = ~f1;
}

ISR(TIMER1_COMPA_vect)
{
    f2 ^= 1;
}