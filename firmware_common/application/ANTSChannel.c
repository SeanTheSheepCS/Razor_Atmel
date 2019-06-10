/*!*********************************************************************************************************************
@file ANTSChannel.c                                                                
@brief Recieves messages from the end gate
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
- void ANTSChannelInitialize(void)
- void ANTSChannelRunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>ANTSChannel"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ANTSChannelFlags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "ANTSChannel_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type ANTSChannel_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp3_u32Timeout;                           /*!< @brief Timeout counter used across states */


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
@fn void ANTSChannelInitialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void ANTSChannelInitialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    ANTSChannel_pfStateMachine = ANTSChannelSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    ANTSChannel_pfStateMachine = ANTSChannelSM_Error;
  }

} /* end ANTSChannelInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void ANTSChannelRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void ANTSChannelRunActiveState(void)
{
  ANTSChannel_pfStateMachine();

} /* end ANTSChannelRunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void ANTSChannelSM_Idle(void)
{
    
} /* end ANTSChannelSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void ANTSChannelSM_Error(void)          
{
  
} /* end ANTSChannelSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
