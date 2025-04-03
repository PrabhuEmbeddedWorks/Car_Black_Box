/* 
 * File:   main.h
 * Author: Prabhu
 *
 * Created on 10 November, 2024, 12:58 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "i2c.h"
#include "ds1307.h"
#include "car_black_box.h"
#include <string.h>
#include "timers.h"

#define LOGIN_SUCCESS         0x01
#define RETURN_BACK           0x02
#define TASK_SUCCESS          0x11
#define TASK_FAIL             0x22

#define DASH_BOARD_FLAG       0x02
#define LOGIN_FLAG            0x04
#define LOGIN_MENU_FLAG       0x06
#define VIEW_LOG_FLAG         0x08
#define CLEAR_LOG_FLAG        0x0A
#define CHANGE_PASSWORD_FLAG  0x0C

#define RESET_PASSWORD        0x11
#define RESET_LOGIN_MENU      0x13
#define VIEW_LOG_RESET        0x15
#define RESET_MEMORY          0x17
#define RESET_CHANGE_PASSWORD 0x19
#define RESET_NOTHING         0xFF

#endif	/* MAIN_H */

