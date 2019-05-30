/*!**********************************************************************************************************************
@file input_pins.h        
@brief Header file for input_pins.c
***********************************************************************************************************************/

#ifndef __INPUTPINS_H
#define __INPUTPINS_H

/***********************************************************************************************************************
Type Definitions
***********************************************************************************************************************/

typedef enum {VOLTAGE_LOW = 0, VOLTAGE_HIGH = 1} InputStateType; 
typedef enum {MOSI_PIN = 0, MISO_PIN = 1} PinNameType;
typedef enum {PIN_PORTA = 0, PIN_PORTB = 0x80} PinPortType;
typedef enum {INPUT_ACTIVE_LOW = 0, INPUT_ACTIVE_HIGH = 1} InputActiveType;

typedef struct 
{
  InputStateType eCurrentState;
  InputStateType eNewState;
  bool bNewActivityFlag;
}InputStatusType;

typedef struct
{
  InputActiveType eActiveState;
  PinPortType ePort;
}PinConfigType;


/***********************************************************************************************************************
Constants / Definitions
***********************************************************************************************************************/
#define PIN_INIT_MSG_TIMEOUT         (u32)1000     /* Time in ms for init message to send */
#define PIN_DEBOUNCE_TIME            (u32)10       /* Time in ms for button debouncing */

#define GPIOA_PINS (u32)(PA_14_BLADE_MOSI | PA_13_BLADE_MISO)
#define INPUT_PINS_IN_USE 2
  
#define MOSI_PIN 0
#define MISO_PIN 1

/***********************************************************************************************************************
Function Declarations
***********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
bool IsPinActive(u32 u32InputPin);
void PinActiveAcknowledge(u32 u32InputPin);

/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void InputPinInitialize(void);                        
void InputPinRunActiveState(void);
u32 GetPinBitLocation(u8 u8Pin_, ButtonPortType ePort_);

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void InputPinSM_Idle(void);                
static void InputPinSM_PinActive(void);        


#endif /* __INPUTPINS_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
