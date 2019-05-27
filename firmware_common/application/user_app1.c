/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

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
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */
extern volatile u32 G_u32Spi0ApplicationFlags;            /*!< @brief From sam3u_spi.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */

static SpiPeripheralType* UserApp1_Spi;
static SpiConfigurationType UserApp1_eBladeDataflashSPI;

static u8 UserApp1_au8RxBuffer[U16_UA1_RX_BUFFER_SIZE];
static u8* UserApp1_pu8RxBufferNextChar;
static u8* UserApp1_pu8RxBufferUnreadChar;

static u8 UserApp1_au8MessageManufacturerID[] = {SPI_DUMMY, 0x9F, 0x27, 0x01, 0x00};
static u32 UserApp1_CurrentMsgToken;

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
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  UserApp1_pu8RxBufferUnreadChar = UserApp1_au8RxBuffer;
  UserApp1_pu8RxBufferNextChar = UserApp1_au8RxBuffer;
  
  UserApp1_eBladeDataflashSPI.u32CsPin = BLADE_CS_PIN;
  UserApp1_eBladeDataflashSPI.eBitOrder = SPI_MSB_FIRST;
  UserApp1_eBladeDataflashSPI.SpiPeripheral = BLADE_SPI;
  UserApp1_eBladeDataflashSPI.pCsGpioAddress = BLADE_BASE_PORT;
  UserApp1_eBladeDataflashSPI.ppu8RxNextByte = &UserApp1_pu8RxBufferNextChar;
  UserApp1_eBladeDataflashSPI.u16RxBufferSize = U16_UA1_RX_BUFFER_SIZE;
  UserApp1_eBladeDataflashSPI.pu8RxBufferAddress = UserApp1_au8RxBuffer;
  
  UserApp1_Spi = SpiRequest(&UserApp1_eBladeDataflashSPI);
  BLADE_SPI_FLAGS = 0;
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    DebugPrintf("Blade dataflash task ready \n\r");
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    UserApp1_CurrentMsgToken = SpiWriteData(UserApp1_Spi, sizeof(UserApp1_au8MessageManufacturerID), UserApp1_au8MessageManufacturerID);
    UserApp1_pfStateMachine = UserApp1SM_WaitResponse;
  }
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */

static void UserApp1SM_WaitResponse(void)
{
  u8 u8ManufacturerId = 0xff;
  u8 u8DeviceId1 = 0xff;
  u8 u8DeviceId2 = 0xff;
  u8 u8Extended = 0xff;
  u8 au8AsciiMsg[] = "FF\n\r";
  
  if(QueryMessageStatus(UserApp1_CurrentMsgToken) == COMPLETE)
  {
    UserApp1_pu8RxBufferUnreadChar++;
    u8ManufacturerId = *UserApp1_pu8RxBufferUnreadChar++;
    u8DeviceId1 = *UserApp1_pu8RxBufferUnreadChar++;
    u8DeviceId2 = *UserApp1_pu8RxBufferUnreadChar++;
    u8Extended = *UserApp1_pu8RxBufferUnreadChar++;
    
    DebugPrintf("Dataflash Manufacturer ID: 0x");
    au8AsciiMsg[0] = HexToASCIICharUpper((u8ManufacturerId & 0xF0) >> 4);
    au8AsciiMsg[1] = HexToASCIICharUpper((u8ManufacturerId & 0x0F) >> 0);
    DebugPrintf(au8AsciiMsg);
    
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
