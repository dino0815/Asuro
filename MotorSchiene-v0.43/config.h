#ifndef _CONFIG_H_
   #define _CONFIG_H_

   //** Servo *****************************************//
   #define SERVO_PORT_DIR DDRA  //DDRB
   #define SERVO_PORT PORTA     //PORTB
   #define SERVO_L (1<<PA6)     //(1<<PB0)
   #define SERVO_R (1<<PA7)     //(1<<PB3)
   #define ALL_SERVOS_LOW     SERVO_PORT=0x00//&=~(SERVO_L&SERVO_R)
   #define ONLY_SERVO_L_HIGH  SERVO_PORT=(SERVO_L)
   #define ONLY_SERVO_R_HIGH  SERVO_PORT=(SERVO_R)

   #define TIMSK TIMSK1

   //** Stepper ****************************************//
   #define STEPPER_PORT_DIR DDRC
   #define STEPPER_PORT PORTC
   #define STEPPER_SPULE_A 0x01//(1<<PC0)
   #define STEPPER_SPULE_B 0x02//(1<<PC1)
   #define STEPPER_SPULE_C 0x04//(1<<PC2)
   #define STEPPER_SPULE_D 0x08//(1<<PC3)

   #define ENDSCHALTER_PORT_DIR DDRA     //DDRD
   #define ENDSCHALTER_PORT     PINA     //PIND
   #define ENDSCHALTER          (1<<PA5) //0x80 //(1<<PD7)
   #define ENDSCHALTER_UNPRESSED  ((ENDSCHALTER_PORT & ENDSCHALTER)!=0)
#endif
