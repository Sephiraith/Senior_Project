/*
 * I2C.c
 *
 *  Created on: May 29, 2020
 *      Author: Colton
 */

#include "my_I2C.h"

/*
 // tm_stm32f4_i2c.c for reference
void TM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data) {
    TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    TM_I2C_WriteData(I2Cx, reg);
    TM_I2C_WriteData(I2Cx, data);
    TM_I2C_Stop(I2Cx);
}

void TM_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
    TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    TM_I2C_WriteData(I2Cx, reg);
    while (count--) {
        TM_I2C_WriteData(I2Cx, *data++);
    }
    TM_I2C_Stop(I2Cx);
}
}
 */

/********************************************************************
 * Purpose: This function establishes a connection with the I2C
 * device given an appropriate I2C base, slave address, number
 * of arguments and then the required arguments.
 *
 * Precondition: The system needs to send commands to the I2C device.
 *
 * Postcondition: The I2C device receives a given set of commands.
 *
 * ******************************************************************/
void I2CSend(uint32_t i2c_base, uint8_t slave_addr, uint8_t num_of_args, ...)
{
    // Give a i2c base and slave address for the master module
    // to transmit to
    I2CMasterSlaveAddrSet(i2c_base, slave_addr, false);

    // A list of the variable arguments passed to the function.
    va_list vargs;

    // Reads from vargs at the current place in the vargs list.
    va_start(vargs, num_of_args);

    // Send data to the FIFO
    I2CMasterDataPut(i2c_base, va_arg(vargs, uint32_t));

    // If single argument, send only one variable to I2C slave.
    if(num_of_args == 1)
    {
        // Tells the MCU to begin sending data.
        I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait for MCU to finish transferring.
        while(I2CMasterBusy(i2c_base));

        // Closes vargs list
        va_end(vargs);
    }

    // Else, start multi-byte transmission to I2C bus
    else
    {
        // Tells the MCU to begin sending data.
        I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait for MCU to finish transferring.
        while(I2CMasterBusy(i2c_base));

        // Use the BURST_SEND_CONT command of the I2C module
        // to send multiple pieces of data num_of_args - 2 times.
        int i = 0;
        for(i = 1; i < (num_of_args - 1); i++)
        {
            // Send individual pieces of data to I2C FIFO
            I2CMasterDataPut(i2c_base, va_arg(vargs, uint32_t));
            // Transfer next piece of data from I2C FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(i2c_base));
        }

        // Send last piece of data to I2C FIFO
        I2CMasterDataPut(i2c_base, va_arg(vargs, uint32_t));
        // Transfer last piece of data from I2C FIFO
        I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait for MCU to finish transferring.
        while(I2CMasterBusy(i2c_base));

        // Closes vargs list
        va_end(vargs);
    }
}

/*
 // tm_stm32f4_i2c.c for reference
uint8_t TM_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) {
    uint8_t received_data;
    TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
    TM_I2C_WriteData(I2Cx, reg);
    TM_I2C_Stop(I2Cx);
    TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_DISABLE);
    received_data = TM_I2C_ReadNack(I2Cx);
    return received_data;
}

void TM_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
    TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
    TM_I2C_WriteData(I2Cx, reg);
    //TM_I2C_Stop(I2Cx);
    TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
    while (count--) {
        if (!count) {
             //Last byte
            *data++ = TM_I2C_ReadNack(I2Cx);
        } else {
            *data++ = TM_I2C_ReadAck(I2Cx);
        }
    }
}
 */

/********************************************************************
 * Purpose: This function establishes a connection with the I2C
 * device given an appropriate I2C base, slave address, and register
 * number.
 *
 * Precondition: The system needs to read data from a chosen I2C
 * device register.
 *
 * Postcondition: The I2C device register returns the wanted data.
 *
 * ******************************************************************/
uint32_t I2CReceive(uint32_t i2c_base, uint8_t slave_addr, uint8_t reg)
{
    // Give a i2c base and slave address for the master module
    // to transmit to
    I2CMasterSlaveAddrSet(i2c_base, slave_addr, false);

    // Specify register to be read from
    I2CMasterDataPut(i2c_base, reg);

    // Send control byte and register address byte to slave device
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);

    // Wait for MCU to finish transaction
    while(I2CMasterBusy(i2c_base));

    // Specify the slave device we are reading from
    I2CMasterSlaveAddrSet(i2c_base, slave_addr, true);

    // Send control byte and read from the specified register
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);

    // Wait for MCU to finish transaction
    while(I2CMasterBusy(i2c_base));

    // Return data received from the specified register
    return I2CMasterDataGet(i2c_base);
}
