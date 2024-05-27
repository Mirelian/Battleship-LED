#include "header.h"

void adc_init(void)
{
    //(pg. 260) Set ADC prescaler to 128 (don't need higher precision for this project)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    //(pg. 258) Set ADC reference voltage to AVCC
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    ADMUX &= ~(1 << ADLAR);

    //(pg. 260) Enable ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t myAnalogRead(uint8_t channel)
{
    // Force input channel to be between 0 and 7 (as ADC pins are PA0-7)
    channel &= 0b00000111;

    // 1. Clear the old channel value (last 5 bits in ADMUX)
    ADMUX &= 0xF8;

    // 2. Select the new channel in ADMUX
    ADMUX |= channel;

    // 3. Start single conversion
    ADCSRA |= (1 << ADSC);

    // 4. Busy wait for conversion to complete
    while (ADCSRA & (1 << ADSC))
        ;

    // Return ADC value
    return (ADC);
}

uint16_t buttonPress(uint8_t channel)
{
    uint16_t value = myAnalogRead(channel);
    if (VALUE_IN_RANGE(value, BTN1))
        return BTN1;
    else if (VALUE_IN_RANGE(value, BTN2))
        return BTN2;
    else if (VALUE_IN_RANGE(value, BTN3))
        return BTN3;
    else if (VALUE_IN_RANGE(value, BTN4))
        return BTN4;
    else if (VALUE_IN_RANGE(value, BTN5))
        return BTN5;
    else if (VALUE_IN_RANGE(value, BTN6))
        return BTN6;
    else
        return 0xFFFF;
}