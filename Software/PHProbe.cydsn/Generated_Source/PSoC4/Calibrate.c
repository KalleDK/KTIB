/*******************************************************************************
* File Name: Calibrate.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <Calibrate.h>

#if !defined(Calibrate__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START Calibrate_intc` */

/* `#END` */

extern cyisraddress CyRamVectors[CYINT_IRQ_BASE + CY_NUM_INTERRUPTS];

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: Calibrate_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_Start(void)
{
    /* For all we know the interrupt is active. */
    Calibrate_Disable();

    /* Set the ISR to point to the Calibrate Interrupt. */
    Calibrate_SetVector(&Calibrate_Interrupt);

    /* Set the priority. */
    Calibrate_SetPriority((uint8)Calibrate_INTC_PRIOR_NUMBER);

    /* Enable it. */
    Calibrate_Enable();
}


/*******************************************************************************
* Function Name: Calibrate_StartEx
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    Calibrate_Disable();

    /* Set the ISR to point to the Calibrate Interrupt. */
    Calibrate_SetVector(address);

    /* Set the priority. */
    Calibrate_SetPriority((uint8)Calibrate_INTC_PRIOR_NUMBER);

    /* Enable it. */
    Calibrate_Enable();
}


/*******************************************************************************
* Function Name: Calibrate_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_Stop(void)
{
    /* Disable this interrupt. */
    Calibrate_Disable();

    /* Set the ISR to point to the passive one. */
    Calibrate_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: Calibrate_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for Calibrate.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(Calibrate_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START Calibrate_Interrupt` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: Calibrate_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling Calibrate_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use Calibrate_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_SetVector(cyisraddress address)
{
    CyRamVectors[CYINT_IRQ_BASE + Calibrate__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: Calibrate_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress Calibrate_GetVector(void)
{
    return CyRamVectors[CYINT_IRQ_BASE + Calibrate__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: Calibrate_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling Calibrate_Start
*   or Calibrate_StartEx will override any effect this method would 
*   have had. This method should only be called after Calibrate_Start or 
*   Calibrate_StartEx has been called. To set the initial
*   priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 3, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_SetPriority(uint8 priority)
{
	uint8 interruptState;
    uint32 priorityOffset = ((Calibrate__INTC_NUMBER % 4u) * 8u) + 6u;
    
	interruptState = CyEnterCriticalSection();
    *Calibrate_INTC_PRIOR = (*Calibrate_INTC_PRIOR & (uint32)(~Calibrate__INTC_PRIOR_MASK)) |
                                    ((uint32)priority << priorityOffset);
	CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Calibrate_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt. 0 - 3, 0 being the highest.
*
*******************************************************************************/
uint8 Calibrate_GetPriority(void)
{
    uint32 priority;
	uint32 priorityOffset = ((Calibrate__INTC_NUMBER % 4u) * 8u) + 6u;

    priority = (*Calibrate_INTC_PRIOR & Calibrate__INTC_PRIOR_MASK) >> priorityOffset;

    return (uint8)priority;
}


/*******************************************************************************
* Function Name: Calibrate_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_Enable(void)
{
    /* Enable the general interrupt. */
    *Calibrate_INTC_SET_EN = Calibrate__INTC_MASK;
}


/*******************************************************************************
* Function Name: Calibrate_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 Calibrate_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*Calibrate_INTC_SET_EN & (uint32)Calibrate__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: Calibrate_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_Disable(void)
{
    /* Disable the general interrupt. */
    *Calibrate_INTC_CLR_EN = Calibrate__INTC_MASK;
}


/*******************************************************************************
* Function Name: Calibrate_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_SetPending(void)
{
    *Calibrate_INTC_SET_PD = Calibrate__INTC_MASK;
}


/*******************************************************************************
* Function Name: Calibrate_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Calibrate_ClearPending(void)
{
    *Calibrate_INTC_CLR_PD = Calibrate__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
