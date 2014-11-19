/**
 * @file wheel_encoder.c
 *
 * @brief Programm zum Testen der ASURO Rad-"Encoder"-Einheit.
 *
 *  Die Ausgabe auf der seriellen Schnittstelle kann per " #define PRINT_OUTPUT "
 *  mitkompiliert werden.
 *
 * @author Ihr_Name
 *
 * @date 16.10.2012 10:51:24
 */

// system includes
#include <avr/io.h>

// project includes
#include "asuro.h"

/**
 * @brief set bit in variable
 * @param a target variable
 * @param b bit number
 */
#define BIT_SET(a,b) ((a) |= (1<<(b)))
/**
 * @brief clear bit in variable
 * @param a target variable
 * @param b bit number
 */
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
/**
 * @brief flip bit in variable
 * @param a target variable
 * @param b bit number
 */
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
/**
 * @brief check bit in variable
 * @param a target variable
 * @param b bit number
 */
#define BIT_CHECK(a,b) ((a) & (1<<(b)))
/**
 * @brief set bitmask in variable
 * @param x target variable
 * @param y bitmask
 */
#define BITMASK_SET(x,y) ((x) |= (y))
/**
 * @brief clear bitmask in variable
 * @param x target variable
 * @param y bitmask
 */
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
/**
 * @brief flip bitmask in variable
 * @param x target variable
 * @param y bitmask
 */
#define BITMASK_FLIP(x,y) ((x) ^= (y))
/**
 * @brief check bitmask in variable
 * @param x target variable
 * @param y bitmask
 */
#define BITMASK_CHECK(x,y) ((x) & (y))
/**
 * @brief right ADC threshold between black and white (simple, w/o hysterese)
 */
#define ODO_SENSITIVITY_RIGHT 650
/**
 * @brief  left ADC threshold between black and white (simple, w/o hysterese)
 */
#define ODO_SENSITIVITY_LEFT 450
/**
 * @brief  array containing both ADC values
 */
uint16_t encoderADC[2];
// function prototypes
/**
 * @brief turn on both wheel encoder LEDs
 */
void turnOnEncoderLEDs(void);
/**
 * @brief turn off Status LED
 */
void turnOffStatusLED(void);
/**
 * @brief enable photo-transistors as input
 */
void enableADCInput(void);
/**
 * @brief initialize ADCs
 */
void initADC(void);
/**
 * @brief update ADC values from both wheels
 */
void updateEncoderADCs(void);
/**
 * @brief  program entry point
 * @return  exit_status (never reached)
 */
int main(void) {
    /// init ASURO
    Init();
    /// turn off status LED
    turnOffStatusLED();
    /// turn on both wheel LEDs
    turnOnEncoderLEDs();
    /// enable input pins
    enableADCInput();
    /// initialize ADC
    initADC();
    /// enter main loop
    for (;;) {
        /// update encoder values
        updateEncoderADCs();
        /// check right encoder -> enable/disable PD6
        if (encoderADC[RIGHT] > ODO_SENSITIVITY_RIGHT) {
            // enable PD6 (front LED)
        } else {
            // disable PD6 (front LED)
        }
        /// check left encoder -> enable/disable PD2
        if (encoderADC[LEFT] > ODO_SENSITIVITY_LEFT) {
            // enable PD2 (status LED red)
        } else {
            // disable PD2 (status LED red)
        }
        /// continue main loop
#ifdef PRINT_OUTPUT
        /// print output
        SerPrint("\nenc_adc: left | right\n");
        PrintInt(encoderADC[LEFT]);
        SerPrint(" | ");
        PrintInt(encoderADC[RIGHT]);
#endif
    }
    /// return exit status (never reached)
    return 0;
}

void turnOnEncoderLEDs() {
    /// enable PD7
}

void turnOffStatusLED() {
    /// disable PD2
    /// disable PB0
}

void enableADCInput() {
    /// set PC0 (ADC0) and PC1 (ADC1) data direction to input
}

void initADC() {
    /// reset ADC configuration
    // REFS1=0 && REFS0=1 -> AVCC with external capacitor at AREF pin (see manual, p. 199)
    /// set AVCC with external capacitor at AREF pin (see manual, p. 199)
    /// enable ADC
    /// set pre-scaler to clock/128
}

void updateEncoderADCs() {
    /// due to 10 bit resolution we need an auxiliary register\n so we have to add high and low register
    /// disable MUXs -> right encoder (see manual, p. 199)
    /// set ADSC bit to 1 -> start the conversion
    /// wait till conversion finished
    /// clear ADC interrupt flag (ADIF)
    /// get right value from registers
    /// enable MUX0 -> left encoder (see manual, p. 199)
    /// set the ADSC bit to 1 - start the conversation
    /// wait for the conversation to stop
    /// clear ADC interrupt flag (ADIF)
    /// get left value from registers
}
