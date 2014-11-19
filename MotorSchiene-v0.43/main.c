/**********************************************************************
 * Master Controll Programm
 * default:   19200 Baud 8N1   
 * Processor: ATMEGA 644
 * CLOCK:     20 MHz
 * Version:   0.43 
 *********************************************************************/
#include <util/delay.h>
#include "config.h"
#include "toolbox.h"
#include "shell.h"

//*********************************************************************//
ISR(USART_RX_vect){
    char c = UDR;
    if(c == 0xFF){    
        uart_putString("\r\n******** RESET ********\r\n"); 
        reset();} //Reset
    if(c == '\t'){ 
        uart_putString("\r\n******** NOTAUS! *******\r\n"); 
        notaus(); 
        uart_putString("\r\n**** und weiter... *****\r\n"); 
        return;} 

    shell_add_to_rxbuf(c);
}

//*********************************************************************//
ISR(TIMER1_OVF_vect){ //used by Servos
   servo_ISR();
}

//*********************************************************************//
ISR(TIMER0_OVF_vect){ //not used
    uart_putString("\r\n******** TIMER0_OVF_vect *******\r\n"); 
    while(1){;}
}

//*********************************************************************//
void port_init(void){
    DDRA = 0x00;  //all pin are input
    PORTA = 0xFF; //all pullup aktiv
    DDRB = 0x00;  //all pin are input
    PORTB = 0xFF; //all pullup aktiv
    DDRC = 0x00;  //all pin are input
    PORTC = 0xFF; //all pullup aktiv
    DDRD = 0x00;  //all pin are input
    PORTD = 0xFF; //all pullup aktiv
}

/***********************************************************************
 *  main programm
 **********************************************************************/
void main(void){
    wdt_enable(WDTO_8S);
    port_init();
    uart_init();
    servo_init();
    stepper_init();

    servo_setpos(40);
    _delay_ms(800);
    servo_setpos(81);
    stepper_calibrate();

    uart_putString("\r\n-- Stereo-Aufname Controller v0.43 -- \r\n");
    sei();
    while(1){
        wdt_enable(WDTO_8S);
       
        while(IS_SET(NEW_CHAR)){
            shell_interpret_char();
        }//end IS_SET(NEW_CHAR

        if(IS_SET(NEW_CMD)){
            shell_interpret_cmd();
        }//end NEW_WORD

        if(IS_SET(STEPPER_ON)){
            stepper_aktion();
        }//end NEW_WORD
    }//while(1)
}//end main() 

