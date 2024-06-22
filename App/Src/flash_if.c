/*
 * flash_if.c
 *
 *  Created on: Jan 13, 2024
 *      Author: EW Dev
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t GetSector(uint32_t Address);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
    HAL_FLASH_Unlock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartSector: start of user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
uint32_t FLASH_If_Erase(uint32_t address)
{
    uint32_t SectorError;
    FLASH_EraseInitTypeDef pEraseInit;

    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_If_Init();

    /* Get the sector where start the user flash area */

    pEraseInit.TypeErase = TYPEERASE_PAGES;
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.Page = GetSector(address);;
    pEraseInit.NbPages = 1;

    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)
    {
        /* Error occurred while page erase */
        return (1);
    }

    return (0);
}

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t FlashAddress, uint32_t *Data, uint32_t DataLength)
{
    uint32_t i = 0;

    for (i = 0; (i < DataLength) && (FlashAddress <= (USER_FLASH_END_ADDRESS - 4)); i++)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
           be done by word */
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FAST, FlashAddress, (uint32_t *)(Data + i)) == HAL_OK)
        {
            /* Check the written value */
            if (*(uint32_t *)FlashAddress != *(uint32_t *)(Data + i))
            {
                /* Flash content doesn't match SRAM content */
                return (FLASHIF_WRITINGCTRL_ERROR);
            }
            /* Increment FLASH destination address */
            FlashAddress += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            return (FLASHIF_WRITING_ERROR);
        }
    }

    return (FLASHIF_OK);
}

/**
  * @brief  Gets the sector of a given address
  * @param  Address: Flash address
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = 1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = 2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = 3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = 4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = 5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = 6;
    }
    return sector;
}
/**
  * @}
  */
