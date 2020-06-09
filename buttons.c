/*
 * buttons.c
 *
 *  Created on: Jun 2, 2020
 *      Author: Colton
 */
#include "buttons.h"

uint8_t PollButtons(void)
{
    uint8_t ret_val = 0;
    if(GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0))
    {
        ret_val = 1;
    }
    else if(GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1))
    {
        ret_val = 2;
    }
    else
    {
        ret_val = 0;
    }

    delay_ms(200);
    return ret_val;
}
