/*******************************************************************************
* File Name: Q1.h  
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

#if !defined(CY_PINS_Q1_H) /* Pins Q1_H */
#define CY_PINS_Q1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Q1_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Q1_Write(uint8 value) ;
void    Q1_SetDriveMode(uint8 mode) ;
uint8   Q1_ReadDataReg(void) ;
uint8   Q1_Read(void) ;
uint8   Q1_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Q1_DRIVE_MODE_BITS        (3)
#define Q1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Q1_DRIVE_MODE_BITS))

#define Q1_DM_ALG_HIZ         (0x00u)
#define Q1_DM_DIG_HIZ         (0x01u)
#define Q1_DM_RES_UP          (0x02u)
#define Q1_DM_RES_DWN         (0x03u)
#define Q1_DM_OD_LO           (0x04u)
#define Q1_DM_OD_HI           (0x05u)
#define Q1_DM_STRONG          (0x06u)
#define Q1_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Q1_MASK               Q1__MASK
#define Q1_SHIFT              Q1__SHIFT
#define Q1_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Q1_PS                     (* (reg32 *) Q1__PS)
/* Port Configuration */
#define Q1_PC                     (* (reg32 *) Q1__PC)
/* Data Register */
#define Q1_DR                     (* (reg32 *) Q1__DR)
/* Input Buffer Disable Override */
#define Q1_INP_DIS                (* (reg32 *) Q1__PC2)


#if defined(Q1__INTSTAT)  /* Interrupt Registers */

    #define Q1_INTSTAT                (* (reg32 *) Q1__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Q1_DRIVE_MODE_SHIFT       (0x00u)
#define Q1_DRIVE_MODE_MASK        (0x07u << Q1_DRIVE_MODE_SHIFT)


#endif /* End Pins Q1_H */


/* [] END OF FILE */
