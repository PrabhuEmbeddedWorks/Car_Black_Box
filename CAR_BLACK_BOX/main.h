#ifndef MAIN_H // HEADER GUARD
#define	MAIN_H

#include <xc.h> // MICROCHIP COMPILER MAIN HEADER
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "i2c.h"
#include "ds1307.h" // I2C COOMUNICATION WITH RTC
#include "car_black_box.h"  // PROJECT SPECIFIC FUNCTIONS
#include <string.h> // STANDARD STRING FUNCTIONS
#include "timers.h"

#define LOGIN_SUCCESS         0x01  // RETURN VALUES: USED TO TRACK FUNCTIONS..
#define RETURN_BACK           0x02  // RETURN RESULTS AND TASK EXECUTION STATES
#define TASK_SUCCESS          0x11
#define TASK_FAIL             0x22

#define DASH_BOARD_FLAG       0x02  // SCREEN NAVIGATION FLAGS
#define LOGIN_FLAG            0x04
#define LOGIN_MENU_FLAG       0x06
#define VIEW_LOG_FLAG         0x08
#define CLEAR_LOG_FLAG        0x0A
#define CHANGE_PASSWORD_FLAG  0x0C

#define RESET_PASSWORD        0x11  // RESET FLAGS: USED TO CONTROL RESETTING..
#define RESET_LOGIN_MENU      0x13  // OF INTERNAL FLAGS, BUFFERS.
#define VIEW_LOG_RESET        0x15
#define RESET_MEMORY          0x17
#define RESET_CHANGE_PASSWORD 0x19
#define RESET_NOTHING         0xFF

#endif	/* MAIN_H */

