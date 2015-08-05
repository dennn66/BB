/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalPS2cubicle [dot] com
           www.lufa-lib.org

/*
  Copyright 2014  Dean Camera (dean [at] fourwalPS2cubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *  \brief Board specific PS2 driver header for the Arduino Leonardo board.
 *  \copydetails Group_PS2s_LEONARDO
 *
 *  \note This file should not be included directly. It is automatically included as needed by the PS2s driver
 *        dispatch header located in LUFA/Drivers/Board/PS2.h.
 */

/** \ingroup Group_PS2
 *  \defgroup Group_PS2_LEONARDO LEONARDO
 *  \brief Board specific PS2 driver header for the Arduino Leonardo board.
 *
 *  Board specific PS2 driver header for the Arduino Leonardo board (http://arduino.cc/en/Main/arduinoBoardLeonardo).
 *
 *  <table>
 *    <tr><th>Name</th><th>Color</th><th>Info</th><th>Active Level</th><th>Port Pin</th></tr>
 *    <tr><td>PS2_DATA</td><td>D5</td><td>PORTC.6</td></tr>
 *    <tr><td>PS2_CLOCK</td><td>D8</td><td>PORTB.4</td></tr>
 *  </table>
 *
 *  @{
 */

#ifndef __PS2_LEONARDO_H__
#define __PS2_LEONARDO_H__

	/* Includes: */
		#include "../../../../Common/Common.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_PS2_H)
			#error Do not include this file directly. Include LUFA/Drivers/Board/PS2.h instead.
		#endif

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Macros: */
			#define PS2_PORTC_PS2S       (PS2_DATA)
			#define PS2_PORTB_PS2S       (PS2_CLOCK)
	#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** PS2 mask for the PS2 data wire on the board. */
			#define PS2_DATA        (1 << 6)

			/** PS2 mask for the PS2 clock wire on the board. */
			#define PS2_CLOCK        (1 << 4)

			/** PS2 mask for all the PS2s on the board. */
			#define PS2_ALL    (PS2_DATA | PS2_CLOCK)

			/** PS2 mask for none of the board PS2s. */
			#define PS2_NO     0

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void PS2_Init(void)
			{
				DDRB  |=  PS2_PORTB_PS2S; //OUTPUT
				PORTB &= ~PS2_PORTB_PS2S; //LOW
				DDRC  |=  PS2_PORTC_PS2S; //OUTPUT
				PORTC &= ~PS2_PORTC_PS2S; //LOW
			}

			static inline void pull_low(const uint8_t PS2Mask)
			{
				DDRB  |=  (PS2Mask & PS2_PORTB_PS2S); //OUTPUT
				DDRC  |=  (PS2Mask & PS2_PORTC_PS2S); //OUTPUT
				PORTB &= ~(PS2Mask & PS2_PORTB_PS2S); //LOW
				PORTC &= ~(PS2Mask & PS2_PORTC_PS2S); //LOW
			}

			static inline void pull_high(const uint8_t PS2Mask)
			{
				DDRB  &= ~(PS2Mask & PS2_PORTB_PS2S); //INPUT
				DDRC  &= ~(PS2Mask & PS2_PORTC_PS2S); //INPUT
				PORTB |=  (PS2Mask & PS2_PORTB_PS2S); //HIGH
				PORTC |=  (PS2Mask & PS2_PORTC_PS2S); //HIGH
			}

			static inline uint8_t digitalRead(const uint8_t PS2Mask)
			{
				DDRB  &= ~(PS2Mask & PS2_PORTB_PS2S); //INPUT
				DDRC  &= ~(PS2Mask & PS2_PORTC_PS2S); //INPUT
				PORTB |=  (PS2Mask & PS2_PORTB_PS2S); //HIGH
				PORTC |=  (PS2Mask & PS2_PORTC_PS2S); //HIGH
				_delay_us(1);
				return  (PINB & (PS2Mask & PS2_PORTB_PS2S))|
					(PINC & (PS2Mask & PS2_PORTC_PS2S));
			}

		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */
