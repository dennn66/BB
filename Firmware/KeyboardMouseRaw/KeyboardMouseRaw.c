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
#include "tmk_core/ps2_mouse.h"
#include "tmk_core/host.h"
#include "tmk_core/ps2.h"

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** Buffer to hold the previously generated Mouse HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevMouseHIDReportBuffer[sizeof(report_mouse_t)];

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
//static uint8_t PrevRawReportBuffer[GENERIC_REPORT_SIZE];

static uint8_t  newMouseHIDReportBuffer[sizeof(report_mouse_t)];
static uint8_t  newKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/* EEPROM config */
#define CONFIG_HEADER_ADDRESS 0
#define CONFIG_MODIFIER_ADDRESS 1
#define CONFIG_DATA_ADDRESS 2
#define CONFIG_DATA_BLOCK_SIZE 5

static uint8_t deviceState = 0;		/* DE device on/off state */
static uint8_t Mode = 0;		/* Spam buttons mode */

/* DE 0-100 - Level in %, 0xFF - unknown */
static uint8_t Params[] = {0xFF, 0xFF, 0xFF, 0xFF};

#define UPDATETIMEOUT 2000   /* DE period for push pull key ms */
static uint16_t UpdateCounter = 0;    		/* HPMPCP update counter, ms*/


static uint32_t PauseTimer = 0; //ms;    	/* Spam buttons timeout, 2s*/


#define TOGGLEIDLE 1000   /* DE period for push pull key ms */
static uint16_t toggleCounter = 0;    		/* DE timer for push pull key*/

static uint8_t expectKeyboardReport = 0;		/* flag to indicate if we expect an USB-report */
static uint8_t expectMouseReport = 0;		/* flag to indicate if we expect an USB-report */
static uint8_t prevButton1State = 0;		/* prev button1 state */

static Button_Task_Sheduler_t* Buttons = 0;
static Button_Task_Sheduler_t* HostButton = 0;

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


Button_Task_Sheduler_t * List_Find_Node(uint8_t KeyCode){
	Button_Task_Sheduler_t * Button=0;
	
	Button = Buttons;
	while(Button !=0) {
		if(Button->Code == KeyCode) return Button;
		Button = (Button_Task_Sheduler_t *) Button->NextTask;
	}
	return (Button_Task_Sheduler_t *) 0;
}


void List_Add_Node(uint8_t KeyCode, uint8_t PauseTime, uint16_t ReleaseTime, uint8_t ConditionTime){
	Button_Task_Sheduler_t * Button=0;
	Button_Task_Sheduler_t * PrevButton=0;
	
	//if(ReleaseTime > 1500) LEDs_TurnOnLEDs(LEDS_LED1);


	
	Button = Buttons;
	while(Button !=0 && (Button->Code != KeyCode)){
		PrevButton = Button;
		Button = (Button_Task_Sheduler_t *) Button->NextTask;
	}
	if(Button == 0){
		Button = (Button_Task_Sheduler_t *)malloc(sizeof(Button_Task_Sheduler_t));
		if(Button == 0) {
			LEDs_TurnOnLEDs(LEDS_LED1);
		} else {
			Button->state = RELEASED;
			Button->timer = 0;
			Button->PauseTime =PauseTime; //0.5s grade
			Button->ReleaseTime = ReleaseTime; //0.1s grade
			Button->ConditionTime = ConditionTime; //0.5s grade
			Button->Code = KeyCode;
			Button->condition = 0;
			Button->NextTask= (void *)0;
			if(Buttons == 0) Buttons = Button;
			else PrevButton->NextTask=(void *)Button;
		}
	} else {
			Button->state = RELEASED;
			Button->PauseTime =PauseTime; //0.5s grade
			Button->ReleaseTime = ReleaseTime; //0.1s grade
			Button->ConditionTime = ConditionTime; //0.5s grade
			Button->timer = 0;
			List_Delete_All_Coditions(Button);
	}
//if(Button->ReleaseTime > 1500) LEDs_TurnOnLEDs(LEDS_LED1);
}

Button_Task_Condition_t * List_Add_Condition(uint8_t KeyCode, uint8_t ctype){
	Button_Task_Sheduler_t * Button=0;
	Button_Task_Condition_t * Condition =0;
	Button_Task_Condition_t * PrevCondition =0;
	
	Button = List_Find_Node( KeyCode);
	if(Button == 0) return( (Button_Task_Condition_t *)0);
	Condition = Button->condition;
	
	while(Condition != 0){
		if(Condition->Type == ctype) return Condition;
		PrevCondition = Condition;
		Condition = (Button_Task_Condition_t *) Condition->NextCondition;
	}
	
	Condition = (Button_Task_Condition_t *)malloc(sizeof(Button_Task_Condition_t));
	if(Condition == 0) {
		LEDs_TurnOnLEDs(LEDS_LED1);
		return( (Button_Task_Condition_t *)0);
	} 

	Condition->NextCondition =  (void *)0;
	Condition->Type = ctype;
	
	if(Button->condition == 0) {
		Button->condition = Condition;
	} else {
		PrevCondition-> NextCondition = (void *) Condition;
	}
	
	return Condition;	
}

void List_Update_Condition(uint8_t KeyCode, uint8_t ctype, uint8_t cmin, uint8_t cmax){
	Button_Task_Condition_t * Condition = 0;
	Condition = List_Add_Condition(KeyCode, ctype);
	if(Condition != 0){
		Condition->Min = cmin;
		Condition->Max =cmax; 
	} 
}

void List_Delete_Condition(uint8_t KeyCode, uint8_t ctype){
	Button_Task_Sheduler_t * Button=0;
	Button_Task_Condition_t * Condition =0;
	Button_Task_Condition_t * PrevCondition =0;
	Button = List_Find_Node( KeyCode);
	if(Button == 0) return;
	Condition = Button->condition;

	while(Condition !=0 && (Condition->Type != ctype)){
		PrevCondition = Condition;
		Condition = (Button_Task_Condition_t *) Condition->NextCondition;
	}
	if(Condition != 0){		
		if(Condition == Button->condition) {
			Button->condition = Condition->NextCondition;	
		} else {
			PrevCondition->NextCondition = Condition->NextCondition; 
		}
		free(Condition); 
	} 
}

void List_Delete_All_Coditions(Button_Task_Sheduler_t * Button){
	Button_Task_Condition_t * Condition =0;
	Button_Task_Condition_t * PrevCondition =0;

	Condition = Button->condition;
	while(Condition !=0){
		PrevCondition = Condition;
		Condition = (Button_Task_Condition_t *) Condition->NextCondition;
		free(PrevCondition);
	}
	Button->condition = 0;
}

uint8_t Check_All_Coditions(Button_Task_Sheduler_t * Button){
	Button_Task_Condition_t * Condition = Button->condition;
	uint8_t match=1;
	while(Condition !=0){
		if(Params[Condition->Type] > 100) match = 0;
		if((Condition->Min < 101) && (Condition->Min > Params[Condition->Type]))  match = 0;
		if((Condition->Max < 101) && (Condition->Max < Params[Condition->Type]))  match = 0;
		Condition = (Button_Task_Condition_t *) Condition->NextCondition;
	}
	return match;
}

void CreateKMReport(Button_Task_Sheduler_t * Button){
	Button_Task_Sheduler_t * PrevButton = 0;
	Button_Task_Sheduler_t * PressedButton = 0;
	Button_Task_Sheduler_t * PressedButtons = 0;

	USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer;
	uint8_t cursor = 0;

	while(Button != 0){
		switch(Button->state){
		case IDLE:
			if(Check_All_Coditions(Button) == 1){				
				if(Button->timer==0){
					Button->state = READY;
				}
			} else {
				Button->state = RELEASED;
				Button->timer=0;			
			}
			break;
		case READY:
		case RELEASED:
			if((Button->timer==0)&& (PauseTimer==0) && (deviceState != 0) && (Check_All_Coditions(Button) == 1)){
				if((Button->ConditionTime != 0) && (Button->state == RELEASED)){
					Button->state = IDLE;
					Button->timer = Button->ConditionTime*500; //500 ms grade	
					break;
				}
				if(Button->Code != 0xFF) {
					if(cursor < 6){ 
						newKeyboardReport->KeyCode[cursor] =	Button->Code;
						cursor++;
						Button->state = PRESSED;
						PauseTimer = Button->PauseTime*500; //500 ms grade
						expectKeyboardReport = 1;
						
						//Remove pressed button from main list
						if(PrevButton==0){
							Buttons = Button->NextTask;
						} else {
							PrevButton->NextTask = Button->NextTask;
						}
						
						// Add pressed button to temporary list
						if(PressedButtons == 0) {
							PressedButtons = Button;
							PressedButton = Button;
						} else {
							PressedButton->NextTask = (void*)Button;
							PressedButton = Button;
						}
						if(PrevButton==0){
							Button = Buttons;
						} else {
							Button = PrevButton;
						}
						PressedButton->NextTask = (void*)0;
					}
				} else {
					report_mouse_t* newMouseReport = (report_mouse_t*)newMouseHIDReportBuffer;
						// Create mouse report 
					newMouseReport->x = 0x00;
					newMouseReport->y = 0x00;
					newMouseReport->buttons |= (1 << 0);
					Button->state = PRESSED;
					PauseTimer = Button->PauseTime*500; //500 ms grade
					expectMouseReport = 1;
				}
			}
			break;
		case PRESSED:
			Button->state = RELEASED;
//			   	if(Button->ReleaseTime > 1500) LEDs_TurnOnLEDs(LEDS_y3);

			Button->timer = 100*((uint32_t)Button->ReleaseTime); //100 ms grade
//				if(Button->ReleaseTime > 2300) LEDs_TurnOnLEDs(LEDS_LED1);

			if(Button->Code == 0xFF) {
				report_mouse_t* newMouseReport = (report_mouse_t*)newMouseHIDReportBuffer;
				newMouseReport->buttons = 0x00;
				expectMouseReport = 1;
			} else {
				expectKeyboardReport = 1;
			}
			break;
		default:
			break;
		}
		PrevButton = Button;
		Button = (Button_Task_Sheduler_t *) Button->NextTask;
	}
			// fill end of report with 0x00
			//memcpy_P( PrevRawReportBuffer, RawReport, MIN(GENERIC_REPORT_SIZE, ReportSize) ); 
		if((cursor < sizeof(newKeyboardReport->KeyCode))) memset( newKeyboardReport->KeyCode+ cursor, 0x00, sizeof(newKeyboardReport->KeyCode)-cursor );

		// Join pressed buttons back to the end of the list
		if(Buttons != 0) {
			PrevButton->NextTask = (void *) PressedButtons;
		} else {
			Buttons = PressedButtons;
		}
}


void List_Delete_Node(uint8_t KeyCode){
	Button_Task_Sheduler_t * Button=0;
	Button_Task_Sheduler_t * PrevButton=0;
	
	Button = Buttons;
	while(Button !=0 && (Button->Code != KeyCode)){
		PrevButton = Button;
		Button = (Button_Task_Sheduler_t *) Button->NextTask;
	}
	if(Button != 0){		
		if(Button == Buttons) {
			Buttons = Button->NextTask;	
		} else {
			PrevButton->NextTask = Button->NextTask; 
		}
		List_Delete_All_Coditions(Button);
		free(Button); 
	} 
}


void List_Delete_All(void){
	Button_Task_Sheduler_t * Button;
	Button_Task_Sheduler_t * PrevButton;

	Button = Buttons;
	while(Button !=0){
		PrevButton = Button;
		Button = (Button_Task_Sheduler_t *) Button->NextTask;
		List_Delete_All_Coditions(PrevButton);
		free(PrevButton);
	}
	LEDs_TurnOffLEDs(LEDS_LED1);
	Buttons = 0;
}

void WriteConfig(void){
	Button_Task_Sheduler_t * Button;
	uint8_t counter=0;
	
	Button = Buttons;
	while(Button !=0){
		uint8_t data[CONFIG_DATA_BLOCK_SIZE];
		data[0] = Button->Code;
		data[1] = (uint8_t) (Button->PauseTime&0xff);
		data[2] = (uint8_t) (Button->ReleaseTime&0xff);
		data[3] = (uint8_t) ((Button->ReleaseTime >> 8)&0xff);
		data[4] = (uint8_t) (Button->ConditionTime&0xff);

		eeprom_write_block(data, (uint8_t *)(CONFIG_DATA_ADDRESS + counter*CONFIG_DATA_BLOCK_SIZE), CONFIG_DATA_BLOCK_SIZE);
		Button = (Button_Task_Sheduler_t *) Button->NextTask;
		counter++;
	}
	eeprom_write_byte((uint8_t*)CONFIG_HEADER_ADDRESS, counter);
	eeprom_write_byte((uint8_t*)CONFIG_MODIFIER_ADDRESS, ((USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer)->Modifier);
}


void ReadConfig(void){
	uint8_t counter = eeprom_read_byte((uint8_t*)CONFIG_HEADER_ADDRESS);
	((USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer)->Modifier = eeprom_read_byte((uint8_t*)CONFIG_MODIFIER_ADDRESS);

	List_Delete_All();
	while(counter >0){	
		uint8_t data[CONFIG_DATA_BLOCK_SIZE];
		eeprom_read_block(data, (uint8_t *)(CONFIG_DATA_ADDRESS + (counter-1)*CONFIG_DATA_BLOCK_SIZE), CONFIG_DATA_BLOCK_SIZE);
		List_Add_Node(
			data[0], 
			data[1], 
			//1000);
			((uint16_t)data[2])+
			((((uint16_t)data[3]) << 8) & 0x0000ff00),data[4]);
		counter--;
	}
}



/*
#define X_IS_NEG  (mstat & (1<<PS2_MOUSE_X_SIGN))
#define Y_IS_NEG  (mstat & (1<<PS2_MOUSE_Y_SIGN))
#define X_IS_OVF  (mstat & (1<<PS2_MOUSE_X_OVFLW))
#define Y_IS_OVF  (mstat & (1<<PS2_MOUSE_Y_OVFLW))
*/

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{

/*
	uint8_t mstat;
	int8_t mx;
	int8_t my;
	static uint8_t buttons_prev = 0;
*/
	/* receives packet from mouse */
//	uint8_t rcv;

	SetupHardware();
	//	LEDs_TurnOnLEDs(LEDS_ALL_LEDS);

	((USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer)->Modifier =	0;
	//ReadConfig();
	_delay_us(1000);
	prevButton1State =  Buttons_GetStatus()& BUTTONS_BUTTON1;

	LEDs_TurnOffLEDs(LEDS_ALL_LEDS);

	GlobalInterruptEnable();
		
	for (;;)
	{			
		uint8_t Button1State = Buttons_GetStatus();
		if(prevButton1State > Button1State){
			deviceState = 1-deviceState;
		}
		prevButton1State = Button1State;
/*
		if(expectMouseReport == 0 ){
			GlobalInterruptDisable();
			PS2_write(PS2_MOUSE_READ_DATA);
			rcv = PS2_read(); //Ack
			if (rcv == PS2_ACK) {
				mstat = PS2_read();
				mx = PS2_read();
				my = PS2_read();

				// if mouse moves or buttons state changes /
				if (mx || my || ((mstat ^ buttons_prev) & PS2_MOUSE_BTN_MASK)) {
					buttons_prev = mstat;

					// PS/2 mouse data is '9-bit integer'(-256 to 255) which is comprised of sign-bit and 8-bit value.
					// bit: 8    7 ... 0
					//      sign \8-bit/
					//
					// Meanwhile USB HID mouse indicates 8bit data(-127 to 127), note that -128 is not used.
					//
					// This converts PS/2 data into HID value. Use only -127-127 out of PS/2 9-bit.
					mx = X_IS_NEG ?
							  ((!X_IS_OVF && -127 <= mx && mx <= -1) ?  mx : -127) :
							  ((!X_IS_OVF && 0 <= mx && mx <= 127) ? mx : 127);
					my = Y_IS_NEG ?
							  ((!Y_IS_OVF && -127 <= my && my <= -1) ?  my : -127) :
							  ((!Y_IS_OVF && 0 <= my && my <= 127) ? my : 127);

					// remove sign and overflow flags
					mstat &= PS2_MOUSE_BTN_MASK;

					// invert coordinate of y to conform to USB HID mouse
					my = -my;
					report_mouse_t* newMouseReport = 			  (report_mouse_t*)newMouseHIDReportBuffer;
					
					newMouseReport->x = mx;
					newMouseReport->y = my;
					newMouseReport->buttons =mstat; //|= (1 << 0);
					expectMouseReport = 1;
				}
			}
			GlobalInterruptEnable();
		}
*/
		ps2_mouse_task();
		HID_Device_USBTask(&Keyboard_HID_Interface);
		HID_Device_USBTask(&Mouse_HID_Interface);
		HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();
	}
}

PS2_to_USB_mouse_send(report_mouse_t *mouse_report){
    report_mouse_t* newMouseReport =  (report_mouse_t*)newMouseHIDReportBuffer;
    if(expectMouseReport == 0 ){
	newMouseReport->x = mouse_report->x;
	newMouseReport->y = mouse_report->y;
	newMouseReport->v = mouse_report->v;
	newMouseReport->h = mouse_report->h;
	newMouseReport->buttons = mouse_report->buttons; //|= (1 << 0);
	expectMouseReport = 1;
    }
}

//ISR(TIMER0_COMPA_vect, ISR_BLOCK)
void led_indicator_task(void)
{
	//Button_Task_Sheduler_t * PrevButton;

	// Control device state LED
	if(deviceState == 0){
		toggleCounter+=5;
		if(toggleCounter >= TOGGLEIDLE){
			LEDs_ToggleLEDs(LEDS_LED2);
			toggleCounter = 0;
		}
	} else {
		LEDs_TurnOnLEDs(LEDS_LED2);
	}
}

void spam_buttons_task(void)
{
	Button_Task_Sheduler_t * Button;
	//Check Host communication timeout
	if(UpdateCounter > UPDATETIMEOUT){
		Params[mobHP] = 0xFF;		
		Params[playerHP] = 0xFF;	
		Params[playerCP] = 0xFF;	
		Params[playerMP] = 0xFF;
		UpdateCounter = UPDATETIMEOUT;
		LEDs_TurnOffLEDs(LEDS_LED2);
	} else {
		UpdateCounter+=5;
	}

	//Decrease button timers
	Button = Buttons;
	while(Button != 0){
 
		Button->timer= (Button->timer <5) ? 0: Button->timer-5;
		Button = (Button_Task_Sheduler_t *) Button->NextTask;
	}

	//Decrease absolute timeout timer
	PauseTimer = (PauseTimer <5) ? 0: PauseTimer-5;
	
	//Prepare Keyboard and mouse reports
	if((expectMouseReport == 0)&& (expectKeyboardReport == 0) ){
		switch(Mode){
		case SPAMBUTTONS:
			CreateKMReport(Buttons);
			break;
		case HOSTCOMMAND:
			CreateKMReport(HostButton);
			break;
		default:
			break;
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

	mouse_driver.send_mouse = &PS2_to_USB_mouse_send;
	host_set_driver(&mouse_driver);
	LEDs_TurnOffLEDs(LEDS_ALL_LEDS);

    uint8_t rcv = ps2_mouse_init();
	if(rcv == PS2_MOUSE_NONE ) {
            LEDs_TurnOnLEDs(LEDS_LED1);
        } else if(rcv == PS2_MOUSE_GENIUS5 ) {
            LEDs_TurnOnLEDs(LEDS_LED2);
        } else if(rcv == PS2_MOUSE_STANDARD ) {
            LEDs_TurnOnLEDs(LEDS_LED3);
        } else if(rcv == PS2_MOUSE_MIE ) {
            LEDs_TurnOnLEDs(LEDS_LED3);
            LEDs_TurnOnLEDs(LEDS_LED2);
        } 
	_delay_ms(5000);

}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{

/*
  //  LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
  	// Start Millisecond Timer Interrupt 
	TIMSK0  = (1 << OCIE0A);
	TCCR0A = (1 << WGM01);  // Clear Timer on Compare (CTC)
	
#if (F_CPU == 8000000)
//	TCCR0B = ((1 << CS01) | (1 << CS00));  // Clock/64
	TCCR0B = (1 << CS02);  // Clock/256
	OCR0A  = (F_CPU / 256 / 200); //200Hz
#elif (F_CPU == 16000000)			
	//TCCR0B = ((1 << CS01) | (1 << CS00));  // Clock/64
	//TCCR0B = (1 << CS02);  // Clock/256

	TCCR0B = ((1 << CS02) | (1 << CS00));  // Clock/1024
	OCR0A  = (F_CPU / 1024 / 200); //200Hz	
#else
	#error Unsupported F_CPU	
#endif
*/
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
 //   LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
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


//	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
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
	/* Determine which interface must have its report generated */
	if (HIDInterfaceInfo == &Keyboard_HID_Interface)
	{
		USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;
		USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer;

		KeyboardReport->Modifier = 0x00;
		memset( KeyboardReport->KeyCode, 0x00, sizeof(KeyboardReport->KeyCode) );

		// Create keyboard report 
		if (expectKeyboardReport != 0) {	
			if (deviceState==1) {
				KeyboardReport->Modifier = newKeyboardReport->Modifier;
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
		
		/* Return status of device */
		RawReport[0] = deviceState;
		*ReportSize = 1;
		return false;
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
		
		if(Service == INTERFACE_ID_Keyboard){
			uint8_t Command = RawReport[1];	
			uint16_t tmp  = 256*RawReport[5]+RawReport[4];
//			if(tmp > 1500) LEDs_TurnOnLEDs(LEDS_LED1);

			switch(Command){
				case CMD_WRITE_CONFIG:
					WriteConfig();
					break;
				case CMD_READ_CONFIG:
					ReadConfig();
					break;
				case CMD_ADD_NODE:
					List_Add_Node(
						RawReport[2], 
						RawReport[3], 
						tmp,
						RawReport[6]);
					break;
				case CMD_DELETE_NODE:
					List_Delete_Node(RawReport[2]);
					break;
				case CMD_DELETE_ALL:
					List_Delete_All();
					break;
				case CMD_SET_MODIFIER:
					((USB_KeyboardReport_Data_t*)newKeyboardHIDReportBuffer)->Modifier = RawReport[2];
					break;
				case CMD_SET_MODE:
					Mode = RawReport[2];
					break;
				case CMD_SET_HPCPMP:
					Params[mobHP] = RawReport[2];		
					Params[playerHP] = RawReport[3];	
					Params[playerCP] = RawReport[4];	
					Params[playerMP] = RawReport[5];	
					UpdateCounter=0;
					break;
				case CMD_ADD_NODE_CONDITION:
					if( ((RawReport[4] < 101) && (RawReport[5] < 101) && (RawReport[4] >= RawReport[5])) ||
						((RawReport[4] > 100) && (RawReport[5] > 100))){
						List_Delete_Condition(
							RawReport[2],    //Key Code
							RawReport[3]);   // Condition Type
					} else {
						List_Update_Condition(
							RawReport[2],   //Key Code
							RawReport[3],   // Condition Type
							RawReport[4],   // Condition Min
							RawReport[5]);  // Condition Max							
					}
					break;
				default:
					break;
			}
		} else if(Service == INTERFACE_ID_Mouse){
			report_mouse_t* newMouseReport = (report_mouse_t*)newMouseHIDReportBuffer;
			newMouseReport->x =	RawReport[1]-128;
			newMouseReport->y =	RawReport[2]-128;
			newMouseReport->buttons =	RawReport[3];
			expectMouseReport = 1;		
		}else if(Service == INTERFACE_ID_GenericHID){
			deviceState =	RawReport[1];
			expectMouseReport = 0;		
			expectKeyboardReport = 0;
		}
		
	}
}

