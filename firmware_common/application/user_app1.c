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
#include "music.h"

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

extern u32 G_u32AntApiCurrentMessageTimeStamp;                            // From ant_api.c
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;            // From ant_api.c
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  // From ant_api.c
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                // From ant_api.c


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static AntAssignChannelInfoType UserApp1_sChannelInfo = {0};
static u8* UserApp1_au8MessageFail = "A failure has occurred, most likely with ant!\n";
static u32 UserApp1_u32DataMsgCount = 0; /* ANT_DATA packets recieved */
static u32 UserApp1_u32TickMsgCount = 0;
static u8 UserApp1_au8NoteNumbers[] =   {0x48,0x49,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x90,0x91,0x92,0x93,0x94,0x95};
static u16 UserApp1_au16Frequencies[] = {C3  ,C3S ,D3  ,D3S ,E3  ,F3  ,F3S ,G3  ,G3S ,A3  ,A3S ,B3  ,C4  ,C4S ,D4  ,D4S ,E4  ,F4  ,F4S ,G4  ,G4S ,A4  ,A4S ,B4  ,C5  ,C5S ,D5  ,D5S ,E5  ,F5  ,F5S ,G5  ,G5S ,A5  ,A5S ,B5  ,C6  ,C6S ,D6  ,D6S ,E6  ,F6  ,F6S ,G6  ,G6S ,A6  ,A6S ,B6  };

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
  CapTouchOn();

  LedOn(RED0);

  /* Configure ANT for this application */
  UserApp1_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  UserApp1_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

  UserApp1_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  UserApp1_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;

  UserApp1_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;

  for( u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }

  if( AntAssignChannel(&UserApp1_sChannelInfo))
  {
    //Channel has been assigned, change the led colour to purple.
    LedOn(BLUE0);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }
  else
  {
    DebugPrintf(UserApp1_au8MessageFail);
    LedBlink(RED0, LED_4HZ);
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

/* Wait for any ANT channel assignment */
static void UserApp1SM_AntChannelAssign(void)
{
  u8 u8status = AntRadioStatusChannel(ANT_CHANNEL_USERAPP);
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    /*Channel assignment was successful!*/
    DebugPrintf("Channel Assignment Successful!");
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }

  if(IsTimeUp(&UserApp1_u32Timeout, 5000))
  {
    DebugPrintf(UserApp1_au8MessageFail);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(RED0);
    LedOff(BLUE0);
    LedBlink(GREEN0, LED_2HZ);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }
} /* end UserApp1SM_Idle() */

static void UserApp1SM_WaitChannelOpen(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    LedOn(GREEN0);
    UserApp1_StateMachine = UserApp1SM_ChannelOpen;
  }

  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE))
  {
    LedOff(GREEN0);
    LedOn(BLUE0);
    LedOn(RED0);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
}

static void UserApp1SM_ChannelOpen(void)
{
  static u8 u8LastState = 0xff;
  static u8 au8TickMessage[] = "EVENT x\n\r"; /* "x" at index [6] will be replaced by the current code */
  static u8 au8DataContent[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static u8 au8DataContentForPrinting[] = "xxxxxxxxxxxxxxxx";
  static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  static u8 au8TestMessage[] = {0,0,0,0,0xA5,0,0,0};
  bool bGotNewData;

  /* Channel stays open until the button is pressed again */
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);

    /* Queues closing the channel. Initializes the u8LastState variable and changes the
     * LED to blinking green to indicate the channel is closing
     */
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    u8LastState = 0xff;
    LedOff(GREEN0);
    LedBlink(GREEN0, LED_2HZ);

    /* Changes state and sets the timer */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
  }

  /* The slave channel may have closed on its own! Has that happened yet? */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) != ANT_OPEN)
  {
    LedOff(GREEN0);
    LedBlink(GREEN0, LED_2HZ);
    u8LastState = 0xff;

    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
  }

  /* Check to see if an ANT message has arrived */
  if(AntReadAppMessageBuffer())
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA) /* Is this a new data message? */
    {
      UserApp1_u32DataMsgCount++;
      /* Linked with a device = solid blue! */
      LedOff(GREEN0);
      LedOn(BLUE0);

      bGotNewData = FALSE;
      for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])
        {
          bGotNewData = TRUE;
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];
          au8DataContent[i] = G_au8AntApiCurrentMessageBytes[i];
          au8DataContentForPrinting[2*i] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16); // ORIGINAL : ...CurrentData[i]
          au8DataContentForPrinting[2*i+1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16); // ORIGINAL: ...CurrentData[i]
        }
      }
      /* Print the new data */
      DebugPrintf(au8DataContentForPrinting);
      interpretData(au8DataContent);
      DebugLineFeed();

    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK) /* Is this a tick message? */
    {
      UserApp1_u32TickMsgCount++;
      if( u8LastState != G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX])
      {
        u8LastState = G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX];
      }
      au8TickMessage[6] = HexToASCIICharUpper(u8LastState);
      DebugPrintf(au8TickMessage);

      switch(u8LastState)
      {
        /* Paired but missing messages gives a blue blink */
        case EVENT_RX_FAIL:
        {
          LedOff(GREEN0);
          LedBlink(BLUE0, LED_2HZ);
          break;
        }
        /* If you stopped searching, turn the LED green */
        case EVENT_RX_FAIL_GO_TO_SEARCH:
        {
          LedOff(BLUE0);
          LedOn(GREEN0);
          break;
        }
        /* The channel automatically closes if the search times out */
        case EVENT_RX_SEARCH_TIMEOUT:
        {
          DebugPrintf("Search timeout\r\n");
          break;
        }
        default:
        {
          LedOff(BLUE0);
          LedOn(GREEN0);
          DebugPrintf("Unexpected Event \r\n");
          break;
        }
      }
    }
  }
}

static void UserApp1SM_WaitChannelClose(void)
{
  /* Has the channel been closed yet? */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CLOSED)
  {
    LedOff(GREEN0);
    LedOn(BLUE0);
    LedOn(RED0);

    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  /* Have we timed out yet? */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE))
  {
    LedOff(GREEN0);
    LedBlink(RED0, LED_4HZ);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}


static void interpretData(u8* au8DataContent)
{
  if(au8DataContent[0] == 0x90)
  {
    u8 nodeNumber = au8DataContent[1];
    u16 frequency = 0;
    for(u8 i = 0; i < NUMBER_OF_POSSIBLE_NOTE_NUMBERS; i++)
    {
      if(nodeNumber == UserApp1_au8NoteNumbers[i])
      {
        frequency = UserApp1_au16Frequencies[i];
        break;
      }
    }
    if(frequency == 0)
    {
      //This note is too low to be represented with our current set of frequencies!
      DebugPrintf("The entered note was outside our frequency range. Consider putting implementation into the C# program to shift the song one octave up if the song has notes like this.");
    }
    else
    {
      PWMAudioSetFrequency(BUZZER1,frequency);
      PWMAudioOn(BUZZER1);
    }
  }
  else if(au8DataContent[0] == 0x80)
  {
    PWMAudioOff(BUZZER1); //Assumes only one note was being played at one time!
  }
  else
  {
    //
  }
}

static void UserApp1SM_Error(void)
{

} /* end UserApp1SM_Error() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
