/*
 * setup.c
 *
 *  Created on: Jun 1, 2020
 *      Author: Colton
 */
#include "setup.h"

void Setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    LCDSetup();

    PSsetup();

    RTCsetup();

    LEDsetup();

    WPSsetup();

    ButtonSetup();
}

void LCDSetup(void)
{

        // GPIO setup - LCD screen
        // Data Bus J4
        GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
        GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

        // Register Select and R/W
        GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
        // Enable
        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
}

void PSsetup(void)
{
    char control = 0;

    // I2C1 Setup - PA6 & PA7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);         // Enable I2C module 1
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);          // Reset Module
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);                 // Configure the pin muxing for -
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);                 // -I2C1 function on port A6 and A7
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);     // Select I2C functions for these pins
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    //GPIOPadConfigSet(uint32_t ui32Port, uint8_t ui8Pins, uint32_t ui32Strength, GPIO_PIN_TYPE_STD);

    // Enable/Init I2C1, use SysClock
    // false = 100kbps, true = 400 kbps
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);

    HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;        // clear I2C FIFOs

    //I2CSend((TSL_ADDR), 2, 0x80, 0x20);
    //I2CSend((TSL_CMD_ADDR), 2, EN_REG, CONF_REG | AEN);
    //enable TSL2591
    //TSL2591_Enable();
    I2CSend(I2C1_BASE, TSL_ADDR, 2, EN_REG | TSL_CMD_ADDR, PON | AEN);
    //I2CSend((0x52), 2, EN_REG, PON | AEN);

    //setting gain
    //TSL2591_Set_Gain(MEDIUM_AGAIN);//25X GAIN
    control = I2CReceive(I2C1_BASE, TSL_ADDR, CONF_REG | TSL_CMD_ADDR);
    //control = I2CReceive(0x53, CONF_REG | 0xA0);
    control &= 0xCF;
    control |= MID_GAIN;
    I2CSend(I2C1_BASE, TSL_ADDR, 2, (CONF_REG | TSL_CMD_ADDR), control);
    //I2CSend(0x52, 2, (CONF_REG | TSL_CMD_ADDR), control);
    //set gain to some global variable here

    //setting time integration
    //TSL2591_Set_IntegralTime(ATIME_200MS);//200ms Integration time
    control = I2CReceive(I2C1_BASE, TSL_ADDR, CONF_REG | TSL_CMD_ADDR);
    //control = I2CReceive(0x53, CONF_REG | 0xA0);
    control &= 0xF8;
    control |= IT_200MS;
    I2CSend(I2C1_BASE, TSL_ADDR, 2, (CONF_REG | TSL_CMD_ADDR), control);
    //I2CSend(0x52, 2, (CONF_REG | TSL_CMD_ADDR), control);
    //set the time to some global variable

    //Set persistence reg/filter
    //TSL2591_Write_Byte(PERSIST_REGISTER, 0x01);//filter
    I2CSend(I2C1_BASE, TSL_ADDR, 2, (0x0C | TSL_CMD_ADDR), CONF_REG);
    //I2CSend(0x52, 2, (0x0C | TSL_CMD_ADDR), 0x01);

    //finally disable
    //TSL2591_Disable();
    I2CSend(I2C1_BASE, TSL_ADDR, 2, EN_REG | TSL_CMD_ADDR, EN_REG);
    //I2CSend((0x52), 2, EN_REG, 0x00);
}

void RTCsetup(void)
{
    //RTC SCL(PE4) and SDA(PE5)
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);         // Enable I2C module 2
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);          // Reset Module
    GPIOPinConfigure(GPIO_PE4_I2C2SCL);                 // Configure the pin muxing for
    GPIOPinConfigure(GPIO_PE5_I2C2SDA);                 // I2C1 function on port E4 and E5
    GPIOPinTypeI2CSCL(GPIO_PORTE_BASE, GPIO_PIN_4);     // Select I2C functions for these pins
    GPIOPinTypeI2C(GPIO_PORTE_BASE, GPIO_PIN_5);

    I2CSend(I2C2_BASE, RTC_ADDR, 2,  RTC_SEC,     TOP);            // 0 seconds
    I2CSend(I2C2_BASE, RTC_ADDR, 2,  RTC_MIN,     TOP);            // 0 minutes
    I2CSend(I2C2_BASE, RTC_ADDR, 2,  RTC_HOUR,    ONE);            // 1 hour
    I2CSend(I2C2_BASE, RTC_ADDR, 2,  RTC_WKDAY,   TUE);            // Tuesday
    I2CSend(I2C2_BASE, RTC_ADDR, 2,  RTC_DATE,    0x02);           // The 2nd
    I2CSend(I2C2_BASE, RTC_ADDR, 2,  RTC_MONTH,   JUN);            // June
    I2CSend(I2C2_BASE, RTC_ADDR, 2,  RTC_YEAR,    0x20);           // 2020
}

void LEDsetup(void)
{
    //LED control pins
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
}
void ButtonSetup(void)
{
    //button read pins
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}
void WPSsetup(void)
{
    // Water pump control pin
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
    // Water level sensor(s) control pins
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 8);
}
