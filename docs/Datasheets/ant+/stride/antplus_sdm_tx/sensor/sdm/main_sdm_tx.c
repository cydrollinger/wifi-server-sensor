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
#include "sdm_tx.h"



extern volatile USHORT usLowPowerMode;                          // low power mode control
#define SDMTX_ANT_CHANNEL                   ((UCHAR) 0)           // Default ANT Channel


static const UCHAR aucNetworkKey[] = ANTPLUS_NETWORK_KEY;

static void ProcessANTSDMTXEvents(ANTPLUS_EVENT_RETURN* pstEvent_);
static void ProcessAntEvents(UCHAR* pucEventBuffer_);

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
void main_sdmtx()
{

   UCHAR* pucRxBuffer;
   ANTPLUS_EVENT_RETURN stSDMEventStruct;

   // Main loop
   // Set network key. Do not send other commands
   // until after a response is recieved to this one.
   ANT_NetworkKey(ANTPLUS_NETWORK_NUMBER, aucNetworkKey);

   // Main loop
   while(TRUE)
   {
      usLowPowerMode = 0;//LPM1_bits;                       // nitialize the low power mode -  move to processor specific file and generic function
      
      pucRxBuffer = ANTInterface_Transaction();                // Check if any data has been recieved from serial
      
      if(pucRxBuffer)
      {
         if(SDMTX_ChannelEvent(pucRxBuffer, &stSDMEventStruct))
            usLowPowerMode = 0;
         
         ProcessANTSDMTXEvents(&stSDMEventStruct);               
         
         ProcessAntEvents(pucRxBuffer);
         ANTInterface_Complete();                              // Release the serial buffer
      }
      
      _BIS_SR(usLowPowerMode);                              // Go to sleep if we can  -  move to processor specific file and generic function
   }   
} 


////////////////////////////////////////////////////////////////////////////
// ProcessANTHRMRXEvents
//
// HRM Reciever event processor  
//
// Processes events recieved from HRM module.
//
// \return: N/A 
///////////////////////////////////////////////////////////////////////////
void ProcessANTSDMTXEvents(ANTPLUS_EVENT_RETURN* pstEvent_)
{

   switch(pstEvent_->eEvent)
   {

      case ANTPLUS_EVENT_TRANSMIT:
      {
         IOBOARD_LED3_OUT &= ~IOBOARD_LED3_BIT;                // Turn ON LED
         printf("Transmit.\n");
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;                 // Turn OFF LED
         break;
      } 
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
// Generic ANT Event handling  
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
                     //command, start opening the HRM channel
                     SDMTX_Open(SDMTX_ANT_CHANNEL, 49, 5); 
                  }
                  break;
               }
            }
         }
      }
   }      
}






