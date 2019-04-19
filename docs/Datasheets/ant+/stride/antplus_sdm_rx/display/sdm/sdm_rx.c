/////////////////////////////////////////////////////////////////////////////////////////
// THE FOLLOWING EXAMPLE CODE IS INTENDED FOR LIMITED CIRCULATION ONLY.
// 
// Please forward all questions regarding this code to ANT Technical Support.
// 
// Dynastream Innovations Inc.
// 228 River Avenue
// Cochrane, Alberta, Canada
// T4C 2C1
// 
// (P) (403) 932-9292
// (F) (403) 932-6521
// (TF) 1-866-932-9292
// (E) support@thisisant.com
// 
// www.thisisant.com
//
// Reference Design Disclaimer
//
// The references designs and codes provided may be used with ANT devices only and remain the copyrighted property of 
// Dynastream Innovations Inc. The reference designs and codes are being provided on an "as-is" basis and as an accommodation, 
// and therefore all warranties, representations, or guarantees of any kind (whether express, implied or statutory) including, 
// without limitation, warranties of merchantability, non-infringement,
// or fitness for a particular purpose, are specifically disclaimed.
//
// ©2008 Dynastream Innovations Inc. All Rights Reserved
// This software may not be reproduced by
// any means without express written approval of Dynastream
// Innovations Inc.
//
/////////////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "config.h"
#include "antinterface.h"
#include "antmessage.h"
#include "antdefines.h"
#include "sdm_rx.h"
#include "antplus.h"



///////////////////////////////////////////////////////////////////////////////
// ANT Network Setup definitions
///////////////////////////////////////////////////////////////////////////////
// ANT Channel
#define SDMRX_CHANNEL_TYPE            ((UCHAR) 0x00)     // Slave

// Channel ID
#define SDMRX_DEVICE_TYPE             ((UCHAR) 0x7C)

// Message Periods
#define SDMRX_MSG_PERIOD              ((USHORT) 0x1FC6)  // decimal 8134 (4.03Hz)


typedef union
{
   UCHAR ucAccumInit;
   
   struct
   {
      BOOL bDistanceInit   :1;
      BOOL bStridesInit    :1;
   } flags;      
} InitFlags;




/////////////////////////////////////////////////////////////////////////////
// STATIC ANT Configuration Block
/////////////////////////////////////////////////////////////////////////////

//// Page data fields
static SDMPage1_Data stPage1Data;
static SDMPage2to13_Data stPage2to13Data;
static CommonPage80_Data stPage80Data;
static CommonPage81_Data stPage81Data;
static CommonPage82_Data stPage82Data;
AccumulatedDistance stAccumulatedData;

static UCHAR ucAntChannel;
static USHORT usDeviceNumber;
static UCHAR ucTransType;



static InitFlags uFlags;
static USHORT usPrevDistance;                     
static UCHAR  ucPrevStride;

// Status masks
#define SDM_LOCATION(x)                            ((x >> 6) & 0x03)
#define SDM_BATTERY_STATUS(x)                      ((x >> 4) & 0x03)
#define SDM_UNIT_HEALTH(x)                         ((x >> 2) & 0x03)
#define SDM_USE_STATE(x)                           (x & 0x03)

// Page 82 bit masks
#define COMMON82_COARSE_BATT_VOLTAGE(x)               (x & 0x0F)
#define COMMON82_BATT_STATUS(x)                       ((x >> 4) & 0x07)
#define COMMON82_CUM_TIME_RESOLUTION(x)               ((x >> 7) & 0x01)


static BOOL HandleResponseEvents(UCHAR* pucBuffer_);
static BOOL HandleDataMessages(UCHAR* pucEventBuffer_, ANTPLUS_EVENT_RETURN* pstEventStruct_);


//----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
// Functon: HRMRX_Init
//
// Descrption:
//
// Initialize all state variables. 
// Params:
// N/A.
//
// returns: N/A. 
/////////////////////////////////////////////////////////////////////////////////////////
void SDMRX_Init()
{
   SDMPage1_Data* pstPage1Data = SDMRX_GetPage1();
   SDMPage2to13_Data* pstPage2to13Data = SDMRX_GetPage2to13();
   CommonPage80_Data* pstPage80Data = Common_GetPage80();
   CommonPage81_Data* pstPage81Data = Common_GetPage81();
   CommonPage82_Data* pstPage82Data = Common_GetPage82();

   // Intialize app variables
   pstPage1Data->ucLastTime = 0;
   pstPage1Data->ucLastTime200 = 0;
   pstPage1Data->ucDistance = 0;
   pstPage1Data->ucDistance16 = 0;
   pstPage1Data->ucInstSpeed = 0;
   pstPage1Data->ucInstSpeed256 = 0;
   pstPage1Data->ucStrideCount = 0;
   pstPage1Data->ucLatency = 0;
   pstPage2to13Data->ucCadence = 0;
   pstPage2to13Data->ucCadence16 = 0;
   pstPage2to13Data->ucModuleLocation = 0;
   pstPage2to13Data->ucBatteryStatus = 0;
   pstPage2to13Data->ucUnitHealth = 0;
   pstPage2to13Data->ucUseState = 0;
   pstPage80Data->ucHwVersion = 0;
   pstPage80Data->usManId = 0;
   pstPage80Data->usModelNumber = 0;
   pstPage81Data->ucSwVersion = 0;
   pstPage81Data->ulSerialNumber = 0;
   pstPage82Data->ulCumOperatingTime = 0;
   pstPage82Data->ucBattVoltage256 = 0;
   pstPage82Data->ucBattVoltage = 0;
   pstPage82Data->ucBattStatus = 0;
   pstPage82Data->ucCumOperatingTimeRes = 0;
   
   
   stAccumulatedData.ulAccumDistance = 0;
   stAccumulatedData.ulAccumStride = 0;
   
   
   uFlags.flags.bDistanceInit = FALSE;
   uFlags.flags.bStridesInit = FALSE;
   usPrevDistance = 0;                     
   ucPrevStride = 0;                       
   
   
   
   
}


/////////////////////////////////////////////////////////////////////////////////////////
// Functon: SDMRX_Open
//
// Descrption:
// Opens SDM recieve channel. Once the channel has been properly initialize an SDMRX_EVENT_INIT_COMPLETE
// event will be generated via the callback. A positive response from this function does not
// indicate tha the channel successfully opened. 
//
// Params:
// usDeviceNumber_: Device number to pair to. 0 for wild-card.
//
// returns: TRUE if all configuration messages were correctly setup and queued. FALSE otherwise. 
/////////////////////////////////////////////////////////////////////////////////////////
BOOL SDMRX_Open(UCHAR ucAntChannel_, USHORT usSearchDeviceNumber_, UCHAR ucTransType_)
{
   SDMRX_Init();
   
   ucAntChannel = ucAntChannel_;
   usDeviceNumber = usSearchDeviceNumber_;
   ucTransType = ucTransType_;
     
   if (!ANT_AssignChannel(ucAntChannel,SDMRX_CHANNEL_TYPE,ANTPLUS_NETWORK_NUMBER ))
      return FALSE;
      
   return(TRUE);
} 

/////////////////////////////////////////////////////////////////////////////////////////
// Functon: SDMRX_Close
//
// Description:
// Closes SDM recieve channel and initializes all state variables. Once the channel has 
// been successfuly closed, an SDMRX_EVENT_CHANNEL_CLOSED event will be generated via the
// callback function/
//
// Params:
// N/A
//
// Returns: TRUE if message was successfully sent to ANT, FALSE otherwise. 
// 
/////////////////////////////////////////////////////////////////////////////////////////
BOOL SDMRX_Close()
{
   return(ANT_CloseChannel(ucAntChannel));
}



/////////////////////////////////////////////////////////////////////////////////////////
// Functon: SDMRX_GetPage1
//
// Descrption:
// Returns a pointer to the page 1 buffer. This function should be called following 
// an SDMRX_EVENT_PAGE 1 event to extract the latest data from page 1. 
//
// Params:
// N/A
//
// returns: Pointer to Page 1 buffer, NULL if data not valid.  
/////////////////////////////////////////////////////////////////////////////////////////
SDMPage1_Data* SDMRX_GetPage1()
{
   return &stPage1Data;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Functon: SDMRX_GetPage2to13
//
// Descrption:
// Returns a pointer to the page 2-13 buffer. This function should be called following 
// an SDMRX_EVENT_PAGE 2to13 event to extract the latest data from page 2-13.  Page 2-13
// utilize this common structure, with pages 3-13 adding additional reserved fields. 
//
// Params:
// N/A
//
// returns: Pointer to Page 2 buffer, NULL if data not valid.  
/////////////////////////////////////////////////////////////////////////////////////////
SDMPage2to13_Data* SDMRX_GetPage2to13()
{
   return &stPage2to13Data;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Functon: CommonPage80_Data
//
// Descrption:
// Returns a pointer to the page 80 buffer. This function should be called following 
// an SDMRX_EVENT_PAGE 80 event to extract the latest data from page 80. 
//
// Params:
// N/A
//
// returns: Pointer to Page 80 buffer, NULL if data not valid.  
/////////////////////////////////////////////////////////////////////////////////////////
CommonPage80_Data* Common_GetPage80()
{
   return &stPage80Data;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Functon: CommonPage81_Data
//
// Descrption:
// Returns a pointer to the page 81 buffer. This function should be called following 
// an SDMRX_EVENT_PAGE 81 event to extract the latest data from page 81. 
//
// Params:
// N/A
//
// returns: Pointer to Page 81 buffer, NULL if data not valid.  
/////////////////////////////////////////////////////////////////////////////////////////
CommonPage81_Data* Common_GetPage81()
{
   return &stPage81Data;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Functon: CommonPage82_Data
//
// Descrption:
// Returns a pointer to the page 81 buffer. This function should be called following 
// an SDMRX_EVENT_PAGE 82 event to extract the latest data from page 82. 
//
// Params:
// N/A
//
// returns: Pointer to Page 81 buffer, NULL if data not valid.  
/////////////////////////////////////////////////////////////////////////////////////////
CommonPage82_Data* Common_GetPage82()
{
   return &stPage82Data;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Functon: SDMRX_GetAccum
//
// Descrption:
// Returns a pointer to accumulated distance and stride data. This function should be 
// called following an SDMRX_EVENT_PAGE 0 event to extract the latest accumulated data. 
//
// Params:
// N/A
//
// returns: Pointer to Page 81 buffer, NULL if data not valid.  
/////////////////////////////////////////////////////////////////////////////////////////
AccumulatedDistance* SDMRX_GetAccum()
{
   return(&stAccumulatedData);
}


//----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////
// SDM_ChannelEvent
//
// Process channel event messages for the SDM 
//
// pucEventBuffer_: Pointer to ANT message buffer.
//
// \return: TRUE if buffer handled. 
////////////////////////////////////////////////////////////////////////////
BOOL SDMRX_ChannelEvent(UCHAR* pucEventBuffer_, ANTPLUS_EVENT_RETURN* pstEventStruct_)
{
   BOOL bTransmit = FALSE;
   UCHAR ucChannel = pucEventBuffer_[BUFFER_INDEX_CHANNEL_NUM] & 0x1F;
   pstEventStruct_->eEvent = ANTPLUS_EVENT_NONE;

   if(ucChannel == ucAntChannel)           // Filtedr out message based on channel ID
   {
   
      if(pucEventBuffer_)
      {
         UCHAR ucANTEvent = pucEventBuffer_[BUFFER_INDEX_MESG_ID];   
         switch( ucANTEvent )
         {
            case MESG_RESPONSE_EVENT_ID:
            {
               bTransmit = HandleResponseEvents(pucEventBuffer_);
               break;
            }
            case MESG_BROADCAST_DATA_ID:                    // Handle both BROADCAST, ACKNOWLEDGED and BURST data the same
            case MESG_ACKNOWLEDGED_DATA_ID:
            case MESG_BURST_DATA_ID:    
            {
               bTransmit = HandleDataMessages(pucEventBuffer_, pstEventStruct_);
               break;
            }
            case MESG_CHANNEL_ID_ID:
            {
               usDeviceNumber = (USHORT)pucEventBuffer_[BUFFER_INDEX_MESG_DATA]; 
               usDeviceNumber |= (USHORT)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+1] << 8;
               ucTransType    = (UCHAR)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+3];

               pstEventStruct_->eEvent = ANTPLUS_EVENT_CHANNEL_ID;
               pstEventStruct_->usParam1 = usDeviceNumber;
               pstEventStruct_->usParam2 = ucTransType;
               break;
            }
         }
      } 
   }

   return (bTransmit);
} 

////////////////////////////////////////////////////////////////////////////
// HandleResponseEvents
//
// \return: N/A. 
////////////////////////////////////////////////////////////////////////////
BOOL HandleResponseEvents(UCHAR* pucBuffer_)
{

   BOOL bTransmit = TRUE;
   
   if(pucBuffer_ && pucBuffer_[BUFFER_INDEX_RESPONSE_CODE] == RESPONSE_NO_ERROR)
   {
      switch(pucBuffer_[BUFFER_INDEX_RESPONSE_MESG_ID])
      {
         case MESG_ASSIGN_CHANNEL_ID:
         {
            ANT_ChannelId(ucAntChannel, usDeviceNumber,SDMRX_DEVICE_TYPE, ucTransType );
            break;
         }
         case MESG_CHANNEL_ID_ID:
         {
            ANT_ChannelRFFreq(ucAntChannel, ANTPLUS_RF_FREQ);
            break;
         }
         case MESG_CHANNEL_RADIO_FREQ_ID:
         {
            ANT_ChannelPeriod(ucAntChannel, SDMRX_MSG_PERIOD);
            break;
         }
         case MESG_CHANNEL_MESG_PERIOD_ID:
         {
            ANT_OpenChannel(ucAntChannel);
            break;
         }
         case MESG_OPEN_CHANNEL_ID:
         case MESG_CLOSE_CHANNEL_ID:            // Fallthrough
         default:
         {
            bTransmit = FALSE;                  // Can go back to sleep   
         }
      }
   }
   return(bTransmit);
}

////////////////////////////////////////////////////////////////////////////
// HandleDataMessages
//
// \return: N/A. 
////////////////////////////////////////////////////////////////////////////
BOOL HandleDataMessages(UCHAR* pucEventBuffer_, ANTPLUS_EVENT_RETURN* pstEventStruct_)
{
   BOOL bTransmit = FALSE;
   UCHAR ucPage = pucEventBuffer_[BUFFER_INDEX_MESG_DATA];
   
   pstEventStruct_->eEvent = ANTPLUS_EVENT_PAGE;
   pstEventStruct_->usParam1 = ucPage;
   
   switch(ucPage)
   {
      case SDM_PAGE_1:
      {
         USHORT usShiftVar;
         stPage1Data.ucLastTime200  = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+1]; 
         stPage1Data.ucLastTime     = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+2]; 
         stPage1Data.ucDistance     = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+3]; 
         stPage1Data.ucDistance16   = ((UCHAR)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+4] >> 4) & 0x0F;
         stPage1Data.ucInstSpeed    = ((UCHAR)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+4]) & 0x0F;
         stPage1Data.ucInstSpeed256 = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+5];
         stPage1Data.ucStrideCount  = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+6];
         stPage1Data.ucLatency      = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7];
         
         usShiftVar = ((USHORT)stPage1Data.ucDistance*16) + (stPage1Data.ucDistance16 & 0x000F); // Convert to (1/16)
         
         if(!uFlags.flags.bDistanceInit)
         {
            usPrevDistance =  usShiftVar;
            uFlags.flags.bDistanceInit = TRUE;
         }
         
         if(!uFlags.flags.bStridesInit)
         {
            ucPrevStride =  stPage1Data.ucStrideCount;
            uFlags.flags.bStridesInit = TRUE;
         }
            
         stAccumulatedData.ulAccumDistance += (ULONG)((usShiftVar - usPrevDistance) & 0x0FFF);                         // Mask upper nibble to handle rollover
         usPrevDistance = usShiftVar;
         stAccumulatedData.ulAccumDistance += (ULONG)((usShiftVar - usPrevDistance) & 0x0FFF);                         // Mask upper nibble to handle rollover
            
         stAccumulatedData.ulAccumStride += (ULONG)((stPage1Data.ucStrideCount - ucPrevStride) & MAX_UCHAR);          // Handles rollover
         ucPrevStride = stPage1Data.ucStrideCount;
         break;
      }
      case SDM_PAGE_2:
      case SDM_PAGE_3:
      case SDM_PAGE_4:
      case SDM_PAGE_5:
      case SDM_PAGE_6:
      case SDM_PAGE_7:
      case SDM_PAGE_8:
      case SDM_PAGE_9:
      case SDM_PAGE_10:
      case SDM_PAGE_11:
      case SDM_PAGE_12:
      case SDM_PAGE_13:
      {
         stPage2to13Data.ucCadence        = pucEventBuffer_[BUFFER_INDEX_MESG_DATA + 3];
         stPage2to13Data.ucCadence16      = ((UCHAR)pucEventBuffer_[BUFFER_INDEX_MESG_DATA + 4] >> 4) & 0x0F;
         stPage2to13Data.ucInstSpeed      = ((UCHAR)pucEventBuffer_[BUFFER_INDEX_MESG_DATA + 4]) & 0x0F;
         stPage2to13Data.ucInstSpeed256   = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+5];
         stPage2to13Data.ucModuleLocation = SDM_LOCATION(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7]);
         stPage2to13Data.ucBatteryStatus  = SDM_BATTERY_STATUS(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7]);
         stPage2to13Data.ucUnitHealth     = SDM_UNIT_HEALTH(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7]);
         stPage2to13Data.ucUseState       = SDM_USE_STATE(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7]);
         break;
      }
      case GLOBAL_PAGE_80:
      {
         stPage80Data.ucHwVersion    = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+3];
         stPage80Data.usManId        = (USHORT)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+4];
         stPage80Data.usManId       |= (USHORT)(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+5] << 8);
         stPage80Data.usModelNumber  = (USHORT)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+6];
         stPage80Data.usModelNumber |= (USHORT)(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7] << 8);
         break;
      }
      case GLOBAL_PAGE_81:
      {
         stPage81Data.ucSwVersion     = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+3];
         stPage81Data.ulSerialNumber  = (ULONG)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+4];
         stPage81Data.ulSerialNumber |= (ULONG)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+5] << 8;
         stPage81Data.ulSerialNumber |= (ULONG)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+6] << 16;
         stPage81Data.ulSerialNumber |= (ULONG)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7] << 24;
         break;
      }
      case GLOBAL_PAGE_82:
      {
         stPage82Data.ulCumOperatingTime    = (ULONG)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+3];
         stPage82Data.ulCumOperatingTime   |= (ULONG)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+4] << 8;
         stPage82Data.ulCumOperatingTime   |= (ULONG)pucEventBuffer_[BUFFER_INDEX_MESG_DATA+5] << 16;
         stPage82Data.ucBattVoltage256      = pucEventBuffer_[BUFFER_INDEX_MESG_DATA+6];
         stPage82Data.ucBattVoltage         = COMMON82_COARSE_BATT_VOLTAGE(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7]);
         stPage82Data.ucBattStatus          = COMMON82_BATT_STATUS(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7]);
         stPage82Data.ucCumOperatingTimeRes = COMMON82_CUM_TIME_RESOLUTION(pucEventBuffer_[BUFFER_INDEX_MESG_DATA+7]);
         break;
      }
      default:
      {
         pstEventStruct_->eEvent = ANTPLUS_EVENT_UNKNOWN_PAGE;
         break;
      }
   }

   if(usDeviceNumber == 0)
   {
      if(ANT_RequestMessage(ucAntChannel, MESG_CHANNEL_ID_ID))
         bTransmit = TRUE;                
   }
   return(bTransmit);
}








