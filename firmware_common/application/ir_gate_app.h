/*!*********************************************************************************************************************
@file IRStartGate.h                                                                
@brief Header file for the IRStartGate
**********************************************************************************************************************/

#ifndef __IRGATEAPP_H
#define __IRGATEAPP_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/

typedef enum {GATE_MODE_START, GATE_MODE_FINISH, GATE_MODE_INTERMEDIATE} GateMode; 
typedef enum {RED_TEAM, BLUE_TEAM} Team;

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void IrGateInitialize(void);
void IrGateRunActiveState(void);

void IrGateResetTimer(void);
void IrGateIncrementTimer(void);
void IrGateDisplayTimer(void);

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void CycleMode(void);
void CycleTeam(void);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void IrGateSM_Idle(void);
static void IrGateSM_TimerActive(void);
static void IrGateSM_ReplyRecieved(void);
static void IrGateSM_Error(void);         



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/


#endif /* __IRGATEAPP_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
