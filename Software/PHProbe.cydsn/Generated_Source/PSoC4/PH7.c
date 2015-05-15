/*******************************************************************************
* File Name: PH7.c  
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
#include "PH7.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        PH7_PC =   (PH7_PC & \
                                (uint32)(~(uint32)(PH7_DRIVE_MODE_IND_MASK << (PH7_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (PH7_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: PH7_Write
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
void PH7_Write(uint8 value) 
{
    uint8 drVal = (uint8)(PH7_DR & (uint8)(~PH7_MASK));
    drVal = (drVal | ((uint8)(value << PH7_SHIFT) & PH7_MASK));
    PH7_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: PH7_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  PH7_DM_STRONG     Strong Drive 
*  PH7_DM_OD_HI      Open Drain, Drives High 
*  PH7_DM_OD_LO      Open Drain, Drives Low 
*  PH7_DM_RES_UP     Resistive Pull Up 
*  PH7_DM_RES_DWN    Resistive Pull Down 
*  PH7_DM_RES_UPDWN  Resistive Pull Up/Down 
*  PH7_DM_DIG_HIZ    High Impedance Digital 
*  PH7_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void PH7_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(PH7__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: PH7_Read
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
*  Macro PH7_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 PH7_Read(void) 
{
    return (uint8)((PH7_PS & PH7_MASK) >> PH7_SHIFT);
}


/*******************************************************************************
* Function Name: PH7_ReadDataReg
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
uint8 PH7_ReadDataReg(void) 
{
    return (uint8)((PH7_DR & PH7_MASK) >> PH7_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(PH7_INTSTAT) 

    /*******************************************************************************
    * Function Name: PH7_ClearInterrupt
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
    uint8 PH7_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(PH7_INTSTAT & PH7_MASK);
		PH7_INTSTAT = maskedStatus;
        return maskedStatus >> PH7_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
