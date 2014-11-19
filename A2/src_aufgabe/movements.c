/**
 * @file movements.c
 *
 * @brief Bewegungs-Test-Funktionen.
 *
 *  Die Ausgabe auf der seriellen Schnittstelle kann per " #define PRINT_OUTPUT "
 *  mitkompiliert werden.
 *
 * @author Peter Rudolph
 *
 * @date 16.10.2012 10:51:24
 */

// print output
//#define PRINT_OUTPUT
#include <util/delay.h>

#include "movements.h"
/**
 * @brief array containing current ADC values
 */
uint16_t encoderADC[2] = { 0, 0 };

void updateEncoderADCs() {
	// use your solution from exercise 1
}

void turnInPlace(int16_t degrees) {
    /// init variables
    /// set counts to turn to apply given degrees
    /// set motor directions according to degrees sign
    /// get initial encoder value
    /// check left encoder -> black/white
    if (encoderADC[LEFT] > ODO_SENSITIVITY_LEFT_WHITE) {
        // set state to white
    } else {
        // set state to black
    }
    /// set motor speeds
    /// enter movement loop
    while (counts < counts_to_turn) {
        /// update encoder values
        /// check left encoder -> black/white
        if (encoderADC[LEFT] > ODO_SENSITIVITY_LEFT_WHITE) {
            // set state to white
        } else if(encoderADC[LEFT] < ODO_SENSITIVITY_LEFT_BLACK) {
            // set state to black
        }
        /// increment counts if encoder state switched
        if (encoder_state != last_encoder_state) {
            // increment counts
            // update state
        }
    }
    /// stop motors on finished movement
}

void driveStraight(int16_t centimeters) {
    /// init variables
    /// set movement parameter to apply given centimeters
    /// set motor directions according to degrees sign
    /// get initial encoder value
    /// check left encoder -> black/white
    if (encoderADC[LEFT] > ODO_SENSITIVITY_LEFT_WHITE) {
        // set state to white
    } else if(encoderADC[LEFT] < ODO_SENSITIVITY_LEFT_BLACK) {
        // set state to black
    }
    /// set motor speeds
    /// enter movement loop
    while (counts < counts_to_drive) {
        /// update encoder values
        /// check left encoder -> black/white
        if (encoderADC[LEFT] > ODO_SENSITIVITY_LEFT) {
            // set state to white
        } else {
            // set state to black
        }
        /// increment counts if encoder state switched
        if (encoder_state != last_encoder_state) {
            // increment counts
            // update state
        }
    }
    /// stop motors on finished movement
} 

void driveEquilateralTriangle(int16_t centimeters){
	int corner = 0;
    for (; corner < 3; corner++) {
        /// drive given centimeters
        /// turn 120 degrees
    }
}
