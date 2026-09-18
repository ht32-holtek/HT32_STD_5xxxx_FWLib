/*********************************************************************************************************//**
 * @file    eeprom_emulation.c
 * @version $Rev:: 1600         $
 * @date    $Date:: 2026-09-07 #$
 * @brief   The source file of EEPROM emulation APIs.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "eeprom_emulation.h"

/** @addtogroup EEPROM_Emulation_Examples EEPROM_Emulation
  * @{
  */

/** @addtogroup EEPROM_Emulation
  * @{
  */


/* Settings ------------------------------------------------------------------------------------------------*/
#define EEPROM_EMU_RAM_TEST_MODE       (0)

/* Private constants ---------------------------------------------------------------------------------------*/
#define EEPROM_EMU_ERASED             (0xFFFF)
#define EEPROM_EMU_TRANSFER           (0xCCCC)
#define EEPROM_EMU_ACTIVE             (0x0000)

#define EEPROM_EMU_READ_PAGE          (0)
#define EEPROM_EMU_WRITE_PAGE         (1)

#if (EEPROM_EMU_LENGTH <= 0)
  #error "EEPROM_EMU_LENGTH is not correct (must >= 1)!"
#endif

#if (EEPROM_EMU_LENGTH > (LIBCFG_FLASH_PAGESIZE / 4 - 2))
  #error "EEPROM_EMU_LENGTH is not correct (must < (Page Size / 4 - 2) )!"
#endif

#if (EEPROM_EMU_START_NUM <= 1)
  #error "EEPROM_EMU_START_NUM is not correct (must >= 2)!"
#endif

#if (EEPROM_EMU_START_NUM > (LIBCFG_FLASH_SIZE / LIBCFG_FLASH_PAGESIZE))
  #error "EEPROM_EMU_START_NUM is not correct (must < (Flash Size / Page Size) )!"
#endif

#if (EEPROM_EMU_RAM_TEST_MODE == 1)
  // TODO
  //#define EEPROM_FLASH_PROGRAM   _RAM_ProgramWordData
  //#define EEPROM_FLASH_ERASE     _RAM_ErasePage
  //#define EEPROM_FLASH_Read16    _RAM_Read16
  //#define EEPROM_FLASH_READ32    _RAM_Read32
#else
  #define EEPROM_FLASH_PROGRAM   FLASH_ProgramWordData
  #define EEPROM_FLASH_ERASE     FLASH_ErasePage
  #define EEPROM_FLASH_READ16    rhw
  #define EEPROM_FLASH_READ32    rw
#endif

/* Global variables ----------------------------------------------------------------------------------------*/
#if (EEPROM_EMU_SEQUENTIAL_ADDR == 0)
extern u16 EEPROM_VarAddrTab[EEPROM_EMU_LENGTH];
#endif

/* Private variables ---------------------------------------------------------------------------------------*/
u32 guActiveAddress = 0xFFFFFFFF;
s8 gActivePage = -1;

#if (EEPROM_EMU_RAM_TEST_MODE == 1)
//TODO
#endif

/* Private function prototypes -----------------------------------------------------------------------------*/
static EEPROM_EMU_State _EEPROM_Format(void);
static EEPROM_EMU_State _EEPROM_FindActivePage(u8 Operation, s8 *SelectedPage);
static EEPROM_EMU_State _EEPROM_FindActiveAddr(void);
static EEPROM_EMU_State _EEPROM_WriteActivePage(u16 DataAddr, u16 Data);
static EEPROM_EMU_State _EEPROM_TransferPage(void);
static EEPROM_EMU_State _EEPROM_IsAddrInRange(u32 uActiveAddress);
static EEPROM_EMU_State _EEPROM_ErasePageHelper(u32 PageAddr);
static EEPROM_EMU_State _EEPROM_SetPageStateHelper(u32 PageAddr, u32 PageState);

/* Private macro -------------------------------------------------------------------------------------------*/
#if (EEPROM_EMU_SEQUENTIAL_ADDR == 0)
#define ADDR_MAP(i)    (EEPROM_VarAddrTab[i])
#else
#define ADDR_MAP(i)    (i)
#endif

#if defined(LIBCFG_FMC_CACHE)
  #define CACHE_PROCESS() {FLASH_CacheCmd(DISABLE); FLASH_CacheCmd(ENABLE);}
#else
  #define CACHE_PROCESS(...)
#endif

#define IS_PAGEADDR_OUTOFRANGE(Addr) ((Addr != EEPROM_EMU_PAGE0_BASE_ADDR) && (Addr != EEPROM_EMU_PAGE1_BASE_ADDR))

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  EEPROM init
  * @retval EepromStatus
  ***********************************************************************************************************/
EEPROM_EMU_State EEPROM_Init(void)
{
  EEPROM_EMU_State EepromStatus;
  u16 Page0Status = EEPROM_FLASH_READ16(EEPROM_EMU_PAGE0_BASE_ADDR);
  u16 Page1Status = EEPROM_FLASH_READ16(EEPROM_EMU_PAGE1_BASE_ADDR);

  gActivePage = -1;
  guActiveAddress = 0xFFFFFFFF;

  // Break Case 1 - Page 0
  if (Page0Status == EEPROM_EMU_ACTIVE && Page1Status == EEPROM_EMU_TRANSFER)
  {
    EepromStatus = _EEPROM_TransferPage();
    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
      return EepromStatus;
    }
  }
  // Break Case 2 - Page 0
  else if (Page0Status == EEPROM_EMU_ERASED && Page1Status == EEPROM_EMU_TRANSFER)
  {
    EepromStatus = _EEPROM_SetPageStateHelper(EEPROM_EMU_PAGE1_BASE_ADDR, EEPROM_EMU_ACTIVE);
    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
      return EepromStatus;
    }
  }
  // Break Case 1 - Page 1
  else if (Page0Status == EEPROM_EMU_TRANSFER && Page1Status == EEPROM_EMU_ACTIVE)
  {
    EepromStatus = _EEPROM_TransferPage();
    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
      return EepromStatus;
    }
  }
  // Break Case 2 - Page 1
  else if (Page0Status == EEPROM_EMU_TRANSFER && Page1Status == EEPROM_EMU_ERASED)
  {
    EepromStatus = _EEPROM_SetPageStateHelper(EEPROM_EMU_PAGE0_BASE_ADDR, EEPROM_EMU_ACTIVE);
    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
      return EepromStatus;
    }
  }
  else if (Page0Status == EEPROM_EMU_ERASED && Page1Status == EEPROM_EMU_ERASED)
  {
    EepromStatus = _EEPROM_Format();
    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
      return EepromStatus;
    }
  }
  else if (Page0Status == EEPROM_EMU_ACTIVE && Page1Status == EEPROM_EMU_ACTIVE)
  {
    return EEPROM_EMU_FLASH_ERROR;
  }
  else if (Page0Status == EEPROM_EMU_TRANSFER && Page1Status == EEPROM_EMU_TRANSFER)
  {
    return EEPROM_EMU_FLASH_ERROR;
  }
  else if (Page0Status == EEPROM_EMU_ACTIVE && Page1Status == EEPROM_EMU_ERASED)
  {
    // Normal operating conditions.
  }
  else if (Page0Status == EEPROM_EMU_ERASED && Page1Status == EEPROM_EMU_ACTIVE)
  {
    // Normal operating conditions.
  }
  else
  {
    /*
      Any unknown page status indicates a Flash error. Depending on the data retention and writeability 
      requirements, the system may either preserve the existing data and report an error, or re-initialize 
      the Flash.
     */
    #if 1
    // Return Flash error, preserve the existing data.
    return EEPROM_EMU_FLASH_ERROR;
    #else
    // Re-initialize the Flash when flash error.
    EepromStatus = _EEPROM_Format();
    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
      return EepromStatus;
    }
    #endif
  }

  EepromStatus = _EEPROM_FindActivePage(EEPROM_EMU_WRITE_PAGE, &gActivePage);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus;
  }

  EepromStatus = _EEPROM_FindActiveAddr();

  if (EepromStatus == EEPROM_EMU_PAGE_FULL)
  {
    // Break Case 3
    EepromStatus = _EEPROM_TransferPage();

    if (EepromStatus == EEPROM_EMU_OPERATION_OK)
    {
      EepromStatus = _EEPROM_FindActiveAddr();
    }
    else
    {
      return EepromStatus;
    }
  }

  return EepromStatus;
}

/*********************************************************************************************************//**
  * @brief  Read data from EEPROM
  * @para   DataAddr: data address
  * @para   Data: pointer of data buffer
  * @retval EepromStatus
  ***********************************************************************************************************/
EEPROM_EMU_State EEPROM_Read(u16 DataAddr, u16 *Data)
{
  EEPROM_EMU_State EepromStatus;
  s8 ActivePage;
  u32 StopAddr, CompareAddr;
  u32 DataAndAddr;

  if (DataAddr == 0xFFFF)
  {
    return EEPROM_EMU_INVALID_PARA;
  }

  if (Data == NULL)
  {
    return EEPROM_EMU_INVALID_PARA;
  }

  /* get active page                                                                                        */
  EepromStatus = _EEPROM_FindActivePage(EEPROM_EMU_READ_PAGE, &ActivePage);

  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus;
  }

  /* search for latest data                                                                                 */
  StopAddr = EEPROM_EMU_START_ADDR + ((u32)ActivePage * EEPROM_EMU_PAGE_SIZE + 4);           // skip page status field
  CompareAddr = EEPROM_EMU_START_ADDR + ((u32)(1 + ActivePage) * EEPROM_EMU_PAGE_SIZE - 4);  // start from page end
  #if 0
  if ((guActiveAddress >= StopAddr) && 
      (guActiveAddress < EEPROM_EMU_START_ADDR + ((u32)(1 + ActivePage) * EEPROM_EMU_PAGE_SIZE)))
  {
    CompareAddr = guActiveAddress - 4;
  }
  else
  {
    CompareAddr = EEPROM_EMU_START_ADDR + ((u32)(1 + ActivePage) * EEPROM_EMU_PAGE_SIZE - 4);
  }
  #endif

  while (CompareAddr >= StopAddr)
  {
    DataAndAddr = EEPROM_FLASH_READ32(CompareAddr);
    if ((u16)(DataAndAddr >> 16) == DataAddr)
    {
      *Data = (u16)(DataAndAddr & 0xFFFF);

      return EEPROM_EMU_OPERATION_OK;
    }
    else
    {
      CompareAddr -= 4;
    }
  }

  return EEPROM_EMU_DATA_NOT_FOUND;
}

/*********************************************************************************************************//**
  * @brief Write data to EEPROM
  * @retval EepromStatus
  ***********************************************************************************************************/
EEPROM_EMU_State EEPROM_Write(u16 DataAddr, u16 Data)
{
  EEPROM_EMU_State EepromStatus;

  if (DataAddr == 0xFFFF)
  {
    return EEPROM_EMU_INVALID_PARA;
  }

  EepromStatus = _EEPROM_WriteActivePage(DataAddr, Data);

  /* check page full                                                                                        */
  if (EepromStatus == EEPROM_EMU_PAGE_FULL)
  {
    //
    // !!! TODO Break Case 3 Test
    //
    EepromStatus = _EEPROM_TransferPage();
  }

  return EepromStatus;
}

/* Private functions ---------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Erase page0, page1 and set page0 active
  * @retval EEPROM_EMU_ADDR_OUTOFRANGE, EEPROM_EMU_FLASH_ERROR, EEPROM_EMU_OPERATION_OK
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_Format(void)
{
  EEPROM_EMU_State EepromStatus;

  /* erase page0                                                                                            */
  EepromStatus = _EEPROM_ErasePageHelper(EEPROM_EMU_PAGE0_BASE_ADDR);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus; // EEPROM_EMU_ADDR_OUTOFRANGE, EEPROM_EMU_FLASH_ERROR
  }

  /* erase page1                                                                                            */
  EepromStatus = _EEPROM_ErasePageHelper(EEPROM_EMU_PAGE1_BASE_ADDR);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus; // EEPROM_EMU_ADDR_OUTOFRANGE, EEPROM_EMU_FLASH_ERROR
  }

  /* set page0 active                                                                                       */
  EepromStatus = _EEPROM_SetPageStateHelper(EEPROM_EMU_PAGE0_BASE_ADDR, EEPROM_EMU_ACTIVE);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus;
  }

  return EEPROM_EMU_OPERATION_OK;
}

/*********************************************************************************************************//**
  * @brief Find active page
  * @retval EEPROM_EMU_INVALID_PARA, EEPROM_EMU_OPERATION_OK, EEPROM_EMU_NO_ACTIVE_PAGE, EEPROM_EMU_FLASH_ERROR
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_FindActivePage(u8 Operation, s8 *SelectedPage)
{
  u16 Page0Status;
  u16 Page1Status;

  if (SelectedPage == NULL)
  {
    return EEPROM_EMU_INVALID_PARA;
  }

  *SelectedPage = -1;
  Page0Status = EEPROM_FLASH_READ16(EEPROM_EMU_PAGE0_BASE_ADDR);
  Page1Status = EEPROM_FLASH_READ16(EEPROM_EMU_PAGE1_BASE_ADDR);

  switch (Operation)
  {
    case EEPROM_EMU_READ_PAGE:

      /*
       * Page 0 is the valid data page in both cases:
       *
       *   Page 0 = ACTIVE, Page 1 = ERASED
       *   Page 0 = ACTIVE, Page 1 = TRANSFER
       */
      if ((Page0Status == EEPROM_EMU_ACTIVE) &&
          ((Page1Status == EEPROM_EMU_ERASED) ||
           (Page1Status == EEPROM_EMU_TRANSFER)))
      {
        *SelectedPage = EEPROM_EMU_PAGE0_ACTIVE;
        return EEPROM_EMU_OPERATION_OK;
      }

      /*
       * Page 1 is the valid data page in both cases:
       *
       *   Page 0 = ERASED,   Page 1 = ACTIVE
       *   Page 0 = TRANSFER, Page 1 = ACTIVE
       */
      if ((Page1Status == EEPROM_EMU_ACTIVE) &&
          ((Page0Status == EEPROM_EMU_ERASED) ||
           (Page0Status == EEPROM_EMU_TRANSFER)))
      {
        *SelectedPage = EEPROM_EMU_PAGE1_ACTIVE;
        return EEPROM_EMU_OPERATION_OK;
      }

      /*
       * No page currently contains valid data.
       */
      if ((Page0Status == EEPROM_EMU_ERASED) &&
          (Page1Status == EEPROM_EMU_ERASED))
      {
        return EEPROM_EMU_NO_ACTIVE_PAGE;
      }

      /*
       * An incomplete recovery state without an ACTIVE page.
       * EEPROM_Init() is responsible for handling these states.
       */
      if (((Page0Status == EEPROM_EMU_TRANSFER) &&
           (Page1Status == EEPROM_EMU_ERASED)) ||
          ((Page0Status == EEPROM_EMU_ERASED) &&
           (Page1Status == EEPROM_EMU_TRANSFER)))
      {
        return EEPROM_EMU_NO_ACTIVE_PAGE;
      }

      /*
       * ACTIVE + ACTIVE, TRANSFER + TRANSFER,
       * or any unknown page header.
       */
      return EEPROM_EMU_FLASH_ERROR;


    case EEPROM_EMU_WRITE_PAGE:

      /*
       * This function is called for normal writing after
       * EEPROM_Init() has completed recovery.
       *
       * Only the following states are valid:
       *
       *   Page 0 = ACTIVE, Page 1 = ERASED
       *   Page 0 = ERASED, Page 1 = ACTIVE
       */
      if ((Page0Status == EEPROM_EMU_ACTIVE) &&
          (Page1Status == EEPROM_EMU_ERASED))
      {
        *SelectedPage = EEPROM_EMU_PAGE0_ACTIVE;
        return EEPROM_EMU_OPERATION_OK;
      }

      if ((Page0Status == EEPROM_EMU_ERASED) &&
          (Page1Status == EEPROM_EMU_ACTIVE))
      {
        *SelectedPage = EEPROM_EMU_PAGE1_ACTIVE;
        return EEPROM_EMU_OPERATION_OK;
      }

      /*
       * ACTIVE + TRANSFER and TRANSFER + ACTIVE must have
       * already been handled by EEPROM_Init().
       *
       * Any remaining state is invalid for normal writing.
       */
      return EEPROM_EMU_FLASH_ERROR;


    default:

      /*
       * Never select a page for an unknown operation.
       */
      return EEPROM_EMU_FLASH_ERROR;
  }
}

/*********************************************************************************************************//**
  * @brief Find active write address
  * @retval EEPROM_EMU_NO_ACTIVE_PAGE, EEPROM_EMU_OPERATION_OK, EEPROM_EMU_PAGE_FULL
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_FindActiveAddr(void)
{
  u32 StopAddr, CompareAddr;

  if (gActivePage == -1)
  {
    return EEPROM_EMU_NO_ACTIVE_PAGE;
  }

  guActiveAddress = 0xFFFFFFFF;

  /* search for lst empty location                                                                          */
  StopAddr = EEPROM_EMU_START_ADDR + ((u32)(1 + gActivePage) * EEPROM_EMU_PAGE_SIZE);   // page end address
  CompareAddr = EEPROM_EMU_START_ADDR + ((u32)gActivePage * EEPROM_EMU_PAGE_SIZE + 4);  // start from page head

  while (CompareAddr < StopAddr)
  {
    if (EEPROM_FLASH_READ32(CompareAddr) == 0xFFFFFFFF)
    {
      guActiveAddress = CompareAddr;

      return EEPROM_EMU_OPERATION_OK;
    }
    else
    {
      CompareAddr += 4;
    }
  }

  return EEPROM_EMU_PAGE_FULL;
}

/*********************************************************************************************************//**
  * @brief Write data into active page
  * @retval EEPROM_EMU_NO_ACTIVE_PAGE, EEPROM_EMU_PAGE_FULL, EEPROM_EMU_ADDR_OUTOFRANGE,
  *         EEPROM_EMU_FLASH_ERROR, EEPROM_EMU_OPERATION_OK
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_WriteActivePage(u16 DataAddr, u16 Data)
{
  FLASH_State FlashStatus;
  u32 StopAddr;

  if (gActivePage == -1)
  {
    return EEPROM_EMU_NO_ACTIVE_PAGE;
  }

  if (_EEPROM_IsAddrInRange(guActiveAddress) != EEPROM_EMU_OPERATION_OK)
  {
    return EEPROM_EMU_ADDR_OUTOFRANGE;
  }

  FlashStatus = EEPROM_FLASH_PROGRAM(guActiveAddress, (((u32)DataAddr << 16) | (u32)Data));
  CACHE_PROCESS();

  if (FlashStatus != FLASH_COMPLETE)
  {
    return EEPROM_EMU_FLASH_ERROR;
  }

  #if 1 // Do the verify after program. Turn off for speed considerations.
  if (EEPROM_FLASH_READ32(guActiveAddress) != ((((u32)DataAddr << 16) | (u32)Data)))
  {
    return EEPROM_EMU_FLASH_ERROR;
  }
  #endif

  StopAddr = EEPROM_EMU_START_ADDR + ((u32)(1 + gActivePage) * EEPROM_EMU_PAGE_SIZE);   // page end address
  if ((guActiveAddress + 4) == StopAddr)
  {
    return EEPROM_EMU_PAGE_FULL;
  }

  guActiveAddress += 4;

  return EEPROM_EMU_OPERATION_OK;
}

/*********************************************************************************************************//**
  * @brief Transfer data from full page to new page
  * @retval EepromStatus
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_TransferPage(void)
{
  EEPROM_EMU_State EepromStatus;
  s8 ActivePage;
  u32 NewPage, OldPage, i;
  u16 VarData;

  /* get NewPage & OldPage status                                                                           */
  EepromStatus = _EEPROM_FindActivePage(EEPROM_EMU_READ_PAGE, &ActivePage);

  if (EepromStatus == EEPROM_EMU_OPERATION_OK)
  {
    if (ActivePage == EEPROM_EMU_PAGE1_ACTIVE)
    {
      NewPage = EEPROM_EMU_PAGE0_BASE_ADDR;
      OldPage = EEPROM_EMU_PAGE1_BASE_ADDR;
      gActivePage = EEPROM_EMU_PAGE0_ACTIVE;
    }
    else if (ActivePage == EEPROM_EMU_PAGE0_ACTIVE)
    {
      NewPage = EEPROM_EMU_PAGE1_BASE_ADDR;
      OldPage = EEPROM_EMU_PAGE0_BASE_ADDR;
      gActivePage = EEPROM_EMU_PAGE1_ACTIVE;
    }
  }
  else
  {
    return EepromStatus;
  }

  /* set NewPage state as EEPROM_EMU_TRANSFER                                                               */
  EepromStatus = _EEPROM_ErasePageHelper(NewPage);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus;
  }
  EepromStatus = _EEPROM_SetPageStateHelper(NewPage, EEPROM_EMU_TRANSFER);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus;
  }

  //
  // !!! TODO Break Case 1 Test
  //

  /* transfer data                                                                                          */
  guActiveAddress = EEPROM_EMU_START_ADDR + ((u32)gActivePage * EEPROM_EMU_PAGE_SIZE + 4);  // start from page head
  for (i = 0; i < EEPROM_EMU_LENGTH; i++)
  {
    /* write other variables                                                                                */
    EepromStatus = EEPROM_Read(ADDR_MAP(i), &VarData);
    if (EepromStatus == EEPROM_EMU_DATA_NOT_FOUND)
    {
      // Variable has never been written. It is normal to skip it.
      continue;
    }

    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
       // Any unexpected read error must abort the transfer. Do not erase the old page.
      return EepromStatus;
    }

    EepromStatus = _EEPROM_WriteActivePage(ADDR_MAP(i), VarData);
    if (EepromStatus != EEPROM_EMU_OPERATION_OK)
    {
      return EepromStatus; 
    }
  }

  /* erase OldPage                                                                                          */
  EepromStatus = _EEPROM_ErasePageHelper(OldPage);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus;
  }

  //
  // !!! TODO Break Case 2 Test
  //

  /* set NewPage state as EEPROM_EMU_ACTIVE                                                                 */
  EepromStatus = _EEPROM_SetPageStateHelper(NewPage, EEPROM_EMU_ACTIVE);
  if (EepromStatus != EEPROM_EMU_OPERATION_OK)
  {
    return EepromStatus;
  }

  return EEPROM_EMU_OPERATION_OK;
}

/*********************************************************************************************************//**
  * @brief Transfer data from full page to new page
  * @retval EEPROM_EMU_OPERATION_OK, EEPROM_EMU_NO_ACTIVE_PAGE, EEPROM_EMU_FLASH_ERROR
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_IsAddrInRange(u32 uActiveAddress)
{
  u32 PageStartAddr;
  u32 PageStopAddr;

  PageStartAddr = EEPROM_EMU_START_ADDR + ((u32)(gActivePage) * EEPROM_EMU_PAGE_SIZE); 
  PageStopAddr  = PageStartAddr + EEPROM_EMU_PAGE_SIZE;

  // First 4 bytes is not for save data
  if ((uActiveAddress < (PageStartAddr + 4)) || (uActiveAddress >= PageStopAddr))
  {
    return EEPROM_EMU_ADDR_OUTOFRANGE;
  }
  
  return EEPROM_EMU_OPERATION_OK;
}

/*********************************************************************************************************//**
  * @brief Erase Page
  * @retval status
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_ErasePageHelper(u32 PageAddr)
{
  if (IS_PAGEADDR_OUTOFRANGE(PageAddr))
  {
    return EEPROM_EMU_ADDR_OUTOFRANGE;
  }

  FLASH_State status = EEPROM_FLASH_ERASE(PageAddr);
  CACHE_PROCESS();

  if (status != FLASH_COMPLETE)
  {
    return EEPROM_EMU_FLASH_ERROR;
  }
  
  #if 0
  // You may do the blank check of the erased page for reliability.
  #endif

  return EEPROM_EMU_OPERATION_OK;
}

/*********************************************************************************************************//**
  * @brief Set the page state
  * @retval status
  ***********************************************************************************************************/
static EEPROM_EMU_State _EEPROM_SetPageStateHelper(u32 PageAddr, u32 PageState)
{
  FLASH_State status;

  if (IS_PAGEADDR_OUTOFRANGE(PageAddr))
  {
    return EEPROM_EMU_ADDR_OUTOFRANGE;
  }

  status = EEPROM_FLASH_PROGRAM(PageAddr, PageState);
  CACHE_PROCESS();

  if (status != FLASH_COMPLETE)
  {
    return EEPROM_EMU_FLASH_ERROR;
  }

  if (EEPROM_FLASH_READ32(PageAddr) != PageState)
  {
    return EEPROM_EMU_FLASH_ERROR;
  }

  return EEPROM_EMU_OPERATION_OK;
}


/**
  * @}
  */

/**
  * @}
  */
