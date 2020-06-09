/*
 * PhotoSensor.h
 *
 *  Created on: May 29, 2020
 *      Author: Colton
 */

#ifndef PHOTOSENSOR_H_
#define PHOTOSENSOR_H_

//Includes
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"

//Address defines
#define TSL_ADDR      0x29 //TSL2591 address
#define TSL_CMD_ADDR  0xA0 //command register
//Register Addresses
#define EN_REG        0x00 //Enable register
#define CONF_REG      0x01 //Config/control register
#define PON           0x01
#define AEN           0x02
#define AIEN          0x10
#define NPIEN         0x80
#define ID_REG        0x12 //Device ID register
#define DATA0_REG     0x14 //low data byte for CH0
#define DATA0_HIGH    0x15 //high data byte for CH0
#define DATA1_REG     0x16 //low data byte for CH1
#define DATA1_HIGH    0x17 //high data byte for CH1
//Lux coefficients
#define LUX_DF        408.0//lux differential
#define LUX_CH0       1.64 //lux coefficient for channel 0
#define LUX_CH1       0.59 //lux coefficient for channel 1
#define LUX_CH2       0.86 //lux coefficient for channel 2
//Timing for integration
//less time = fast read w/ low resolution
//more time = slow read w/ higher resolution
#define IT_100MS      0x00
#define IT_200MS      0x01
#define IT_300MS      0x02
#define IT_400MS      0x03
#define IT_500MS      0x04
#define IT_600MS      0x05
//Gains used for sensor sensitivity
#define LOW_GAIN      0x00
#define MID_GAIN      0x10
#define HIGH_GAIN     0x20
#define MAX_GAIN      0x30

// Prototypes
void delay_ms(uint32_t ui32Ms);
uint8_t bcd_to_dec(uint8_t value);
uint32_t hex_to_dec(uint32_t value);
void print_integer(uint32_t number);
float GetLux(uint16_t ch0, uint16_t ch1);

#endif /* PHOTOSENSOR_H_ */

/*
 * //Address defines
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
const uint8_t DATA0_HIGH   = 0x15; //high data byte for CH0
const uint8_t DATA1_REG    = 0x16; //low data byte for CH1
const uint8_t DATA1_HIGH   = 0x17; //high data byte for CH1
//Lux coefficients
const float LUX_DF         = 408.0;
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
 *
 * */
