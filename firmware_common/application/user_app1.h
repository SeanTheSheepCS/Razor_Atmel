/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define COUNTER_LIMIT_MS (u32)2500
#define ALTERNATING_COLOUR_LIMIT_MS (u32)500
#define CYCLE_COLOURS_LIMIT_MS (u32)1000

#define E4FREQ_HZ (u32)330
#define G4FREQ_HZ (u32)392
#define C4FREQ_HZ (u32)262
#define D4FREQ_HZ (u32)294
#define F4FREQ_HZ (u32)349

#define QUARTERNOTETIME_MS (u32)500

/* THE FOLLOWING ARE DEFINITIONS USED FOR THE BETTER VERSION OF JINGLE BOOLS */
/*
#define METRONOME (u32) 1000

#define FULL_NOTE (u32)(METRONOME)
#define HALF_NOTE (u32)(METRONOME/2)
#define QUARTER_NOTE (u32)(METRONOME/4)
#define EIGHTH_NOTE (u32)(METRONOME/8)
#define FN FULL_NOTE
#define HN HALF_NOTE
#define QN QUARTER_NOTE
#define EN EIGHTH_NOTE

#define E4 E4FREQ_HZ
#define G4 G4FREQ_HZ
#define C4 C4FREQ_HZ
#define D4 D4FREQ_HZ
#define F4 F4FREQ_HZ
*/

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);
void FestiveLightsPatternOne(bool reset);
void AllLedsOff(void);
void CycleLeds(void);
bool Jingle(bool reset);
static bool betterJingle(bool reset);
bool isTimeUp(bool reset, u32 u32time);
void testLCD(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         


#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
