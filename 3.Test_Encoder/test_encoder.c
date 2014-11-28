/**
 * @file main.c
 * @brief Programm zum Testen einer Regelungsfunktionen zum Geradeausfahren.
 *  Die Ausgabe auf der seriellen Schnittstelle kann per " #define PRINT_OUTPUT "
 *  mitkompiliert werden.
 * @author Tobias Reitmaier
 * @date 19.10.2012 09:10:47
 */
// system includes
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "asuro.h"

#define SHOULD_TICKS 180
#define PRINT_OUTPUT

#define asuro_init() Init()

int is_ticks[2] = { 0, 0 }; /// @brief number of ticks we counted per second
int speed[2] = { 0, 0 };    /// @brief current controlled speed
float m_inverse[2];         /// @brief 1/m of the function y = m * x + t 
float t[2];                 /// @brief t of the function y = m * x + t 

//////////////////////////////////////////////////////////////////////////////
/// @brief  this linear regression function describes the context between the motor speed
/// and the measured number of ticks. It corresponds to the inverse function of y = m * x + t.
/// t = mean(y) - m * mean(x)
/// m = sum_i [(x_i - mean(x))(y_i - mean(y))] / sum_i [x_i - mean(x)]
/// @return the speed value for the given number of ticks
//////////////////////////////////////////////////////////////////////////////
int getSpeed(int ticks, float m_inverse, float t) {
	return roundf((ticks - t) * m_inverse);
}

//////////////////////////////////////////////////////////////////////////////
int main(void) {
	unsigned long time_start, time_end;

	/// individual values of the regression function for the left and the right wheel
	m_inverse[LEFT] = 0.95176;
	m_inverse[RIGHT] = 0.97979;

	t[LEFT] = 13.44175;
	t[RIGHT] = 25.57775;

  asuro_init();
	EncoderInit(); 	/// init encoders
	EncoderStart(); 	/// start encoder conversion

	/// start driving
	speed[LEFT] = getSpeed(SHOULD_TICKS, m_inverse[LEFT], t[LEFT]);
	speed[RIGHT] = getSpeed(SHOULD_TICKS, m_inverse[RIGHT], t[RIGHT]);
	MotorSpeed(speed[LEFT], speed[RIGHT]);

	for(;;){
		/// setup timing
		time_start = Gettime();
		time_end = time_start + 1000;

		/// get encoder values
		is_ticks[LEFT] = encoder[LEFT];
		is_ticks[RIGHT] = encoder[RIGHT];

		/// reset encoder values
		EncoderSet(0, 0);

		/// calculate the difference between the ticks of the left and the right wheel
		int ticks_diff = is_ticks[LEFT] - is_ticks[RIGHT];

		/// decrease the left ticks by the half difference
		speed[LEFT] = getSpeed(SHOULD_TICKS - floorf(ticks_diff / 2) - 1, m_inverse[LEFT], t[LEFT]);

		/// increase the right ticks by the half difference
		speed[RIGHT] = getSpeed(SHOULD_TICKS + ceilf(ticks_diff / 2) + 1, m_inverse[RIGHT],	t[RIGHT]);

		/// set the new speeds
		MotorSpeed(speed[LEFT], speed[RIGHT]);

    SerPrint( "\n\r Motor Speed(left|right): " ); /// print output
    if( speed[LEFT] < 1000 ) SerPrint( " " );
    if( speed[LEFT] < 100 ) SerPrint( " " );
    if( speed[LEFT] < 10 ) SerPrint( " " );
    PrintInt( speed[LEFT] );
    SerPrint( " ,  " );
    if( speed[RIGHT] < 1000 ) SerPrint( " " );
    if( speed[RIGHT] < 100 ) SerPrint( " " );
    if( speed[RIGHT] < 10 ) SerPrint( " " );
    PrintInt( speed[RIGHT] );

		/// wait to achieve timing
    while( Gettime() < time_end ){;}

  }//end mainloop

  return 0;
}//end main()

