/*
 * Dynastream Innovations Inc.
 * Cochrane, AB, CANADA
 *
 * Copyright © 1998-2009 Dynastream Innovations Inc.
 * All rights reserved. This software may not be reproduced by
 * any means without express written approval of Dynastream
 * Innovations Inc.
 */
  
#include <stdio.h>
#include <assert.h>

#include "demo.h"

#include "types.h"
#include "antdefines.h"
#include "antmessage.h"

#include "libant.h"

#include <stdio.h>
#include <assert.h>

#define ENABLE_EXTENDED_MESSAGES // Un - comment to enable extended messages.

#define USER_BAUDRATE			   (57600)  // For AP1, use 50000; for AT3/AP2, use 57600
#define USER_RADIOFREQ			   (35)     // RF Frequency + 2400 MHz

#define USER_ANTCHANNEL			   (0)      // ANT channel to use
#define USER_DEVICENUM			   (49)     // Device number
#define USER_DEVICETYPE			   (1)      // Device type
#define USER_TRANSTYPE			   (1)      // Transmission type

#define USER_NETWORK_KEY         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,}
#define USER_NETWORK_NUM         (0)      // The network key is assigned to this network number


#define MAX_CHANNEL_EVENT_SIZE   (MESG_MAX_SIZE_VALUE)     // Channel event buffer size, assumes worst case extended message size
#define MAX_RESPONSE_SIZE        (MESG_MAX_SIZE_VALUE)     // Protocol response buffer size


// Indexes into message recieved from ANT 
#define MESSAGE_BUFFER_DATA1_INDEX ((UCHAR) 0)
#define MESSAGE_BUFFER_DATA2_INDEX ((UCHAR) 1)
#define MESSAGE_BUFFER_DATA3_INDEX ((UCHAR) 2)
#define MESSAGE_BUFFER_DATA4_INDEX ((UCHAR) 3)
#define MESSAGE_BUFFER_DATA5_INDEX ((UCHAR) 4)
#define MESSAGE_BUFFER_DATA6_INDEX ((UCHAR) 5)
#define MESSAGE_BUFFER_DATA7_INDEX ((UCHAR) 6)
#define MESSAGE_BUFFER_DATA8_INDEX ((UCHAR) 7)
#define MESSAGE_BUFFER_DATA9_INDEX ((UCHAR) 8)
#define MESSAGE_BUFFER_DATA10_INDEX ((UCHAR) 9)
#define MESSAGE_BUFFER_DATA11_INDEX ((UCHAR) 10)
#define MESSAGE_BUFFER_DATA12_INDEX ((UCHAR) 11)
#define MESSAGE_BUFFER_DATA13_INDEX ((UCHAR) 12)
#define MESSAGE_BUFFER_DATA14_INDEX ((UCHAR) 13)



// Local Variables
static UCHAR ucChannelType;               // Channel type as chosen by user (master or slave)
static UCHAR aucChannelBuffer[MAX_CHANNEL_EVENT_SIZE];
static UCHAR aucResponseBuffer[MAX_RESPONSE_SIZE];
static UCHAR aucTransmitBuffer[ANT_STANDARD_DATA_PAYLOAD_SIZE];
static BOOL bDone;
static BOOL bDisplay;


// Declare callback functions into the library.
static BOOL Test_ChannelCallback(UCHAR ucChannel_, UCHAR ucEvent_);
static BOOL Test_ResponseCallback(UCHAR ucChannel_, UCHAR ucMessageId_);
static void PrintMenu();


////////////////////////////////////////////////////////////////////////////////
// main
//
// Usage:
//
// c:\DEMO_DLL.exe [device_no] [channel_type]
//
// ... where
//
// device_no:     USB Device port, starting at 0
// channel_type:  Master = 0, Slave = 1
//
// ... example
//
// c:\Demo_DLL.exe 0 0
// 
// Comment to USB port 0 and open a Master channel
//
// If optional arguements are not supplied, user will 
// be prompted to enter these after the program starts.
//
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

   UCHAR ucDeviceNumber = 0xFF;
   UCHAR ucChannelType = CHANNEL_TYPE_INVALID;


   if(argc > 2)
   {
      ucDeviceNumber = (UCHAR) atoi(argv[1]); 
      ucChannelType = (UCHAR) atoi(argv[2]);     
      
   }

   Test_Init(ucDeviceNumber,ucChannelType);
   Test_Start();
	return 0;
}



////////////////////////////////////////////////////////////////////////////////
// Test_Init
//
// Initialize test parameters.
// 
// ucDeviceNumber_: USB Device Number (0 for first USB stick plugged and so on)
//                  If not specified on command line, 0xFF is passed in as invalid.
// ucChannelType_:  ANT Channel Type. 0 = Master, 1 = Slave
//                  If not specified, 2 is passed in as invalid.
//
////////////////////////////////////////////////////////////////////////////////
void Test_Init(UCHAR ucDeviceNumber_, UCHAR ucChannelType_)
{

	BOOL bStatus;
	
   // Load the ANT DLL functions. 
   if(!ANT_Load())
   {
      printf("Failed to load ANT Library\n");
      exit(0);
   }

   // Get library version
   if(ANT_LibVersionSupport())
   {
      printf("ANT Library Version %s\n", ANT_LibVersion());
   }


   // If no device number was specified on the command line, 
   // prompt the user for input.
   if(ucDeviceNumber_ == 0xFF)
   {
	   printf("Device number?\n"); fflush(stdout);
	   scanf("%u", &ucDeviceNumber_);
   } 
   
   
	ucChannelType = ucChannelType_;
   bDone = FALSE;
   bDisplay = TRUE;
   
   // The device number depends on how many USB sticks have been
   // plugged into the PC. The first USB stick plugged will be 0
   // the next 1 and so on.
   // 
   // The Baud Rate depends on the ANT solution being used. AP1
   // is 50000, all others are 57600   
   bStatus = ANT_Init(ucDeviceNumber_, USER_BAUDRATE);
 	assert(bStatus);


   
   // Assign callback functions. One for serial message responses 
   // and the other for channel events. Each channel event can have 
   // its own callback function defined. Since we are only 
   // going to open one channel, setup one callback function
   // for the channel callback
   ANT_AssignResponseFunction(Test_ResponseCallback, aucResponseBuffer);
   ANT_AssignChannelEventFunction(USER_ANTCHANNEL,Test_ChannelCallback, aucChannelBuffer); 
	
	printf("Initialization was successful!\n"); fflush(stdout);


	return;
}

////////////////////////////////////////////////////////////////////////////////
// Test_Start
//
// Start the Test program.
// 
////////////////////////////////////////////////////////////////////////////////
void Test_Start()
{
   // Print out the menu to start
   PrintMenu();
   
   // If the channel type has not been set at the command line,
   // prompt the user to specify now.
   if(ucChannelType == CHANNEL_TYPE_INVALID)
   {
	   printf("Channel Type? (Master = 0, Slave = 1)\n");
	   scanf("%u", &ucChannelType);
	}
 	
   // Reset system
	printf("Resetting module...\n");
	ANT_ResetSystem();         // Soft reset
	ANT_Nap(1000);             // Need to delay after a reset 
      
	//Start the test by setting network key
	printf("Setting network key...\n");
	UCHAR ucNetKey[8] = USER_NETWORK_KEY;
	
   // Set Network key to start of ANT initialization
   ANT_SetNetworkKey(USER_NETWORK_NUM, ucNetKey);
  
   while(!bDone)
   {   
   
      UCHAR ucChar;
      scanf("%c",&ucChar);
      
      switch(ucChar)
      {
         case 'M':
         case 'm':
         {
            // Printout options
            PrintMenu();
            break;
         }
         case 'Q':
         case 'q':
         {
            // Quit
            
            printf("Close Channel\n");
            ANT_CloseChannel(USER_ANTCHANNEL);
            
            break;
         }
         case 'A':
         case 'a':
         {
            // Send Acknowledged data
            UCHAR aucTempBuffer[] = {1,2,3,4,5,6,7,8};
            ANT_SendAcknowledgedData(USER_ANTCHANNEL, aucTempBuffer);
            break;
         }
         case 'B':
         case 'b':
         {
            // Send Burst Data (10 packets)
            UCHAR aucTempBuffer[8*10];
            
            for (int i = 0; i < 8*10; i++)
               aucTempBuffer[i] = i;   
            
            ANT_SendBurstTransfer(USER_ANTCHANNEL, aucTempBuffer, 10);
            break;
         }
            
         case 'r':
         case 'R':
         {
            // Reset system
            printf("Resetting module...\n");
            ANT_ResetSystem();         // Soft reset
            ANT_Nap(1000);             // Need to delay after a reset 
            
            // Restart the test by setting network key
            printf("Setting network key...\n");
            ANT_SetNetworkKey(USER_NETWORK_NUM, ucNetKey);
            break;
         }
            
         case 'c':
         case 'C':
         {
            // Request capabilites.
            ANT_RequestMessage(USER_ANTCHANNEL, MESG_CAPABILITIES_ID);
            break;
         }
         case 'v':
         case 'V':
         {
            // Request version
            ANT_RequestMessage(USER_ANTCHANNEL, MESG_VERSION_ID);
            break;
         }
         case 'S':
         case 's':
         {
            // Request channel status
            ANT_RequestMessage(USER_ANTCHANNEL, MESG_CHANNEL_STATUS_ID);
            break;
         }
         
         case 'I':
         case 'i':
         {
            // Request channel ID
            ANT_RequestMessage(USER_ANTCHANNEL, MESG_CHANNEL_ID_ID);
            break;
            
         }
         case 'd':
         case 'D':
         {
            bDisplay = !bDisplay;
            
            break;
         }
         default:
         {
            break;
            
         }
      }         
      ANT_Nap(0);
   }
   
   // Clean up ANT
   printf("Disconnecting module...\n");
   
   ANT_Close();
	printf("Test has completed successfully!\n");

	return;
}



////////////////////////////////////////////////////////////////////////////////
// Test_ChannelCallback
//
// Callback function passed to ANT DLL. Called whenever a channel event
// is recieved. 
// 
// ucChannel_: Channel number of event
// ucEvent_: Channel event as described in section 9.5.6 of the messaging document
////////////////////////////////////////////////////////////////////////////////
BOOL Test_ChannelCallback(UCHAR ucChannel_, UCHAR ucEvent_)
{

   BOOL bPrintBuffer = FALSE;
   UCHAR ucDataOffset = MESSAGE_BUFFER_DATA2_INDEX;   // For most data messages


	switch(ucEvent_)
	{   
      case EVENT_TX:
      {
         // This event indicates that a message has just been
         // sent over the air. We take advantage of this event to set
         // up the data for the next message period.                
         static UCHAR ucIncrement = 0;      // Increment the first byte of the buffer
               
         aucTransmitBuffer[0] = ucIncrement++;
      
         // Broadcast data will be sent over the air on 
         // the next message period.
         ANT_SendBroadcastData( USER_ANTCHANNEL,aucTransmitBuffer);
         if(bDisplay)
         {
            // Echo what the data will be over the air on the next message period.  
            printf("Tx:(%d): [%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x]\n",
               USER_ANTCHANNEL,
               aucTransmitBuffer[MESSAGE_BUFFER_DATA1_INDEX],
               aucTransmitBuffer[MESSAGE_BUFFER_DATA2_INDEX],
               aucTransmitBuffer[MESSAGE_BUFFER_DATA3_INDEX],
               aucTransmitBuffer[MESSAGE_BUFFER_DATA4_INDEX],
               aucTransmitBuffer[MESSAGE_BUFFER_DATA5_INDEX],
               aucTransmitBuffer[MESSAGE_BUFFER_DATA6_INDEX],
               aucTransmitBuffer[MESSAGE_BUFFER_DATA7_INDEX],
               aucTransmitBuffer[MESSAGE_BUFFER_DATA8_INDEX]);
         }
         else
         {
            static int iIndex = 0;
            static char ac[] = {'|','/','-','\\'};
            printf("Tx: %c\r",ac[iIndex++]); fflush(stdout);
            iIndex &= 3;
         }
         
         break;
      }
      case EVENT_RX_FLAG_ACKNOWLEDGED:
      case EVENT_RX_FLAG_BURST_PACKET:  
      case EVENT_RX_FLAG_BROADCAST:
      {
         // This is an event generated by the DLL. It is not strictly
         // returned by the ANT part. To enable, must call ANT_RxExtMesgsEnable first.
         
         // This event has a flag at the end of the data payload
         // that indicates additional information. Process this here
         // and then fall-through to process the data payload.
        
         
         UCHAR ucFlag = aucChannelBuffer[MESSAGE_BUFFER_DATA10_INDEX]; 
                  
         if(bDisplay && ucFlag & ANT_EXT_MESG_BITFIELD_DEVICE_ID)
         {
            // Channel ID of the device that we just recieved a message from.
            USHORT usDeviceNumber = aucChannelBuffer[MESSAGE_BUFFER_DATA11_INDEX] | (aucChannelBuffer[MESSAGE_BUFFER_DATA12_INDEX] << 8); 
            UCHAR ucDeviceType =  aucChannelBuffer[MESSAGE_BUFFER_DATA13_INDEX];
            UCHAR ucTransmissionType = aucChannelBuffer[MESSAGE_BUFFER_DATA14_INDEX];
         
            printf("Chan ID(%d/%d/%d) - ", usDeviceNumber, ucDeviceType, ucTransmissionType);
         }
         // INTENTIONAL FALLTHROUGH
      }
      case EVENT_RX_ACKNOWLEDGED:
      case EVENT_RX_BURST_PACKET:  
      case EVENT_RX_BROADCAST:
      {
         // This is an event generated by the DLL. It is not strictly
         // returned by the ANT part.
         
         // Display recieved message
         bPrintBuffer = TRUE;
         ucDataOffset = MESSAGE_BUFFER_DATA2_INDEX;   // For most data messages
         
         if(bDisplay)
         {
            if(ucEvent_ == EVENT_RX_ACKNOWLEDGED || ucEvent_ == EVENT_RX_FLAG_ACKNOWLEDGED)
               printf("Acked Rx:(%d): ", aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX]);             
            else if(ucEvent_ == EVENT_RX_BURST_PACKET || ucEvent_ == EVENT_RX_FLAG_BURST_PACKET)
               printf("Burst(0x%02x) Rx:(%d): ", ((aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX] & 0xE0) >> 5), aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX] & 0x1F );             
            else      
               printf("Rx:(%d): ", aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX]);             
         }
         break;
      }
      
      case EVENT_RX_EXT_ACKNOWLEDGED:
      case EVENT_RX_EXT_BURST_PACKET:
      case EVENT_RX_EXT_BROADCAST:
      {
         // This is an event generated by the DLL. It is not strictly
         // returned by the ANT part. To enable, must call ANT_RxExtMesgsEnable first.
         
         // The "extended" part of this message is the 4-byte channel
         // id of the device that we recieved this message from. This event
         // is only available on the AT3. The AP2 uses the EVENT_RX_FLAG_BROADCAST
         // as shown above.
         
         // Channel ID of the device that we just recieved a message from.
         USHORT usDeviceNumber = aucChannelBuffer[MESSAGE_BUFFER_DATA2_INDEX] | (aucChannelBuffer[MESSAGE_BUFFER_DATA3_INDEX] << 8); 
         UCHAR ucDeviceType =  aucChannelBuffer[MESSAGE_BUFFER_DATA4_INDEX];
         UCHAR ucTransmissionType = aucChannelBuffer[MESSAGE_BUFFER_DATA5_INDEX];
         
         bPrintBuffer = TRUE;
         ucDataOffset = MESSAGE_BUFFER_DATA6_INDEX;   // For most data messages
         
         if(bDisplay)
         {
         
            // Display the channel id
            printf("Chan ID(%d/%d/%d) ", usDeviceNumber, ucDeviceType, ucTransmissionType );
         
            if(ucEvent_ == EVENT_RX_EXT_ACKNOWLEDGED)
               printf("- Acked Rx:(%d): ", aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX]);             
            else if(ucEvent_ == EVENT_RX_EXT_BURST_PACKET)
               printf("- Burst(0x%02x) Rx:(%d): ", ((aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX] & 0xE0) >> 5), aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX] & 0x1F );             
            else      
               printf("- Rx:(%d): ", aucChannelBuffer[MESSAGE_BUFFER_DATA1_INDEX]);             
         } 
         
         break;
      }
      case EVENT_RX_SEARCH_TIMEOUT:
      {
         printf("Search Timeout on channel %d\n",ucChannel_); 
         break;
      }
      case EVENT_RX_FAIL:
      {
         printf("Rx Fail on channel %d\n", ucChannel_);
         break;
      }
      case EVENT_TRANSFER_RX_FAILED:
      {
         printf("Burst recieve has failed\n", ucChannel_);
         break;
      }
      case EVENT_TRANSFER_TX_COMPLETED:     
      {
         printf("Tranfer Completed.\n");
         break;
      }
      case EVENT_TRANSFER_TX_FAILED:        
      {
         printf("Tranfer Failed.\n");      
         break;
      }
      case EVENT_CHANNEL_CLOSED:            
      {
         // This event should be used to determine that the channel is closed.
         printf("Channel Closed\n");
         printf("Unassign Channel\n");
         ANT_UnAssignChannel(USER_ANTCHANNEL);
         break;
      }
      case EVENT_RX_FAIL_GO_TO_SEARCH:      
      {
         printf("Goto Search.\n");
         break;
      }
      case EVENT_CHANNEL_COLLISION:         
      {
         printf("Channel (%d) Collision\n");
         break;
      }
      case EVENT_TRANSFER_TX_START:        
      {
         printf("Burst Started\n");      
         break;
      }
      default:
      {
         printf("Unknown Channel(%d) Event %d\n",ucChannel_,ucEvent_); 
         break;
      }
      
   }
   
   // If we recieved a data message, diplay its contents here. 
   if(bPrintBuffer)
   {
      if(bDisplay)
      {
         printf("[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x]\n",
            aucChannelBuffer[ucDataOffset + 0],             
            aucChannelBuffer[ucDataOffset + 1],
            aucChannelBuffer[ucDataOffset + 2],
            aucChannelBuffer[ucDataOffset + 3],
            aucChannelBuffer[ucDataOffset + 4],
            aucChannelBuffer[ucDataOffset + 5],
            aucChannelBuffer[ucDataOffset + 6],
            aucChannelBuffer[ucDataOffset + 7]);
   
      }
      else
      {
         static int iIndex = 0;
         static char ac[] = {'|','/','-','\\'};
         printf("Rx: %c\r",ac[iIndex++]); fflush(stdout);
         iIndex &= 3;

      }
   }
   
   return(TRUE);
}



////////////////////////////////////////////////////////////////////////////////
// Test_ResponseCallback
//
// Callback function passed to ANT DLL. Called whenever a message is recieved from ANT
// unless that message is a channel event message. 
// 
// ucChannel_: Channel number of relevent to the message
// ucMessageId_: Message ID of the message recieved from ANT.
////////////////////////////////////////////////////////////////////////////////
BOOL Test_ResponseCallback(UCHAR ucChannel_, UCHAR ucMessageId_)
{
   BOOL bSuccess = FALSE;
      
   switch(ucMessageId_)
   {
      case MESG_STARTUP_MESG_ID:
      {
         printf("RESET Complete, reason: ");
         
         UCHAR ucReason = aucResponseBuffer[MESSAGE_BUFFER_DATA1_INDEX];
         
         if(ucReason == RESET_POR)
            printf("RESET_POR");
         
         if(ucReason & RESET_SUSPEND)
            printf("RESET_SUSPEND ");
         if(ucReason & RESET_SYNC)
            printf("RESET_SYNC ");
         if(ucReason & RESET_CMD)
            printf("RESET_CMD ");
         if(ucReason & RESET_WDT)
            printf("RESET_WDT ");
         if(ucReason & RESET_RST)
            printf("RESET_RST ");
            
         printf("\n");   
           
         break;
      }
      case MESG_CAPABILITIES_ID:
      {
         printf("CAPABILITIES:\n");
         printf("   Max ANT Channels: %d\n",aucResponseBuffer[MESSAGE_BUFFER_DATA1_INDEX]);
         printf("   Max ANT Networks: %d\n",aucResponseBuffer[MESSAGE_BUFFER_DATA2_INDEX]);
         
         UCHAR ucStandardOptions = aucResponseBuffer[MESSAGE_BUFFER_DATA3_INDEX];
         UCHAR ucAdvanced = aucResponseBuffer[MESSAGE_BUFFER_DATA4_INDEX];
         UCHAR ucAdvanced2 = aucResponseBuffer[MESSAGE_BUFFER_DATA5_INDEX];

         printf("Standard Options:\n");
            

         if( ucStandardOptions & CAPABILITIES_NO_RX_CHANNELS )
            printf("CAPABILITIES_NO_RX_CHANNELS\n");
         if( ucStandardOptions & CAPABILITIES_NO_TX_CHANNELS )
            printf("CAPABILITIES_NO_TX_CHANNELS\n");
         if( ucStandardOptions & CAPABILITIES_NO_RX_MESSAGES )
            printf("CAPABILITIES_NO_RX_MESSAGES\n");
         if( ucStandardOptions & CAPABILITIES_NO_TX_MESSAGES )
            printf("CAPABILITIES_NO_TX_MESSAGES\n");
         if( ucStandardOptions & CAPABILITIES_NO_ACKD_MESSAGES )
            printf("CAPABILITIES_NO_ACKD_MESSAGES\n");
         if( ucStandardOptions & CAPABILITIES_NO_BURST_TRANSFER )
            printf("CAPABILITIES_NO_BURST_TRANSFER\n");

         printf("Advanced Options:\n");

         if( ucAdvanced & CAPABILITIES_OVERUN_UNDERRUN )
            printf("CAPABILITIES_OVERUN_UNDERRUN\n");
         if( ucAdvanced & CAPABILITIES_NETWORK_ENABLED )
            printf("CAPABILITIES_NETWORK_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_AP1_VERSION_2 )
            printf("CAPABILITIES_AP1_VERSION_2\n");
         if( ucAdvanced & CAPABILITIES_SERIAL_NUMBER_ENABLED )
            printf("CAPABILITIES_SERIAL_NUMBER_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_PER_CHANNEL_TX_POWER_ENABLED )
            printf("CAPABILITIES_PER_CHANNEL_TX_POWER_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_LOW_PRIORITY_SEARCH_ENABLED )
            printf("CAPABILITIES_LOW_PRIORITY_SEARCH_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_SCRIPT_ENABLED )
            printf("CAPABILITIES_SCRIPT_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_SEARCH_LIST_ENABLED )
            printf("CAPABILITIES_SEARCH_LIST_ENABLED\n");

         
         printf("Advanced 2 Options 1:\n");
         
         if( ucAdvanced2 & CAPABILITIES_LED_ENABLED )
            printf("CAPABILITIES_LED_ENABLED\n");
         if( ucAdvanced2 & CAPABILITIES_EXT_MESSAGE_ENABLED )
            printf("CAPABILITIES_EXT_MESSAGE_ENABLED\n");
         if( ucAdvanced2 & CAPABILITIES_SCAN_MODE_ENABLED )
            printf("CAPABILITIES_SCAN_MODE_ENABLED\n");
         if( ucAdvanced2 & CAPABILITIES_RESERVED )
            printf("CAPABILITIES_RESERVED\n");
         if( ucAdvanced2 & CAPABILITIES_PROX_SEARCH_ENABLED )
            printf("CAPABILITIES_PROX_SEARCH_ENABLED\n");
         if( ucAdvanced2 & CAPABILITIES_EXT_ASSIGN_ENABLED )
            printf("CAPABILITIES_EXT_ASSIGN_ENABLED\n");
         if( ucAdvanced2 & CAPABILITIES_FREE_1 )
            printf("CAPABILITIES_FREE_1\n");
         if( ucAdvanced2 & CAPABILITIES_FIT1_ENABLED )
            printf("CAPABILITIES_FIT1_ENABLED\n");
            
         break;
      }
      case MESG_CHANNEL_STATUS_ID:
      {
         printf("Got Status\n");
         
          
         char astrStatus[][32] = {  "STATUS_UNASSIGNED_CHANNEL",
                                    "STATUS_ASSIGNED_CHANNEL",
                                    "STATUS_SEARCHING_CHANNEL",
                                    "STATUS_TRACKING_CHANNEL"   };
         
         UCHAR ucStatusByte = aucResponseBuffer[MESSAGE_BUFFER_DATA2_INDEX] & STATUS_CHANNEL_STATE_MASK; // MUST MASK OFF THE RESERVED BITS

         printf("STATUS: %s\n",astrStatus[ucStatusByte]); 
         break;
      }      
      case MESG_CHANNEL_ID_ID:
      {
      
         // Channel ID of the device that we just recieved a message from.
         USHORT usDeviceNumber = aucResponseBuffer[MESSAGE_BUFFER_DATA2_INDEX] | (aucResponseBuffer[MESSAGE_BUFFER_DATA3_INDEX] << 8); 
         UCHAR ucDeviceType =  aucResponseBuffer[MESSAGE_BUFFER_DATA4_INDEX];
         UCHAR ucTransmissionType = aucResponseBuffer[MESSAGE_BUFFER_DATA5_INDEX];
      
      
         printf("CHANNEL ID: (%d/%d/%d)\n", usDeviceNumber, ucDeviceType, ucTransmissionType);
         
         
         break;
      }    
      case MESG_VERSION_ID:
      {     
      
         printf("VERSION: %s\n", (char*) &aucResponseBuffer[MESSAGE_BUFFER_DATA1_INDEX]);
         break;
      }
      
      
      
      case MESG_RESPONSE_EVENT_ID:
      {   
     
         switch(aucResponseBuffer[MESSAGE_BUFFER_DATA3_INDEX])
         {
            case RESPONSE_NO_ERROR:
            {
               switch (aucResponseBuffer[MESSAGE_BUFFER_DATA2_INDEX])
               {
                  case MESG_NETWORK_KEY_ID:
                  {
			            printf("Network Key Set\n");
	                  printf("Assigning channel...\n");
			      
                     //Find out channel type
			            do
			            {
				            if(ucChannelType == CHANNEL_TYPE_MASTER)
				            {
					            bSuccess = ANT_AssignChannel(USER_ANTCHANNEL, PARAMETER_TX_NOT_RX, USER_NETWORK_NUM);
				            }
				            else if(ucChannelType == CHANNEL_TYPE_SLAVE)
				            {
					            bSuccess = ANT_AssignChannel(USER_ANTCHANNEL, 0, USER_NETWORK_NUM);
				            }
				            else
				            {
					            ucChannelType = CHANNEL_TYPE_INVALID;
					            printf("Error: Invalid channel type.\n");
				            }
			            }
                     while(ucChannelType == CHANNEL_TYPE_INVALID);
                  
                     break;
                  }
		            case MESG_ASSIGN_CHANNEL_ID:
		            {
			            printf("Channel Assigned\n");
			            printf("Setting Channel ID...\n");
			            bSuccess = ANT_SetChannelId(USER_ANTCHANNEL, USER_DEVICENUM, USER_DEVICETYPE, USER_TRANSTYPE);
			            break;
		            }

		            case MESG_CHANNEL_ID_ID:
		            {
			            printf("Channel ID Set\n");
			            printf("Setting RF Frequency...\n");
			            bSuccess = ANT_SetChannelRFFreq(USER_ANTCHANNEL, USER_RADIOFREQ);
			            break;
		            }

		            case MESG_CHANNEL_RADIO_FREQ_ID:
		            {
			            printf("Radio Frequency set\n");
			            printf("Opening channel...\n");
			            bSuccess = ANT_OpenChannel(USER_ANTCHANNEL);
			            break;
		            }

		            case MESG_OPEN_CHANNEL_ID:
                  {
			            printf("Channel opened\n");
                     
#if defined (ENABLE_EXTENDED_MESSAGES)                     
                     printf("Enable extended message\n");
                     ANT_RxExtMesgsEnable(TRUE);
#endif
			            break;
                  }
		            case MESG_UNASSIGN_CHANNEL_ID:
		            {
			            printf("Unassigned Channel\n");
                     printf("Press any key to exit\n");
                     bDone = TRUE;
                     break;
		            } 
                  case MESG_CLOSE_CHANNEL_ID:
                  {
                     break;
                  } 
               
                  case MESG_RX_EXT_MESGS_ENABLE_ID:
                  {
                     printf("Extended messages enabled\n");
                     break;
                  }
                             
                  default:
                  {
                     break;
                  }
               }
               break;
            }
            case CHANNEL_IN_WRONG_STATE:          
            {
               printf("Channel in wrong state\n");
               break;
            }
            case CHANNEL_NOT_OPENED:              
            {
               printf("Channel not opened\n");
               break;
            }
            case CHANNEL_ID_NOT_SET: //??             
            {
               printf("Channel ID not set\n");
               break;
            }
            case CLOSE_ALL_CHANNELS: // Start RX Scan mode              
            {
               printf("Close all channels\n");
               break;
            }
            case TRANSFER_IN_PROGRESS: // TO ack message ID            
            {
               printf("Tranfer in progress\n");
               break;
            }
            case TRANSFER_SEQUENCE_NUMBER_ERROR:  
            {
               printf("Transfer sequence number error\n");
               break;
            }
            case TRANSFER_IN_ERROR:               
            {
   
               printf("Transfer in error\n");
               break;
            }
            case INVALID_MESSAGE:                 
            {
               printf("Invalid message\n");
               break;
            }
            case INVALID_NETWORK_NUMBER:          
            {
               printf("Invalid network number\n");
               break;
            }
            case INVALID_LIST_ID:                 
            {
               printf("Invalid list ID\n");
               break;
            }
            case INVALID_SCAN_TX_CHANNEL:         
            {
               printf("Invalid Scanning transmit channel\n");
               break;
            }
            case INVALID_PARAMETER_PROVIDED:      
            {
               printf("Invalid parameter provided\n");
   
               break;
            }
            case EVENT_QUE_OVERFLOW:              
            {
               printf("Queue overflow\n");
               break;
            }
            default:
            {
               printf("Unknown Error Code %d to message %d\n",aucResponseBuffer[MESSAGE_BUFFER_DATA3_INDEX],aucResponseBuffer[MESSAGE_BUFFER_DATA2_INDEX]);
               break; 
            }
         }                    
      }
   } 
   return(TRUE);
}




////////////////////////////////////////////////////////////////////////////////
// PrintMenu
//
// Start the Test program.
// 
////////////////////////////////////////////////////////////////////////////////
void PrintMenu()
{
   // Printout options
   printf("\n");   
   printf("M - Print this menu\n");
   printf("A - Send Acknowledged message\n");
   printf("B - Send Burst message\n");
   printf("R - Reset\n");
   printf("C - Request Capabilites\n");  
   printf("V - Request Version\n");  
   printf("I - Request Channel ID\n");  
   printf("S - Request Status\n");  
   printf("D - Toggle Display\n");  
   printf("Q - Quit\n");  
   printf("\n");   
   fflush(stdout);
}









