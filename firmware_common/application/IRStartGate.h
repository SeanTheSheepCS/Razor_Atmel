/*!*********************************************************************************************************************
@file IRStartGate.h                                                                
@brief Header file for the IRStartGate
**********************************************************************************************************************/

#ifndef __IRSTARTGATE_H
#define __IRSTARTGATE_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void IRStartGateInitialize(void);
void IRStartGateRunActiveState(void);

void IRStartGateResetTimer(void);
void IRStartGateIncrementTimer(void);
void IRStartGateDisplayTimer(void);

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void IRStartGateSM_Idle(void);
static void IRStartGateSM_GetInitValues(void);
static void IRStartGateSM_TimerActive(void);
static void IRStartGateSM_ReplyRecieved(void);
static void IRStartGateSM_Error(void);         



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/


#endif /* __IRSTARTGATE_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
