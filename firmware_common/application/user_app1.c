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
u8 u8volume = 76; /* The volume */

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
  LcdClearScreen();
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
  renderBar();
} /* end UserApp1SM_Idle() */
  
/*
static void testLCD()
{
  u8 array[15][4] = {{0xD1,0x17,0x04,0x0E},
                    {0x51,0x10,0x04,0x11},
                    {0x51,0x10,0x04,0x11},
                    {0xDF,0x17,0x04,0x11},
                    {0x51,0x10,0x04,0xD1},
                    {0x51,0x10,0x04,0x91},
                    {0xD1,0xF7,0x7D,0x4E},
                    {0x00,0x00,0x00,0x00},
                    {0x91,0x04,0x04,0x47},
                    {0x51,0x05,0x05,0x49},
                    {0x51,0x05,0x05,0x51},
                    {0x51,0x04,0x04,0x51},
                    {0x55,0x04,0x04,0x51},
                    {0x55,0x04,0x04,0x09},
                    {0x8E,0x7D,0x7D,0x47}};
  //u8 array[9] = {1,1,1,1,1,1,1,1,1

  const u8* adress = &(array[0][0]);
  PixelBlockType block = {0,0,15,(8*4)};
  LcdLoadBitmap(adress,&block);
}
*/

static void renderBar(void)
{
  u8 u8arr_Bar[31][1] = {{0x00},
                 {0x7E},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x42},
                 {0x7E},
                 {0x00}};
  //Bar refers to the vanilla bar right now, which assumes no volume. We now mess with the bar as necessary.

  //Volume goes from top to bottom, beginning at the top if volume is at max or the bottom if the volume is at the bottom.
  //A volume of 100 means we should begin at 3 and go to 27, a volume of 96 means we should start at 4 and go to 27. Volume only increments in fours.
  u8 u8startIndex = 28 - (u8volume / 4);
  for(u8 u8index = u8startIndex; u8index <= 27; u8index++)
  {
    u8arr_Bar[u8index][0] = 0x5A; //This represents a block of the bar filled in.
  }
  
  
  const u8* u8pAddress = &(u8arr_Bar[0][0]);
  PixelBlockType PBTbarInfo = {0,0,30, (1*8)};
  LcdLoadBitmap(u8pAddress,&PBTbarInfo);
}

static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
