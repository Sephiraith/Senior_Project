/*
 * LED.c
 *
 *  Created on: May 31, 2020
 *      Author: Colton
 */
#include "LED.h"

void led_settings(uint8_t leds)
{
    switch(leds)
    {
    case 0: //all lights on
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, LED_ON);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, LED_ON);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, LED_ON);
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, LED_ON);
        break;
    case 1: //two outer lights on
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, LED1_OFF);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, LED_ON);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, LED3_OFF);
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, LED_ON);
        break;
    case 2: //all lights off
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, LED1_OFF);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, LED2_OFF);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, LED3_OFF);
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, LED4_OFF);
        break;
    default: //all lights off
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, LED1_OFF);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, LED2_OFF);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, LED3_OFF);
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, LED4_OFF);
        break;
    }
}
