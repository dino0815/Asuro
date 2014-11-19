#ifndef _STEPPER_H
#define _STEPPER_H

#include "config.h"
#include "toolbox.h"

#define byte unsigned char

// 3DCam: 200 Schritte = 2,5cm
// 3DCam: 3100 Schritte verfahrweg
   byte stepper_halfstep[8];       //Schrittmuster
   byte stepper_step;              //Momentaner Schritt der Schrittfolge
   signed char stepper_direktion;      //0 = Stop, 1 = Auseinander, -1 = Zusamen
//   unsigned int stepper_count;         //Noch zu fahrende Steps 
   unsigned int stepper_current_pos;   //0 = zusammen, 3100 = ganz außen
   unsigned int stepper_target_pos;    //0 = zusammen, 3100 = ganz außen
   unsigned int stepper_target_maxpos; //0 = zusammen, 3100 = ganz außen
   unsigned int stepper_maxspeed;
   unsigned int stepper_delay;
   unsigned int stepper_delay_count;
//   unsigned int stepper_speed;

   unsigned int stepper_get_speed(void);
   unsigned int stepper_get_current_pos(void);
   unsigned int stepper_get_target_pos(void);
   void stepper_set_speed(unsigned int speed);
   void stepper_add_speed(unsigned int speed);
   void stepper_sub_speed(unsigned int speed);
   void stepper_set_target_pos(unsigned int pos);
   void stepper_add_target_pos(unsigned int pos);
   void stepper_sub_target_pos(unsigned int pos);
   void stepper_init(void);
   void stepper_calibrate(void);
   void stepper_aktion(void);
//   void stepper_up(void);
//   void stepper_down(void);
//   void stepper_off(void);
#endif
