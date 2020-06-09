/*
 * RTC.h
 *
 *  Created on: May 29, 2020
 *      Author: Colton
 */

#ifndef RTC_H_
#define RTC_H_

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

//defines
#define RTC_SEC          0x00
#define RTC_MIN          0x01
#define RTC_HOUR         0x02
#define RTC_WKDAY        0x03
#define RTC_DATE         0x04
#define RTC_MONTH        0x05
#define RTC_YEAR         0x06
#define RTC_ADDR         0x68
#define STATUS_REG       0x0F
#define CONTROL_REG      0x0E

//enum days{SAT = 0, SUN, MON, TUE, WED, THU, FRI};
//enum minutes{TOP = 0x00, QUARTER = 0x15, HALF = 0x30, BOTTOM = 0x45};
//enum hours{ONE = 0x01, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN = 0x10, ELEVEN = 0x11, TWELVE = 0x12};
//enum months{JAN = 0x01, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT = 0x10, NOV = 0x11, DEC = 0x12};



#endif /* RTC_H_ */
