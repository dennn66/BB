
	/* Includes: */

#include "KeyboardMouseRaw.h"

static Button_Task_Scheduler_t* Buttons = 0;
//static Button_Task_Scheduler_t* HostButton = 0;
//static uint8_t Mode = 0;		/* Spam buttons mode */
static uint32_t PauseTimer = 0; //ms;    	/* Spam buttons timeout, 2s*/

#define UPDATETIMEOUT 2000   /* DE period for push pull key ms */
static uint16_t UpdateCounter = 0;    		/* HPMPCP update counter, ms*/

/* DE 0-100 - Level in %, 0xFF - unknown */


static uint8_t Params[] = {0xFF, //mobHP
							0xFF, //playerHP
							0xFF, //playerCP
							0xFF, //playerMP
							0xFF, //playerVP
							0xFF, //mobMP
							0xFF, //targetType
							0x00, //starState
							0x00, //CheckSkillType
							0x00, //CheckSkillType2
							0x00, //CheckSkillType3
							0x00, //CheckSkillType4
							0x00, //CheckSkillType5
							0x00};//CheckSkillType6


//void set_spam_buttons_mode(uint8_t mode) {
//	Mode=;
//}

void host_timeout_task(uint16_t time_delta)
{
	//Check Host communication timeout
	if(UpdateCounter > UPDATETIMEOUT){
		Params[mobHP]      = 0xFF;		
		Params[playerHP]   = 0xFF;	
		Params[playerCP]   = 0xFF;	
		Params[playerMP]   = 0xFF;
		Params[playerVP]   = 0xFF;	
		Params[mobMP]      = 0xFF;		
		Params[targetType] = NOTARGET;	
		Params[starState]  = 0;
		Params[CheckSkillType]  = 0;
		Params[CheckSkillType2] = 0;
		Params[CheckSkillType3] = 0;
		Params[CheckSkillType4] = 0;
		Params[CheckSkillType5] = 0;
		Params[CheckSkillType6] = 0;	

		UpdateCounter = UPDATETIMEOUT;
		LEDs_TurnOffLEDs(LEDS_LED2);
	} else {
		UpdateCounter+=time_delta;
	}
}

void set_HPCPMP(uint8_t mobhp, uint8_t hp, uint8_t cp, uint8_t mp, uint8_t vp, uint8_t mobmp){
	Params[mobHP] = mobhp;		
	Params[playerHP] = hp;	
	Params[playerCP] = cp;	
	Params[playerMP] = mp;	
	Params[playerVP] = vp;	
	Params[mobMP] = mobmp;	
	UpdateCounter=0;
}

void set_TargetState(uint8_t targettype, uint8_t starstate, uint8_t skillstate, uint8_t skillstate2, uint8_t skillstate3, uint8_t skillstate4){
	Params[targetType] = targettype;	
	Params[starState] = starstate;	
	Params[CheckSkillType] = skillstate;
	Params[CheckSkillType2] = skillstate2;
	Params[CheckSkillType3] = skillstate3;
	Params[CheckSkillType4] = skillstate4;
	UpdateCounter=0;
}

void set_SkillState(uint8_t skillstate, uint8_t skillstate2, uint8_t skillstate3, uint8_t skillstate4, uint8_t skillstate5, uint8_t skillstate6){
	Params[CheckSkillType] = skillstate;
	Params[CheckSkillType2] = skillstate2;
	Params[CheckSkillType3] = skillstate3;
	Params[CheckSkillType4] = skillstate4;
	Params[CheckSkillType5] = skillstate5;
	Params[CheckSkillType6] = skillstate6;	
	
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
		CreateKMReport(Buttons);
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


void List_Add_Node(uint8_t KeyCode, uint8_t PauseTime, uint16_t ReleaseTime, uint8_t ConditionTime, uint8_t flag){
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
			Button->flag = (flag <<2) | RELEASED;
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
			Button->flag = (flag <<2) | RELEASED;
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
	//if(ctype == CheckSkillType) setDebugFlag(1 );
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
	if((((Button->flag&0b11110000) >> 4) &	((get_activeState()&0b00111100) >> 2)) == 0)   return 0; 	
		
	while(Condition !=0){
		if(Condition->Type == targetType){
			//  check target conditions			
			uint8_t TargetCondition = Condition->Min;
			uint8_t StarStateCondition = Condition->Max;

//				setDebugFlag(0); 			
			if(((1 << Params[targetType]) & TargetCondition) == 0 ) return 0;
			
			if((StarStateCondition & 0b00000010) > 0) {
				if(	Params[starState] != (StarStateCondition & 0b00000001)) return 0;
			}			
		} else if(Condition->Type == CheckSkillType){
			//  check if skill is ready conditions
				//setDebugFlag(1 );
				uint8_t skillbit = Condition->Min;
				if(skillbit < MAXCONDITIONAMOUNT){
					uint8_t byte = skillbit >> 3;
					uint8_t bit = skillbit - (byte << 3);	
					//if(CheckSkillType+byte == CheckSkillType6) setDebugFlag(1 );				
					if(((Params[CheckSkillType+byte]) & (1<<bit)) == 0) return 0;
				}
		} else{
		
		    if(!((Condition->Type == mobHP) && (Params[targetType] != TARGETMOB))){
				if(!((Condition->Type == mobMP) && (Params[targetType] != TARGETMEORPET))){
					if(Params[Condition->Type] > 100) return 0;
					if((Condition->Min < 101) && (Condition->Min > Params[Condition->Type]))  return 0;
					if((Condition->Max < 101) && (Condition->Max < Params[Condition->Type]))  return 0;
				}
			}
		}
		Condition = (Button_Task_Condition_t *) Condition->NextCondition;
	}
	return 1;
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
					Button->timer = Button->ConditionTime*500; //500 ms grade	
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
					PauseTimer = Button->PauseTime*500; //500 ms grade
					set_expectMouseReport(1);
				}
			}
			break;
		case PRESSED:
			Button->flag = (Button->flag & (~BTN_FLAG_STATE_MASK)) | RELEASED;
//			   	if(Button->ReleaseTime > 1500) LEDs_TurnOnLEDs(LEDS_y3);

			Button->timer = 100*((uint32_t)Button->ReleaseTime); //100 ms grade
//				if(Button->ReleaseTime > 2300) LEDs_TurnOnLEDs(LEDS_LED1);

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



