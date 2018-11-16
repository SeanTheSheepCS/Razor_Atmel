/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  //Make the LED blink 
  HEARTBEAT_OFF();
  AllLedsOff();
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();
  
  //Make the LED blink 
  
} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{ 
  
  static bool shouldJingle = FALSE;

  if(WasButtonPressed(BUTTON0))
  {
    shouldJingle = TRUE;
    AllLedsOff();
    FestiveLightsPatternOne(TRUE);
    ButtonAcknowledge(BUTTON0);
  }
  if(IsButtonPressed(BUTTON1))
  {
    shouldJingle = FALSE;
    AllLedsOff();
    PWMAudioOff(BUZZER1);
  }
  if(shouldJingle)
  {
    shouldJingle = Jingle(FALSE);
    FestiveLightsPatternOne(FALSE);
  }
  else
  {
    CycleLeds();
  }
  
  if(IsButtonHeld(BUTTON1,500))
  {
    Jingle(TRUE); //This resets the jingle timer.
  }
} /* end UserApp1SM_Idle() */
    
static void AllLedsOff(void)
{
  static LedNumberType aeAllLeds[] = {GREEN0,RED0,BLUE0,GREEN1,RED1,BLUE1,GREEN2,RED2,BLUE2,GREEN3,RED3,BLUE3};
  //static LedNumberType aeCurrentLedOne[] = {GREEN1,RED1,BLUE1};
  //static LedNumberType aeCurrentLedTwo[] = {GREEN2,RED2,BLUE2};
  //static LedNumberType aeCurrentLedThree[] = {GREEN3,RED3,BLUE3};
  for (u32 u32index = 0; u32index < 12; u32index++)
  {
    LedOff(aeAllLeds[u32index]);
  }
}

static void CycleLeds(void)
{
  static LedNumberType aeSelectedLed[] = {GREEN0,BLUE0,RED0};
  static LedNumberType aeSelectedLedOne[] = {GREEN1,BLUE1,RED1};
  static LedNumberType aeSelectedLedTwo[] = {GREEN2,BLUE2,RED2};
  static LedNumberType aeSelectedLedThree[] = {GREEN3,BLUE3,RED3};
  static bool ascending = TRUE;
  static u32 u32ledSelector = 0;
  static u32 u32timePassed_MS = 0;
  
  if(u32timePassed_MS < CYCLE_COLOURS_LIMIT_MS)
  {
    if(u32timePassed_MS%100==0)
    {
      if(ascending)
      {
        LedPWM(aeSelectedLed[u32ledSelector],u32timePassed_MS/100);
        LedPWM(aeSelectedLedOne[u32ledSelector],u32timePassed_MS/100);
        LedPWM(aeSelectedLedTwo[u32ledSelector],u32timePassed_MS/100);
        LedPWM(aeSelectedLedThree[u32ledSelector],u32timePassed_MS/100);
      }
      else
      {
        LedPWM(aeSelectedLed[u32ledSelector],10-u32timePassed_MS/100);
        LedPWM(aeSelectedLedOne[u32ledSelector],10-u32timePassed_MS/100);
        LedPWM(aeSelectedLedTwo[u32ledSelector],10-u32timePassed_MS/100);
        LedPWM(aeSelectedLedThree[u32ledSelector],10-u32timePassed_MS/100);
      }
    }
  }
  else
  {
    if(ascending)
    {
      ascending = FALSE;
      u32timePassed_MS = 0;
      if(u32ledSelector == 0) //If you selected the green,
      {
        u32ledSelector = 2; //You now dim the red.
      }
      else if(u32ledSelector == 1) //If you selected the blue,
      {
        u32ledSelector = 0; //You now dim the green.
      }
      else if(u32ledSelector == 2) //If you selected the red,
      {
        u32ledSelector = 1; //You now dim the blue.
      }
    }
    else
    {
      ascending = TRUE;
      u32timePassed_MS = 0;
      if(u32ledSelector == 0) //If you dimmed the green,
      {
        u32ledSelector = 2; //You now light up the red.
      }
      else if(u32ledSelector == 1) //If you dimmed the blue,
      {
        u32ledSelector = 0; //You now light up the green.
      }
      else if(u32ledSelector == 2) //If you dimmed the red,
      {
        u32ledSelector = 1; //You now light up the blue.
      }
    }
  }
  u32timePassed_MS++;
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */

static void FestiveLightsPatternOne(bool reset)
{
  //Increment counter every millisecond
  static bool hasReachedLED0 = FALSE;
  static bool hasReachedLED1 = FALSE;
  static bool hasReachedLED2 = FALSE;
  static bool hasReachedLED3 = FALSE;
  static u32 u32alternatingColoursCounter = 0;
  
  if(reset)
  {
    hasReachedLED0 = FALSE;
    hasReachedLED1 = FALSE;
    hasReachedLED2 = FALSE;
    hasReachedLED3 = FALSE;
    u32alternatingColoursCounter = 0;
    return;
  }
  
  u32alternatingColoursCounter++;
  if((u32alternatingColoursCounter > ALTERNATING_COLOUR_LIMIT_MS)&&(!hasReachedLED0))
  {
    //If 0.5 seconds have passed, turn on the leftmost LED
    LedPWM(GREEN0, LED_PWM_35);
    hasReachedLED0 = TRUE;
  }
  if(u32alternatingColoursCounter > 2*ALTERNATING_COLOUR_LIMIT_MS&&(!hasReachedLED1))
  {
    //If 1 second has passed, turn on the next LED
    LedPWM(RED1, LED_PWM_35);
    LedPWM(GREEN0, LED_PWM_35); //Needed since the LED may have been turned off by AllLedsOff()
    hasReachedLED1 = TRUE;
  }
  if(u32alternatingColoursCounter > 3*ALTERNATING_COLOUR_LIMIT_MS&&(!hasReachedLED2))
  {
    //If 1.5 seconds have passed, turn on the next LED
    LedPWM(GREEN2, LED_PWM_35);
    LedPWM(RED1, LED_PWM_35);
    LedPWM(GREEN0, LED_PWM_35); //Needed since the LED may have been turned off by AllLedsOff()
    hasReachedLED2 = TRUE;
  }
  if(u32alternatingColoursCounter > 4*ALTERNATING_COLOUR_LIMIT_MS&&(!hasReachedLED3))
  {
    //If 2 seconds have passed, turn on the last LED
    LedPWM(RED3, LED_PWM_35);
    LedPWM(GREEN2, LED_PWM_35);
    LedPWM(RED1, LED_PWM_35);
    LedPWM(GREEN0, LED_PWM_35); //Needed since the LED may have been turned off by AllLedsOff()
    hasReachedLED3 = TRUE;
  }
  if(u32alternatingColoursCounter > 5*ALTERNATING_COLOUR_LIMIT_MS)
  {
    //If 2.5 seconds have passed, turn off all the LEDs
    u32alternatingColoursCounter = 0;
    LedOff(GREEN0);
    LedOff(RED1);
    LedOff(GREEN2);
    LedOff(RED3);
    hasReachedLED0 = FALSE;
    hasReachedLED1 = FALSE;
    hasReachedLED2 = FALSE;
    hasReachedLED3 = FALSE;
  }
  
}

static bool Jingle(bool reset)
{
  static u32 u32timePassed_MS = 0; //The number of milliseconds that the jingle has been played for
  static u32 u32jingleQNotesPassed = 0; //The number of quarter notes that have been played in the jingle
  static u32 u32noteTime_MS = 0; //The number of milliseconds the current note is to be played for
  
  if(reset)
  {
    u32timePassed_MS = 0;
    return FALSE;
  }
  
  u32jingleQNotesPassed = u32timePassed_MS / QUARTERNOTETIME_MS; //Sets QNotes passed based on the time passed
  
  //Choose what note to play based on the number of quarter notes that have passed.
  
  if(u32jingleQNotesPassed == 0)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 2)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS * 2;
  }
  else if(u32jingleQNotesPassed == 4)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 6)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS * 2;
  }
  else if(u32jingleQNotesPassed == 8)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 9)
  {
    PWMAudioSetFrequency(BUZZER1,G4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 10)
  {
    PWMAudioSetFrequency(BUZZER1,C4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 11)
  {
    PWMAudioSetFrequency(BUZZER1,D4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 12)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS*4;
  }
  else if(u32jingleQNotesPassed == 16)
  {
    PWMAudioSetFrequency(BUZZER1,F4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 21)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 23)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS / 2;
  }
  else if(u32jingleQNotesPassed == 24)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 26)
  {
    PWMAudioSetFrequency(BUZZER1,D4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 27)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 28)
  {
    PWMAudioSetFrequency(BUZZER1,D4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS*2;
  }
  else if(u32jingleQNotesPassed == 30)
  {
    PWMAudioSetFrequency(BUZZER1,G4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 31)
  {
    PWMAudioSetFrequency(BUZZER1,0);
    u32noteTime_MS = 0;
  }
  //2ND LINE
  if(u32jingleQNotesPassed == 32)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 34)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS * 2;
  }
  else if(u32jingleQNotesPassed == 36)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 38)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS * 2;
  }
  else if(u32jingleQNotesPassed == 40)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 41)
  {
    PWMAudioSetFrequency(BUZZER1,G4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 42)
  {
    PWMAudioSetFrequency(BUZZER1,C4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 43)
  {
    PWMAudioSetFrequency(BUZZER1,D4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 44)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS*4;
  }
  else if(u32jingleQNotesPassed == 48)
  {
    PWMAudioSetFrequency(BUZZER1,F4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 53)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 55)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS / 2;
  }
  else if(u32jingleQNotesPassed == 56)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 58)
  {
    PWMAudioSetFrequency(BUZZER1,D4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 59)
  {
    PWMAudioSetFrequency(BUZZER1,E4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS;
  }
  else if(u32jingleQNotesPassed == 60)
  {
    PWMAudioSetFrequency(BUZZER1,D4FREQ_HZ);
    u32noteTime_MS = QUARTERNOTETIME_MS*4;
  }
  else if(u32jingleQNotesPassed > 64)
  {
    return FALSE;
  }
  
  
  //Buzzer should turn off for the last 100 ms. If the note lasts less than 100ms, then it should wait  1/5 of the length of the note.
  u32 u32playTime = 0;
  if(u32noteTime_MS < 100)
  {
    u32playTime = (4/5)*u32noteTime_MS;
  }
  else
  {
    u32playTime = u32noteTime_MS-100;
  }
  
  
  if(u32timePassed_MS % u32noteTime_MS < u32playTime)
  {
    PWMAudioOn(BUZZER1);
  }
  else
  {
    PWMAudioOff(BUZZER1);
  }
  
  
  u32timePassed_MS++;
  
  return TRUE;
}

static bool betterJingle(bool reset)
{
  
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
