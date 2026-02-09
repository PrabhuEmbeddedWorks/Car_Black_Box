#include "main.h"
#pragma config WDTE = OFF  //Watchdog Timer Enable bit, WDT disabled to avoid..
                           //unexpected resets
char *gear[] = {"GN","GR","G1","G2","G3","G4"}; //GEAR STRINGS
    //           0    1    2    3     4    5

static void init_config(void) { // INITIALISATION FUNCTION
    /* ADC + PORTA configuration */
    ADCON1 = 0x0E;     // AN0 analog, rest digital
    TRISA |= 0x20;     // RA5 as input (exit key)

    //Initialize I2C
    init_i2c(100000); // I2C COMMUNICATION WITH 100HZ CLOCK
    //Initialize RTC
    init_ds1307();
    //Initialize CLCD
    init_clcd();
    //Initialize DKP
    init_digital_keypad();
    //Initialize ADC
    init_adc();
    
    //Initialize Timer2 Module FOR TIMING OPERATIONS LIKE LOGIN TIMEOUT
    init_timer2();
    
    PEIE = 1;
    GIE = 1;
    
}
void main(void) { // MAIN FUNCTION
    char event[3] = "ON"; // STORE EVENT TYPE, ON : SYSTEM POWERED ON
    unsigned char speed = 0; //1 byte, VARIABLE TO STORE SPEED FROM ADC(0-99)
    unsigned char control_flag = DASH_BOARD_FLAG; // FLAG TO INDICATE..
                                                 // CURRENT SCREEN OR STATE
    
    unsigned char key, reset_flag, menu_pos; // VARIABLES FOR KEY INPUT RESET
                                             // TRIGGER AND MENU SELECTION
    unsigned char gr=0; // INDEX TO KEEP TRACK OF CURRENT GEAR
    
    init_config();
    log_car_event(event, speed);
    
    //Password for system "2424", DEFAULT
    eeprom_write(0x00, '2'); //RB4
    eeprom_write(0x01, '4'); //RB3
    eeprom_write(0x02, '2'); //RB4
    eeprom_write(0x03, '4'); //RB3
    
    
    while(1) // MAIN INFINITE LOOP
    {
        speed = (unsigned char)(read_adc() / 10); //0 to 1023/10
        if (speed > 99)
        {
            speed = 99;
        }
        key = read_digital_keypad(STATE);// READ KEY INPUT IN STATE MODE
        //To avoid bouncing effect
        for (int j=300;j--;);
        if ( key == SW1)
        {
            strcpy(event,"C "); //Collision
            log_car_event(event, speed); // LOG COLLISION WITH CURRENT SPEED
        }
        else if ( key == SW2 && gr < 6) 
        {
            strcpy(event,gear[gr]); // GET CURRENT GEAR FROM ARRAY
            log_car_event(event, speed);
            gr++; // INCREMENT GEAR INDEX
        }
        else if(key == SW3 && gr > 0) 
        {
            gr--;
            strcpy(event, gear[gr]); // UPDATE
            log_car_event(event, speed); // LOG GEAR CHANGE
        }
        else if((control_flag == DASH_BOARD_FLAG) && (key == SW4 || key == SW5))
        {
            clear_screen();
            clcd_print(" ENTER PASSWORD",LINE1(0));
            clcd_write(CURSOR_POS, INST_MODE); // SET CURSOR POSITION
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE); // BLINKING CURSOR
            __delay_us(100);
        
            control_flag = LOGIN_FLAG; // MOVE TO LOGIN SCREEN
            reset_flag = RESET_PASSWORD;// SET RESET FLAG TO INITIATE LOGIN
            /* Switching on the Timer2 */
            TMR2ON = 1; // 5 SECONDS TIMEOUT
            
        }
        
        
        // LOGIN MENU OPTION SELECTION
        else if (control_flag == LOGIN_MENU_FLAG && key == SW6)
        {
            switch (menu_pos)
            {
                case 0: // View log
                    clear_screen();
                    clcd_print("# TIME    EV  SP", LINE1(0));
                    control_flag = VIEW_LOG_FLAG;
                    reset_flag = VIEW_LOG_RESET;
                    break;

                case 1: // Clear log
                    clear_screen();
                    control_flag = CLEAR_LOG_FLAG;
                    reset_flag = RESET_MEMORY;
                    break;

                case 2: // Change password
                    clear_screen();
                    control_flag = CHANGE_PASSWORD_FLAG;
                    reset_flag = RESET_CHANGE_PASSWORD;
                    break;
            }
        }

        
        
        // MAIN STATE MACHINE
        switch(control_flag)
        {
            case DASH_BOARD_FLAG: //Dash board screen
            
                display_dash_board(event,speed); // CURRENT EVENT AND SPEED
                break;
            case LOGIN_FLAG:
                switch(login(key, reset_flag))
                {
                    case RETURN_BACK: //5 sec time is over
                    control_flag = DASH_BOARD_FLAG; // RETURN TO DASH BOARD..
                                                    // IF LOGIN TIMEOUT
                    TMR2ON = 0; // STOP TIMER
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                    break;
                    
                    case TASK_SUCCESS:
                        control_flag = LOGIN_MENU_FLAG; // LOGIN SUCCESS..
                                                        // SHOW MENU
                        reset_flag = RESET_LOGIN_MENU; // SET FLAG FOR NEW MENU
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        continue; // RESTART LOOP
                        break;
                }
                break;
            case LOGIN_MENU_FLAG:
               menu_pos = login_menu(key, reset_flag); // MENU SELECTION LOGIC
                break;
            case VIEW_LOG_FLAG:
                if (view_log(key, reset_flag) == RETURN_BACK)
                {
                    control_flag = LOGIN_MENU_FLAG;   // BACK TO LOGIN MENU
                    reset_flag = RESET_LOGIN_MENU;
                    clear_screen();
                }
                break;




            case CLEAR_LOG_FLAG:
                if (clear_log(reset_flag) == TASK_SUCCESS) // CLEAR EEPROM LOGS
                {
                  clear_screen();
                  control_flag = LOGIN_MENU_FLAG; // RETURN TO MENU
                  reset_flag = RESET_LOGIN_MENU;  
                  continue; // SKIP TO TOP OF WHILE LOOP
                }
                break;
            case CHANGE_PASSWORD_FLAG:
                switch (change_password(key, reset_flag))
                {
                  case TASK_SUCCESS: 
                  clear_screen();
                  clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // HIDE CURSOR
                  __delay_us(100);
                  control_flag = DASH_BOARD_FLAG; // RETURN TO DASHBOARD
                  reset_flag = RESET_NOTHING; 
                  continue;
                 
                }
                break;
        }
        reset_flag = RESET_NOTHING; // CLEAR OR RESET FLAG AFTER EACH STATE
    }
        
}
