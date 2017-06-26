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

/** \file
 *
 *  Main source file for the KeyboardMouseRaw demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "KeyboardMouseRaw.h"

uint8_t Boot_Key ATTR_NO_INIT; //__attribute__ ((section (".noinit")))
#define MAGIC_BOOT_KEY            0xCA

#define BOOTLOADER_START_ADDRESS  0x3800 //((FLASH_SIZE_BYTES - BOOTLOADER_SEC_SIZE_BYTES) >> 1)
void Bootloader_Jump_Check(void) ATTR_INIT_SECTION(3);


/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** Buffer to hold the previously generated Mouse HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevMouseHIDReportBuffer[sizeof(report_mouse_t)];

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[OUT_REPORT_SIZE];

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
//static uint8_t PrevRawReportBuffer[GENERIC_REPORT_SIZE];

static uint8_t  newMouseHIDReportBuffer[sizeof(report_mouse_t)];
static uint8_t  newKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/* EEPROM config */

static uint8_t deviceStatus = 0;		/* DE device on/off state */

		
uint8_t get_deviceStatus(void) {return deviceStatus;};
void set_deviceStatus(int8_t state) {deviceStatus = state;};

uint8_t get_deviceState(void) {return deviceStatus& (1<<DEVICE_STATUS);};
void set_deviceState(int8_t state) {
	deviceStatus &= ~(1<<DEVICE_STATUS);
	deviceStatus |= state&(1<<DEVICE_STATUS);
};

uint8_t get_modifierState(void) {return deviceStatus& ((1<<DEVICE_CTRL) | (1<<DEVICE_SHIFT));};
void set_modifierState(int8_t state) {
	deviceStatus &= ~((1<<DEVICE_CTRL) | (1<<DEVICE_SHIFT));
	deviceStatus |= state&((1<<DEVICE_CTRL) | (1<<DEVICE_SHIFT));
};


uint8_t get_modeState(void) {return deviceStatus& (1<<DEVICE_MODE);};
void set_modeState(int8_t state) {
	deviceStatus &= ~(1<<DEVICE_MODE);
	deviceStatus |= state&(1<<DEVICE_MODE);
};



static uint8_t groupsStatus = 0;		/* DE groups state */
uint8_t get_groupsStatus(void) {return groupsStatus;};
void set_groupsStatus(uint8_t groups) {groupsStatus = groups;};

static uint8_t DebugFlag = 0;	
static uint8_t ShowRcvStat = 0;	
void toggleShowRcvStat(void) {ShowRcvStat = 1 - ShowRcvStat;};


void setDebugFlag(int8_t state) {	DebugFlag = state;};

#define TOGGLEIDLE 1000   /* DE period for push pull key ms */
static uint16_t toggleCounter = 0;    		/* DE timer for push pull key*/

static uint8_t expectKeyboardReport = 0;		/* flag to indicate if we expect an USB-report */
static uint8_t expectMouseReport = 0;		/* flag to indicate if we expect an USB-report */


uint8_t* get_newMouseHIDReportBuffer(void){return newMouseHIDReportBuffer;};
uint8_t* get_newKeyboardHIDReportBuffer(void){return newKeyboardHIDReportBuffer;};
uint8_t get_expectKeyboardReport(void){return expectKeyboardReport;};
uint8_t get_expectMouseReport(void){return expectMouseReport;};
void set_expectKeyboardReport(uint8_t state){expectKeyboardReport = state;};
void set_expectMouseReport(uint8_t state){expectMouseReport = state;};


static host_driver_t mouse_driver;

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the keyboard HID
 *  interface within the device.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = INTERFACE_ID_Keyboard,
				.ReportINEndpoint             =
					{
						.Address              = KEYBOARD_IN_EPADDR,
						.Size                 = HID_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
			},
	};

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the mouse HID
 *  interface within the device.
 */
USB_ClassInfo_HID_Device_t Mouse_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = INTERFACE_ID_Mouse,
				.ReportINEndpoint             =
					{
						.Address              = MOUSE_IN_EPADDR,
						.Size                 = HID_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevMouseHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevMouseHIDReportBuffer),
			},
	};



/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Generic_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = INTERFACE_ID_GenericHID,
				.ReportINEndpoint             =
					{
						.Address              = GENERIC_IN_EPADDR,
						.Size                 = HID_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
			},
	};



/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{

	static uint16_t last_led_timer = 0;
	static uint8_t prevButton1State = 0;		/* prev button1 state */
	static uint8_t keycode = 0x68;		/* prev button1 state */

	SetupHardware();
	_delay_us(1000);
	prevButton1State =  Buttons_GetStatus()& BUTTONS_BUTTON1;

	LEDs_TurnOffLEDs(LEDS_ALL_LEDS);

	if(Boot_Key == MAGIC_BOOT_KEY) {
		for (int i = 0; i<10; i++){
			LEDs_ToggleLEDs(LEDS_LED1);
			Delay_MS(i*30);
			LEDs_ToggleLEDs(LEDS_LED1);
			Delay_MS(100);
		}
	}

	GlobalInterruptEnable();
	
		
	for (;;)
	{			
		uint8_t Button1State = Buttons_GetStatus();
		if(prevButton1State > Button1State){
			//set_deviceState(1-get_deviceState());
			// Create keyboard report 
			if (expectKeyboardReport == 0) {	
				USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer;
				newKeyboardReport->KeyCode[0] = keycode++;
				newKeyboardReport->KeyCode[1] = 0;
				newKeyboardReport->KeyCode[2] = 0;
				newKeyboardReport->KeyCode[3] = 0;
				newKeyboardReport->KeyCode[4] = 0;
				newKeyboardReport->KeyCode[5] = 0; //0x22 == "5";
				newKeyboardReport->Modifier = 0;			
				expectKeyboardReport = 1;
			};
		}
		prevButton1State = Button1State;


		ps2_mouse_task();		
		led_indicator_task(timer_elapsed(last_led_timer));
		host_timeout_task(timer_elapsed(last_led_timer));
		if(get_modeState() == 0) spam_buttons_task(timer_elapsed(last_led_timer));
		last_led_timer = timer_read();
		HID_Device_USBTask(&Keyboard_HID_Interface);
		HID_Device_USBTask(&Mouse_HID_Interface);
		HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();
	}
}


void PS2_to_USB_mouse_send(report_mouse_t *mouse_report){

	static uint8_t ps2mouse_suspend=0;
    report_mouse_t* newMouseReport =  (report_mouse_t*)newMouseHIDReportBuffer;
	
	mouse_report->buttons = mouse_report->buttons&USB_MOUSE_BTN_MASK;
	
    if(expectMouseReport == 0 && ps2mouse_suspend == 0){

	mouse_report->x = (mouse_report->x < -63) ? -127 : mouse_report->x*2;
	mouse_report->x = (mouse_report->x >  63) ?  127 : mouse_report->x*2;

	mouse_report->y = (mouse_report->y < -63) ? -127 : mouse_report->y*2;
	mouse_report->y = (mouse_report->y >  63) ?  127 : mouse_report->y*2;


	newMouseReport->x = mouse_report->x;
	newMouseReport->y = mouse_report->y;
	newMouseReport->v = mouse_report->v;
	newMouseReport->h = mouse_report->h;
	newMouseReport->buttons = mouse_report->buttons; //|= (1 << 0);
	expectMouseReport = 1;

    }
	switch(mouse_report->buttons){
		case (1 << USB_MOUSE_BTN_4th):
			set_deviceState(1);
			break;
		case ((1 << USB_MOUSE_BTN_MIDDLE)|(1 << USB_MOUSE_BTN_LEFT)):
			set_deviceState(1);
			break;
		case (1 << USB_MOUSE_BTN_5th):
			set_deviceState(0);
			break;
		case ((1 << USB_MOUSE_BTN_MIDDLE)|(1 << USB_MOUSE_BTN_RIGHT)):
			set_deviceState(0);
			break;
		case ((1 << USB_MOUSE_BTN_4th)|(1 << USB_MOUSE_BTN_MIDDLE)):
			ps2mouse_suspend = 0;
			break;
		case ((1 << USB_MOUSE_BTN_5th)|(1 << USB_MOUSE_BTN_MIDDLE)):
			ps2mouse_suspend = 1;
			break;
	}
}


//ISR(TIMER0_COMPA_vect, ISR_BLOCK)
void led_indicator_task(uint16_t time_delta)
{
	// Control device state LED
	if(get_deviceState() == 0){
		toggleCounter+=time_delta;
		if(toggleCounter >= TOGGLEIDLE/5){
			LEDs_ToggleLEDs(LEDS_LED1);
			toggleCounter = 0;
		}
	} else {
		if(DebugFlag > 0){
			toggleCounter+=time_delta;
			if(toggleCounter >= TOGGLEIDLE/20){
				LEDs_ToggleLEDs(LEDS_LED1);
				toggleCounter = 0;
			}
		} else if(get_modeState() > 0){
			toggleCounter+=time_delta;
			if(toggleCounter >= TOGGLEIDLE/100){
				LEDs_ToggleLEDs(LEDS_LED1);
				toggleCounter = 0;
			}
		} else {
		
			if(ShowRcvStat == 0){
				LEDs_TurnOffLEDs(LEDS_LED1);
			} else {
		
				LEDs_TurnOnLEDs(LEDS_LED1);
			}
		}
	}	
	
}




void find_mouse(void){
	if(ps2_get_mouse_type() == PS2_MOUSE_NONE){
		LEDs_TurnOffLEDs(LEDS_ALL_LEDS);
		uint8_t ps2_mouse = ps2_mouse_init();
		if(ps2_mouse == PS2_MOUSE_NONE ) {
			LEDs_TurnOnLEDs(LEDS_LED1);
		} else if(ps2_mouse == PS2_MOUSE_GENIUS5 ) {
			LEDs_TurnOnLEDs(LEDS_LED2);
		} else if(ps2_mouse == PS2_MOUSE_STANDARD ) {
			LEDs_TurnOnLEDs(LEDS_LED3);
		} else if(ps2_mouse == PS2_MOUSE_MIE ) {
			LEDs_TurnOnLEDs(LEDS_LED3);
			LEDs_TurnOnLEDs(LEDS_LED2);
		} 
	}
}



/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware()
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	Buttons_Init();
	LEDs_Init();
	USB_Init();
	timer_init();

	mouse_driver.send_mouse = &PS2_to_USB_mouse_send;
	host_set_driver(&mouse_driver);

	find_mouse();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{

}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
 	/* Stop Millisecond Timer Interrupt */
	TCCR0B = 0;

}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);
	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Mouse_HID_Interface);
	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

	USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
	HID_Device_ProcessControlRequest(&Mouse_HID_Interface);
	HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
	HID_Device_MillisecondElapsed(&Mouse_HID_Interface);
	HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{

	if ((HIDInterfaceInfo != &Keyboard_HID_Interface)&&(HIDInterfaceInfo != &Mouse_HID_Interface)) LEDs_ToggleLEDs(LEDS_LED3);

	/* Determine which interface must have its report generated */
	if (HIDInterfaceInfo == &Keyboard_HID_Interface)
	{
		USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
		USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer;

		KeyboardReport->Modifier = 0x00;
		memset( KeyboardReport->KeyCode, 0x00, sizeof(KeyboardReport->KeyCode) );

		// Create keyboard report 
		if (expectKeyboardReport != 0) {	
			if (get_deviceState() > 0) {
				unsigned char Modifier = (get_modifierState()) >> DEVICE_CTRL;
				KeyboardReport->Modifier = newKeyboardReport->Modifier | Modifier;
			} else {
				KeyboardReport->Modifier = 0;			
			}
//			memcpy_P( newKeyboardReport->KeyCode, KeyboardReport->KeyCode, sizeof(KeyboardReport->KeyCode) ); 
			KeyboardReport->KeyCode[0] = newKeyboardReport->KeyCode[0];
			KeyboardReport->KeyCode[1] = newKeyboardReport->KeyCode[1];
			KeyboardReport->KeyCode[2] = newKeyboardReport->KeyCode[2];
			KeyboardReport->KeyCode[3] = newKeyboardReport->KeyCode[3];
			KeyboardReport->KeyCode[4] = newKeyboardReport->KeyCode[4];
			KeyboardReport->KeyCode[5] = newKeyboardReport->KeyCode[5]; //0x22 == "5";

			expectKeyboardReport = 0;
		};	
	
		*ReportSize = sizeof(USB_KeyboardReport_Data_t);
		return false;
	}
	else if (HIDInterfaceInfo == &Mouse_HID_Interface)
	{
		report_mouse_t* MouseReport = (report_mouse_t*)ReportData;
		report_mouse_t* newMouseReport = (report_mouse_t*)newMouseHIDReportBuffer;

		MouseReport->x = 0x00;
		MouseReport->y = 0x00;
		MouseReport->h = 0x00;
		MouseReport->v = 0x00;
		//MouseReport->buttons = 0x00;

		// Create mouse report 
		if (expectMouseReport != 0) {	
			MouseReport->x = newMouseReport->x;
			MouseReport->y = newMouseReport->y;
			MouseReport->v = newMouseReport->v;
			MouseReport->h = newMouseReport->h;
			MouseReport->buttons = newMouseReport->buttons;
			expectMouseReport = 0;
		};	
		*ReportSize = sizeof(report_mouse_t);
		return true;
	}
	else if (HIDInterfaceInfo == &Generic_HID_Interface)
	{
		uint8_t *RawReport = (uint8_t*)ReportData;
		LEDs_ToggleLEDs(LEDS_LED2);

		/* Return status of device */
		// 0 - Device Status
		// 1 - Device Mode
		// 2 - NC
		// 3 - NC
		// 4 - NC
		// 5 - NC
		// 6 - Ctrl status
		// 7 - Shift status
		RawReport[0] = get_deviceStatus();
		RawReport[1] = get_groupsStatus();

		*ReportSize = 2;
		return true;
	} 
	return false;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{


	if (HIDInterfaceInfo == &Keyboard_HID_Interface)
	{
		/* // Recieve Keyboard leds state
		uint8_t  LEDMask   = LEDS_NO_LEDS;
		uint8_t* LEDReport = (uint8_t*)ReportData;

		if (*LEDReport & HID_KEYBOARD_LED_NUMLOCK)
		  LEDMask |= LEDS_LED1;

		if (*LEDReport & HID_KEYBOARD_LED_CAPSLOCK)
		  LEDMask |= LEDS_LED2;

		if (*LEDReport & HID_KEYBOARD_LED_SCROLLLOCK)
		  LEDMask |= LEDS_LED3;

		LEDs_SetAllLEDs(LEDMask);
		*/
	} else if(HIDInterfaceInfo == &Generic_HID_Interface) {
		uint8_t *RawReport = (uint8_t*)ReportData;
		uint8_t Service = RawReport[0];		
		
		if(Service == SERVICE_CONFIG){
			uint8_t Command = RawReport[1];	
			switch(Command){
				case CMD_ADD_NODE:
					List_Add_Node(RawReport+2); 
					break;
				case CMD_DELETE_NODE:
					List_Delete_Node(RawReport[2]);
					break;
				case CMD_DELETE_ALL:
					List_Delete_All();
					break;
				case CMD_SET_SKILL_STATE:
					toggleShowRcvStat(); 
					set_SkillState(RawReport+2); // 
					break;
				case CMD_JUMP_TO_BOOTLOADER:
					Jump_To_Bootloader(); 
					break;
				default:
					break;
			}
		} else if(Service == SERVICE_MOUSE){
			if(get_modeState() > 0) {
				report_mouse_t* newMouseReport = (report_mouse_t*)newMouseHIDReportBuffer;
				newMouseReport->x =	RawReport[1]-128;
				newMouseReport->y =	RawReport[2]-128;
				newMouseReport->buttons =	RawReport[3];
				expectMouseReport = 1;		
			}
		} else if(Service == SERVICE_DEVICE){
		// Set device status
		// 0 - Device Status
		// 1 - Device Mode
		// 2 - NC
		// 3 - NC
		// 4 - NC
		// 5 - NC
		// 6 - Ctrl state
		// 7 - Shift state

			set_deviceStatus(RawReport[1]);
			set_groupsStatus(RawReport[2]);

			expectMouseReport = 0;		
			expectKeyboardReport = 0;
		} else if(Service == SERVICE_KEYBOARD){					
			if(get_modeState() > 0) {
				USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer;
				newKeyboardReport->KeyCode[0] = RawReport[1];
				newKeyboardReport->KeyCode[1] = RawReport[2];
				newKeyboardReport->KeyCode[2] = RawReport[3];
				newKeyboardReport->KeyCode[3] = RawReport[4];
				newKeyboardReport->KeyCode[4] = RawReport[5];
				newKeyboardReport->KeyCode[5] = RawReport[6]; //0x22 == "5";
				newKeyboardReport->Modifier = RawReport[7];			
				expectKeyboardReport = 1;
			} 
		}  
		
	}
}


// Entering the Bootloader via Software 

void Bootloader_Jump_Check(void)
{
	Boot_Key = eeprom_read_byte((uint8_t*)CONFIG_HEADER_ADDRESS);
	if ( (Boot_Key == MAGIC_BOOT_KEY))
    {	
        Boot_Key = 0;
		eeprom_write_byte((uint8_t*)CONFIG_HEADER_ADDRESS, Boot_Key);
		Boot_Key = MAGIC_BOOT_KEY;
        ((void (*)(void))BOOTLOADER_START_ADDRESS)();
    }
}

void Jump_To_Bootloader(void)
{
	wdt_disable();
    // If USB is used, detach from the bus and reset it
    USB_Disable();
    // Disable all interrupts
    cli();
	LEDs_TurnOffLEDs(LEDS_LED1);
    // Wait two seconds for the USB detachment to register on the host
	for (int i = 0; i<10; i++){
		LEDs_ToggleLEDs(LEDS_LED1);
		Delay_MS(80);
		LEDs_ToggleLEDs(LEDS_LED1);
		Delay_MS(120);
	}
	LEDs_TurnOnLEDs(LEDS_LED1);
	Delay_MS(300);
	LEDs_TurnOffLEDs(LEDS_LED1);
	Delay_MS(100);
	
    // Set the bootloader key to the magic value and force a reset
    Boot_Key = MAGIC_BOOT_KEY;
	eeprom_write_byte((uint8_t*)CONFIG_HEADER_ADDRESS, Boot_Key);
    wdt_enable(WDTO_250MS);
    for (;;){
	    LEDs_ToggleLEDs(LEDS_LED1);
		Delay_MS(30);
	};
}