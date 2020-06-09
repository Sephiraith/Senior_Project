// Library includes
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
//#include <stdarg.h>
#include <stdbool.h>
#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"

// User-made includes
#include "LCD.h"
#include "LED.h"
#include "RTC.h"
#include "setup.h"
#include "my_I2C.h"
#include "buttons.h"
#include "PhotoSensor.h"

uint8_t CheckPhotoSensor(void);         // Checks the light level to see if the LEDs need to turn off
void    CreatePreselects(void);         // Creates the preselects
void    PumpControl(uint8_t pump_state);// controls whether the pump is on or off
void    uartPutStr(const char *str);    // Prints strings using UARTCharPut
void    readRTC(void);                  // Reads the current hour on the RTC

// preselect struct
struct options {
   char     plant[20];  // Plant name
   uint8_t  lights;     // Intensity of light (2 or 4 bulbs on)
   uint8_t  light_timer;// Number of hours of needed light
   uint8_t  water;      // Number of hours of water needed
};

// global volatile variables
struct options preselects[10]; //array of preselects
uint8_t index           = 0;   // struct index

int main(void) {

    enum states{IDLE, HOME, WATER, PRESELECTS}; // States for main bare-metal OS
    enum buttons{MENU = 1, OPTIONS};    // Cases for button selections

    uint8_t count            = 0;   // timer for preselect menu inactivity
    uint8_t state            = IDLE;// current state
    uint8_t print            = 0;   // print flag
    uint8_t water_level      = 1;   // water sensor flag
    uint8_t light_level      = 0;   // photo sensor flag
    char reservoir_state[10];       // current reservoir status

    //uint8_t check = 0;

    strcpy(reservoir_state, "Good");

    // Initialize the system and peripherals
    Setup();

    // Creates the preselects
    CreatePreselects();

    //turns on the pump and lights for demo
    led_settings(1);
    PumpControl(PUMP_ON);

    // delay needed after setup and before commands can be written
    delay_ms(1000);

    while (1)
    {
        switch(state)
        {
        case IDLE:
            if(print == 0)
            {
                //UARTprintf("\033[2J\033[HEnter Text: "); //erase screen, cursor at (0,0)
                //UARTCharPut(UART0_BASE, '\n');
                uartPutStr("Plant Name/Preselect: ");
                uartPutStr(preselects[index].plant);
                uartPutStr("Reservoir Status: ");
                uartPutStr(reservoir_state);
                uartPutStr("Press '2' for preselects");
                UARTCharPut(UART0_BASE, '\n');
                UARTCharPut(UART0_BASE, '\r');
                print = 1;
                count = 0;
            }

            light_level = CheckPhotoSensor(); //returns an integer value for current light level
            water_level = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2); // returns a 0 or 4
            //check = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
            if(water_level == 0 || light_level == 1)
            {
                state = HOME;
            }
            else if(PollButtons() == 2)
            {
                state = PRESELECTS;
            }
            readRTC();
            break;
        case HOME:
            //1 means that the light level has gotten above 2500 lumens/m^2
            if(light_level == 1)
            {
                led_settings(2);
            }
            else
            {
                led_settings(0);
            }
            // 0 means that the water level sensor has fallen below safe pump operating levels
            if(water_level == 0)
            {
                PumpControl(PUMP_OFF);
                strcpy(reservoir_state, "Refill");
            }
            else
            {
                PumpControl(PUMP_ON);
                strcpy(reservoir_state, "Good");
            }
            // set print back to 0 to reprint the current status in IDLE
            print = 0;
            state = IDLE;
            break;
        case WATER:
            //comes in here to water the plant for a period of time each day.
            break;
        case PRESELECTS:
            //scroll through preselects and then return to main menu
            //delay_ms(50);
            uartPutStr("Press '1' for Main Menu");
            uartPutStr("Press '2' for Preselects");
            uartPutStr("Current Plant Name/Preselect: ");
            uartPutStr(preselects[index].plant);
            do{
                switch(PollButtons())
                {
                case MENU:
                    state = IDLE;
                    break;
                case OPTIONS:
                    ++index;
                    if(index > 9)
                        index = 0;
                    count = 0;
                    delay_ms(50);
                    break;
                default:
                    ++count;
                    delay_ms(1000);
                    break;
                }
                uartPutStr("Current Plant Name/Preselect: ");
                uartPutStr(preselects[index].plant);
                UARTCharPut(UART0_BASE, '\r');
            }while(state == PRESELECTS && count != 10);
            print = 0;
            //display preselect
            break;
        default:
            state = IDLE;
            break;
        }

        delay_ms(500);
    }
    //LCD state machine went here
//    States(0); // State 0
//    States(1); // State 1
//    States(2); // State 2
//    States(3); // State 3
//    States(4); // State 4
//    States(5); // State 5
}

uint8_t CheckPhotoSensor(void)
{
    // stuff for getting lux
    uint32_t check  = 0;
    uint8_t ret_val = 0;
    uint8_t quarter = 0;
    uint16_t half   = 0;
    uint32_t full   = 0;

    I2CSend(I2C1_BASE, TSL_ADDR, 2, (EN_REG | TSL_CMD_ADDR), (PON | AEN));
    delay_ms(400);
    UARTCharPut(UART0_BASE, 'L');
    quarter = I2CReceive(I2C1_BASE, TSL_ADDR, DATA0_REG | TSL_CMD_ADDR);
    half    = I2CReceive(I2C1_BASE, TSL_ADDR, DATA0_HIGH | TSL_CMD_ADDR);
    half  <<= 8;
    half   |= quarter;
    full    = half;
    full  <<= 16;

    quarter = I2CReceive(I2C1_BASE, TSL_ADDR, DATA1_REG | TSL_CMD_ADDR);
    half    = I2CReceive(I2C1_BASE, TSL_ADDR, DATA1_HIGH | TSL_CMD_ADDR);
    half  <<= 8;
    half   |= quarter;
    full   |= half;

    print_integer(GetLux(((full & 0xffff0000) >> 16), (full & 0xffff)));

    UARTCharPut(UART0_BASE, ' ');

    //UARTCharPut(UART0_BASE, '\n');
    UARTCharPut(UART0_BASE, '\r');
    //disable
    I2CSend(I2C1_BASE, TSL_ADDR, 2, EN_REG | TSL_CMD_ADDR, 0x00);
    delay_ms(500);

    check = GetLux(((full & 0xffff0000) >> 16), (full & 0xffff));

    if(2500 < check)
    {
        ret_val = 1;
    }

    return ret_val;
}

void readRTC(void)
{
    uint8_t hour = 0;
    hour   = bcd_to_dec(I2CReceive(I2C2_BASE, RTC_ADDR, RTC_HOUR));// Hour 19

    if(hour == 18) // = 6pm
    {
        //turn on pump
        PumpControl(PUMP_ON);
    }
    else if(hour == 10) // = 10am
    {
        //turn on lights
        led_settings(0);
    }
    if((10 + preselects[index].light_timer) == hour) //18 = 6pm
    {
        //turn off lights
        led_settings(2);
    }
    else if((10 + preselects[index].light_timer) == hour) //17 = 5pm
    {
        //turn off lights
        led_settings(2);
    }
    else if((10 + preselects[index].light_timer) == hour) //16 = 4pm
    {
        //turn off lights
        led_settings(2);
    }
    else if((10 + preselects[index].light_timer) == hour) //15 = 3pm
    {
        //turn off lights
        led_settings(2);
    }
    else if((10 + preselects[index].light_timer) == hour) //14 = 2pm
    {
        //turn off lights
        led_settings(2);
    }
    else if((10 + preselects[index].light_timer) == hour) //13 = 1pm
    {
        //turn off lights
        led_settings(2);
    }

    if((15 + preselects[index].water) == hour) //21 = 9pm
    {
        //turn off pump
        PumpControl(PUMP_OFF);
    }
    else if((15 + preselects[index].water) == hour) //19 = 7pm
    {
        //turn off pump
        PumpControl(PUMP_OFF);
    }

}

void uartPutStr(const char *str)
{
    while(*str) UARTCharPut(UART0_BASE, *str++);
    UARTCharPut(UART0_BASE, ' ');
    //UARTCharPut(UART0_BASE, '\n');
    //UARTCharPut(UART0_BASE, '\r');
}

void CreatePreselects(void)
{

    // NOTE: Should code in some number
    // of days a plant prefers to be watered later.
    // water pump has a rate of 240L/hour which is ~63Gal/hour
    // mister restricts it to about 4.5Gal/hour
    uint8_t plant_num = 0; //0
    // basil
    strcpy(preselects[plant_num].plant, "Basil       ");
    preselects[plant_num].lights        = 0;
    preselects[plant_num].light_timer   = 6;
    preselects[plant_num].water         = 3;

    ++plant_num; //1
    // Strawberry
    strcpy(preselects[plant_num].plant, "Strawberry  ");
    preselects[plant_num].lights        = 0;
    preselects[plant_num].light_timer   = 8;
    preselects[plant_num].water         = 3;

    ++plant_num; //2
    // Parsley
    strcpy(preselects[plant_num].plant, "Parsley     ");
    preselects[plant_num].lights        = 0;
    preselects[plant_num].light_timer   = 5;
    preselects[plant_num].water         = 3;

    ++plant_num; //3
    // Orchid
    strcpy(preselects[plant_num].plant, "Orchid      ");
    preselects[plant_num].lights        = 1;
    preselects[plant_num].light_timer   = 3;
    preselects[plant_num].water         = 1;

    ++plant_num; //4
    // Exotic Angel
    strcpy(preselects[plant_num].plant, "Exotic Angel");
    preselects[plant_num].lights        = 1;
    preselects[plant_num].light_timer   = 4;
    preselects[plant_num].water         = 1;

    ++plant_num; //5
    // Bamboo
    strcpy(preselects[plant_num].plant, "Bamboo      ");
    preselects[plant_num].lights        = 1;
    preselects[plant_num].light_timer   = 3;
    preselects[plant_num].water         = 1;

    ++plant_num; //6
    // Aloe Vera
    strcpy(preselects[plant_num].plant, "Aloe Vera   ");
    preselects[plant_num].lights        = 0;
    preselects[plant_num].light_timer   = 7;
    preselects[plant_num].water         = 1;

    ++plant_num; //7
    // Green Onions
    strcpy(preselects[plant_num].plant, "Green Onions");
    preselects[plant_num].lights        = 0;
    preselects[plant_num].light_timer   = 5;
    preselects[plant_num].water         = 1;

    ++plant_num; //8
    // Cilantro
    strcpy(preselects[plant_num].plant, "Cilantro    ");
    preselects[plant_num].lights        = 0;
    preselects[plant_num].light_timer   = 6;
    preselects[plant_num].water         = 1;

    ++plant_num; //9
    // Thyme
    strcpy(preselects[plant_num].plant, "Thyme       ");
    preselects[plant_num].lights        = 0;
    preselects[plant_num].light_timer   = 5;
    preselects[plant_num].water         = 1;
}

void PumpControl(uint8_t pump_state)
{
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, pump_state);
}
