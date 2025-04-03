#include "main.h"
/*Function definition*/
unsigned char clock_reg[3]; // HH MM SS 
char time[7]; //HHMMSS
char log[11]; //Time+Event+Speed  HHMMSSEVSP
char pos = -1; //Character position variable
int event_count = 0; // By default 1 ON event is there
extern unsigned char sec;
extern unsigned char return_time;

char *menu[] = {"View log", "Clear log", "Change passwrd"}; //Global array pointer

void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); //HH -> BCD
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD
    //HH
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    //MM
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    //SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0'; //HHMMSS
}
void display_time(void)
{
    get_time();
    //HH
    clcd_putch(time[0],LINE2(1));
    clcd_putch(time[1],LINE2(2));
    clcd_putch(':',LINE2(3));
    //MM
    clcd_putch(time[2],LINE2(4));
    clcd_putch(time[3],LINE2(5));
    clcd_putch(':',LINE2(6));
    //SS
    clcd_putch(time[4],LINE2(7));
    clcd_putch(time[5],LINE2(8));
    //HH:MM:SS
    
}
void display_dash_board(char event[], unsigned char speed)
{
    clcd_print("TIME    EV  SP",LINE1(2));
    
    //To display time
    display_time();
    
    //To display event
    clcd_print(event,LINE2(10));
    
    //To display speed
    clcd_putch(speed/10+'0',LINE2(14));
    clcd_putch(speed%10+'0',LINE2(15));
}
void log_event(void)
{
    unsigned char add;
    pos++; //0 to 9, after 10, address = 5
    if (pos == 10)
    {
        pos = 0;
    }
    add = pos*10 + 5; //0*10+5=5, 1*10+5=15...
    for(int i = 0; log[i] != '\0'; i++)
    {
        eeprom_write(add, log[i]);
        add++; //5th to 14th address
    }
    if(event_count < 9) //Total 10 events
        event_count++; // 0 1 2 3 4 5 6 7 8 9 --> 10
}
void log_car_event(char event[], unsigned char speed)
{
    get_time(); //time -> HHMMSSS
    
    strncpy(log, time, 6); 
    strncpy(&log[6], event, 2);
    log[8] = (speed/10) + '0';
    log[9] = (speed%10) + '0';
    log[10] = '\0';
    
    log_event();
}
void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(100);
    
}
char login(unsigned char key, unsigned char reset_flag)
{
 static char npassword[4]; //4 digit password
 static unsigned char attempt_rem, i;
 char spassword[4];
 if(reset_flag == RESET_PASSWORD)
 {
    return_time = 5; //Wait for 5 sec
    attempt_rem = '3';
    i = 0;
    npassword[0] = '\0';
    npassword[1] = '\0';
    npassword[2] = '\0';
    npassword[3] = '\0'; 
    key = ALL_RELEASED;
 }
 
 if(return_time == 0) // 5 sec
 {
     return RETURN_BACK;
 }
 
 if(key == SW4 && i < 4) //SW -> '4'
 {
     npassword[i] = '4';
     clcd_putch('*', LINE2(6 + i)); //Next location after 6
     i++;
     return_time = 5;
 }
 else if (key == SW5 && i < 4) // SW5 -> '2'
 {
     npassword[i] = '2';
     clcd_putch('*', LINE2(6 + i));
     i++;
     return_time = 5;
 }
 //User entered 4 digit password
 if (i == 4)
 {
   for (int j = 0; j < 4; j++)
   {
       spassword[j] = eeprom_read(j);  //0 1 2 3
   }
   if (strncmp(npassword, spassword, 4) == 0)  //Password is correct
   {
       return TASK_SUCCESS;
   }
   else 
   {
       attempt_rem--;
       if(attempt_rem == '0')  //Lost all attempts
       {
          clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
          __delay_us(100);
          clcd_print(" You are blocked", LINE1(0));
          clcd_print("Wait...for 60sec", LINE2(0));
          sec = 60;
          while (sec != 0)  // sec = 0 after 60 seconds
          {
             clcd_putch((sec/10) + '0', LINE2(11)); 
             clcd_putch((sec%10) + '0', LINE2(12)); 
          }
          attempt_rem = '3';
       }
       else 
       {
           clear_screen();
           clcd_print(" WRONG PASSWORD ",LINE1(0));
           clcd_putch(attempt_rem, LINE2(0));
           clcd_print(" attempt remains", LINE2(1));
           __delay_ms(3000); // 3 sec
       }
            clear_screen();
            clcd_print(" ENTER PASSWORD",LINE1(0));
            clcd_write(CURSOR_POS, INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            i = 0;
            return_time = 5;
   }
 }
 return 0x10;
}
/* 
  0 - View log
  1 - Clear log
  2 - Change password
  */ 
// char *menu[] = {"View log", "Clear log", "Change password"};
//                menu[0]      menu[1]        menu[2]

char login_menu(unsigned char key, unsigned char reset_flag)
{
   static char menu_pos; // Menu position
   if (reset_flag == RESET_LOGIN_MENU)
   {
       menu_pos = 0;
   }
   if(key == SW5 && menu_pos < 2) // Down key
   {
       menu_pos++; //1 2
       clear_screen();
   }
   else if(key == SW4 && menu_pos > 0) // UP key
   {
      menu_pos--; // 0
      clear_screen();
   }
   if (menu_pos < 2) //Menu pos = 2
   {
       clcd_putch('*', LINE1(0));
       clcd_print(menu[menu_pos], LINE1(2));
       clcd_print(menu[menu_pos + 1], LINE2(2)); //menu[1]
   }
   else if (menu_pos == 2)
   {
       clcd_print(menu[menu_pos - 1], LINE1(2)); //1
       clcd_print(menu[menu_pos], LINE2(2)); //2
       clcd_putch('*', LINE2(0));
   }
   
   return menu_pos;
}
void view_log(unsigned char key, unsigned char reset_flag)
{
    char rlog[11];
    unsigned char add;
    static unsigned char rpos;
    //No event are there -> Clearing logs
    if (event_count == -1) //No log to show
    {
      clcd_print("No logs", LINE2(2));  
    }
    else 
    {
    if (reset_flag == VIEW_LOG_RESET)
    {
        rpos = 0;
    }
    if (key == SW5 && rpos < event_count -1) //If < 4
    {
       rpos++; //0 1 2 3
    }
    else if (key == SW4 && rpos > 0)
    {
        rpos--;
    }
    for (int i =0; i < 10; i++)
    {
        add = rpos * 10 + 5; // 5 15 25
        rlog[i] = eeprom_read(add + i);  //5th to 14th 15th to 25th
    }
    // rpos -> "HHMMSSEVSP"
    //          0123456789
    
    clcd_putch(rpos%10 + '0',LINE2(0));
    //HH
    clcd_putch(rlog[0], LINE2(2));
    clcd_putch(rlog[1], LINE2(3));
    clcd_putch(':', LINE2(4));
    //MM
    clcd_putch(rlog[2], LINE2(5));
    clcd_putch(rlog[3], LINE2(6));
    clcd_putch(':', LINE2(7));
    //SS
    clcd_putch(rlog[4], LINE2(8));
    clcd_putch(rlog[5], LINE2(9));
    
    //Event
    clcd_putch(rlog[6], LINE2(11));
    clcd_putch(rlog[7], LINE2(12));
    
    //Speed
    clcd_putch(rlog[8], LINE2(14));
    clcd_putch(rlog[9], LINE2(15));
    
    }
}
char clear_log(unsigned char reset_flag)
{
    if(reset_flag == RESET_MEMORY)
    {
      event_count = -1;// No event
      pos = -1; // For address
      clcd_print(" Logs Cleared ", LINE1(0));
      clcd_print(" Successfully ", LINE2(0));
      __delay_ms(3000); //3 sec
      
      return TASK_SUCCESS;
    }
    return TASK_FAIL;
}

char change_password(unsigned char key, unsigned char reset_flag)
{
    static char pwd [9]; // Single array -> Entered password + re-enter password (4+4+null)
    static int pos, once;
    if(reset_flag == RESET_CHANGE_PASSWORD)
    {
        pos = 0;
        once = 1;
    }
    if(pos < 4 && once) // 0 1 2 3
    {
        once = 0;
        clcd_print(" Enter new pwd", LINE1(0));
        clcd_write(LINE2(0), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
    }
    else if (pos >= 4 && (once == 0)) 
    {
        once = 1;
        clear_screen();
        clcd_print("Re-enter new pwd", LINE1(0));
        clcd_write(LINE2(0), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
    }
    
    
    if(key == SW4) // '4'
    {
        pwd[pos] = '4';
        clcd_putch('*', LINE2(pos % 4)); // 0 1 2 3 4 (4%4=0,5%4=1...)
        pos++;
    }
    else if (key == SW5) // '2'
    {
        pwd[pos] = '2';
        clcd_putch('*', LINE2(pos % 4)); // 0 1 2 3
        pos++;   
    }
    if (pos == 8)
    {                                            //    01234567
        if (strncmp(pwd, &pwd[4] , 4) == 0)  // pwd -> 24242424
        {
            for(int i = 0; i < 4; i++)
            {
               eeprom_write(i, pwd[i]); // 0 1 2 3
            }
               clear_screen();
               clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
               __delay_us(100);
               clcd_print("Password Changed", LINE1(0));
               clcd_print("  Successfully", LINE2(0));
               __delay_ms(3000); //3 sec
               return TASK_SUCCESS;
        }
        else // Entered and re-entered password are not matching
        {
               clear_screen();
               clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
               __delay_us(100);
               clcd_print("Password Changed", LINE1(0));
               clcd_print("  Failed", LINE2(0));
               __delay_ms(3000); //3 sec
               return TASK_SUCCESS;
        }
    }
    return 0x10;
}