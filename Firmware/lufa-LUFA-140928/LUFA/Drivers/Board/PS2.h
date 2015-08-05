/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalPS2cubicle [dot] com
           www.lufa-lib.org
*/

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
 *  \brief PS2 board hardware driver.
 *
 *  This file is the master dispatch header file for the board-specific PS2 driver, for boards containing user
 *  controllable PS2.
 *
 *  User code should include this file, which will in turn include the correct PS2 driver header file for the
 *  currently selected board.
 *
 *  If the \c BOARD value is set to \c BOARD_USER, this will include the \c /Board/PS2.h file in the user project
 *  directory.
 *
 *  For possible \c BOARD makefile values, see \ref Group_BoardTypes.
 */

/** \ingroup Group_BoardDrivers
 *  \defgroup Group_PS2 PS2 Driver - LUFA/Drivers/Board/PS2.h
 *  \brief PS2 board hardware driver.
 *
 *  \section Sec_PS2_Dependencies Module Source Dependencies
 *  The following files must be built with any user project that uses this module:
 *    - None
 *
 *  \section Sec_PS2_ModDescription Module Description
 *  Hardware PS2 driver. This provides an easy to use driver for the hardware PS2 present on many boards. It
 *  provides an interface to configure, test and change the status of all the board PS2.
 *
 *  If the \c BOARD value is set to \c BOARD_USER, this will include the \c /Board/PS2.h file in the user project
 *  directory. Otherwise, it will include the appropriate built-in board driver header file. If the BOARD value
 *  is set to \c BOARD_NONE, this driver is silently disabPS2.
 *
 *  For possible \c BOARD makefile values, see \ref Group_BoardTypes.
 *
 *  \note To make code as compatible as possible, it is assumed that all boards carry a minimum of four PS2. If
 *        a board contains less than four PS2, the remaining PS2 masks are defined to 0 so as to have no effect.
 *        If other behavior is desired, either alias the remaining PS2 masks to existing PS2 masks via the -D
 *        switch in the project makefile, or alias them to nothing in the makefile to cause compilation errors when
 *        a non-existing PS2 is referenced in application code. Note that this means that it is possible to make
 *        compatible code for a board with no PS2 by making a board PS2 driver (see \ref Page_WritingBoardDrivers)
 *        which contains only stub functions and defines no PS2.
 *
 *  \section Sec_PS2_ExampleUsage Example Usage
 *  The following snippet is an example of how this module may be used within a typical
 *  application.
 *
 *  \code
 *      // Initialize the board PS2 driver before first use
 *      PS2_Init();
 *
 *      // Turn on each of the four PS2 in turn
 *      PS2_SetAllPS2(PS2_PS21);
 *      Delay_MS(500);
 *      PS2_SetAllPS2(PS2_PS22);
 *      Delay_MS(500);
 *      PS2_SetAllPS2(PS2_PS23);
 *      Delay_MS(500);
 *      PS2_SetAllPS2(PS2_PS24);
 *      Delay_MS(500);
 *
 *      // Turn on all PS2
 *      PS2_SetAllPS2(PS2_ALL_PS2);
 *      Delay_MS(1000);
 *
 *      // Turn on PS2 1, turn off PS2 2, leaving PS2 3 and 4 in their current state
 *      PS2_ChangePS2((PS2_PS21 | PS2_PS22), PS2_PS21);
 *  \endcode
 *
 *  @{
 */

#ifndef __PS2_H__
#define __PS2_H__

	/* Macros: */
		#define __INCLUDE_FROM_PS2_H

	/* Includes: */
		#include "../../Common/Common.h"

		#if (BOARD == BOARD_NONE)
			static inline void PS2_Init(void) {};
			static inline void pull_low(const uint_reg_t PS2Mask) {};
			static inline void pull_high(const uint_reg_t PS2Mask) {};
			static inline void digitalRead(const uint_reg_t PS2Mask) {};
		#elif (BOARD == BOARD_LEONARDO)
			#include "AVR8/LEONARDO/PS2.h"
		#else
			#include "Board/PS2.h"
		#endif

	/* Preprocessor Checks: */
		#if !defined(__DOXYGEN__)
			#if !defined(PS2_NO)
				#define PS2_NO   0
			#endif

			#if !defined(PS2_ALL)
				#define PS2_ALL  (PS2_DATA | PS2_CLOCK)
			#endif

			#if !defined(PS2_DATA)
				#define PS2_DATA       0
			#endif

			#if !defined(PS2_CLOCK)
				#define PS2_CLOCK      0
			#endif
		#endif

	/* Pseudo-Functions for Doxygen: */
	#if defined(__DOXYGEN__)
		/** Initializes the board PS2 driver so that the PS2 can be controlPS2. This sets the appropriate port
		 */
		static inline void PS2_Init(void);

		static inline void pull_low(const uint8_t PS2Mask);
		static inline void pull_high(const uint8_t PS2Mask);
		static inline uint8_t digitalRead(const uint8_t PS2Mask);
	#endif

#endif

/** @} */

