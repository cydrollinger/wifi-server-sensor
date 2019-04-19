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
#include "antinterface.h"
#include "antdefines.h"
#include "timer.h"
#include "printf.h"

#include "ioboard.h"
#include "sdm_rx.h"



// ANT Channel settings
#define ANT_CHANNEL_SDMRX                          ((UCHAR) 0)        // Default ANT Channel

//Status flags
#define SDM_LOCATION_LACES                   ((UCHAR) 0)
#define SDM_LOCATION_MIDSOLE                 ((UCHAR) 1)
#define SDM_LOCATION_CHEST                   ((UCHAR) 2)
#define SDM_LOCATION_OTHER                   ((UCHAR) 3)
#define SDM_BATTERY_STATUS_NEW               ((UCHAR) 0)
#define SDM_BATTERY_STATUS_GOOD              ((UCHAR) 1)
#define SDM_BATTERY_STATUS_OK                ((UCHAR) 2)
#define SDM_BATTERY_STATUS_LOW               ((UCHAR) 3)
#define SDM_UNIT_HEALTH_OK                   ((UCHAR) 0)
#define SDM_UNIT_HEALTH_ERROR                ((UCHAR) 1)
#define SDM_UNIT_HEALTH_WARNING              ((UCHAR) 2)
#define SDM_USE_STATE_INACTIVE               ((UCHAR) 0)
#define SDM_USE_STATE_ACTIVE                 ((UCHAR) 1)


//other defines
#define SDM_PRECISION                                 ((ULONG)1000)

extern volatile USHORT usLowPowerMode;                                         // low power mode control
static const UCHAR aucNetworkKey[] = ANTPLUS_NETWORK_KEY;

void ProcessANTSDMRXEvents(ANTPLUS_EVENT_RETURN* pstEvent_);
void ProcessAntEvents(UCHAR* pucEventBuffer_);

//----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////
// main
//
// main function  
//
// Configures device simulator and HRM TX channel.
//
// \return: This function does not return. 
////////////////////////////////////////////////////////////////////////////
void main_sdm()
{

   UCHAR* pucRxBuffer;
   ANTPLUS_EVENT_RETURN stEventStruct;


   // Set network key. Do not send other commands
   // until after a response is recieved to this one.
   ANT_NetworkKey(ANTPLUS_NETWORK_NUMBER, aucNetworkKey);

   // Main loop
                                         
   while(TRUE)
   {
      usLowPowerMode = 0;//LPM1_bits;                          // initialize the low power mode -  move to processor specific file and generic function
      
      pucRxBuffer = ANTInterface_Transaction();                // Check if any data has been recieved from serial
      
      if(pucRxBuffer)
      {
      
         if(SDMRX_ChannelEvent(pucRxBuffer, &stEventStruct))
            usLowPowerMode = 0;
         ProcessANTSDMRXEvents(&stEventStruct);

         ProcessAntEvents(pucRxBuffer);
         ANTInterface_Complete();                              // Release the serial buffer
      }
      
      _BIS_SR(usLowPowerMode);                                 // go to sleep if we can  -  move to processor specific file and generic function
   }
} 


////////////////////////////////////////////////////////////////////////////
// ProcessANTSDMRXEvents
//
// SDM Reciever event processor  
//
// Processes events recieved from SDM module.
//
// \return: N/A 
///////////////////////////////////////////////////////////////////////////
void ProcessANTSDMRXEvents(ANTPLUS_EVENT_RETURN* pstEvent_)
{
   switch (pstEvent_->eEvent)
   {
      case ANTPLUS_EVENT_CHANNEL_ID:
      {
         // Can store this device number for future pairings so that 
         // wild carding is not necessary.
         printf("Device Number is %d\n", pstEvent_->usParam1);
         printf("Transmission type is %d\n\n", pstEvent_->usParam2);
         break;
      }
      case ANTPLUS_EVENT_PAGE:
      {
         IOBOARD_LED3_OUT &= ~IOBOARD_LED3_BIT;    // TURN ON LED 3
         // Get data correspinding to the page. Only get the data you 
         // care about.
         switch(pstEvent_->usParam1)
         {
            case SDM_PAGE_1:
            {
               SDMPage1_Data* pstPage1Data = SDMRX_GetPage1();
               AccumulatedDistance* pstAccumData = SDMRX_GetAccum();

               printf("Speed/Distance Monitor Page 1\n");

               printf("Time: %u", (ULONG)pstPage1Data->ucLastTime);
               printf(".%03u s\n", (ULONG)((ULONG)pstPage1Data->ucLastTime200*SDM_PRECISION)/200);
               printf("Instantaneous speed: %u",(ULONG)pstPage1Data->ucInstSpeed);
               printf(".%03u m/s\n", (ULONG)((ULONG)pstPage1Data->ucInstSpeed256*SDM_PRECISION + 128)/256);
               printf("Distance: %u", (ULONG)pstPage1Data->ucDistance);
               printf(".%u m\n", (ULONG)((ULONG)pstPage1Data->ucDistance16*SDM_PRECISION + 8)/16);
               printf("Stride count: %u\n", (ULONG)pstPage1Data->ucStrideCount);
               printf("Latency: %u", (ULONG)((ULONG)pstPage1Data->ucLatency)/32);
               printf(".%03u s\n\n", (ULONG)((ULONG)pstPage1Data->ucLatency*SDM_PRECISION + 16)/32);

               printf("Accumulated Distance: %u", pstAccumData->ulAccumDistance >> 4);
               printf(".%03u m\n", (((pstAccumData->ulAccumDistance & 0x000F)*SDM_PRECISION) + 8) / 16);

               printf("Accumulated stride count: %u strides\n\n", pstAccumData->ulAccumStride);

               break;
            }
            case SDM_PAGE_2:
            {
               SDMPage2to13_Data* pstPage2to13Data = SDMRX_GetPage2to13();

               printf("Speed/Distance Monitor Page 2\n");

               printf("Cadence: %u", (ULONG)pstPage2to13Data->ucCadence);
               printf(".%03u strides/min\n", (ULONG)((ULONG)pstPage2to13Data->ucCadence16*SDM_PRECISION + 8)/16);
               printf("Instanteous speed: %u", (ULONG)pstPage2to13Data->ucInstSpeed);
               printf(".%03u m/s\n", (ULONG)((ULONG)pstPage2to13Data->ucInstSpeed256*SDM_PRECISION + 128)/256);
               printf("Status: ");

               switch (pstPage2to13Data->ucModuleLocation)
               {
                  case SDM_LOCATION_LACES:
                     printf("module located in the laces, ");
                     break;
                  case SDM_LOCATION_MIDSOLE:
                     printf("module located in the midsole, ");
                     break;
                  case SDM_LOCATION_CHEST:
                     printf("module located on the chest, ");
                     break;
                  case SDM_LOCATION_OTHER:
                  default:
                     printf("module located elsewhere, ");
                     break;
               }
               switch (pstPage2to13Data->ucBatteryStatus)
               {
                  case SDM_BATTERY_STATUS_NEW:
                     printf("battery new, ");
                     break;
                  case SDM_BATTERY_STATUS_GOOD:
                     printf("battery good, ");
                     break;
                  case SDM_BATTERY_STATUS_OK:
                     printf("battery ok, ");
                     break;
                  case SDM_BATTERY_STATUS_LOW:
                  default:
                     printf("battery low, ");
                     break;
               }
               switch (pstPage2to13Data->ucUnitHealth)
               {
                  case SDM_UNIT_HEALTH_OK:
                     printf("sensor health ok, ");
                     break;
                  case SDM_UNIT_HEALTH_ERROR:
                     printf("sensor error, ");
                     break;
                  case SDM_UNIT_HEALTH_WARNING:
                     printf("sensor warning, ");
                     break;
                  default:
                     printf("sensor status unavailable, ");
                     break;
               }
               switch (pstPage2to13Data->ucUseState)
               {
                  case SDM_USE_STATE_INACTIVE:
                     printf("sensor state is inactive.\n");
                     break;
                  case SDM_USE_STATE_ACTIVE:
                     printf("sensor state is active.\n");
                     break;
                  default:
                     printf("sensor state unavailable.\n");
                     break;
               }
               printf("\n");
               break;
            }
            case GLOBAL_PAGE_80:
            {
               CommonPage80_Data* pstPage80Data = Common_GetPage80();

               printf("Common Data Page 80\n");

               printf("Manufacturer ID: %u\n", (USHORT)pstPage80Data->usManId);
               printf("Model %u, ", (USHORT)pstPage80Data->usModelNumber);
               printf("Hardware rev. %d\n\n", pstPage80Data->ucHwVersion);
               break;
            }
            case GLOBAL_PAGE_81:
            {
               CommonPage81_Data* pstPage81Data = Common_GetPage81();

               printf("Common Data Page 81\n");

               printf("Software version %d, ", pstPage81Data->ucSwVersion); 
               printf("SN# 0x%04X", (USHORT)((pstPage81Data->ulSerialNumber >> 16) & MAX_USHORT));
               printf("%04X\n\n", (USHORT)(pstPage81Data->ulSerialNumber & MAX_USHORT)); //display limited by 16-bit CPU
               break;
            }
            case GLOBAL_PAGE_82:
            {
               CommonPage82_Data* pstPage82Data = Common_GetPage82();
               UCHAR ucTimeRes = pstPage82Data->ucCumOperatingTimeRes ? 2 : 16; //2 or 16 second resolution

               printf("Common Data Page 82\n");

               printf("Cumulative operating time: %u", (ULONG)((pstPage82Data->ulCumOperatingTime * ucTimeRes) / 3600)); // 3600s/hr
               printf(".%03u hrs\n", (ULONG)((((pstPage82Data->ulCumOperatingTime * ucTimeRes) % 3600) * SDM_PRECISION) + 1800) /3600); // 3600s/hr
               if (pstPage82Data->ucBattVoltage != GBL82_COARSE_BATT_INVALID_VOLTAGE)
               {
                  printf("Battery voltage: %u", pstPage82Data->ucBattVoltage);
                  printf("%03u V, ", (ULONG)(((pstPage82Data->ucBattVoltage256 * SDM_PRECISION) + 128) / 256));
               }
               else
                  printf("Invalid battery voltage, ");

               switch (pstPage82Data->ucBattStatus)
               {
                  case GBL82_BATT_STATUS_NEW:
                     printf("battery new.");
                     break;
                  case GBL82_BATT_STATUS_GOOD:
                     printf("battery good.");
                     break;
                  case GBL82_BATT_STATUS_OK:
                     printf("battery ok.");
                     break;
                  case GBL82_BATT_STATUS_LOW:
                     printf("battery low.");
                     break;
                  case GBL82_BATT_STATUS_CRITICAL:
                     printf("battery critical.");
                     break;
                  default:
                     printf("invalid battery status.");
                     break;
               }

               printf("\n\n");

               break;
            }
            default:
            {
               // ASSUME PAGE 0
               printf("Invalid or undefined page\n\n");
               break; 
            }
         }
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;    // TURN OFF LED 3
         break;
      }

      case ANTPLUS_EVENT_UNKNOWN_PAGE:  // Decode unknown page manually
      case ANTPLUS_EVENT_NONE:
      default:
      {
         break;
      }  
   }
}
////////////////////////////////////////////////////////////////////////////
// ProcessAntEvents
//
// \return: N/A 
///////////////////////////////////////////////////////////////////////////
void ProcessAntEvents(UCHAR* pucEventBuffer_)
{
   
   if(pucEventBuffer_)
   {
      UCHAR ucANTEvent = pucEventBuffer_[BUFFER_INDEX_MESG_ID];   
      
      switch( ucANTEvent )
      {
         case MESG_RESPONSE_EVENT_ID:
         {
            switch(pucEventBuffer_[BUFFER_INDEX_RESPONSE_CODE])
            {
               case EVENT_RX_SEARCH_TIMEOUT:
               {
                  IOBOARD_LED0_OUT |= IOBOARD_LED0_BIT;         
                  break;
               }
               case EVENT_TX:
               {
                  break;
               }
               
               case RESPONSE_NO_ERROR:
               {   
                  if (pucEventBuffer_[3] == MESG_OPEN_CHANNEL_ID)
                  {
                     IOBOARD_LED0_OUT &= ~IOBOARD_LED0_BIT;         
                     printf("initialization is complete.\n");
                  }
                  else if (pucEventBuffer_[3] == MESG_CLOSE_CHANNEL_ID)
                  {
                  }
                  else if (pucEventBuffer_[3] == MESG_NETWORK_KEY_ID)
                  {
                     //Once we get a response to the set network key
                     //command, start opening the SDM channel
                     SDMRX_Open(ANT_CHANNEL_SDMRX, 0, 0); 
                  }
                  
                  
                  
                  
                  break;
               }
            }
         }
      }
   }      
}
