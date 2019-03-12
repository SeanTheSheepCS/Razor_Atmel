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
  // FUNCTIONALITY I DO NOT WANT RIGHT NOW
  //antMasterTest();
  //manageVolumeBar();
  //anageBottomButtons();

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
  static u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
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
          au8DataContent[2*i] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16); // ORIGINAL : ...CurrentData[i]
          au8DataContent[2*i+1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16); // ORIGINAL: ...CurrentData[i]
        }
      }
      if(bGotNewData)
      {
        /* Print the new data */
        DebugPrintf(au8DataContent);
        interpretData(au8DataContent);
        DebugLineFeed();
      }
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

/*
static void interpretData(u8* au8DataContent)
{
  if(au8DataContent[0] == 0x90)
  {
    //
  }
  else if(au8DataContent[0] == 0x80)
  {
    //
  }
  else
  {
    //
  }
}
*/
static void antMasterTest(void)
{
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  u8 au8dataContent[] = "xxxxxxxxxxxxxxxx";

  //Update text if a button was pressed

  //au8TestMessage[0] = 0x00;
  if(IsButtonPressed(BUTTON0))
  {
    au8TestMessage[0] = 0xff;
  }

  //ANT STUFF
  if(AntReadAppMessageBuffer())
  {
    /* New message from ANT task */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      //We were given data
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        //au8dataContent[2*i] = (G_au8AntApiCurrentMessageBytes[i] / 16);
        //au8dataContent[(2*i)+1] = (G_au8AntApiCurrentMessageBytes[i] % 16);
        au8dataContent[i] = G_au8AntApiCurrentMessageBytes[i];
        for(u8 j = 0; j < 4; j++)
        {
          au8TestMessage[j] = au8dataContent[j];
        }
      }
    }
    else if (G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      /* A channel period has gone by, typically new data should be queued to be sent here */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
    }
  }
}

static void manageBottomButtons(void)
{
  static u8 u8bottomRow = 63; //The bottom row of the screen is row 63.
  static u8 u8noOfComponents = 2; //The number of components
  static u8 u8buttonZeroCycleCount = 0; //The number of button zero presses, with a max of noOfComponents
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    u8buttonZeroCycleCount++;
    if(u8buttonZeroCycleCount == u8noOfComponents)
    {
      //We reached the last component, so we should go back to the start.
      u8buttonZeroCycleCount = 0;
    }
  }
  renderHomeButtonNineByNine(u8bottomRow - 9,0, u8buttonZeroCycleCount == 0); //Invert if cycle is on zero
  renderBackButtonNineByNine(u8bottomRow - 9,8, u8buttonZeroCycleCount == 1); //Invert if cycle is on one
}

static void manageVolumeBar(void)
{
  static u8 u8nextVolume = 0;
  static u32 u32millisecondsPassedSinceLastChange = 0;
  u8nextVolume = 100 - ((100.0/255.0)*CaptouchCurrentVSlidePosition()); //The "100 minus" thing is needed as the volume bar works in the opposite direction as the slider. (100/255) is to set the max slider value to max volume
  u8nextVolume = (u8nextVolume/4)*4; //Rounds it to the nearest multiple of four (?)
  if(u8volume == u8nextVolume)
  {
    //No change has taken place since the last time. How long has it been?
    if(u32millisecondsPassedSinceLastChange > 5000)
    {
      unRenderBar();
    }
    else
    {
      u8volume = u8nextVolume;
      u32millisecondsPassedSinceLastChange++;
      renderBar();
    }
  }
  else
  {
    //The volume has changed!
    u8volume = u8nextVolume;
    u32millisecondsPassedSinceLastChange = 0;
    renderBar();
  }
}

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

static void unRenderBar(void)
{
  u8 u8arr_Bar[31][1] = {{0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00},
                 {0x00}};
  //Rendering this will overwrite the bar
  const u8* u8pAddress = &(u8arr_Bar[0][0]);
  PixelBlockType PBTbarInfo = {0,0,30, (1*8)};
  LcdLoadBitmap(u8pAddress,&PBTbarInfo);
}

static void UserApp1SM_Error(void)
{

} /* end UserApp1SM_Error() */

static void renderHomeButtonNineByNine(u16 u16row, u16 u16col, bool shouldInvert)
{
  u8 u8pphomeButton[9][2] = {{0xFF, 0x01}, //55
                           {0x01, 0x01}, //56
                           {0x11, 0x01}, //57
                           {0x39, 0x01}, //58
                           {0x7D, 0x01}, //59
                           {0x29, 0x01}, //60
                           {0x29, 0x01}, //61
                           {0x01, 0x01}, //62
                           {0xFF, 0x01}}; //63
  if(shouldInvert)
  {
    invertButtonBitMapNineByNine(&(u8pphomeButton[0][0]),9,2);
  }
  const u8* u8pAddress = &(u8pphomeButton[0][0]);
  PixelBlockType PBThomeButtonInfo = {u16row,u16col,9, (2*8)};
  LcdLoadBitmap(u8pAddress,&PBThomeButtonInfo);
}

static void renderBackButtonNineByNine(u16 u16row, u16 u16col, bool shouldInvert)
{
  u8 u8ppbackButton[9][2] = {{0xFF, 0x01}, //55
                           {0x01, 0x01}, //56
                           {0x51, 0x01}, //57
                           {0x79, 0x01}, //58
                           {0x7D, 0x01}, //59
                           {0x79, 0x01}, //60
                           {0x51, 0x01}, //61
                           {0x01, 0x01}, //62
                           {0xFF, 0x01}}; //63
  if(shouldInvert)
  {
    invertButtonBitMapNineByNine(&(u8ppbackButton[0][0]),9,2);
  }
  const u8* u8pAddress = &(u8ppbackButton[0][0]);
  PixelBlockType PBTbackButtonInfo = {u16row,u16col,9, (2*8)};
  LcdLoadBitmap(u8pAddress,&PBTbackButtonInfo);
}

static void invertButtonBitMapNineByNine(u8* u8pMap,u8 u8rows, u8 u8cols)
{
  for(u8 i = 0; i < (u8rows*u8cols); i++)
  {
    //For a nine by nine button, the 1, 3, 5, 7, ... indices are ignored since they form up the edge of the button
    if(i%2==0)
    {
      if(u8pMap[i] == 0xFF)
      {
        //This is either the top or bottom ledge, leave it alone.
      }
      else
      {
        //Not the top or bottom edge, inverting time!
        u8pMap[i] = 0xFF - u8pMap[i]; //This inverts the button but ruins our edge!
        u8pMap[i] += 1; //Returns our edge!
      }
    }
  }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
