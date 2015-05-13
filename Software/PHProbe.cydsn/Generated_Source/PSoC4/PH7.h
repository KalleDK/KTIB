/*******************************************************************************
* File Name: PH7.h  
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

#if !defined(CY_PINS_PH7_H) /* Pins PH7_H */
#define CY_PINS_PH7_H

#include "cytypes.h"
#include "cyfitter.h"
#include "PH7_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    PH7_Write(uint8 value) ;
void    PH7_SetDriveMode(uint8 mode) ;
uint8   PH7_ReadDataReg(void) ;
uint8   PH7_Read(void) ;
uint8   PH7_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define PH7_DRIVE_MODE_BITS        (3)
#define PH7_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - PH7_DRIVE_MODE_BITS))

#define PH7_DM_ALG_HIZ         (0x00u)
#define PH7_DM_DIG_HIZ         (0x01u)
#define PH7_DM_RES_UP          (0x02u)
#define PH7_DM_RES_DWN         (0x03u)
#define PH7_DM_OD_LO           (0x04u)
#define PH7_DM_OD_HI           (0x05u)
#define PH7_DM_STRONG          (0x06u)
#define PH7_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define PH7_MASK               PH7__MASK
#define PH7_SHIFT              PH7__SHIFT
#define PH7_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PH7_PS                     (* (reg32 *) PH7__PS)
/* Port Configuration */
#define PH7_PC                     (* (reg32 *) PH7__PC)
/* Data Register */
#define PH7_DR                     (* (reg32 *) PH7__DR)
/* Input Buffer Disable Override */
#define PH7_INP_DIS                (* (reg32 *) PH7__PC2)


#if defined(PH7__INTSTAT)  /* Interrupt Registers */

    #define PH7_INTSTAT                (* (reg32 *) PH7__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define PH7_DRIVE_MODE_SHIFT       (0x00u)
#define PH7_DRIVE_MODE_MASK        (0x07u << PH7_DRIVE_MODE_SHIFT)


#endif /* End Pins PH7_H */


/* [] END OF FILE */
