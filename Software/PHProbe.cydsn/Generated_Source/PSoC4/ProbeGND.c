/*******************************************************************************
* File Name: ProbeGND.c  
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
#include "ProbeGND.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        ProbeGND_PC =   (ProbeGND_PC & \
                                (uint32)(~(uint32)(ProbeGND_DRIVE_MODE_IND_MASK << (ProbeGND_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (ProbeGND_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: ProbeGND_Write
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
void ProbeGND_Write(uint8 value) 
{
    uint8 drVal = (uint8)(ProbeGND_DR & (uint8)(~ProbeGND_MASK));
    drVal = (drVal | ((uint8)(value << ProbeGND_SHIFT) & ProbeGND_MASK));
    ProbeGND_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: ProbeGND_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  ProbeGND_DM_STRONG     Strong Drive 
*  ProbeGND_DM_OD_HI      Open Drain, Drives High 
*  ProbeGND_DM_OD_LO      Open Drain, Drives Low 
*  ProbeGND_DM_RES_UP     Resistive Pull Up 
*  ProbeGND_DM_RES_DWN    Resistive Pull Down 
*  ProbeGND_DM_RES_UPDWN  Resistive Pull Up/Down 
*  ProbeGND_DM_DIG_HIZ    High Impedance Digital 
*  ProbeGND_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void ProbeGND_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(ProbeGND__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: ProbeGND_Read
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
*  Macro ProbeGND_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 ProbeGND_Read(void) 
{
    return (uint8)((ProbeGND_PS & ProbeGND_MASK) >> ProbeGND_SHIFT);
}


/*******************************************************************************
* Function Name: ProbeGND_ReadDataReg
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
uint8 ProbeGND_ReadDataReg(void) 
{
    return (uint8)((ProbeGND_DR & ProbeGND_MASK) >> ProbeGND_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(ProbeGND_INTSTAT) 

    /*******************************************************************************
    * Function Name: ProbeGND_ClearInterrupt
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
    uint8 ProbeGND_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(ProbeGND_INTSTAT & ProbeGND_MASK);
		ProbeGND_INTSTAT = maskedStatus;
        return maskedStatus >> ProbeGND_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
