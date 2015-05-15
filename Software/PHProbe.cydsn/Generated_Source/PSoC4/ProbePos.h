/*******************************************************************************
* File Name: ProbePos.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_ProbePos_H) /* Pins ProbePos_H */
#define CY_PINS_ProbePos_H

#include "cytypes.h"
#include "cyfitter.h"
#include "ProbePos_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    ProbePos_Write(uint8 value) ;
void    ProbePos_SetDriveMode(uint8 mode) ;
uint8   ProbePos_ReadDataReg(void) ;
uint8   ProbePos_Read(void) ;
uint8   ProbePos_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define ProbePos_DRIVE_MODE_BITS        (3)
#define ProbePos_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - ProbePos_DRIVE_MODE_BITS))

#define ProbePos_DM_ALG_HIZ         (0x00u)
#define ProbePos_DM_DIG_HIZ         (0x01u)
#define ProbePos_DM_RES_UP          (0x02u)
#define ProbePos_DM_RES_DWN         (0x03u)
#define ProbePos_DM_OD_LO           (0x04u)
#define ProbePos_DM_OD_HI           (0x05u)
#define ProbePos_DM_STRONG          (0x06u)
#define ProbePos_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define ProbePos_MASK               ProbePos__MASK
#define ProbePos_SHIFT              ProbePos__SHIFT
#define ProbePos_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ProbePos_PS                     (* (reg32 *) ProbePos__PS)
/* Port Configuration */
#define ProbePos_PC                     (* (reg32 *) ProbePos__PC)
/* Data Register */
#define ProbePos_DR                     (* (reg32 *) ProbePos__DR)
/* Input Buffer Disable Override */
#define ProbePos_INP_DIS                (* (reg32 *) ProbePos__PC2)


#if defined(ProbePos__INTSTAT)  /* Interrupt Registers */

    #define ProbePos_INTSTAT                (* (reg32 *) ProbePos__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define ProbePos_DRIVE_MODE_SHIFT       (0x00u)
#define ProbePos_DRIVE_MODE_MASK        (0x07u << ProbePos_DRIVE_MODE_SHIFT)


#endif /* End Pins ProbePos_H */


/* [] END OF FILE */
