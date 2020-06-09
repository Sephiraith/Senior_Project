/*
 * LCD.h
 *
 *  Created on: May 23, 2020
 *      Author: Colton
 */

#ifndef LCD_H_
#define LCD_H_

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
// Numerical 0-9
#define LCD0  0x30
#define LCD1  0x31
#define LCD2  0x32
#define LCD3  0x33
#define LCD4  0x34
#define LCD5  0x35
#define LCD6  0x36
#define LCD7  0x37
#define LCD8  0x38
#define LCD9  0x39
// upper case Alphabet A-Z
#define LCDA  0x41
#define LCDB  0x42
#define LCDC  0x43
#define LCDD  0x44
#define LCDE  0x45
#define LCDF  0x46
#define LCDG  0x47
#define LCDH  0x48
#define LCDI  0x49
#define LCDJ  0x4A
#define LCDK  0x4B
#define LCDL  0x4C
#define LCDM  0x4D
#define LCDN  0x4E
#define LCDO  0x4F
#define LCDP  0x50
#define LCDQ  0x51
#define LCDR  0x52
#define LCDS  0x53
#define LCDT  0x54
#define LCDU  0x55
#define LCDV  0x56
#define LCDW  0x57
#define LCDX  0x58
#define LCDY  0x59
#define LCDZ  0x5A
// Special characters
#define LCD_S 0x02       // space character
// Commands
#define COM_CLR     0x01 // Clear display
#define COM_CHOME   0x02 // Set cursor to 'home'
#define COM_EMRS    0x07 // Entry Mode Set: increment direction 'right', and display shift 'n'
#define COM_DISP    0x0C // Sets display on, cursor/cursor-blink off
#define COM_FUNC    0x30 // Function set: 8-bit, dual line, 5x8 dot font
#define COM_DC      0x0E // Display and Cursor on
#define COM_EMR     0x06 // Entry Mode Set: increment 'right, shift 'off'
#define COM_L1      0x80 // Line 1 starting address
//#define COM_WRITE   0x
//#define COM_DDRAM   0x8* // Can be used later to set DDRAM addressing and data access

// Prototypes
void delay_us(uint32_t ui32Us);
void setLCDBus(uint8_t data);
void SetWrite(bool en, bool rs);   //Set register select and write
void States(uint8_t state);

#endif /* LCD_H_ */
