/*******************************************************************************
* File Name: ProbeSignal.h  
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

#if !defined(CY_PINS_ProbeSignal_H) /* Pins ProbeSignal_H */
#define CY_PINS_ProbeSignal_H

#include "cytypes.h"
#include "cyfitter.h"
#include "ProbeSignal_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    ProbeSignal_Write(uint8 value) ;
void    ProbeSignal_SetDriveMode(uint8 mode) ;
uint8   ProbeSignal_ReadDataReg(void) ;
uint8   ProbeSignal_Read(void) ;
uint8   ProbeSignal_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define ProbeSignal_DRIVE_MODE_BITS        (3)
#define ProbeSignal_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - ProbeSignal_DRIVE_MODE_BITS))

#define ProbeSignal_DM_ALG_HIZ         (0x00u)
#define ProbeSignal_DM_DIG_HIZ         (0x01u)
#define ProbeSignal_DM_RES_UP          (0x02u)
#define ProbeSignal_DM_RES_DWN         (0x03u)
#define ProbeSignal_DM_OD_LO           (0x04u)
#define ProbeSignal_DM_OD_HI           (0x05u)
#define ProbeSignal_DM_STRONG          (0x06u)
#define ProbeSignal_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define ProbeSignal_MASK               ProbeSignal__MASK
#define ProbeSignal_SHIFT              ProbeSignal__SHIFT
#define ProbeSignal_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ProbeSignal_PS                     (* (reg32 *) ProbeSignal__PS)
/* Port Configuration */
#define ProbeSignal_PC                     (* (reg32 *) ProbeSignal__PC)
/* Data Register */
#define ProbeSignal_DR                     (* (reg32 *) ProbeSignal__DR)
/* Input Buffer Disable Override */
#define ProbeSignal_INP_DIS                (* (reg32 *) ProbeSignal__PC2)


#if defined(ProbeSignal__INTSTAT)  /* Interrupt Registers */

    #define ProbeSignal_INTSTAT                (* (reg32 *) ProbeSignal__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define ProbeSignal_DRIVE_MODE_SHIFT       (0x00u)
#define ProbeSignal_DRIVE_MODE_MASK        (0x07u << ProbeSignal_DRIVE_MODE_SHIFT)


#endif /* End Pins ProbeSignal_H */


/* [] END OF FILE */
