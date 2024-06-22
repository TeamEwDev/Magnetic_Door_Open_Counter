/*
 * flash_if.h
 *
 *  Created on: Jan 13, 2024
 *      Author: EW Dev
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)ADDR_FLASH_SECTOR_0 + 0x800) /* Base @ of Sector 1, 2 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)ADDR_FLASH_SECTOR_1 + 0x800) /* Base @ of Sector 2, 2 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)ADDR_FLASH_SECTOR_2 + 0x800) /* Base @ of Sector 3, 2 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)ADDR_FLASH_SECTOR_3 + 0x800) /* Base @ of Sector 4, 2 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)ADDR_FLASH_SECTOR_4 + 0x800) /* Base @ of Sector 5, 2 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)ADDR_FLASH_SECTOR_5 + 0x800) /* Base @ of Sector 6, 2 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)ADDR_FLASH_SECTOR_6 + 0x800) /* Base @ of Sector 7, 1228 Kbyte */

/* Error code */
enum
{
    FLASHIF_OK = 0,
    FLASHIF_ERASEKO,
    FLASHIF_WRITINGCTRL_ERROR,
    FLASHIF_WRITING_ERROR
};

enum
{
    FLASHIF_PROTECTION_NONE         = 0,
    FLASHIF_PROTECTION_PCROPENABLED = 0x1,
    FLASHIF_PROTECTION_WRPENABLED   = 0x2,
    FLASHIF_PROTECTION_RDPENABLED   = 0x4,
};

/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        FLASH_BASE + FLASH_SIZE - 256UL
/* Define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */
#define APPLICATION_ADDRESS   0x08004000


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void              FLASH_If_Init(void);
uint32_t          FLASH_If_Erase(uint32_t StartSector);
uint32_t          FLASH_If_Write(uint32_t FlashAddress, uint32_t *Data, uint32_t DataLength);
uint16_t          FLASH_If_GetWriteProtectionStatus(void);
HAL_StatusTypeDef FLASH_If_WriteProtectionConfig(uint32_t modifier);

#endif  /* __FLASH_IF_H */
