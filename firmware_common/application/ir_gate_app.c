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
- void IrGateInitialize(void)
- void IrGateRunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>IRStartGate"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32IrGateFlags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "IrGate_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type IrGate_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 IrGate_u32Timeout;                           /*!< @brief Timeout counter used across states */
static u8* IrGate_au8ReadyMessageWithTeam = "Ready For RED Team!";
static Team IrGate_tTeam = RED_TEAM;
static u8 IrGate_au8TimeDisplay[] = "Time: 00:00.000";
static u8* IrGate_au8ModeDisplay = "Mode: START";
static GateMode IrGate_gmCurrentMode = GATE_MODE_START;

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
@fn void IrGateInitialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void IrGateInitialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, IrGate_au8ReadyMessageWithTeam);
  LCDMessage(LINE2_START_ADDR, IrGate_au8ModeDisplay);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    IrGate_pfStateMachine = IrGateSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    IrGate_pfStateMachine = IrGateSM_Error;
  }

} /* end IrGateInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void IrGateRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void IrGateRunActiveState(void)
{
  IrGate_pfStateMachine();

} /* end IrGateRunActiveState */

static void IrGateIncrementTimer()
{
  /* MILLISECOND MANAGER */
  for(u8 i = 14; i >= 12; i--)
  {
    if(IrGate_au8TimeDisplay[i] != '9')
    {
      IrGate_au8TimeDisplay[i] = IrGate_au8TimeDisplay[i] + 1;
      return;
    }
    else
    {
      IrGate_au8TimeDisplay[i] = '0';
    }
  }
  
  /* SECOND MANAGER */
  if(IrGate_au8TimeDisplay[10] != '9')
  {
    IrGate_au8TimeDisplay[10] = IrGate_au8TimeDisplay[10] + 1;
    return;
  }
  else
  {
    IrGate_au8TimeDisplay[10] = '0';
  }
  
  if(IrGate_au8TimeDisplay[9] != '5')
  {
    IrGate_au8TimeDisplay[9]++;
    return;
  }
  else
  {
    IrGate_au8TimeDisplay[9] = '0';
  }
  
  /* MINUTE MANAGER */
  if(IrGate_au8TimeDisplay[7] != '9')
  {
    IrGate_au8TimeDisplay[7]++;
    return;
  }
  else
  {
    IrGate_au8TimeDisplay[7] = '0';
  }
  
  if(IrGate_au8TimeDisplay[6] != '5')
  {
    IrGate_au8TimeDisplay[6]++;
    return;
  }
  else
  {
    IrGate_au8TimeDisplay[6] = '0';
  }
} /* end IrGateIncrementTimer */

static void IrGateDisplayTimer()
{
  LCDClearChars(LINE1_START_ADDR, 15);
  LCDMessage(LINE1_START_ADDR, IrGate_au8TimeDisplay);
}

static void IrGateResetTimer()
{
  IrGate_au8TimeDisplay[14] = '0';
  IrGate_au8TimeDisplay[13] = '0';
  IrGate_au8TimeDisplay[12] = '0';
  IrGate_au8TimeDisplay[10] = '0';
  IrGate_au8TimeDisplay[9]  = '0';
  IrGate_au8TimeDisplay[7]  = '0';
  IrGate_au8TimeDisplay[6]  = '0';
}


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

void CycleMode()
{
  if(IrGate_gmCurrentMode == GATE_MODE_START)
  {
    IrGate_au8ModeDisplay = "Mode: INTERMEDIATE";
    IrGate_gmCurrentMode = GATE_MODE_INTERMEDIATE;
  }
  else if(IrGate_gmCurrentMode == GATE_MODE_INTERMEDIATE)
  {
    IrGate_au8ModeDisplay = "Mode: FINISH";
    IrGate_gmCurrentMode = GATE_MODE_FINISH;
  }
  else if(IrGate_gmCurrentMode == GATE_MODE_FINISH)
  {
    IrGate_au8ModeDisplay = "Mode: START";
    IrGate_gmCurrentMode = GATE_MODE_START;
  }
  LCDClearChars(LINE2_START_ADDR, 18);
  LCDMessage(LINE2_START_ADDR, IrGate_au8ModeDisplay);
}

void CycleTeam()
{
  if(IrGate_tTeam == RED_TEAM)
  {
    IrGate_au8ReadyMessageWithTeam = "Ready For BLUE Team!";
    IrGate_tTeam = BLUE_TEAM;
    ANTMChannelSetAntFrequency(11);
    ANTSChannelSetAntFrequency(11);
  }
  else if(IrGate_tTeam == BLUE_TEAM)
  {
    IrGate_au8ReadyMessageWithTeam = "Ready For RED Team!";
    IrGate_tTeam = RED_TEAM;
    ANTMChannelSetAntFrequency(91);
    ANTSChannelSetAntFrequency(91);
  }
  LCDClearChars(LINE1_START_ADDR, 20);
  LCDMessage(LINE1_START_ADDR, IrGate_au8ReadyMessageWithTeam);
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void IrGateSM_Idle(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    CycleTeam();
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    CycleMode();
  }
  else if(HasThePinBeenActivated(UPIMO_PIN) && IrGate_gmCurrentMode == GATE_MODE_START)
  {
    IrGate_pfStateMachine = IrGateSM_TimerActive;
    PinActiveAcknowledge(UPIMO_PIN);
  }
  //else if( /* GOT AN ANT MESSAGE THAT WAS A START TIMER COMMAND */)
  //{
  //  
  //}
} /* end IRStartGateSM_Idle() */
     
static void IrGateSM_TimerActive(void)
{
  IrGateIncrementTimer();
  if(G_u32SystemTime1ms % 19 == 0)
  {
    IrGateDisplayTimer();
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
    LCDClearChars(LINE1_START_ADDR, 20);
    LCDMessage(LINE1_START_ADDR, IrGate_au8ReadyMessageWithTeam);
    IrGateResetTimer();
    LedOff(GREEN);
    IrGate_pfStateMachine = IrGateSM_Idle;
  } 
} /* end IrGateSM_TimerActive() */

static void IrGateSM_ReplyRecieved(void)
{
  
}


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void IrGateSM_Error(void)          
{
  
} /* end IRStartGateSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
