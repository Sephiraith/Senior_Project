/*
 * setup.h
 *
 *  Created on: Jun 1, 2020
 *      Author: Colton
 */

#ifndef SETUP_H_
#define SETUP_H_

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
// user includes
#include "LCD.h"
#include "RTC.h"
#include "my_I2C.h"
#include "PhotoSensor.h"

//defines
#define PUMP_ON  1
#define PUMP_OFF 0

// Prototypes
void Setup(void);       // Initializes the system
void PSsetup(void);     // Initializes TSL2591 photo-sensor
void LCDSetup(void);    // Initializes the LCD
void RTCsetup(void);    // Initializes the RTC
void LEDsetup(void);    // Initializes the LEDs
void WPSsetup(void);    // Initializes the water level sensor and water pump
void ButtonSetup(void); // Initializes the buttons

// RTC enums
enum days{SAT = 0, SUN, MON, TUE, WED, THU, FRI};
enum minutes{TOP = 0x00, QUARTER = 0x15, HALF = 0x30, BOTTOM = 0x45};
enum hours{ONE = 0x01, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN = 0x10, ELEVEN = 0x11, TWELVE = 0x12};
enum months{JAN = 0x01, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT = 0x10, NOV = 0x11, DEC = 0x12};

#endif /* SETUP_H_ */
