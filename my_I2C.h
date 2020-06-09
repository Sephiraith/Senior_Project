/*
 * I2C.h
 *
 *  Created on: May 29, 2020
 *      Author: Colton Morris
 */

#ifndef MY_I2C_H_
#define MY_I2C_H_

// includes
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
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

// Prototypes
uint32_t I2CReceive(uint32_t i2c_base, uint8_t slave_addr, uint8_t reg);
void     I2CSend(uint32_t i2c_base, uint8_t slave_addr, uint8_t num_of_args, ...);

//uint32_t I2C2Receive(uint8_t slave_addr, uint8_t reg);
//void I2C2Send(uint8_t slave_addr, uint8_t num_of_args, ...);

#endif /* MY_I2C_H_ */
