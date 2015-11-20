
	/* Includes: */

#include "KeyboardMouseRaw.h"

static Button_Task_Scheduler_t* Buttons = 0;
static Button_Task_Scheduler_t* HostButton = 0;
static uint8_t Mode = 0;		/* Spam buttons mode */
static uint32_t PauseTimer = 0; //ms;    	/* Spam buttons timeout, 2s*/

#define UPDATETIMEOUT 2000   /* DE period for push pull key ms */
static uint16_t UpdateCounter = 0;    		/* HPMPCP update counter, ms*/

/* DE 0-100 - Level in %, 0xFF - unknown */
static uint8_t Params[] = {0xFF, 0xFF, 0xFF, 0xFF};


void set_spam_buttons_mode(uint8_t mode) {
	Mode=mode;
}

void host_timeout_task(uint16_t time_delta)
{
	//Check Host communication timeout
	if(UpdateCounter > UPDATETIMEOUT){
		Params[mobHP] = 0xFF;		
		Params[playerHP] = 0xFF;	
		Params[playerCP] = 0xFF;	
		Params[playerMP] = 0xFF;
		UpdateCounter = UPDATETIMEOUT;
		LEDs_TurnOffLEDs(LEDS_LED2);
	} else {
		UpdateCounter+=time_delta;
	}
}

void set_HPCPMP(uint8_t mobhp, uint8_t hp, uint8_t cp, uint8_t mp){
	Params[mobHP] = mobhp;		
	Params[playerHP] = hp;	
	Params[playerCP] = cp;	
	Params[playerMP] = mp;	
	UpdateCounter=0;
}

void spam_buttons_task(uint16_t time_delta)
{
	Button_Task_Scheduler_t * Button;

	//Decrease button timers
	Button = Buttons;
	while(Button != 0){
 
		Button->timer= (Button->timer <time_delta) ? 0: Button->timer-time_delta;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
	}

	//Decrease absolute timeout timer
	PauseTimer = (PauseTimer <time_delta) ? 0: PauseTimer-time_delta;
	
	//Prepare Keyboard and mouse reports
	if((get_expectMouseReport() == 0)&& (get_expectKeyboardReport() == 0) ){
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

Button_Task_Scheduler_t * List_Find_Node(uint8_t KeyCode){
	Button_Task_Scheduler_t * Button=0;
	
	Button = Buttons;
	while(Button !=0) {
		if(Button->Code == KeyCode) return Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
	}
	return (Button_Task_Scheduler_t *) 0;
}


void List_Add_Node(uint8_t KeyCode, uint8_t PauseTime, uint16_t ReleaseTime, uint8_t ConditionTime){
	Button_Task_Scheduler_t * Button=0;
	Button_Task_Scheduler_t * PrevButton=0;
	
	//if(ReleaseTime > 1500) LEDs_TurnOnLEDs(LEDS_LED1);


	
	Button = Buttons;
	while(Button !=0 && (Button->Code != KeyCode)){
		PrevButton = Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
	}
	if(Button == 0){
		Button = (Button_Task_Scheduler_t *)malloc(sizeof(Button_Task_Scheduler_t));
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
	Button_Task_Scheduler_t * Button=0;
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
	Button_Task_Scheduler_t * Button=0;
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

void List_Delete_All_Coditions(Button_Task_Scheduler_t * Button){
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

uint8_t Check_All_Coditions(Button_Task_Scheduler_t * Button){
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

void CreateKMReport(Button_Task_Scheduler_t * Button){
	Button_Task_Scheduler_t * PrevButton = 0;
	Button_Task_Scheduler_t * PressedButton = 0;
	Button_Task_Scheduler_t * PressedButtons = 0;

	USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)get_newKeyboardHIDReportBuffer();
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
			if((Button->timer==0)&& (PauseTimer==0) && (get_deviceState() != 0) && (Check_All_Coditions(Button) == 1)){
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
						set_expectKeyboardReport(1);
						
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
					report_mouse_t* newMouseReport = (report_mouse_t*)get_newMouseHIDReportBuffer();
						// Create mouse report 
					newMouseReport->x = 0x00;
					newMouseReport->y = 0x00;
					newMouseReport->buttons |= (1 << 0);
					Button->state = PRESSED;
					PauseTimer = Button->PauseTime*500; //500 ms grade
					set_expectMouseReport(1);
				}
			}
			break;
		case PRESSED:
			Button->state = RELEASED;
//			   	if(Button->ReleaseTime > 1500) LEDs_TurnOnLEDs(LEDS_y3);

			Button->timer = 100*((uint32_t)Button->ReleaseTime); //100 ms grade
//				if(Button->ReleaseTime > 2300) LEDs_TurnOnLEDs(LEDS_LED1);

			if(Button->Code == 0xFF) {
				report_mouse_t* newMouseReport = (report_mouse_t*)get_newMouseHIDReportBuffer();
				newMouseReport->buttons = 0x00;
				set_expectMouseReport(1);
			} else {
				set_expectKeyboardReport(1);
			}
			break;
		default:
			break;
		}
		PrevButton = Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
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
	Button_Task_Scheduler_t * Button=0;
	Button_Task_Scheduler_t * PrevButton=0;
	
	Button = Buttons;
	while(Button !=0 && (Button->Code != KeyCode)){
		PrevButton = Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
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
	Button_Task_Scheduler_t * Button;
	Button_Task_Scheduler_t * PrevButton;

	Button = Buttons;
	while(Button !=0){
		PrevButton = Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
		List_Delete_All_Coditions(PrevButton);
		free(PrevButton);
	}
	LEDs_TurnOffLEDs(LEDS_LED1);
	Buttons = 0;
}

void WriteConfig(void){
	Button_Task_Scheduler_t * Button;
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
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
		counter++;
	}
	eeprom_write_byte((uint8_t*)CONFIG_HEADER_ADDRESS, counter);
	eeprom_write_byte((uint8_t*)CONFIG_MODIFIER_ADDRESS, ((USB_KeyboardReport_Data_t*)get_newKeyboardHIDReportBuffer())->Modifier);
}


void ReadConfig(void){
	uint8_t counter = eeprom_read_byte((uint8_t*)CONFIG_HEADER_ADDRESS);
	((USB_KeyboardReport_Data_t*)get_newKeyboardHIDReportBuffer())->Modifier = eeprom_read_byte((uint8_t*)CONFIG_MODIFIER_ADDRESS);

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

