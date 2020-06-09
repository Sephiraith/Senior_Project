/*
 * LED.h
 *
 *  Created on: May 30, 2020
 *      Author: Colton
 */

#ifndef LED_H_
#define LED_H_

// Includes
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

//Defines
#define LED1_OFF 4
#define LED2_OFF 4
#define LED3_OFF 8
#define LED4_OFF 2
#define LED_ON  0

void led_settings(uint8_t leds);

#endif /* LED_H_ */
