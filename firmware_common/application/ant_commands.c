/*!*********************************************************************************************************************
@file IrGatepp.c                                                                
@brief Responsible for: 
          -starting and managing the timer
          -recieving input from the board attached to the blade dock
------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- u8 messageToAntCommand(u8 au8Message[])


**********************************************************************************************************************/

#include "configuration.h"

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
u8 messageToAntCommand(u8 au8Message[])
{
  static u8 au8BeginMessage[] = ANT_MESSAGE_BEGIN_TIMER;
  static u8 au8EndMessage[] = ANT_MESSAGE_END_TIMER;
  static bool isBeginMessage = TRUE;
  static bool isEndMessage = TRUE;
  for(u8 i = 0; i < ANT_MESSAGE_LENGTH_BYTES; i++)
  {
    if(au8Message[i] != au8BeginMessage[i])
    {
      isBeginMessage = FALSE;
    }
    if(au8Message[i] != au8EndMessage[i])
    {
      isEndMessage = FALSE;
    }
  }
  if(isBeginMessage)
  {
    return ANT_COMMAND_BEGIN_TIMER;
  }
  if(isEndMessage)
  {
    return ANT_COMMAND_END_TIMER;
  }
  return ANT_COMMAND_INVALID;
}

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
