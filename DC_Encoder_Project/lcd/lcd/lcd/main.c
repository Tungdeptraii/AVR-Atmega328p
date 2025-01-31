/** 
 * ---------------------------------------------------+ 
 * @desc        Main file
 * ---------------------------------------------------+ 
 * @copyright   Copyright (C) 2020 Marian Hrinko.
 * @author      Marian Hrinko
 * @email       mato.hrinko@gmail.com
 * @datum       10.11.2020
 * @file        main.c
 * @version     1.0
 * @tested      AVR Atmega328p
 * ---------------------------------------------------+
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include "voltmeter.h"

/**
 * @desc   Main function
 *
 * @param  Void
 *
 * @return int
 */
int main(void)
{
  // voltmeter
  Voltmeter();

  // EXIT
  // ------------------------------------------------- 
  // return & exit
  return 0;
}