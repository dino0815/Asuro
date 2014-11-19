#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "servo.h" 
#include "stepper.h"

#define byte unsigned char
//typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned int  uint16;

volatile byte flag;
#define IS_SET(mode) flag & mode
#define SET(mode) flag = flag | mode
#define UNSET(mode) flag = flag & ~mode
#define NEW_CHAR   0x01 ///Zeichen im Buffer
#define NEW_CMD    0x02 ///Neuer Comando-String verfügbar
#define STEPPER_ON 0x04 ///Schrittmotor Aktiv (hat sein Ziel noch nicht erreicht)
#define CMD_MODE   0x40 ///Comando Modus
#define ADD_VALUE  0x10 ///Value soll hinzugezählt werden
#define SUB_VALUE  0x20 ///Value soll abgezogen werden

void reset(void);
void help(void);
char myStrComp(char* str1, char* str2);
void notaus(void);

#endif
