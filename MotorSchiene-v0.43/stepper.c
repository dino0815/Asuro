#include "stepper.h"

/**********************************************************************/
void stepper_init(void){
   STEPPER_PORT       &= ~(0x0F); // Unteren 4Bit Ausschalten
   STEPPER_PORT_DIR   |= 0x0F;    // Unteren 4Bit als Ausgang
   stepper_halfstep[0] = STEPPER_SPULE_A;
   stepper_halfstep[1] = STEPPER_SPULE_A | STEPPER_SPULE_B;
   stepper_halfstep[2] = STEPPER_SPULE_B;
   stepper_halfstep[3] = STEPPER_SPULE_B | STEPPER_SPULE_C;
   stepper_halfstep[4] = STEPPER_SPULE_C;
   stepper_halfstep[5] = STEPPER_SPULE_C | STEPPER_SPULE_D;
   stepper_halfstep[6] = STEPPER_SPULE_D;
   stepper_halfstep[7] = STEPPER_SPULE_D | STEPPER_SPULE_A;

   stepper_step         = 0;   //Momentaner Schritt der Schrittfolge
//   stepper_direktion   = 0;   //0 = Stop, 1 = Auseinander, -1 = Zusamen
//   stepper_count       = 0;   //Noch zu fahrende Steps (2cm = 1000)
   stepper_current_pos  = 0;   //0 = zusammen, 5700 = ganz außen
   stepper_target_pos   = 0;   //0 = zusammen, 5700 = ganz außen
   stepper_target_maxpos= 1400;
   stepper_maxspeed     = 3500;
//   stepper_speed       = 0;
   stepper_delay        = 3000;
   stepper_delay_count  = 0;
   
// init Timer0
//   TCNT0  = 255;
//   TCCR0  = 0x05; //vorteiler=1024 (datasheet ATmega8 Seite 72)
//   TIMSK |=  _BV(TOIE1);
}//end init_stepper

/**********************************************************************
 *  Stepper Code
 **********************************************************************/
void stepper_calibrate(){
    const char cursor[8] = "-//||\\\\-";
    uart_putString("\r\n** Calibrate**\r\n"); 
    while(ENDSCHALTER_UNPRESSED){ //Bis Endschalter
    wdt_enable(WDTO_8S);
    uart_putByte('\b');
    uart_putByte(cursor[stepper_step]);
    if(stepper_step <= 0) stepper_step = 7;
    else                  stepper_step--;            
    STEPPER_PORT = stepper_halfstep[stepper_step];
    _delay_ms(10);
   }//end while
    if(stepper_step >= 7) stepper_step = 0;
    else                  stepper_step++;      
    if(stepper_step >= 7) stepper_step = 0;
    else                  stepper_step++;      
   stepper_current_pos =0;
   stepper_target_pos  =0;
}
/**********************************************************************/
void stepper_aktion(){
   //nicht blockierendes Delay
   if(stepper_delay_count){stepper_delay_count--; return;}
   stepper_delay_count = stepper_delay;

//   if((stepper_current_pos & 0x000f) == 0){
//        snprintf(uart_buffer, UART_BUFFER_SIZE, "\r\nAbstand = %d", stepper_current_pos);
//        uart_putString(uart_buffer);
//   }//end if

   //Schrittmotor Auswertung
   if(stepper_target_pos == stepper_current_pos){  
      STEPPER_PORT = 0x00;    
      UNSET(STEPPER_ON);
      snprintf(uart_buffer, UART_BUFFER_SIZE, "Abstand = %d\r\n", stepper_current_pos);
      uart_putString(uart_buffer);
   }else if(stepper_target_pos < stepper_current_pos){   
      if(stepper_step <= 0) stepper_step = 7;
      else                  stepper_step--;      
      stepper_current_pos--;
      STEPPER_PORT = stepper_halfstep[stepper_step];

   }else if(stepper_target_pos > stepper_current_pos){   
      if(stepper_step >= 7) stepper_step = 0;
      else                  stepper_step++;      
      stepper_current_pos++;
      STEPPER_PORT = stepper_halfstep[stepper_step];
   }//end Schrittmotor Auswertung  
}//end stepper_aktion()
/**********************************************************************/
void stepper_set_speed(unsigned int speed){
    if(speed > stepper_maxspeed){ 
        //char uart_buffer[30];
        snprintf(uart_buffer,30, " -> %d(max.)\r\n", stepper_maxspeed);
        uart_putString(uart_buffer);
        speed = stepper_maxspeed; 
    }//end if
    stepper_delay  = 5500 - speed;
}
/**********************************************************************/
void stepper_add_speed(unsigned int speed){
    if((speed + 5500 - stepper_delay) > stepper_maxspeed){ 
        //char uart_buffer[30];
        snprintf(uart_buffer, UART_BUFFER_SIZE, " -> %d(max.)", stepper_maxspeed);
        uart_putString(uart_buffer);
        stepper_delay  = 5500 - stepper_maxspeed;
    }else{
        stepper_delay = (5500 - stepper_delay) - speed;
    }//end if
}
/**********************************************************************/
void stepper_sub_speed(unsigned int speed){
    if(speed > (5500 - stepper_delay)){ 
        uart_putString(" -> 0(min.)\r\n");
        stepper_delay  = 5500; 
    }else{
        stepper_delay = (5500 - stepper_delay) + speed;
    }//end if
}
/**********************************************************************/
unsigned int stepper_get_speed(void){
   return (5500 - stepper_delay);
}
/**********************************************************************/
void stepper_set_target_pos(unsigned int pos){
    if(pos > stepper_target_maxpos){
        //char uart_buffer[20];
        snprintf(uart_buffer, UART_BUFFER_SIZE, " -> %d(max.)\r\n", stepper_target_maxpos);
        uart_putString(uart_buffer);
        stepper_target_pos = stepper_target_maxpos; 
    }else{
        stepper_target_pos  = pos;
    }//end if
    SET(STEPPER_ON);
}
/**********************************************************************/
void stepper_add_target_pos(unsigned int pos){
    if((stepper_target_pos + pos) > stepper_target_maxpos){
        //char uart_buffer[20];
        snprintf(uart_buffer, UART_BUFFER_SIZE, " -> %d(max.)\r\n", stepper_target_maxpos);
        uart_putString(uart_buffer);
        stepper_target_pos = stepper_target_maxpos; 
    }else{
        stepper_target_pos  = (stepper_target_pos + pos);
    }//end if
    SET(STEPPER_ON);
}
/**********************************************************************/
void stepper_sub_target_pos(unsigned int pos){
    if((stepper_target_pos - pos) > stepper_target_maxpos){ //underflow!
        uart_putString(" -> 0(min.)\r\n");
        stepper_target_pos = 0; 
    }else{
        stepper_target_pos  = (stepper_target_pos - pos);
    }//end if
    SET(STEPPER_ON);
}
/**********************************************************************/
unsigned int stepper_get_current_pos(void){
   return stepper_current_pos;
}
/**********************************************************************/
unsigned int stepper_get_target_pos(void){
   return stepper_target_pos;
}
