
/*
#define UART_WBUFFER_SIZE 8
char uart_wBuffer[UART_RBUFFER_SIZE];
byte uart_wBuffer_rpos; //lese Position
byte uart_wBuffer_wpos; //schreibe Position
*/
#define CMD_BUFFER_LENGTH 20
char cmd_buffer[CMD_BUFFER_LENGTH];
byte cmd_buffer_counter = 0;

volatile byte flag = 0;
#define IS_SET(mode) flag & mode
#define SET(mode) flag = flag | mode
#define UNSET(mode) flag = flag & ~mode

#define NEW_CHAR  0x01 //Wenn ein Zeichen empfangen wurde
#define NEW_WORD  0x02 //Wenn "Space" empfangen wurde
#define NEW_LINE  0x04 //Wenn "Return" empfangen wurde

#define CMD_MODE   0x10 //Comando
#define VALUE_MODE 0x20 //Wert
#define ESC_MODE   0x40 //Wenn "ESC"  empfangen wurde
#define CTRL_MODE  0x80

//*********************************************************************//
//*********************************************************************//

//*********************************************************************//
//*********************************************************************//

/***********************************************************************
 *  main programm
 **********************************************************************/
void main(void){
    init_ports();
    init_uart();
    uart_putString("Interupt UART-Shell \r\n");
    sei();
    while(1){
        PORTC = 0x00;
        if(IS_SET(CMD_MODE))  {PORTC |= 0x01;};
        if(IS_SET(VALUE_MODE)) {PORTC |= 0x04;};
        if(IS_SET(0)){PORTC ^= 0x08;};

        if(IS_SET(NEW_LINE)){
            UNSET(NEW_LINE);
            //Hier wird der Befehl dann umgesetzt! :) ..
        }//end NEW_LINE

        if(IS_SET(NEW_WORD)){
            UNSET(NEW_WORD);
            if(IS_SET(CMD_MODE)){
                UNSET(CMD_MODE);
                uart_interpret_verb();

            }else if(IS_SET(VALUE_MODE)) { 
                UNSET(VALUE_MODE);
                uart_interpret_value();

            }else{
                SET(CMD_MODE);
                uart_putByte(' ');}
        }//end NEW_WORD

        if(IS_SET(NEW_CHAR)){
            UNSET(NEW_CHAR);
            uart_interpret_char();
        }//end NEW_CHAR
    }//while(1)
}//end main() 

//*********************************************************************//
void uart_interpret_char(void){
    char* pc = &uart_rBuffer[uart_rBuffer_rpos];
    switch(uart_rBuffer[uart_rBuffer_rpos]){
    case 0x0D: // ENTER
        SET(NEW_LINE);
    break;

    case 0x20: // SPACE
        SET(NEW_WORD);
    break;

    case 0x1b: // ESC
        SET(ESC_MODE);
    break;
    case 0x7F: // Backspace (Picocom) evtl. Zeichensatzprobleme ?
    case 0x08: // Backspace
        if(cmd_buffer_counter > 0){ //wenn min. ein Zeichen im Buffer
            cmd_buffer[--cmd_buffer_counter] = 0x00; //Löschen im Buffer
            // Löschen im Terminal
            uart_putByte('\b'); // ein Zeichen zurück
            uart_putByte(' ');  // Zeichen Leeren
            uart_putByte('\b'); // ein Zeichen zurück
        }else{
            uart_putByte(0x07);//beep
        }//end if
    break;

    default  :
        if((IS_SET(CMD_MODE)) | (IS_SET(VALUE_MODE))){
            //Gültiges Zeichen und ist noch Platz im Buffer?  
            if(*pc < 0x7F && *pc > 0x20 && cmd_buffer_counter < CMD_BUFFER_LENGTH-1){
                if(*pc > 0x60){ *pc = (*pc)&(~0x20); } //Alle Buchstaben Groß! 
               cmd_buffer[cmd_buffer_counter++] = *pc; // Zeichen Speichern
               uart_putByte(*pc); //Echo
            }else{
               uart_putByte(0x07);//beep
            }//end if(c < ...   
        }else{ //Einzelzeichen interpretation (Keypad)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }//end IS_SET(NOW_...
     }//end switch
 
//    uart_putByte(uart_rBuffer[uart_rBuffer_rpos]);//echo!

    //Nach der Verarbeitung:
    uart_rBuffer_rpos++;
    if(uart_rBuffer_rpos >= UART_RBUFFER_SIZE){uart_rBuffer_rpos = 0;}
    if(uart_rBuffer_rpos == uart_rBuffer_wpos){UNSET(NEW_CHAR);}
}//end uart_interpret_char

//*********************************************************************//
void uart_interpret_verb(void){
    if((strcmp(cmd_buffer, ""))==0){
        SET(CMD_MODE);
        uart_putByte(0x07);//beep
    }else if((myStrComp(cmd_buffer, "SET"))==0){
//    }else if((myStrComp(cmd_buffer, "set"))==0){
//    }else if((strcmp(cmd_buffer, "set"))==0){
        uart_putString("\b\b\bSET ");
        SET(VALUE_MODE);
    }else if((strcmp(cmd_buffer, "GET"))==0){
        uart_putString("\b\b\bGET ");
        SET(VALUE_MODE);
    }else if((strcmp(cmd_buffer, "abstand"))==0){
        uart_putString("\b\b\b\b\b\b\bABSTAND ");
        SET(VALUE_MODE);
    }else if((strcmp(cmd_buffer, "winkel"))==0){
        uart_putString("\b\b\b\b\b\bWINKEL ");
        SET(VALUE_MODE);
    }else{ 
        uart_putByte(0x07); //beep
        uart_putString("\r\n Befehl unbekannt!\r\n");
    }//ende der String-Vergleiche

    //Buffer leeren!
    while(cmd_buffer_counter > 0){
        uart_putByte(cmd_buffer[cmd_buffer_counter]);
        cmd_buffer[--cmd_buffer_counter] = 0x00;
    }//end while
}//end uart_interpret_verb
 
//*********************************************************************//
void uart_interpret_value(void){
    UNSET(NEW_WORD);
    //Prüfen ob ein int aus dem buffer gelesen werden kann!
    uart_putString("\r\n Mach ich nicht!\r\n");
    if((strcmp(cmd_buffer, ""))!=0){
        UNSET(VALUE_MODE);
    }
    //Buffer leeren!
    while(cmd_buffer_counter > 0){
        cmd_buffer[--cmd_buffer_counter] = 0x00;
    }//end while
}//end uart_interpret_value


/*****************************************************************************/
ISR(USART_TXC_vect) {
//    uart_putByte(0x07); //Macht BING aber Achtung: produziert natürlich eine Endlosschleife :)
}

#define UART_RBUFFER_SIZE 8
volatile char uart_rBuffer[UART_RBUFFER_SIZE];
volatile byte uart_rBuffer_rpos; //lese Position
volatile byte uart_rBuffer_wpos; //schreibe Position
/*
#define UART_WBUFFER_SIZE 8
char uart_wBuffer[UART_RBUFFER_SIZE];
byte uart_wBuffer_rpos; //lese Position
byte uart_wBuffer_wpos; //schreibe Position
*/
#define CMD_BUFFER_LENGTH 20
char cmd_buffer[CMD_BUFFER_LENGTH];
byte cmd_buffer_counter = 0;


//*********************************************************************//
void uart_interpret_char(void){
    char* pc = &uart_rBuffer[uart_rBuffer_rpos];
    switch(uart_rBuffer[uart_rBuffer_rpos]){
    case 0x0D: // ENTER
        SET(NEW_LINE);
    break;

    case 0x20: // SPACE
        SET(NEW_WORD);
    break;

    case 0x1b: // ESC
        SET(ESC_MODE);
    break;
    case 0x7F: // Backspace (Picocom) evtl. Zeichensatzprobleme ?
    case 0x08: // Backspace
        if(cmd_buffer_counter > 0){ //wenn min. ein Zeichen im Buffer
            cmd_buffer[--cmd_buffer_counter] = 0x00; //Löschen im Buffer
            // Löschen im Terminal
            uart_putByte('\b'); // ein Zeichen zurück
            uart_putByte(' ');  // Zeichen Leeren
            uart_putByte('\b'); // ein Zeichen zurück
        }else{
            uart_putByte(0x07);//beep
        }//end if
    break;

    default  :
        if((IS_SET(CMD_MODE)) | (IS_SET(VALUE_MODE))){
            //Gültiges Zeichen und ist noch Platz im Buffer?  
            if(*pc < 0x7F && *pc > 0x20 && cmd_buffer_counter < CMD_BUFFER_LENGTH-1){
                if(*pc > 0x60){ *pc = (*pc)&(~0x20); } //Alle Buchstaben Groß! 
               cmd_buffer[cmd_buffer_counter++] = *pc; // Zeichen Speichern
               uart_putByte(*pc); //Echo
            }else{
               uart_putByte(0x07);//beep
            }//end if(c < ...   
        }else{ //Einzelzeichen interpretation (Keypad)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }//end IS_SET(NOW_...
     }//end switch
 
//    uart_putByte(uart_rBuffer[uart_rBuffer_rpos]);//echo!

    //Nach der Verarbeitung:
    uart_rBuffer_rpos++;
    if(uart_rBuffer_rpos >= UART_RBUFFER_SIZE){uart_rBuffer_rpos = 0;}
    if(uart_rBuffer_rpos == uart_rBuffer_wpos){UNSET(NEW_CHAR);}
}//end uart_interpret_char

//*********************************************************************//
void uart_interpret_verb(void){
    if((strcmp(cmd_buffer, ""))==0){
        SET(CMD_MODE);
        uart_putByte(0x07);//beep
    }else if((myStrComp(cmd_buffer, "SET"))==0){
//    }else if((myStrComp(cmd_buffer, "set"))==0){
//    }else if((strcmp(cmd_buffer, "set"))==0){
        uart_putString("\b\b\bSET ");
        SET(VALUE_MODE);
    }else if((strcmp(cmd_buffer, "GET"))==0){
        uart_putString("\b\b\bGET ");
        SET(VALUE_MODE);
    }else if((strcmp(cmd_buffer, "abstand"))==0){
        uart_putString("\b\b\b\b\b\b\bABSTAND ");
        SET(VALUE_MODE);
    }else if((strcmp(cmd_buffer, "winkel"))==0){
        uart_putString("\b\b\b\b\b\bWINKEL ");
        SET(VALUE_MODE);
    }else{ 
        uart_putByte(0x07); //beep
        uart_putString("\r\n Befehl unbekannt!\r\n");
    }//ende der String-Vergleiche

    //Buffer leeren!
    while(cmd_buffer_counter > 0){
        uart_putByte(cmd_buffer[cmd_buffer_counter]);
        cmd_buffer[--cmd_buffer_counter] = 0x00;
    }//end while
}//end uart_interpret_verb
 
//*********************************************************************//
void uart_interpret_value(void){
    UNSET(NEW_WORD);
    //Prüfen ob ein int aus dem buffer gelesen werden kann!
    uart_putString("\r\n Mach ich nicht!\r\n");
    if((strcmp(cmd_buffer, ""))!=0){
        UNSET(VALUE_MODE);
    }
    //Buffer leeren!
    while(cmd_buffer_counter > 0){
        cmd_buffer[--cmd_buffer_counter] = 0x00;
    }//end while
}//end uart_interpret_value

/* //////////////////////////////////////////////////////////////////////////
//Alte ISR  -- RINGBUFFER
    char c = UDR;
    byte next_wpos = uart_rBuffer_wpos + 1;
    if(next_wpos >= UART_RBUFFER_SIZE){next_wpos = 0;}

    if(next_wpos == uart_rBuffer_rpos){   
        //Schreib-Pos. holt Lese-Pos. ein => Buffer ist Voll
        uart_putByte(0x07);//Bell
        //Zeichen wird verworfen!
    }else{
        uart_rBuffer[uart_rBuffer_wpos] = c;
        SET(NEW_CHAR);
        uart_rBuffer_wpos = next_wpos;
    }//end if
*/

        if(*value == '+'){
            SET(ADD_VALUE);
            value++;
        }else if(*value == '-'){
            SET(SUB_VALUE);
            value++;
        }//end if(VORZEICHEN)

        uint8 count = 0;
        char* ende = value;
        while(*ende <= 0x39 && *ende >= 0x30){ ende++; count++;} //gehe gültige ziffern durch
        if(*ende != '\0')
            uart_putString("\r\n Ungültiger Wert! (Value)\r\n");
            return;
        }else if(count > 5 || (count == 5 && *value > 0x35) //Wert > 59000 -> zu groß
            uart_putString("\r\n Wert zu groß!\r\n");
            return;
        }

        uint16  value_int = 0;
        while(*value != '\0'){
            //value_int = (value_int * 10) + Neue Ziffer
            value_int = (value_int << 3) + (value_int << 1) +  (*value - 0x30);
            value++;
        }//end while(value != ende)



