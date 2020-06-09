/*
 * LCD.c
 *
 *  Created on: May 25, 2020
 *      Author: Colton
 */
#include "LCD.h"

void delay_us(uint32_t ui32Us)
{
    SysCtlDelay(ui32Us * (SysCtlClockGet() / 3 / 1000000));
}

/*********************************
 * Writes the data to the LCD bus
 * *******************************/

void setLCDBus(uint8_t data)
{
    uint8_t temp = 0;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 8);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    }

    data >>= 1;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 16);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
    }

    data >>= 1;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 32);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
    }

    data >>= 1;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 64);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
    }

    data >>= 1;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 128);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
    }

    data >>= 1;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 64);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0);
    }

    data >>= 1;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, 128);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, 0);
    }

    data >>= 1;
    temp = (data & 0x01);
    if(temp)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
    }
    // GPIO setup - LCD screen
    // Data Bus J4
//    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
//    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
//    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
}

void SetWrite(bool en, bool rs)
{
    if(en)
    {
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 16);  // set enable high
    }
    else
    {
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);   // set enable low
    }
    if(rs)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 64);  // Set register select high
    }
    else
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);   // Set register select low
    }

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0);   // Set R/W low
}

void States(uint8_t state)
{
    switch(state)
    {
    case 0:
        SetWrite(1, 0);         // sets the enable pin high
        delay_us(2);            // Delay needed for setup
        setLCDBus(COM_FUNC);    // Tells the LCD to operate in 8-bit single line and 5x8 dot mode
        delay_us(2);            // Delay needed for setup
        SetWrite(0, 0);         // sets the enable pin low
        break;
    case 1:
        SetWrite(1, 0);         // sets the enable pin high
        delay_us(2);            // Delay needed for setup
        setLCDBus(COM_CLR);     // Clears the display
        delay_us(2);            // Delay needed for setup
        setLCDBus(COM_CHOME);   // Returns the cursor back to it's starting address
        delay_us(2);            // Delay needed for setup
        SetWrite(0, 0);         // sets the enable pin low
        break;
    case 2:
        SetWrite(1, 0);         // sets the enable pin high
        delay_us(2);            // Delay needed for setup
        setLCDBus(COM_DC);      // Turns on the LCD display and cursor
        delay_us(2);            // Delay needed for setup
        SetWrite(0, 0);         // sets the enable pin low
        break;
    case 3:
        SetWrite(1, 0);         // sets the enable pin high
        delay_us(2);            // Delay needed for setup
        setLCDBus(COM_EMR);     //
        delay_us(2);            // Delay needed for setup
        SetWrite(0, 0);         // sets the enable pin low
        break;
    case 4:
        SetWrite(1, 0);         // sets the enable pin high
        delay_us(2);            // Delay needed for setup
        setLCDBus(COM_L1);      // Sets the cursor/DDRAM address to line 1
        delay_us(2);            // Delay needed for setup
        SetWrite(0, 0);         // sets the enable pin low
        break;
    case 5:
        SetWrite(1, 1);         // sets the enable and register select pins high
        delay_us(2);            // Delay needed for setup
        setLCDBus(LCDA);        // Sets the cursor/DDRAM address to line 1
        delay_us(2);            // Delay needed for setup
        SetWrite(0, 0);         // sets the enable and register select pins low
        break;
    }

}
