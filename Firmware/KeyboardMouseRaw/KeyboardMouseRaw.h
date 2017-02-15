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
		#include "ButtonTskScheduler.h"

		#include "tmk_core/ps2_mouse.h"
		#include "tmk_core/host.h"
		#include "tmk_core/ps2.h"
		#include "tmk_core/timer.h"

		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Board/Buttons.h>
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


		#define DEVICE_STATUS_MASK      0b00000011
		#define DEVICE_STATUS_STATE      0
		#define DEVICE_STATUS_ACTIVE     1
		
				/* Return status of device */
		// 0 - Device Status
		// 1 - Device Mode
		// 2 - Group 0 status
		// 3 - Group 1 status
		// 4 - Group 2 status
		// 5 - Group 3 status
		// 6 - Ctrl status
		// 7 - Shift status
		#define DEVICE_STATUS	0
		#define DEVICE_MODE		1
		#define GROUP_0			2
		#define GROUP_1			3
		#define GROUP_2			4
		#define GROUP_3			5
		#define DEVICE_CTRL		6
		#define DEVICE_SHIFT	7
		#define DEVICE_MASK      0b11111111

		
		
#define USB_MOUSE_BTN_MASK      0x1F
#define USB_MOUSE_BTN_LEFT      0
#define USB_MOUSE_BTN_RIGHT     1
#define USB_MOUSE_BTN_MIDDLE    2
#define USB_MOUSE_BTN_4th       3
#define USB_MOUSE_BTN_5th       4


	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);
		void EVENT_USB_Device_StartOfFrame(void);
		
		void PS2_to_USB_mouse_send(report_mouse_t *mouse_report);
		
		void led_indicator_task(uint16_t time_delta);
		void host_timeout_task(uint16_t time_delta);
		void toggleShowRcvStat(void) ;

		void setDebugFlag(int8_t state);
		uint8_t get_activeState(void);
		uint8_t get_deviceState(void);
		uint8_t get_deviceMode(void) ;
		uint8_t* get_newMouseHIDReportBuffer(void);
		uint8_t* get_newKeyboardHIDReportBuffer(void);
		uint8_t get_expectKeyboardReport(void);
		uint8_t get_expectMouseReport(void);
		void set_expectKeyboardReport(uint8_t state);
		void set_expectMouseReport(uint8_t state);
		
		void find_mouse(void);

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
												  

		enum Commands_t
		{
				CMD_WRITE_CONFIG = 0, /** Write active config to EEPROM */
				CMD_READ_CONFIG = 1, /** Load config from EEPROM */
				CMD_ADD_NODE = 2, /** Add button to active config */
				CMD_DELETE_NODE = 3, /** Delete button from active config */
				CMD_DELETE_ALL = 4, /** Delete all button from active config  */
				CMD_UNUSED = 5, /** unused  */
				CMD_SET_HPCPMP = 6, /** set param values  */
				CMD_ADD_NODE_CONDITION = 7, /** set param values  */
				CMD_SET_TARGET_STATE = 8, /** set star state and type of target, old version  */
				CMD_SET_SKILL_STATE = 9, /** set skill state, new version  */
		};
		
		
#define SERVICE_CONFIG     0     /**< set Device config */
#define SERVICE_MOUSE      1     /**< set Mouse msg */
#define SERVICE_DEVICE     2     /**< set Device operating mode */
#define SERVICE_KEYBOARD   3     /**< set Keyboard msg */

		

#endif

