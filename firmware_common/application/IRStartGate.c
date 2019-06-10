/*!*********************************************************************************************************************
@file IRStartGate.c                                                                
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
- void IRStartGateInitialize(void)
- void IRStartGateRunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>IRStartGate"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32IRStartGateFlags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "IRStartGate_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type IRStartGate_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 IRStartGate_u32Timeout;                           /*!< @brief Timeout counter used across states */
static u8 IRStartGate_au8GenericReadyMessage[] = "Ready!";
static u8 IRStartGate_au8TimeDisplay[] = "Time: 00:00.000";

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void IRStartGateInitialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void IRStartGateInitialize(void)
{
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, IRStartGate_au8GenericReadyMessage);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    IRStartGate_pfStateMachine = IRStartGateSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    IRStartGate_pfStateMachine = IRStartGateSM_Error;
  }

} /* end IRStartGateInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void IRStartGateRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void IRStartGateRunActiveState(void)
{
  IRStartGate_pfStateMachine();

} /* end IRStartGateRunActiveState */

static void IRStartGateIncrementTimer()
{
  /* MILLISECOND MANAGER */
  for(u8 i = 14; i >= 12; i--)
  {
    if(IRStartGate_au8TimeDisplay[i] != '9')
    {
      IRStartGate_au8TimeDisplay[i] = IRStartGate_au8TimeDisplay[i] + 1;
      return;
    }
    else
    {
      IRStartGate_au8TimeDisplay[i] = '0';
    }
  }
  
  /* SECOND MANAGER */
  if(IRStartGate_au8TimeDisplay[10] != '9')
  {
    IRStartGate_au8TimeDisplay[10] = IRStartGate_au8TimeDisplay[10] + 1;
    return;
  }
  else
  {
    IRStartGate_au8TimeDisplay[10] = '0';
  }
  
  if(IRStartGate_au8TimeDisplay[9] != '5')
  {
    IRStartGate_au8TimeDisplay[9]++;
    return;
  }
  else
  {
    IRStartGate_au8TimeDisplay[9] = '0';
  }
  
  /* MINUTE MANAGER */
  if(IRStartGate_au8TimeDisplay[7] != '9')
  {
    IRStartGate_au8TimeDisplay[7]++;
    return;
  }
  else
  {
    IRStartGate_au8TimeDisplay[7] = '0';
  }
  
  if(IRStartGate_au8TimeDisplay[6] != '5')
  {
    IRStartGate_au8TimeDisplay[6]++;
    return;
  }
  else
  {
    IRStartGate_au8TimeDisplay[6] = '0';
  }
} /* end IRStartGateIncrementTimer */

static void IRStartGateDisplayTimer()
{
  LcdClearChars(LINE1_START_ADDR, 15);
  LcdMessage(LINE1_START_ADDR, IRStartGate_au8TimeDisplay);
}

static void IRStartGateResetTimer()
{
  IRStartGate_au8TimeDisplay[14] = '0';
  IRStartGate_au8TimeDisplay[13] = '0';
  IRStartGate_au8TimeDisplay[12] = '0';
  IRStartGate_au8TimeDisplay[10] = '0';
  IRStartGate_au8TimeDisplay[9] = '0';
  IRStartGate_au8TimeDisplay[7] = '0';
  IRStartGate_au8TimeDisplay[6] = '0';
}


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void IRStartGateSM_Idle(void)
{
  if(HasThePinBeenActivated(UPIMO_PIN))
  {
    IRStartGate_pfStateMachine = IRStartGateSM_TimerActive;
    PinActiveAcknowledge(UPIMO_PIN);
  }
} /* end IRStartGateSM_Idle() */
     
static void IRStartGateSM_TimerActive(void)
{
  IRStartGateIncrementTimer();
  if(G_u32SystemTime1ms % 19 == 0)
  {
    IRStartGateDisplayTimer();
  }
  
  if(IsPinActive(UPIMO_PIN))
  {
    LedOn(GREEN);
  }
  else
  {
    LedOff(GREEN);
  }
  if(IsButtonPressed(BUTTON0))
  {
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, IRStartGate_au8GenericReadyMessage);
    IRStartGateResetTimer();
    IRStartGate_pfStateMachine = IRStartGateSM_Idle;
  } 
} /* end IRStartGateSM_TimerActive() */

static void IRStartGateSM_ReplyRecieved(void)
{
  
}


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void IRStartGateSM_Error(void)          
{
  
} /* end IRStartGateSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
