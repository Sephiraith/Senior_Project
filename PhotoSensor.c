/*
 * PhotoSensor.c
 *
 *  Created on: May 29, 2020
 *      Author: Colton
 */
#include "PhotoSensor.h"

void delay_ms(uint32_t ui32Ms) {
    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}

uint8_t bcd_to_dec(uint8_t value){
    return value - 6 * (value >> 4);
}

uint32_t hex_to_dec(uint32_t value)
{
    int i = 0;
    int is1 = 0;
    uint32_t sum = 0;
    while(i < 32)
    {
        is1 = ((value >> i) & 0x00000001);
        if(is1 == 1)
        {
            sum += pow(2, i);
        }
        ++i;
    }

    return sum;
}

void print_integer(uint32_t number)
{
    char ascii[10];
    uint32_t temp = number;
    int checker = 9;
    int i = 0;
    char n;
    for(i = 0; i < 10; ++i)
    {
        ascii[i] = ((temp % 10) + 48);

        temp /= 10;
    }
    n = ascii[checker];
    while(n == '0')
    {
        --checker;
        n = ascii[checker];
    }

    for(i = checker; i >= 0; --i)
    {
        UARTCharPut(UART0_BASE , ascii[i]);
    }
}


float GetLux(uint16_t ch0, uint16_t ch1)
{
    float atime = 200.0;
    float again = 25.0;
    float cpl, lux;

    // Check for overflow conditions first
    if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF)) {
      // Signal an overflow
      return -1;
    }

    cpl = (atime * again) / LUX_DF;

    lux = (((float)ch0 - (float)ch1)) * (1.0F - ((float)ch1 / (float)ch0)) / cpl;

    return lux;
}
