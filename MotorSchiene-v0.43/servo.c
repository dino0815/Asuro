#include "servo.h"

/**********************************************************************/
void servo_init(void){
   servo_pos    = 66;          //ServoPosition
   servo_maxpos = 250;         //ServoPosition
   servo_status = 0;           //Position im PWM-Zyklus
   
   SERVO_PORT_DIR |= ( SERVO_L | SERVO_R );
// init Timer1
   TCNT1 = 0-16000U;
   TCCR1A=0;
   TCCR1B=0x02;   //vorteiler=8
   TIMSK |= _BV(TOIE1);// | _BV(TOIE2);
}//end init_servo

/**********************************************************************/
void servo_ISR(void){
   if(servo_status==0){
      ALL_SERVOS_LOW;
      TCNT1 = 0-50000U; //20ms (@20MHz) to Next Aktion 
      servo_status=1;
   }else if(servo_status==1){
      ONLY_SERVO_L_HIGH; 
      TCNT1 = 0-(2500+(servo_pos*8));
      servo_status=2;
   }else{// servo_status==2
      ONLY_SERVO_R_HIGH; 
      TCNT1 = 0-(5000-(servo_pos*8));
      servo_status=0;
   }//end if
}//end servo_ISR()
/**********************************************************************/
void servo_ISR_old(void){
    switch(servo_status){
       case 0: ALL_SERVOS_LOW;
               //FixMe: Hab noch keine Lösung gefunden :( .. es geht aber anscheinend
               //Warning: Große Ganzzahl implizit auf vorzeichenlosen Typen abgeschnitten
               TCNT1 = 0-40000U; //20ms to Next Aktion 
               break;
       case 1: ONLY_SERVO_L_HIGH; 
               TCNT1 = 0-(2000+(servo_pos*8));
               break;
       case 2: ONLY_SERVO_R_HIGH; 
               TCNT1 = 0-(4000-(servo_pos*8));
               break;
    }//end switch
    servo_status++;                 //next status 
    if(servo_status==3){servo_status=0;}  //reset status
}//end servo_ISR()

/**********************************************************************/
void servo_setpos(byte b){
    if(b > servo_maxpos){
//        //char uart_buffer[20];
        snprintf(uart_buffer, UART_BUFFER_SIZE, " -> %d(max.)\r\n", servo_maxpos);
        uart_putString(uart_buffer);
        servo_pos = servo_maxpos; 
    }else{servo_pos = b;}
}//end servo_setpos()

/**********************************************************************/
void servo_addpos(byte b){
    int temp = servo_pos + b;
    if(temp > servo_maxpos){
        //char uart_buffer[20];
        snprintf(uart_buffer, UART_BUFFER_SIZE, " -> %d(max.)\r\n", servo_maxpos);
        uart_putString(uart_buffer);
        servo_pos = servo_maxpos;
    }else{servo_pos = (byte)temp;}
}//end servo_addpos()

/**********************************************************************/
void servo_subpos(byte b){
    int temp = servo_pos - b;
    if(temp < 0){
        uart_putString(" -> 0(min.)\r\n");
        servo_pos = 0;}
    else{servo_pos = (byte)temp;}
}//end servo_subpos()

/**********************************************************************/
byte servo_getpos(void){
    return servo_pos;
}//end servo_setpos()

