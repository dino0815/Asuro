/**
 * @file main.c
 *
 * @brief Programm zum Testen einer Regelungsfunktionen zum Geradeausfahren.
 *
 *  Die Ausgabe auf der seriellen Schnittstelle kann per " #define PRINT_OUTPUT "
 *  mitkompiliert werden.
 *
 * @author Tobias Reitmaier
 *
 * @date 19.10.2012 09:10:47
 */
// system includes
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

// workspace includes
#include "asuro.h"

// project includes
// global variables

/**
 * @brief number of ticks we want to count per second
 *
 * Entsprechend der geforderten Geschwindigkeit, ergeben sich die ticks_should wie folgt :
 * 
 * v in [m/s] * t in [s] / meter pro tick in [m/tick] 
 *
 */
#define SHOULD_TICKS 180
/**
 * @brief number of ticks we counted per second
 */
int is_ticks[2] = { 0, 0 };
/**
 * @brief current controlled speed
 */
int speed[2] = { 0, 0 };
/**
 * @brief 1/m of the function y = m * x + t
 */
float m_inverse[2];
/**
 * @brief t of the function y = m * x + t
 */
float t[2];

// function prototypes
/**
 * @brief  this linear regression function describes the context between the motor speed
 * and the measured number of ticks. It corresponds to the inverse function of y = m * x + t.
 *
 * @return the speed value for the given number of ticks
 */
int getSpeed(int ticks, float m_inverse, float t);
/**
 * @brief  program entry point
 * @return  exit_status (never reached)
 */
int main(void) {
    /// init variables
    unsigned long time_start, time_end;
	/// set the individual values of the regression function for the left and the right wheel
    /// init ASURO
    Init();
    /// init encoders (asuro-lib)
    /// start encoder conversion (asuro-lib)
	/// start driving (use getSpeed(SHOULD_TICKS) to get the correct speed values)
	/// set motor speed (asuro-lib)
    /// enter main loop
    for (;;) {
        /// setup timing (start & end time(start time + 1sec) | (asuro-lib))
        /// setup is_ticks from encoder values (asuro-lib)
        /// reset encoder values (asuro-lib)
		/// calculate the differenz of is_ticks for the left and the right wheel
        /// control speed (SHOULD_TICKS -+ (ticks_diff / 2))
        /// limit speed if needed
        /// set motor speed (asuro-lib)
        /// wait to achieve timing (asuro-lib)
    }
    /// return exit status (never reached)
    return 0;
}

int getSpeed(int ticks, float m_inverse, float t) {
	/// the inverse function of the linear regression
	return 0;
}
