/*******************************************************************************
* File Name: CALI.c  
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
#include "CALI.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        CALI_PC =   (CALI_PC & \
                                (uint32)(~(uint32)(CALI_DRIVE_MODE_IND_MASK << (CALI_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (CALI_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: CALI_Write
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
void CALI_Write(uint8 value) 
{
    uint8 drVal = (uint8)(CALI_DR & (uint8)(~CALI_MASK));
    drVal = (drVal | ((uint8)(value << CALI_SHIFT) & CALI_MASK));
    CALI_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: CALI_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  CALI_DM_STRONG     Strong Drive 
*  CALI_DM_OD_HI      Open Drain, Drives High 
*  CALI_DM_OD_LO      Open Drain, Drives Low 
*  CALI_DM_RES_UP     Resistive Pull Up 
*  CALI_DM_RES_DWN    Resistive Pull Down 
*  CALI_DM_RES_UPDWN  Resistive Pull Up/Down 
*  CALI_DM_DIG_HIZ    High Impedance Digital 
*  CALI_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void CALI_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(CALI__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: CALI_Read
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
*  Macro CALI_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 CALI_Read(void) 
{
    return (uint8)((CALI_PS & CALI_MASK) >> CALI_SHIFT);
}


/*******************************************************************************
* Function Name: CALI_ReadDataReg
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
uint8 CALI_ReadDataReg(void) 
{
    return (uint8)((CALI_DR & CALI_MASK) >> CALI_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(CALI_INTSTAT) 

    /*******************************************************************************
    * Function Name: CALI_ClearInterrupt
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
    uint8 CALI_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(CALI_INTSTAT & CALI_MASK);
		CALI_INTSTAT = maskedStatus;
        return maskedStatus >> CALI_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
