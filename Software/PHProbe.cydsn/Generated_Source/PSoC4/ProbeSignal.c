/*******************************************************************************
* File Name: ProbeSignal.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "ProbeSignal.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        ProbeSignal_PC =   (ProbeSignal_PC & \
                                (uint32)(~(uint32)(ProbeSignal_DRIVE_MODE_IND_MASK << (ProbeSignal_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (ProbeSignal_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: ProbeSignal_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void ProbeSignal_Write(uint8 value) 
{
    uint8 drVal = (uint8)(ProbeSignal_DR & (uint8)(~ProbeSignal_MASK));
    drVal = (drVal | ((uint8)(value << ProbeSignal_SHIFT) & ProbeSignal_MASK));
    ProbeSignal_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: ProbeSignal_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  ProbeSignal_DM_STRONG     Strong Drive 
*  ProbeSignal_DM_OD_HI      Open Drain, Drives High 
*  ProbeSignal_DM_OD_LO      Open Drain, Drives Low 
*  ProbeSignal_DM_RES_UP     Resistive Pull Up 
*  ProbeSignal_DM_RES_DWN    Resistive Pull Down 
*  ProbeSignal_DM_RES_UPDWN  Resistive Pull Up/Down 
*  ProbeSignal_DM_DIG_HIZ    High Impedance Digital 
*  ProbeSignal_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void ProbeSignal_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(ProbeSignal__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: ProbeSignal_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro ProbeSignal_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 ProbeSignal_Read(void) 
{
    return (uint8)((ProbeSignal_PS & ProbeSignal_MASK) >> ProbeSignal_SHIFT);
}


/*******************************************************************************
* Function Name: ProbeSignal_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 ProbeSignal_ReadDataReg(void) 
{
    return (uint8)((ProbeSignal_DR & ProbeSignal_MASK) >> ProbeSignal_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(ProbeSignal_INTSTAT) 

    /*******************************************************************************
    * Function Name: ProbeSignal_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 ProbeSignal_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(ProbeSignal_INTSTAT & ProbeSignal_MASK);
		ProbeSignal_INTSTAT = maskedStatus;
        return maskedStatus >> ProbeSignal_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
