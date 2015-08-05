/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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

#ifndef _KEYBOARD_MOUSE_H_
#define _KEYBOARD_MOUSE_H_

#undef LCD

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <util/delay.h>     /* for _delay_ms() */
		#include <stdbool.h>
		#include <string.h>
		#include <stdlib.h>

		#include "Descriptors.h"

		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Board/Buttons.h>
		#include <LUFA/Drivers/Board/PS2.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>
		
		#define BV(bit)		(1<<(bit))

		#define HIGH		1
		#define LOW		0

	/* Macros: */
		/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
		#define LEDMASK_USB_NOTREADY      LEDS_LED1

		/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
		#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
		#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)

		/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
		#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)

	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);
		void EVENT_USB_Device_StartOfFrame(void);

		bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
		                                         uint8_t* const ReportID,
		                                         const uint8_t ReportType,
		                                         void* ReportData,
		                                         uint16_t* const ReportSize);
		void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
		                                          const uint8_t ReportID,
		                                          const uint8_t ReportType,
		                                          const void* ReportData,
		                                          const uint16_t ReportSize);
												  
		enum Mode_t
		{
			SPAMBUTTONS = 0, /** Spam buttons  */
			HOSTCOMMAND = 1, /** Wait for host command  */
		};
												  
		enum ButtonState_t
		{
			RELEASED = 0, /** Button is released */
			PRESSED = 1, /** Button is pressed  */
			IDLE = 2, /** Button is waiting condition timer  */
			READY = 3, /** Button ready to push after condition timer timeout */
		};

		enum Commands_t
		{
				CMD_WRITE_CONFIG = 0, /** Write active config to EEPROM */
				CMD_READ_CONFIG = 1, /** Load config from EEPROM */
				CMD_ADD_NODE = 2, /** Add button to active config */
				CMD_DELETE_NODE = 3, /** Delete button from active config */
				CMD_DELETE_ALL = 4, /** Delete all button from active config  */
				CMD_SET_MODIFIER = 5, /** set active config Modifier  */
				CMD_SET_HPCPMP = 6, /** set param values  */
				CMD_ADD_NODE_CONDITION = 7, /** set param values  */
				CMD_SET_MODE = 8, /** set mode  */
		};

		enum Params_t
		{
				mobHP = 0, /** Mob HP */
				playerHP = 1, /** Player HP */
				playerCP = 2, /** Player CP */
				playerMP = 3, /** Player MP */
		};



	/* Type Defines: */
	
					/** Type define for Button extended conditions
		 */
		typedef struct
		{
			// Conditions description
			uint8_t Type;
			uint8_t Min;
			uint8_t Max;
			void*    NextCondition;
		} Button_Task_Condition_t;

		/** Type define for Button task sheduler
		 */
		typedef struct
		{
			//void*    PrevTask;
			uint8_t Code;

			// Key description
			uint8_t PauseTime; // Siliance time when button is pressed (don't push other buttons). grade = 0.5s
			uint16_t ReleaseTime; //Cooldown time between buttons presses. grade = 0.1s
			uint8_t ConditionTime; // How many time condition must be active before press button. grade = 0.5s
			uint8_t state;
			uint32_t timer;
			Button_Task_Condition_t* condition;
			void*    NextTask;
		} Button_Task_Sheduler_t;
	

		void List_Delete_All_Coditions(Button_Task_Sheduler_t * Button);										  
	

#endif

