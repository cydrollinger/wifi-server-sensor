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
#include "antmessage.h"
#include "antdefines.h"
#include "ANTInterface.h"
#include "ioboard.h"
#include "sdm_tx.h"
#include "antplus.h"
#include "timer.h"



// ANT Channel
#define SDMTX_CHANNEL_TYPE            ((UCHAR) 0x10)     // Master

// Channel ID
#define SDMTX_DEVICE_TYPE             ((UCHAR) 0x7C)	

// Message Periods
#define SDMTX_MSG_PERIOD              ((USHORT) 0x1FC6)	// decimal 8134 (4.03Hz)

// SImulator definitions
#define SDM_STRIDE_PER_MINUTE  ((UCHAR) 60)

//Status definitions
#define ANT_SDM_STATE_SHIFT            0
#define ANT_SDM_STATE_MASK             ((UCHAR) 0x03 << ANT_SDM_STATE_SHIFT)
#define ANT_SDM_STATE_INACTIVE         ((UCHAR) 0x00 << ANT_SDM_STATE_SHIFT)
#define ANT_SDM_STATE_ACTIVE           ((UCHAR) 0x01 << ANT_SDM_STATE_SHIFT)

#define ANT_SDM_HEALTH_SHIFT           2
#define ANT_SDM_HEALTH_MASK            ((UCHAR) (0x03 << ANT_SDM_HEALTH_SHIFT))
#define ANT_SDM_HEALTH_OK              ((UCHAR) (0x00 << ANT_SDM_HEALTH_SHIFT))
#define ANT_SDM_HEALTH_ERROR           ((UCHAR) (0x01 << ANT_SDM_HEALTH_SHIFT))
#define ANT_SDM_HEALTH_WARNING         ((UCHAR) (0x02 << ANT_SDM_HEALTH_SHIFT))

#define ANT_SDM_BAT_SHIFT              4
#define ANT_SDM_BAT_MASK               ((UCHAR) (0x03 << ANT_SDM_BAT_SHIFT))
#define ANT_SDM_BAT_NEW                ((UCHAR) (0x00 << ANT_SDM_BAT_SHIFT))
#define ANT_SDM_BAT_GOOD               ((UCHAR) (0x01 << ANT_SDM_BAT_SHIFT))
#define ANT_SDM_BAT_OK                 ((UCHAR) (0x02 << ANT_SDM_BAT_SHIFT))
#define ANT_SDM_BAT_LOW                ((UCHAR) (0x03 << ANT_SDM_BAT_SHIFT))

#define ANT_SDM_LOCATION_SHIFT         6
#define ANT_SDM_LOCATION_MASK          ((UCHAR) (0x03 << ANT_SDM_LOCATION_SHIFT))
#define ANT_SDM_LOCATION_LACES         ((UCHAR) (0x00 << ANT_SDM_LOCATION_SHIFT))
#define ANT_SDM_LOCATION_MIDSOLE       ((UCHAR) (0x01 << ANT_SDM_LOCATION_SHIFT))
#define ANT_SDM_LOCATION_CHEST         ((UCHAR) (0x02 << ANT_SDM_LOCATION_SHIFT))


///////////////////////////////////////////////////////////////////////////////
// Application layer definitions
///////////////////////////////////////////////////////////////////////////////
static UCHAR aucTxBuf[8];			// Primary transmit buffer
static ULONG ulRunTime;				// Accumulated runtime (1/32768 seconds)
static USHORT usStrideCount;	   // Accumulated stride count (strides)
static USHORT usDistance16;		// Accumulated distance (1/16 meters)
static USHORT usSPM16;				// Current cadence (1/16 strides per minute)
static USHORT usSpeed256;			// Current speed (1/256 meters per second)
static USHORT usLastTime256;      // Time of last measurement (1/256 seconds)
static USHORT usLatency256;		// Latency (1/256 seconds)

static USHORT usLatencyOffset;    // For sim use only, used to align time as per channel event with pulse event time.
static BOOL bInit;               // Helpful for realistice latency calculations.                 
    
static UCHAR ucAntChannel;      
static USHORT usDeviceNumber;    
static UCHAR ucTransType;
static UCHAR ucStridesPerMinute;
static UCHAR ucAlarmNumber;
        
   
static void Encode(void);
static BOOL HandleResponseEvents(UCHAR* pucBuffer_);
static BOOL HandleDataMessages(UCHAR* pucBuffer_, ANTPLUS_EVENT_RETURN* pstEventStruct_);


/////////////////////////////////////////////////////////////////////////////////////////
// Functon: SDMTX_Init
//
// Descrption:
//
// Initialize all state variables. 
// Params:
// N/A.
//
// returns: N/A. 
/////////////////////////////////////////////////////////////////////////////////////////
void SDMTX_Init()
{
   // Intialize app variables
   usStrideCount = 0;
   usDistance16 = 0;
   usSpeed256 = 0;
   usSPM16 = 0;
   ulRunTime = 0;	
   usLastTime256 = 0;
   usLatency256 = 0;
   bInit = FALSE;
   
   ucStridesPerMinute = SDM_STRIDE_PER_MINUTE;
      
}


////////////////////////////////////////////////////////////////////////////
// SDMTX_Open
//
// Initlize SDM transmitter, queues up all configuration commands. 
//
// ucAntChannel_: ANT channel number
// ucDeviceNumber_: ANT channel ID device number
//
// \return: TRUE if init sucesssful. 
////////////////////////////////////////////////////////////////////////////
BOOL SDMTX_Open(UCHAR ucAntChannel_, USHORT usDeviceNumber_, UCHAR ucTransType_)
{
   SDMTX_Init();
   
   ucAntChannel = ucAntChannel_;
   usDeviceNumber = usDeviceNumber_;
   ucTransType = ucTransType_;  
   
   if(usDeviceNumber_ == 0)
      usDeviceNumber_ = 2;
      
   if(ucTransType_ == 0)
      ucTransType_ = 5;
 
   if (!ANT_AssignChannel(ucAntChannel,SDMTX_CHANNEL_TYPE,ANTPLUS_NETWORK_NUMBER ))
      return FALSE;
      
   // Register the timer for HRM simulator
   ucAlarmNumber = Timer_RegisterAlarm( SDMTX_PulseEvent, ALARM_FLAG_CONTINUOUS);
      
   return(TRUE);
} 

////////////////////////////////////////////////////////////////////////////
// SDMTX_ChannelEvent
//
// Process ANT channel event. 
//
// pucEventBuffer_: pointer to buffer containg data recieved from ANT
// pstEventStruct_: Pointer to event return structure
//
// !!IMPORTANT: If this function returns true, it means that there is 
// a pending message to transmit to ANT. Do not sleep thread.
//
// \return: TRUE if a packet is to be transmitted 
////////////////////////////////////////////////////////////////////////////
BOOL SDMTX_ChannelEvent(UCHAR* pucEventBuffer_, ANTPLUS_EVENT_RETURN* pstEventStruct_)
{
   BOOL bTransmit;
   
   UCHAR ucChannel = pucEventBuffer_[BUFFER_INDEX_CHANNEL_NUM] & 0x1F;
   pstEventStruct_->eEvent = ANTPLUS_EVENT_NONE;
   
   if(ucChannel == ucAntChannel)
   {
      if(pucEventBuffer_)
      {
         UCHAR ucANTEvent = pucEventBuffer_[BUFFER_INDEX_MESG_ID];   
      
         switch(ucANTEvent)
         {
            case MESG_RESPONSE_EVENT_ID:
            {
               switch(pucEventBuffer_[BUFFER_INDEX_RESPONSE_CODE])
               {
                  case RESPONSE_NO_ERROR:
                  {   
                     bTransmit = HandleResponseEvents(pucEventBuffer_);
                     break;
                  }
         
                  case EVENT_TX:
                  {
                     bTransmit = HandleDataMessages(pucEventBuffer_, pstEventStruct_);
                     break;
                  }
                  default:
                  {
                     // Handle other events at the application level
                     break;
                  }
               }
            }
         }
      }
   }
   return(bTransmit);
}

////////////////////////////////////////////////////////////////////////////
// SDMTX_PulseEvent
//
// Simulates a device event. Based on this event, the transmitter 
// data is simulated.  
//
// ulTime256: Time of event in (1/256)s
//
// \return: N/A. 
////////////////////////////////////////////////////////////////////////////
void SDMTX_PulseEvent(USHORT usTime1024, UCHAR ucAlarmNumber_)
{
  // ulTime256 is in 1/256 seconds
  USHORT usTime256;
  ULONG ulCadenceCalc;
  ULONG ulSpeedCalc;
  USHORT usStridePeriod;
  
  // Convert time to 256
  usTime256 = usTime1024 >> 2;
  
 usStridePeriod = (USHORT) (usTime256 - usLastTime256);                     // Latest time minus last time
  
  usLastTime256 = usTime256;
    
  // Event count
  usStrideCount++;
  usDistance16 += STRIDE_LENGTH;
  
  // Cadence calculation
  ulCadenceCalc = (ULONG) 0x0003C000;        // 60s*256*16 (resoultion of time period is (1/256), 
  ulCadenceCalc /= usStridePeriod;           // stride calculation needs to be in (1/16)
  usSPM16 = (USHORT) ulCadenceCalc;
  
  // Speed calculation
  ulSpeedCalc = usSPM16 * STRIDE_LENGTH;     // SPM (1/16) X side length (1/16) = speed per minute (1/256) 
  ulSpeedCalc /= (ULONG) 60;                 // Divide down 60 to get speed (1/256) / s
  usSpeed256 = (USHORT) ulSpeedCalc;
}


////////////////////////////////////////////////////////////////////////////
// Encode
//
// Encodes SDM page data before sending to ANT. 
// 
// \return: N/A. 
////////////////////////////////////////////////////////////////////////////
void Encode(void)
{
   static UCHAR ucMessageCount = 0; 
   static UCHAR ucPageNumber = 0;			  // Page number tracker
   static UCHAR ucExtMesgType = 0;			  // Extended page number tracker
   ULONG ulRunTime256;                      // Run time (1/256s)

   ulRunTime += ((ULONG)SDMTX_MSG_PERIOD);

   // Calculate latency (difference between current time and time of last measurement)
   ulRunTime256 = ulRunTime >> 7;           // Runtime is (1/32768), so multiply by 2^7 to bring up to (1/256)
//   if(ulRunTime256 > ulLastTime256)
		usLatency256 = (USHORT) (ulRunTime256 - usLastTime256);					
//   else
//		usLatency256 = (USHORT) ((0xFFFFFFFF - ulLastTime256) + ulRunTime256 + 1);	

   usLatency256 -= usLatencyOffset;
   
   // Figure out what page to send
   // based on the message count.
   ucPageNumber = 1;
   
   // Page 1 and 2 alternate
   if(ucMessageCount & 0x02)
      ucPageNumber = SDM_PAGE_2;
   
   // Send global pages every 65th message, 
   if(ucMessageCount++ >= 64)
   {  
      // Always send two in a row    
      if(ucExtMesgType++ & 0x02)
         ucPageNumber = GLOBAL_PAGE_81;  
      else
         ucPageNumber = GLOBAL_PAGE_80;  
           
      ucMessageCount = 0;      
   }
	
   // Format the required page.
   switch (ucPageNumber)
   {
      case SDM_PAGE_1:
      {
         aucTxBuf[0] = SDM_PAGE_1;
         aucTxBuf[1] = (USHORT) ((usLastTime256 & 0xFF) * 25) >> 5;  // fractional time (1/200 seconds)
         aucTxBuf[2] = (usLastTime256 >> 8) & 0xFF; // last time (seconds)
         aucTxBuf[3] = (usDistance16 >> 4) & 0xFF;  // cumulative distance (meters)                     
         aucTxBuf[4] = (usDistance16 << 4) & 0xF0;  // fractional distance (1/16 meters)
         aucTxBuf[4] |= ((usSpeed256 >> 8) & 0x0F); // current speed (m/s)
         aucTxBuf[5] = usSpeed256 & 0xFF;           // fractional speed (1/256 m/s)
         aucTxBuf[6] = usStrideCount & 0xFF;        // cumulative stride count (number of strides) 
         aucTxBuf[7] = (usLatency256 >> 3) & 0xFF;	 // update latency (1/32 seconds) 
         break;
      }
      case SDM_PAGE_2:
      {
         aucTxBuf[0] = SDM_PAGE_2;
         aucTxBuf[1] = 0xFF;                        // Reserved
         aucTxBuf[2] = 0xFF;                        // Reserved
         aucTxBuf[3] = (usSPM16 >> 4) & 0xFF;		 // current cadence (strides/min)
         aucTxBuf[4] = (usSPM16 << 4) & 0xF0;		 // fractional cadence (1/16 strides/min)
         aucTxBuf[4] |= (usSpeed256 >> 8) & 0x0F;   // current speed (m/s)
         aucTxBuf[5] = usSpeed256 & 0xFF;           // fractional speed (1/256 m/s)
         aucTxBuf[6] = 0xFF;                        // Reserved
         aucTxBuf[7] = ANT_SDM_STATE_ACTIVE | ANT_SDM_HEALTH_OK | ANT_SDM_BAT_GOOD | ANT_SDM_LOCATION_LACES;    // Status
         break;
      }
     
      case GLOBAL_PAGE_80:
      {
         aucTxBuf[0] = GLOBAL_PAGE_80;
         aucTxBuf[1] = 0xFF;                        // Reserved
         aucTxBuf[2] = 0xFF;                        // Reserved
         aucTxBuf[3] = SDMTX_HW_VERSION;            // HW version
         aucTxBuf[4] = LOW_BYTE(SDMTX_MFG_ID);      // low byte of Mfg ID
         aucTxBuf[5] = HIGH_BYTE(SDMTX_MFG_ID);     // high byte of Mfg ID
         aucTxBuf[6] = LOW_BYTE(SDMTX_MODEL_NUMBER);  // low byte of Model Number
         aucTxBuf[7] = HIGH_BYTE(SDMTX_MODEL_NUMBER); // high byte of Model Number
         break;
		}
      case GLOBAL_PAGE_81:
      {
         aucTxBuf[0] = GLOBAL_PAGE_81;
         aucTxBuf[1] = 0xFF;                        // Reserved
         aucTxBuf[2] = 0xFF;                        // Reserved
         aucTxBuf[3] = SDMTX_SW_VERSION;            // SW version
         aucTxBuf[4] = BYTE0(SDMTX_SERIAL_NUMBER);  // bits 0-7 of serial #
         aucTxBuf[5] = BYTE1(SDMTX_SERIAL_NUMBER);  // bits 8-15 of serial #
         aucTxBuf[6] = BYTE2(SDMTX_SERIAL_NUMBER);  // bits 16-23 of serial #
         aucTxBuf[7] = BYTE3(SDMTX_SERIAL_NUMBER);  // bits 24-31 of serial #   
	      break;
       }
       default:
       {
         break;
       }
    }
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
            ANT_ChannelId(ucAntChannel, usDeviceNumber,SDMTX_DEVICE_TYPE, ucTransType );
            break;
         }
         case MESG_CHANNEL_ID_ID:
         {
            ANT_ChannelRFFreq(ucAntChannel, ANTPLUS_RF_FREQ);
            break;
         }
         case MESG_CHANNEL_RADIO_FREQ_ID:
         {
            ANT_ChannelPeriod(ucAntChannel, SDMTX_MSG_PERIOD);
            break;
         }
         case MESG_CHANNEL_MESG_PERIOD_ID:
         {
            USHORT usCounts = 60*ALARM_TIMER_PERIOD;
            ANT_OpenChannel(ucAntChannel);
            Timer_Start( ucAlarmNumber, usCounts/(USHORT)ucStridesPerMinute);    // Start the HRM PULSE timer         
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
BOOL HandleDataMessages(UCHAR* pucBuffer_, ANTPLUS_EVENT_RETURN* pstEventStruct_)
{
   BOOL bTransmit;

   if(!bInit)
   {
      usLatencyOffset = usLastTime256;
      bInit = TRUE;
   }


   Encode();
   if(ANT_Broadcast(ucAntChannel, aucTxBuf))
   {
      pstEventStruct_->eEvent = ANTPLUS_EVENT_TRANSMIT;
      bTransmit = TRUE;
   }

   return(bTransmit);
}


