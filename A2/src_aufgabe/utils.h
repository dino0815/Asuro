/**
 * @file utils.h
 *
 * @brief Beinhaltet nützliche Funktions-Makros und Includes
 *
 * @author Peter Rudolph
 * @date 17.11.2012
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

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

#if !defined(__DOXYGEN__)
static inline int availableRAM();
#endif
/**
 * @brief check available bytes in RAM
 * @return available bytes count
 */
int availableRAM() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#endif /* UTILS_H_ */
