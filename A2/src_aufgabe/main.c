/**
 * @file main.c
 *
 * @brief Programm zum Testen von Bewegungsfunktionen.
 *
 *  Die Ausgabe auf der seriellen Schnittstelle kann per " #define PRINT_OUTPUT "
 *  mitkompiliert werden.
 *
 * @author Peter Rudolph
 *
 * @date 16.10.2012 10:51:24
 */
#if !(defined(TASK_1) || defined(TASK_2) || defined(TASK_3))
#error NO TASK DEFINED! (define at least one task as additional symbol in project!)
#endif

// system includes
#include <avr/io.h>
#include <util/delay.h>

// workspace includes
#include "asuro.h"

// project includes
#include "utils.h"
#include "movements.h"
/**
 * @brief LED toggle state
 */
uint8_t toggle_state = OFF;
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
 * @brief toggle status LED
 */
void toggleStatusLED(void);
/**
 * @brief toggle status LED and wait till switch pressed
 */
void waitForUser(void);
/**
 * @brief program entry point
 * @return exit_status (never reached)
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
#ifdef TASK_1
        /// turn 90 degrees
        turnInPlace(90);
        /// wait for user interaction
        waitForUser();
#endif
#if defined TASK_2
        /// drive 50 centimeters
        driveStraight(50);
        /// wait for user interaction
        waitForUser();
#endif
#ifdef TASK_3
        /// drive trinagle
		driveEquilateralTriangle(20)
        /// wait for user interaction
        waitForUser();
#endif
        /// continue main loop
    }
    /// return exit status (never reached)
    return 0;
}

void turnOnEncoderLEDs() {
    /// enable PD7
    ODOMETRIE_LED_ON;
}

void turnOffStatusLED() {
    /// disable PD2
    GREEN_LED_OFF;
    /// disable PB0
    RED_LED_OFF;
}

void enableADCInput() {
    /// set PC0 (ADC0) and PC1 (ADC1) data direction to input
    DDRC &= ~((1 << PC0) | (1 << PC1));
}

void initADC() {
    /// reset ADC configuration
    ADMUX = 0;
    ADCSRA = 0;
    // REFS1=0 && REFS0=1 -> AVCC with external capacitor at AREF pin (see manual, p. 199)
    /// set AVCC with external capacitor at AREF pin (see manual, p. 199)
    ADMUX |= (1 << REFS0);
    /// enable ADC
    ADCSRA |= (1 << ADEN);
    /// set pre-scaler to clock/128
    ADCSRA |= (1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2);
}

void toggleStatusLED(void) {
    /// toggle current LED state
    if (toggle_state) {
        GREEN_LED_OFF;
        RED_LED_ON;
        toggle_state = OFF;
    } else {
        GREEN_LED_ON;
        RED_LED_OFF;
        toggle_state = ON;
    }
}

void waitForUser() {
    volatile uint16_t counter = 0;
    /// toggle LED while no switch pressed
    while (!PollSwitch()) {
        if (!(counter++ % 500))
            toggleStatusLED();
    }
    /// turn of status LED if switch pressed
    turnOffStatusLED();
    /// wait a second
    _delay_ms(1000);
}
