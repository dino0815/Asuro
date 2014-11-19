#ifndef _SHELL_H_
#define _SHELL_H_
/**********************************************************************
 * Shell-Modul
 * Version:   0.1
 *********************************************************************/
#include "toolbox.h"

////////////////////////////////////////////////////////////
///Buffer, in dem unbearbeitete Zeichen gesammelt werden
#define RXBUF_LENGTH 16
char rxbuf[RXBUF_LENGTH]; ///ZeichenBuffer (Ringbuffer)
uint8 rxbuf_rpos;         ///Leseposition
uint8 rxbuf_wpos;         ///Schreibposition

////////////////////////////////////////////////////////////
///Buffer, in dem der Comando String gesammelt wird
#define CMDSTRBUF_LENGTH 16
char cmdstrbuf[CMDSTRBUF_LENGTH]; ///ComandoStringBuffer 1
byte cmdstrbuf_count;              ///Zeichenanzahl im aktuellen ComandoStringBuffer

void shell_add_to_rxbuf(char c); //untested
void shell_interpret_char(void); //untested
void shell_add_char_to_cmdstrbuf(char c); //untested
void shell_exec_key(char key); //untested
void shell_interpret_cmd(void); //untested
uint16 shell_interpret_value_str(char* value_str); //untested

#endif
