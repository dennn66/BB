
	/* Includes: */

#include "KeyboardMouseRaw.h"

static Button_Task_Scheduler_t* Buttons = 0;
static uint32_t PauseTimer = 0; //ms;    	/* Spam buttons timeout, 2s*/

#define UPDATETIMEOUT 2000   /* DE period for push pull key ms */
static uint16_t UpdateCounter = 0;    		/* HPMPCP update counter, ms*/

/* DE 0-100 - Level in %, 0xFF - unknown */


static uint8_t Params[] = {
							0x00, //CheckSkillType
							0x00, //CheckSkillType2
							0x00, //CheckSkillType3
							0x00, //CheckSkillType4
							0x00, //CheckSkillType5
							0x00  //CheckSkillType6
							};




void host_timeout_task(uint16_t time_delta)
{
	//Check Host communication timeout
	if(UpdateCounter > UPDATETIMEOUT){
		for(int i = 0; i < sizeof(Params); i++) Params[i]  = 0;
		UpdateCounter = UPDATETIMEOUT;
		LEDs_TurnOffLEDs(LEDS_LED2);
	} else {
		UpdateCounter+=time_delta;
	}
}


void set_SkillState(uint8_t *skillstate){
	for(int i = 0; i < sizeof(Params); i++) Params[i]  = skillstate[i];
	UpdateCounter=0;
}


Button_Task_Scheduler_t * List_Find_Node(uint8_t index){
	Button_Task_Scheduler_t * Button=0;
	
	Button = Buttons;
	while(Button !=0) {
		if(Button->index == index) return Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
	}
	return (Button_Task_Scheduler_t *) 0;
}


void List_Add_Node(uint8_t * node){
	

	Button_Task_Scheduler_t * Button=Buttons;
	Button_Task_Scheduler_t * PrevButton=0;
		
	while(Button != 0 && (Button->index != node[0])){
		PrevButton = Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
	}

	if(Button == 0) {	
		Button = (Button_Task_Scheduler_t *)malloc(sizeof(Button_Task_Scheduler_t));
		if(Button == 0) {
			LEDs_TurnOnLEDs(LEDS_LED1);
			return;
		} 
		if(Buttons == 0) Buttons = Button;
		else PrevButton->NextTask=(void *)Button;
		Button->NextTask = (void *)0;

	}	


	/*
    hpbuf[0] = index;                                                           //0
    hpbuf[1] = key_code;                                                        //1
    *((uint32_t*)(hpbuf+2)) = (uint32_t)(PauseTime*1000);  //; 1 ms grade           //2, 3, 4, 5
    *((uint32_t*)(hpbuf+6)) = (uint32_t)(ReleaseTime*1000);  //; 1 ms grade         //6, 7, 8, 9
    *((uint32_t*)(hpbuf+10)) = (uint32_t)(ConditionTime*1000);  //; 1 ms grade      //10, 11, 12, 13
    hpbuf[14] = groups;                                                         //14
    hpbuf[15] = 0;                                                              //15

    if(Ctrl)      hpbuf[15] |= 0b00001000; // Ctrl
    if(Shift)     hpbuf[15] |= 0b00000100; // Shift
	*/

	Button->index 			 = node[0];
	Button->Code 			 = node[1];
	Button->PauseTime 		 = *((uint32_t*)(node+2)); //1 ms grade  // 2-5
	Button->ReleaseTime 	 = *((uint32_t*)(node+6)); //1 ms grade  // 6-9
	Button->ConditionTime 	 = *((uint32_t*)(node+10)); //1 ms grade // 10-13			
	Button->groups 			 = node[14];			
	Button->flag = node[15];
	Button->timer = 0;
}



uint8_t Check_All_Coditions(Button_Task_Scheduler_t * Button){	 
	if((Button->groups &	get_groupsStatus()) == 0)   return 0; 	

	if(Button->index < MAXCONDITIONAMOUNT){
		uint8_t byte = Button->index >> 3;
		uint8_t bit = Button->index - (byte << 3);				
		if(((Params[byte]) & (1<<bit)) == 0) return 0;
	}	
	return 1;
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
	CreateKMReport2();
}

void CreateKMReport2(void){
	Button_Task_Scheduler_t * Button = Buttons;
	Button_Task_Scheduler_t * PrevButton = 0;
	Button_Task_Scheduler_t * PressedButton = 0;
	uint8_t KeyboardReportExpected = 0;
	uint8_t MouseReportExpected = 0;

	USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)get_newKeyboardHIDReportBuffer();
	if(get_expectKeyboardReport() == 0){
		memset( newKeyboardReport->KeyCode, 0x00, sizeof(newKeyboardReport->KeyCode));
		newKeyboardReport->Modifier = 0;
	}
	
	report_mouse_t* newMouseReport = (report_mouse_t*)get_newMouseHIDReportBuffer();
		// Create mouse report 
	if(get_expectMouseReport() == 0 ){
		newMouseReport->x = 0x00;
		newMouseReport->y = 0x00;
		newMouseReport->buttons = 0x00;
	}



	while(Button != 0){
		switch(Button->flag&BTN_FLAG_STATE_MASK){
			case IDLE:
				if(Check_All_Coditions(Button) == 1){				
					if(Button->timer==0){
						Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | READY;
					}
				} else {
					Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | RELEASED;
					Button->timer=0;			
				}
				break;
			case READY:
			case RELEASED:
				if(
						(Button->timer==0)&& 
						(PauseTimer==0) && 
						(get_deviceState() != 0) && 
						(Check_All_Coditions(Button) == 1)
					){
					if((Button->ConditionTime != 0) && ((Button->flag&BTN_FLAG_STATE_MASK) == RELEASED)){
						Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | IDLE;
						Button->timer = Button->ConditionTime; //1 ms grade	
						break;
					}
					if((get_expectMouseReport() == 0)&& (get_expectKeyboardReport() == 0) ){
						if(Button->Code < 0xE8) {					
							newKeyboardReport->KeyCode[0] =	Button->Code;						
							if((Button->flag & (1 << BTN_FLAG_CTRL))>0) newKeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTCTRL;
							if((Button->flag & (1 << BTN_FLAG_SHIFT))>0) newKeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
							set_expectKeyboardReport(1);
							
						} else if((Button->Code > 0xEF) && (Button->Code < 0xF5)){

							//0xF0 USB_MOUSE_BTN_LEFT      
							//0xF1 USB_MOUSE_BTN_RIGHT     
							//0xF2 USB_MOUSE_BTN_MIDDLE    
							//0xF3 USB_MOUSE_BTN_4th       
							//0xF4 USB_MOUSE_BTN_5th       
							
							newMouseReport->buttons = (1 << (Button->Code - 0xF0));		
							set_expectMouseReport(1);
						}
						Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | PRESSED;							
						PauseTimer = Button->PauseTime; //1 ms grade
						// Save PressedButton
						PressedButton = Button;
						
						//Remove pressed button from main list
						if(PrevButton==0){
							Buttons = Button->NextTask;
						} else {
							PrevButton->NextTask = PressedButton->NextTask;
						}
						PressedButton->NextTask = (void*)0;
						Button = PrevButton;
					}
				}
				break;
			case PRESSED:
				if(Button->Code < 0xE8) {
					if(newKeyboardReport->KeyCode[0] !=	Button->Code){
						Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | RELEASED;
						Button->timer = Button->ReleaseTime; //1 ms grade
						KeyboardReportExpected = 1;										
					}
				} else {
					if(newMouseReport->buttons != (1 << (Button->Code - 0xF0))){
						Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | RELEASED;
						Button->timer = Button->ReleaseTime; //1 ms grade
						MouseReportExpected = 1;
					}
				}			

				break;
			default:
				break;
		}
		PrevButton = Button;
		if(Button == 0){
			Button = Buttons;
		} else {
			Button = Button->NextTask;
		}
	}
	// Join pressed buttons back to the end of the list
	if(Buttons == 0) {
		Buttons = PressedButton;		
	} else {
		PrevButton->NextTask = PressedButton;
	}	
	if(get_expectKeyboardReport() == 0 && KeyboardReportExpected == 1)	set_expectKeyboardReport(1);
	if(get_expectMouseReport()    == 0 && MouseReportExpected    == 1)	set_expectMouseReport(1);
}






void CreateKMReport(Button_Task_Scheduler_t * Button){
	Button_Task_Scheduler_t * PrevButton = 0;
	Button_Task_Scheduler_t * PressedButton = 0;
	Button_Task_Scheduler_t * PressedButtons = 0;

	USB_KeyboardReport_Data_t* newKeyboardReport = (USB_KeyboardReport_Data_t*)get_newKeyboardHIDReportBuffer();
	uint8_t cursor = 0;

	while(Button != 0){
		switch(Button->flag&BTN_FLAG_STATE_MASK){
		case IDLE:
			if(Check_All_Coditions(Button) == 1){				
				if(Button->timer==0){
				    Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | READY;
				}
			} else {
				Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | RELEASED;
				Button->timer=0;			
			}
			break;
		case READY:
		case RELEASED:
			if(
					(Button->timer==0)&& 
					(PauseTimer==0) && 
					(get_deviceState() != 0) && 
					(Check_All_Coditions(Button) == 1)
				){
				if((Button->ConditionTime != 0) && ((Button->flag&BTN_FLAG_STATE_MASK) == RELEASED)){
					Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | IDLE;
					Button->timer = Button->ConditionTime; //1 ms grade	
					break;
				}
				if(Button->Code < 0xE8) {
					if(cursor < 1){ 
						newKeyboardReport->KeyCode[cursor] =	Button->Code;
						newKeyboardReport->Modifier = 0;
						if((Button->flag & (1 << BTN_FLAG_CTRL))>0) newKeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTCTRL;
						if((Button->flag & (1 << BTN_FLAG_SHIFT))>0) newKeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
						cursor++;
						Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | PRESSED;
						
						PauseTimer = Button->PauseTime; //1 ms grade
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
				} else if((Button->Code > 0xEF) && (Button->Code < 0xF5)){
					report_mouse_t* newMouseReport = (report_mouse_t*)get_newMouseHIDReportBuffer();
						// Create mouse report 
					newMouseReport->x = 0x00;
					newMouseReport->y = 0x00;

					//0xF0 USB_MOUSE_BTN_LEFT      
					//0xF1 USB_MOUSE_BTN_RIGHT     
					//0xF2 USB_MOUSE_BTN_MIDDLE    
					//0xF3 USB_MOUSE_BTN_4th       
					//0xF4 USB_MOUSE_BTN_5th       
					
					newMouseReport->buttons |= (1 << (Button->Code - 0xF0));
					newMouseReport->buttons &= USB_MOUSE_BTN_MASK;		
			
					Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | PRESSED;
					PauseTimer = Button->PauseTime; //1 ms grade
					set_expectMouseReport(1);
				}
			}
			break;
		case PRESSED:
			Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | RELEASED;
//			   	if(Button->ReleaseTime > 1500) LEDs_TurnOnLEDs(LEDS_y3);

			Button->timer = Button->ReleaseTime; //1 ms grade

			if((Button->Code > 0xEF) && (Button->Code < 0xF5)) {
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






void List_Delete_Node(uint8_t index){
	Button_Task_Scheduler_t * Button=0;
	Button_Task_Scheduler_t * PrevButton=0;
	
	Button = Buttons;
	while(Button !=0 && (Button->index != index)){
		PrevButton = Button;
		Button = (Button_Task_Scheduler_t *) Button->NextTask;
	}
	if(Button != 0){		
		if(Button == Buttons) {
			Buttons = Button->NextTask;	
		} else {
			PrevButton->NextTask = Button->NextTask; 
		}
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
		free(PrevButton);
	}
	LEDs_TurnOffLEDs(LEDS_LED1);
	Buttons = 0;
}



