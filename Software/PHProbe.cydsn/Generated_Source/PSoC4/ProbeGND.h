/*******************************************************************************
* File Name: ProbeGND.h  
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

#if !defined(CY_PINS_ProbeGND_H) /* Pins ProbeGND_H */
#define CY_PINS_ProbeGND_H

#include "cytypes.h"
#include "cyfitter.h"
#include "ProbeGND_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    ProbeGND_Write(uint8 value) ;
void    ProbeGND_SetDriveMode(uint8 mode) ;
uint8   ProbeGND_ReadDataReg(void) ;
uint8   ProbeGND_Read(void) ;
uint8   ProbeGND_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define ProbeGND_DRIVE_MODE_BITS        (3)
#define ProbeGND_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - ProbeGND_DRIVE_MODE_BITS))

#define ProbeGND_DM_ALG_HIZ         (0x00u)
#define ProbeGND_DM_DIG_HIZ         (0x01u)
#define ProbeGND_DM_RES_UP          (0x02u)
#define ProbeGND_DM_RES_DWN         (0x03u)
#define ProbeGND_DM_OD_LO           (0x04u)
#define ProbeGND_DM_OD_HI           (0x05u)
#define ProbeGND_DM_STRONG          (0x06u)
#define ProbeGND_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define ProbeGND_MASK               ProbeGND__MASK
#define ProbeGND_SHIFT              ProbeGND__SHIFT
#define ProbeGND_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ProbeGND_PS                     (* (reg32 *) ProbeGND__PS)
/* Port Configuration */
#define ProbeGND_PC                     (* (reg32 *) ProbeGND__PC)
/* Data Register */
#define ProbeGND_DR                     (* (reg32 *) ProbeGND__DR)
/* Input Buffer Disable Override */
#define ProbeGND_INP_DIS                (* (reg32 *) ProbeGND__PC2)


#if defined(ProbeGND__INTSTAT)  /* Interrupt Registers */

    #define ProbeGND_INTSTAT                (* (reg32 *) ProbeGND__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define ProbeGND_DRIVE_MODE_SHIFT       (0x00u)
#define ProbeGND_DRIVE_MODE_MASK        (0x07u << ProbeGND_DRIVE_MODE_SHIFT)


#endif /* End Pins ProbeGND_H */


/* [] END OF FILE */
