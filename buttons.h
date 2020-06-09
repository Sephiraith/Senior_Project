/*
 * buttons.h
 *
 *  Created on: Jun 1, 2020
 *      Author: Colton
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

// includes
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
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

uint8_t PollButtons(void);

#endif /* BUTTONS_H_ */
