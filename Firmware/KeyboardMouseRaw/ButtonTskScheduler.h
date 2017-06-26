

#ifndef _BUTTON_TSK_SCHEDULER_H_
#define _KEYBOARD_MOUSE_H_


#define BTN_FLAG_STATE_MASK 0b00000011
#define BTN_FLAG_CTRL 3
#define BTN_FLAG_SHIFT 2

#define TARGETMEORPET   0
#define TARGETCHAR      1
#define TARGETMOB       2
#define NOTARGET        3
#define MAXCONDITIONAMOUNT 48

												  
	enum ButtonState_t
	{
		RELEASED = 0, /** Button is released */
		PRESSED = 1, /** Button is pressed  */
		IDLE = 2, /** Button is waiting condition timer  */
		READY = 3, /** Button ready to push after condition timer timeout */
	};



	/* Type Defines: */
	
  	/** Type define for Button task sheduler */
	
	
	typedef struct
	{
		// Key description	
		uint8_t index;
		uint8_t Code;
		uint32_t PauseTime; // Siliance time when button is pressed (don't push other buttons). grade = 0.1s
		uint32_t ReleaseTime; //Cooldown time between buttons presses. grade = 0.1s
		uint32_t ConditionTime; // How many time condition must be active before press button. grade = 0.1s
		uint8_t groups;
		uint8_t flag;
		uint32_t timer;
		void*    NextTask;
	} Button_Task_Scheduler_t;


	Button_Task_Scheduler_t * List_Find_Node(uint8_t KeyCode);
	void List_Add_Node(uint8_t * node);
	
	uint8_t Check_All_Coditions(Button_Task_Scheduler_t * Button);
	void CreateKMReport(Button_Task_Scheduler_t * Button);
	void CreateKMReport2(void);
	void List_Delete_Node(uint8_t KeyCode);
	void List_Delete_All(void);
	void spam_buttons_task(uint16_t time_delta);
	//void set_spam_buttons_mode(uint8_t mode); 
	void host_timeout_task(uint16_t time_delta);
	void set_SkillState(uint8_t *skillstate);


#endif