/*******************************************************************************
* File Name: PH4.c  
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
#include "PH4.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        PH4_PC =   (PH4_PC & \
                                (uint32)(~(uint32)(PH4_DRIVE_MODE_IND_MASK << (PH4_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (PH4_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: PH4_Write
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
void PH4_Write(uint8 value) 
{
    uint8 drVal = (uint8)(PH4_DR & (uint8)(~PH4_MASK));
    drVal = (drVal | ((uint8)(value << PH4_SHIFT) & PH4_MASK));
    PH4_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: PH4_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  PH4_DM_STRONG     Strong Drive 
*  PH4_DM_OD_HI      Open Drain, Drives High 
*  PH4_DM_OD_LO      Open Drain, Drives Low 
*  PH4_DM_RES_UP     Resistive Pull Up 
*  PH4_DM_RES_DWN    Resistive Pull Down 
*  PH4_DM_RES_UPDWN  Resistive Pull Up/Down 
*  PH4_DM_DIG_HIZ    High Impedance Digital 
*  PH4_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void PH4_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(PH4__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: PH4_Read
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
*  Macro PH4_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 PH4_Read(void) 
{
    return (uint8)((PH4_PS & PH4_MASK) >> PH4_SHIFT);
}


/*******************************************************************************
* Function Name: PH4_ReadDataReg
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
uint8 PH4_ReadDataReg(void) 
{
    return (uint8)((PH4_DR & PH4_MASK) >> PH4_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(PH4_INTSTAT) 

    /*******************************************************************************
    * Function Name: PH4_ClearInterrupt
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
    uint8 PH4_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(PH4_INTSTAT & PH4_MASK);
		PH4_INTSTAT = maskedStatus;
        return maskedStatus >> PH4_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
