/**********************************************************************
 * Shell-Modul
 * Version:   0.1
 *********************************************************************/
#include "shell.h"
//#include "uart.h"

//#define _DEBUG_

//********************************************************************//
void shell_add_to_rxbuf(char c){ //untested
    byte next_wpos = rxbuf_wpos + 1;
    if(next_wpos >= RXBUF_LENGTH){ next_wpos = 0; }

    if(next_wpos == rxbuf_rpos){
        //Buffer ist Voll
        uart_putByte(0x07);//Bell
        //Zeichen wird verworfen!
    }else{
        rxbuf[rxbuf_wpos] = c;
        SET(NEW_CHAR);
        rxbuf_wpos = next_wpos;
    }//end if
}//end uart_Rx_ISR()

//********************************************************************//
void shell_interpret_char(void){ //untested
    char c = rxbuf[rxbuf_rpos];

    rxbuf_rpos++;
    if(rxbuf_rpos >= RXBUF_LENGTH){ rxbuf_rpos = 0; }
    if(rxbuf_rpos == rxbuf_wpos)UNSET(NEW_CHAR);

    if(IS_SET(CMD_MODE)){
        if(c == 0x0d){ //"Enter"
            cmdstrbuf[cmdstrbuf_count] = '\0';
            UNSET(CMD_MODE);
            SET(NEW_CMD);
        }else{         
            shell_add_char_to_cmdstrbuf(c);
        }//end if(is_Enter)
    }else{ //KEY_MODE
        shell_exec_key(c);
    }//end if(CMD_MODE)
}//end shell_interpret_char()

//********************************************************************//
void shell_add_char_to_cmdstrbuf(char c){ //untested
    //TODO: ESC Problem

    if(c == 0x7F || c == 0x08){/*Del bzw. Backspace*/
        if(cmdstrbuf_count > 0){ //wenn min. ein Zeichen im Buffer
           cmdstrbuf[--cmdstrbuf_count] = 0x00; //Löschen im Buffer
           // Löschen im Terminal
           uart_putByte('\b'); // ein Zeichen zurück
           uart_putByte(' ');  // Zeichen Leeren
           uart_putByte('\b'); // ein Zeichen zurück
        }else{
           uart_putByte(0x07);//beep
        }//end if
        return;
    }//end if(Del or BS)

    if(c < 0x7F && c >= 0x20 && cmdstrbuf_count < CMDSTRBUF_LENGTH){
        cmdstrbuf[cmdstrbuf_count] = c;
        cmdstrbuf_count++;
        uart_putByte(c);//echo
    }else{
        //Buffer ist Voll
        uart_putByte(0x07);//Bell
        //Zeichen wird verworfen!
    }//end if

}//end add_key_to_csb

//********************************************************************//
void shell_exec_key(const char key){
    //char uart_buffer[40];
    if(key == '$'){
        uart_putByte('$');
        SET(CMD_MODE);
    }else if(key == '0'){
         stepper_calibrate();
    }else if(key == '1'){
         servo_setpos(1);
    }else if(key == '2'){
         servo_setpos(64);
    }else if(key == '3'){
         servo_setpos(128);
    }else if(key == '4'){
         servo_setpos(192);
    }else if(key == '5'){
         servo_setpos(250);
    }else if(key == '6'){
         stepper_add_speed(20);
    }else if(key == '7'){
         stepper_sub_speed(20);
    }else if(key == '8'){
        //stepper_off();
    }else if(key == '9'){
         stepper_set_speed(500);        
    }else if(key == 'q'){
         if(servo_pos<250){servo_pos++;}
    }else if(key == 'w'){
         if(servo_pos>0){servo_pos--;}
    }else if(key == 'a'){
         stepper_add_target_pos(20);
    }else if(key == 's'){
         stepper_sub_target_pos(20);
    }else{
        uart_putByte(0x07);//Bell
        uart_putByte(key);
        uart_putByte('\b');
    }//end if
}//end exec_key

//********************************************************************//
void shell_interpret_cmd(void){
    char* cmd_str   = cmdstrbuf;    //zeigt dann auf den Command-String
    char* value_str = cmdstrbuf;    //zeigt dann auf den Value-String
    UNSET(ADD_VALUE);
    UNSET(SUB_VALUE);
    UNSET(NEW_CMD);

///////////////////////////////////////////////////////////////////////
//Teil 1: String zerlegen in Komando(cmd_str) und Wert(value_str)  ////
    while(*cmd_str != '\0' && *cmd_str == ' ' ){ cmd_str++; } //Führende Leerzeichen überspringen!
    if(*cmd_str == '\0') return; //Abbruch, kein Komando-String! 

    // weiter bis zum Trennzeichen
    value_str = cmd_str; 
    while(*value_str != '\0' && *value_str != ' ' ){ value_str++; }
    if(*value_str == '\0'){        //Kein Value-Wert vorhanden, cmd_str schon terminiert
        value_str = NULL; 
    }else{ 
        *value_str = '\0'; //cmd-String abschließen.
        value_str++;
        while( *value_str != '\0' && *value_str == ' ' ){ value_str++; } //weitere Leerzeichen überspringen.
        if(*value_str == '\0'){  value_str = NULL; }   //Doch kein Value-Wert vorhanden.
    }//end if(!*value)

    if(value_str && *value_str == '+'){
        SET(ADD_VALUE);
        value_str++;
    }else if(value_str && *value_str == '-'){
        SET(SUB_VALUE);
        value_str++;
    }//end if(VORZEICHEN)

#ifdef _DEBUG_
    uart_putString("  cmdstrbuf = ");
    uart_putString(cmdstrbuf);
    uart_putString("\r\n");

    uart_putString("  cmd_str = ");
    uart_putString(cmd_str);
    uart_putString("\r\n");

    uart_putString("  value_str = ");
    uart_putString(value_str);
    uart_putString("\r\n");
#endif
///////////////////////////////////////////////////////////////////////
//Teil 2:  Auswerten:  ////////////////////////////////////////////////
    /*** Hilfe ***/           
    if((strcmp(cmd_str, "help"))==0){
        help(); 
    /*** Servo ***/           
    }else if((strcmp(cmd_str, "winkel"))==0){
        if(value_str){ 
            uint16 ang = shell_interpret_value_str(value_str);
            if(ang != -1){
               if(ang > 255){ ang = 255;}
               if(IS_SET(ADD_VALUE))     
                   {servo_addpos((byte)ang);UNSET(ADD_VALUE);}
               else if(IS_SET(SUB_VALUE))
                   {servo_subpos((byte)ang);UNSET(SUB_VALUE);}
               else                      
                   {servo_setpos((byte)ang);}
               uart_putString(" OK\r\n");
               snprintf(uart_buffer, UART_BUFFER_SIZE, "Winkel = %d\r\n", servo_pos);
               uart_putString(uart_buffer);
            }//end if
        }else{                
            snprintf(uart_buffer, UART_BUFFER_SIZE, " = %u\r\n",servo_getpos());
            uart_putString(uart_buffer);
        }//end if(value_str)            
    /*** Stepper ***/           
    }else if((strcmp(cmd_str, "abstand"))==0){
        if(value_str){ 
            uint16 gap = shell_interpret_value_str(value_str);
            if(gap != -1){
//               if(gap > 3000){ gap = 3000; uart_putString(" -> 3000(max.)");}           
               if(IS_SET(ADD_VALUE))     
                   {stepper_add_target_pos(gap);UNSET(ADD_VALUE);}
               else if(IS_SET(SUB_VALUE))
                   {stepper_sub_target_pos(gap);UNSET(SUB_VALUE);}
               else                      
                   {stepper_set_target_pos(gap);}
               uart_putString(" OK\r\n");
            }//end if
            snprintf(uart_buffer, UART_BUFFER_SIZE, "momentaner Abstand = %d\r\n", stepper_current_pos);
            uart_putString(uart_buffer);
         }else{                
            snprintf(uart_buffer, UART_BUFFER_SIZE, " = %u\r\n",stepper_get_target_pos());
            uart_putString(uart_buffer);
        }//end if(value_str) 
           
    /*** Speed ***/           
    }else if((strcmp(cmd_str, "speed"))==0){
        if(value_str){ 
            uint16 speed = shell_interpret_value_str(value_str);
            if(speed != -1){
               stepper_set_speed(speed);
               uart_putString(" OK\r\n");
            }//end if
         }else{                
            snprintf(uart_buffer, UART_BUFFER_SIZE, " = %u\r\n",stepper_get_speed());
            uart_putString(uart_buffer);
        }//end if(value_str)            
    /*** Reset ***/           
    }else if((strcmp(cmd_str, "reset")) == 0){
        reset();
    /*** Sonst ***/           
    }else{ 
        uart_putByte(0x07); //beep
        uart_putString(" (Befehl unbekannt!)\r\n");
    }//ende der String-Vergleiche

///////////////////////////////////////////////////////////////////////
// Aufräumen:  ////////////////////////////////////////////////////////
    while(cmdstrbuf_count > 0){
        cmdstrbuf[--cmdstrbuf_count] = 0x00;
    }//end while   
    cmd_str = NULL;
    value_str = NULL;
//    uart_putString("\r\n");
}//end shell_interpret_cmd()

//********************************************************************//
uint16 shell_interpret_value_str(char* value_str){
    uint16 erg = 0;

    while(*value_str != '\0'){
        if((*value_str <= 0x29) || (*value_str >= 0x40)){
            uart_putString(" (Ungültiger Wert!)\r\n");
            return -1; }

        if(erg > 6552){
            uart_putString(" (Wert zu groß!)\r\n");
            return -1;}
        
//        erg = (erg*10) + (*value_str - 0x30);
        erg = (erg << 3) + (erg << 1) + (*value_str - '0');
        value_str++;
    }//end while
    return erg;
}//end shell_interpret_value_str()

