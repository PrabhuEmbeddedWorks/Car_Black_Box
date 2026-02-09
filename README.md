# Car Black Box Implementation

# Project Demonstration Video
A complete working simulation of this project is available on YouTube:
🔗 https://youtu.be/X9iID5KMKsU

# Overview
This project implements a **Car Black Box system** using the **PIC16F877A microcontroller**, simulated on **PICSimLab**.
The system records critical vehicle parameters such as **speed, time, and events** into EEPROM, similar to real automotive black box systems used for safety analysis and accident investigation.
The project demonstrates core embedded systems concepts including ADC, RTC, EEPROM logging, circular buffer management, and secure user interaction.

# Features
* Real-time dashboard displaying **Time, Event, and Speed**
* **ADC-based speed sensing** using potentiometer (AN0)
* **RTC (DS1307)** for accurate time stamping
* **EEPROM-based circular buffer** supporting up to **10 log entries**
* **Latest event displayed first** during log viewing
* Collision event logging
* Password-protected access to secured features
* Clear log and change password functionality
* User blocking mechanism after multiple wrong password attempts
* State-machine-based system design

# System Dashboard
The LCD dashboard continuously displays:
* **TIME** – Current time in HH:MM:SS format
* **EV** – Current event or gear status
* **SP** – Vehicle speed derived from ADC input

# Event Logging
Each event log consists of:
* Time (HHMMSS)
* Event type (Gear / Collision)
* Speed

Logs are stored in EEPROM using a **circular buffer**:

* Maximum of **10 events**
* When full, the oldest event is overwritten
* During log viewing, the **most recent event is shown first**

# User Controls
# Dashboard Controls

| Button              | Function        |
| ------------------- | --------------- |
| RB0                 | Collision event |
| RB1                 | Gear up         |
| RB2                 | Gear down       |
| Potentiometer (AN0) | Speed control   |

# Login & Menu Controls

| Button | Function         |
| ------ | ---------------- |
| RB3    | Enter login mode |
| RB4    | Move down        |
| RB3    | Move up          |
| RB5    | Select / Enter   |
| RA5    | Exit View Log    |

# Login Menu Options
After successful password authentication, the following options are available:
1. View Log
2. Clear Log
3. Change Password

# View Log Behavior
* Displays **latest stored event first**
* RB4 scrolls to older events
* RB3 scrolls to newer events
* RA5 exits View Log and returns to the login menu
* After clearing logs, the display shows **“No Logs – Logs Cleared”**

# Security Features
* 4-digit password-based authentication
* Password stored securely in EEPROM
* Password masking during entry
* System blocks access for **60 seconds** after three wrong password attempts
* Password can be updated via Change Password option
* After password change, the system returns to dashboard mode
* Re-login requires the updated password

# Hardware and Software Details
# Hardware (Simulated)
* Microcontroller: PIC16F877A
* LCD (16x2)
* Potentiometer (AN0)
* Digital push buttons
* DS1307 RTC
* EEPROM (internal)
# Software
* Programming Language: Embedded C
* IDE: MPLAB X
* Compiler: XC8
* Simulator: PICSimLab

# Key Concepts Implemented
* Embedded C programming
* ADC and analog sensor interfacing
* I2C communication
* RTC interfacing
* EEPROM data logging
* Circular buffer implementation
* Finite State Machine (FSM)
* Secure authentication logic

# Project Structure

```
├── main.c
├── car_black_box_def.c
├── car_black_box.h
├── adc.c / adc.h
├── clcd.c / clcd.h
├── digital_keypad.c / digital_keypad.h
├── ds1307.c / ds1307.h
├── i2c.c / i2c.h
├── timers.c / timers.h
```

# How to Run
1. Open the project in **MPLAB X**
2. Compile using **XC8**
3. Load the hex file into **PICSimLab**
4. Run the simulation and interact using buttons and potentiometer

# Applications
* Automotive event data recorders
* Embedded systems learning projects
* EEPROM logging demonstrations
* Interview and academic evaluations

