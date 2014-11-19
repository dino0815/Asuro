/**
 * @file movements.h
 *
 * @brief Bewegungs-Test-Funktionen. (Header)
 *
 *  Die Ausgabe auf der seriellen Schnittstelle kann per " #define PRINT_OUTPUT "
 *  mitkompiliert werden.
 *
 * @author Peter Rudolph
 *
 * @date 16.10.2012 10:51:24
 */
#ifndef MOVEMENTS_H_
#define MOVEMENTS_H_
#include "asuro.h"
/**
 * @brief encoder state BLACK
 */
#define BLACK 0
/**
 * @brief encoder state WHITE
 */
#define WHITE 1
/**
 * @brief ASURO axle length in [mm]
 */
#define AXLE_LENGTH 104
/**
 * @brief pi approximated
 */
#define PI 3.14159265f
/**
 * @brief motor speed we want to turn with
 */
#define SPEED 150
/**
 * @brief ADC hysterese
 */
#define HYSTERESE 50
/**
 * @brief right ADC threshold between black and white
 */
#define ODO_SENSITIVITY_RIGHT 650
#define ODO_SENSITIVITY_RIGHT_WHITE (ODO_SENSITIVITY_RIGHT + HYSTERESE)
#define ODO_SENSITIVITY_RIGHT_BLACK (ODO_SENSITIVITY_RIGHT - HYSTERESE)
/**
 * @brief  left ADC threshold between black and white
 */
#define ODO_SENSITIVITY_LEFT 450
#define ODO_SENSITIVITY_LEFT_WHITE (ODO_SENSITIVITY_LEFT + HYSTERESE)
#define ODO_SENSITIVITY_LEFT_BLACK (ODO_SENSITIVITY_LEFT - HYSTERESE)
/**
 * @brief update ADC values from both wheels
 */
void updateEncoderADCs(void);
/**
 * @brief turn ASURO given degrees in place
 * @param degrees degrees to turn
 */
void turnInPlace(int16_t degrees);
/**
 * @brief drive ASURO straight given centimeters
 * @param centimeters centimeters to drive
 */
void driveStraight(int16_t centimeters);
/**
 * @brief drive ASURO an equilateral triangle with the given side length
 * @param centimeters of the side length
 */
void driveEquilateralTriangle(int16_t centimeters);
#endif /* MOVEMENTS_H_ */
