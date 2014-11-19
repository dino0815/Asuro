#ifndef _SERVO_H
#define _SERVO_H

#include "config.h"
#include "toolbox.h"

#define byte unsigned char

byte servo_minpos;         //kleinste ServoPosition
byte servo_maxpos;         //größte ServoPosition
byte servo_pos;            //ServoPosition
byte servo_status;         //Position im PWM-Zyklus

void servo_init(void);
void servo_ISR(void);
void servo_setpos(byte b);
void servo_addpos(byte b);
void servo_subpos(byte b);
byte servo_getpos(void);

#endif
