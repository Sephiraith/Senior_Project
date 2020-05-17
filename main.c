#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

/*
 * tsl2591Defines.h
 *
 *  Created on: Apr 14, 2020
 *      Author: Colton
 */

//#ifndef STM32F4XX_STDPERIPH_DRIVER_INC_TSL2591DEFINES_H_
//#define STM32F4XX_STDPERIPH_DRIVER_INC_TSL2591DEFINES_H_

//Address defines
const uint8_t TSL_ADDR     = 0x29; //TSL2591 address
const uint8_t TSL_CMD_ADDR = 0xA0; //command register
//Register Addresses
const uint8_t EN_REG       = 0x00; //Enable register
const uint8_t CONF_REG     = 0x01; //Config/control register
const uint8_t PON          = 0x01;
const uint8_t AEN          = (0x02);
const uint8_t AIEN         = (0x10);
const uint8_t NPIEN        = (0x80);
const uint8_t ID_REG       = 0x12; //Device ID register
const uint8_t DATA0_REG    = 0x14; //low data byte for CH0
const uint8_t DATA1_REG    = 0x16; //low data byte for CH1
//Lux coefficients
const float LUX_CH0        = 1.64; //lux coefficient for channel 0
const float LUX_CH1        = 0.59; //lux coefficient for channel 1
const float LUX_CH2        = 0.86; //lux coefficient for channel 2
//Timing for integration
//less time = fast read w/ low resolution
//more time = slow read w/ higher resolution
const uint8_t IT_100MS     = 0x00;
const uint8_t IT_200MS     = 0x01;
const uint8_t IT_300MS     = 0x02;
const uint8_t IT_400MS     = 0x03;
const uint8_t IT_500MS     = 0x04;
const uint8_t IT_600MS     = 0x05;
//Gains used for sensor sensitivity
const uint8_t LOW_GAIN     = 0x00;
const uint8_t MID_GAIN     = 0x10;
const uint8_t HIGH_GAIN    = 0x20;
const uint8_t MAX_GAIN     = 0x30;

//#endif /* STM32F4XX_STDPERIPH_DRIVER_INC_TSL2591DEFINES_H_ */

void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...);
uint32_t I2CReceive(uint8_t slave_addr, uint8_t reg);
void delay_ms(uint32_t ui32Ms);
uint8_t bcd_to_dec(uint8_t value);
//float GetLux();

int main(void) {

    uint16_t data = 0;
    //uint16_t temp = 0;
    char control = 0;
    //char gain = 0;
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTCharPut(UART0_BASE, 'P');

    // I2C1 Setup - PA6 & PA7
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);         // Enable I2C module 1
        SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);          // Reset Module
        GPIOPinConfigure(GPIO_PA6_I2C1SCL);                 // Configure the pin muxing for
        GPIOPinConfigure(GPIO_PA7_I2C1SDA);                 // I2C1 function on port A6 and A7
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
        I2CSend((0x29), 2, EN_REG | TSL_CMD_ADDR, PON | AEN);
        //I2CSend((0x52), 2, EN_REG, PON | AEN);

        //setting gain
        //TSL2591_Set_Gain(MEDIUM_AGAIN);//25X GAIN
        control = I2CReceive(0x29, CONF_REG | 0xA0);
        //control = I2CReceive(0x53, CONF_REG | 0xA0);
        control &= 0xCF;
        control |= MID_GAIN;
        I2CSend(0x29, 2, (CONF_REG | TSL_CMD_ADDR), control);
        //I2CSend(0x52, 2, (CONF_REG | TSL_CMD_ADDR), control);
        //set gain to some global variable here

        //set time integration
        //TSL2591_Set_IntegralTime(ATIME_200MS);//200ms Integration time
        control = I2CReceive(0x29, CONF_REG | 0xA0);
        //control = I2CReceive(0x53, CONF_REG | 0xA0);
        control &= 0xF8;
        control |= IT_200MS;
        I2CSend(0x29, 2, (CONF_REG | TSL_CMD_ADDR), control);
        //I2CSend(0x52, 2, (CONF_REG | TSL_CMD_ADDR), control);
        //set the time to some global variable

        //Set persistence reg/filter
        //TSL2591_Write_Byte(PERSIST_REGISTER, 0x01);//filter
        I2CSend(0x29, 2, (0x0C | TSL_CMD_ADDR), 0x01);
        //I2CSend(0x52, 2, (0x0C | TSL_CMD_ADDR), 0x01);

        //finally disable
        //TSL2591_Disable();
        I2CSend((0x29), 2, EN_REG | TSL_CMD_ADDR, 0x00);
        //I2CSend((0x52), 2, EN_REG, 0x00);

        delay_ms(1000);

    while (1)
    {
        //enable
        I2CSend((0x29), 2, EN_REG | TSL_CMD_ADDR, PON | AEN);
        delay_ms(400);
        //if (UARTCharsAvail(UART0_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
        data = bcd_to_dec(I2CReceive(TSL_ADDR, DATA0_REG | TSL_CMD_ADDR));
        if(data >= 100)
        {
            UARTCharPut(UART0_BASE , ((data / 100) + 48));
        }
        UARTCharPut(UART0_BASE , (((data / 10) % 10) + 48));
        UARTCharPut(UART0_BASE , ((data % 10) + 48));
        // %10
        // /10 %10
        // /100

        //data = I2CReceive(0x52, DATA0_REG | TSL_CMD_ADDR);
        //temp = data & 0x0F;
        //UARTCharPut(UART0_BASE , ((data >> 4) + 48));
        //UARTCharPut(UART0_BASE , (temp + 48));


        UARTCharPut(UART0_BASE, ' ');

        data = bcd_to_dec(I2CReceive(TSL_ADDR, DATA1_REG | TSL_CMD_ADDR));
        //data = I2CReceive(0x52, DATA1_REG | TSL_CMD_ADDR);
        if(data >= 100)
        {
            UARTCharPut(UART0_BASE , ((data / 100) + 48));
        }
        UARTCharPut(UART0_BASE , (((data / 10) % 10) + 48));
        UARTCharPut(UART0_BASE , ((data % 10) + 48));

        UARTCharPut(UART0_BASE, '\n');
        UARTCharPut(UART0_BASE, '\r');
        //disable
        I2CSend((0x29), 2, EN_REG | TSL_CMD_ADDR, 0x00);
        delay_ms(500);
    }

}

//sends an I2C command to the specified slave
void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...)
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);

    //stores list of variable number of arguments
    va_list vargs;

    //specifies the va_list to "open" and the last fixed argument
    //so vargs knows where to start looking
    va_start(vargs, num_of_args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));

    //if there is only one argument, we only need to use the
    //single send I2C function
    if(num_of_args == 1)
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));

        //"close" variable argument list
        va_end(vargs);
    }

    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));

        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        int i = 0;
        for(i = 1; i < (num_of_args - 1); i++)
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C1_BASE));
        }

        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));

        //"close" variable args list
        va_end(vargs);
    }
}

//read specified register on slave device
uint32_t I2CReceive(uint8_t slave_addr, uint8_t reg)
{
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);

    //specify register to be read
    I2CMasterDataPut(I2C1_BASE, reg);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C1_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, true);

    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C1_BASE));

    //return data pulled from the specified register
    return I2CMasterDataGet(I2C1_BASE);
}

void delay_ms(uint32_t ui32Ms) {
    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}

uint8_t bcd_to_dec(uint8_t value){
    return value - 6 * (value >> 4);
}

//float GetLux()
//{
//    uint8_t data[2];
//    data[0]     = I2CReceive();
//    data[1]
//        //printf("Lux isn't being read properly or it is too dark.");
//    }
//    else
//    {
//        data0 = data[0];
//        data1 = data[1];
//        lux1  = (data0 - (LUX_CH0 * data1))/lux_div;
//        lux2  = ((LUX_CH1 * data0) - (LUX_CH2 * data1))/lux_div;
//    }
//
//    if(lux1 > lux2)
//    {
//        lux = lux1;
//    }
//    else
//    {
//        lux = lux2;
//    }
//
//    return round(lux);
//}

//    UARTCharPut(UART0_BASE, 'E');
//    UARTCharPut(UART0_BASE, 'n');
//    UARTCharPut(UART0_BASE, 't');
//    UARTCharPut(UART0_BASE, 'e');
//    UARTCharPut(UART0_BASE, 'r');
//    UARTCharPut(UART0_BASE, ' ');
//    UARTCharPut(UART0_BASE, 'T');
//    UARTCharPut(UART0_BASE, 'e');
//    UARTCharPut(UART0_BASE, 'x');
//    UARTCharPut(UART0_BASE, 't');
//    UARTCharPut(UART0_BASE, ':');
//    UARTCharPut(UART0_BASE, ' ');

