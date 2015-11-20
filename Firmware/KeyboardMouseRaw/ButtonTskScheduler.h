

#ifndef _BUTTON_TSK_SCHEDULER_H_
#define _KEYBOARD_MOUSE_H_


#define CONFIG_HEADER_ADDRESS 0
#define CONFIG_MODIFIER_ADDRESS 1
#define CONFIG_DATA_ADDRESS 2
#define CONFIG_DATA_BLOCK_SIZE 5

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


		enum Params_t
		{
				mobHP = 0, /** Mob HP */
				playerHP = 1, /** Player HP */
				playerCP = 2, /** Player CP */
				playerMP = 3, /** Player MP */
		};

	/* Type Defines: */
	
	/** Type define for Button extended conditions */
	typedef struct
	{
		// Conditions description
		uint8_t Type;
		uint8_t Min;
		uint8_t Max;
		void*    NextCondition;
	} Button_Task_Condition_t;

	/** Type define for Button task sheduler */
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
	} Button_Task_Scheduler_t;


	Button_Task_Scheduler_t * List_Find_Node(uint8_t KeyCode);
	void List_Add_Node(uint8_t KeyCode, uint8_t PauseTime, uint16_t ReleaseTime, uint8_t ConditionTime);
	Button_Task_Condition_t * List_Add_Condition(uint8_t KeyCode, uint8_t ctype);
	void List_Update_Condition(uint8_t KeyCode, uint8_t ctype, uint8_t cmin, uint8_t cmax);
	void List_Delete_Condition(uint8_t KeyCode, uint8_t ctype);
	void List_Delete_All_Coditions(Button_Task_Scheduler_t * Button);
	uint8_t Check_All_Coditions(Button_Task_Scheduler_t * Button);
	void CreateKMReport(Button_Task_Scheduler_t * Button);
	void List_Delete_Node(uint8_t KeyCode);
	void List_Delete_All(void);
	void WriteConfig(void);
	void ReadConfig(void);
	void spam_buttons_task(uint16_t time_delta);
	void set_spam_buttons_mode(uint8_t mode); 
	void host_timeout_task(uint16_t time_delta);
	void set_HPCPMP(uint8_t mobhp, uint8_t hp, uint8_t cp, uint8_t mp);


#endif