/*******************************************************************************
* File Name: ProbePos.c  
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
#include "ProbePos.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        ProbePos_PC =   (ProbePos_PC & \
                                (uint32)(~(uint32)(ProbePos_DRIVE_MODE_IND_MASK << (ProbePos_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (ProbePos_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: ProbePos_Write
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
void ProbePos_Write(uint8 value) 
{
    uint8 drVal = (uint8)(ProbePos_DR & (uint8)(~ProbePos_MASK));
    drVal = (drVal | ((uint8)(value << ProbePos_SHIFT) & ProbePos_MASK));
    ProbePos_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: ProbePos_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  ProbePos_DM_STRONG     Strong Drive 
*  ProbePos_DM_OD_HI      Open Drain, Drives High 
*  ProbePos_DM_OD_LO      Open Drain, Drives Low 
*  ProbePos_DM_RES_UP     Resistive Pull Up 
*  ProbePos_DM_RES_DWN    Resistive Pull Down 
*  ProbePos_DM_RES_UPDWN  Resistive Pull Up/Down 
*  ProbePos_DM_DIG_HIZ    High Impedance Digital 
*  ProbePos_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void ProbePos_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(ProbePos__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: ProbePos_Read
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
*  Macro ProbePos_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 ProbePos_Read(void) 
{
    return (uint8)((ProbePos_PS & ProbePos_MASK) >> ProbePos_SHIFT);
}


/*******************************************************************************
* Function Name: ProbePos_ReadDataReg
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
uint8 ProbePos_ReadDataReg(void) 
{
    return (uint8)((ProbePos_DR & ProbePos_MASK) >> ProbePos_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(ProbePos_INTSTAT) 

    /*******************************************************************************
    * Function Name: ProbePos_ClearInterrupt
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
    uint8 ProbePos_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(ProbePos_INTSTAT & ProbePos_MASK);
		ProbePos_INTSTAT = maskedStatus;
        return maskedStatus >> ProbePos_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
