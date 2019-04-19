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
#include "ioboard.h"
#include "antfs.h"
#include "mem.h"
#include "timer.h"
#include "printf.h"


extern volatile USHORT usLowPowerMode;               // low power mode control

// Configuration options
#define ANTFS_CLIENT_ESN      ((ULONG) 0xABCDEF12)   // serial number of client device
#define ANTFS_CLIENT_DEV_TYPE   ((USHORT) 416)       // beacon device type
#define ANTFS_CLIENT_MANUF_ID   ((USHORT) 2)         // beacon manufacturer ID
#define ANTFS_CLIENT_NAME       { "Ref Design" }     // client's friendly name
#define ANTFS_CLIENT_PASSKEY    { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 }   // length must match ANTFS_PASSKEY_SIZE

static ANTFS_EVENT_RETURN stANTFSEvent;
static ANTFS_DIR_STRUCT stTempDirStruct;    // Current directory file structure
static ANTFS_REQUEST_INFO stResponseInfo;   // Parameters for response to a download request

static USHORT usFileIndex;                  // Index of the current file downloaded/uploaded
static ULONG ulFileOffset;                  // Current offset
static USHORT usCurrentCRC;                 // Current CRC
static BOOL bPairingRequest;
static BOOL bUploadSuccess;

static const UCHAR aucFriendlyName[] = ANTFS_CLIENT_NAME;   // Client's friendly name
static const UCHAR aucPassKey[] = ANTFS_CLIENT_PASSKEY;     // Authentication string (passkey)

static void ProcessButtonState();
static void ProcessAntEvents(UCHAR* pucEventBuffer_);
static void ProcessANTFSEvents(ANTFS_EVENT_RETURN* pstEvent_);

////////////////////////////////////////////////////////////////////////////
// main
//
// main function  
//
// Configures ANT-FS client
//
// \return: This function does not return. 
////////////////////////////////////////////////////////////////////////////
void main_antfs_client()
{
   ANTFS_PARAMS stParams = {ANTFS_CLIENT_ESN, ANTFS_CLIENT_DEV_TYPE, ANTFS_CLIENT_MANUF_ID, ANTFS_LINK_FREQ, ANTFS_DEFAULT_BEACON | DATA_AVAILABLE_FLAG_MASK, aucPassKey, aucFriendlyName}; 

   UCHAR* pucRxBuffer;

   // Initialize ANT-FS
   ANTFS_Init(&stParams);
   bPairingRequest = FALSE;

   printf("Initializing ANT-FS client\n");

   // There is no need to delay after this ANT RESET command. 
   // The delay is built into the ANTInterface layer. 
   // Once a 500 ms timeout has been reached, the ANTInterface will issue a RESPONSE_NO_ERROR code. 
   // This can be used to send further commands (in this case, open the ANT-FS beacon)
   ANT_Reset();

   // Main loop
   while(TRUE)
   {
      usLowPowerMode = 0;//LPM1_bits;                    // Initialize the low power mode -  move to processor specific file and generic function
      
      IOBoard_Check_Button();                            // Check the button state
      ProcessButtonState();
      
      if(ANTFS_CheckEvents(&stANTFSEvent))               // Check for ANT-FS events
        ProcessANTFSEvents(&stANTFSEvent);               // Process ANT-FS Events
      
      pucRxBuffer = ANTInterface_Transaction();          // Check if any data has been received from serial interface
      
      if(pucRxBuffer)
      {
         if(ANTFS_ProcessMessage(pucRxBuffer))           // Process messages
            usLowPowerMode = 0;   

         ProcessAntEvents(pucRxBuffer);                  // Process as generic ANT Events 
    
         ANTInterface_Complete();                        // Release the serial buffer
      }

      _BIS_SR(usLowPowerMode);                           // go to sleep if we can  -  move to processor specific file and generic function
   }
}


////////////////////////////////////////////////////////////////////////////
// ProcessAntEvents
//
// Generic ANT event handling
//
// \return: N/A 
///////////////////////////////////////////////////////////////////////////
void ProcessAntEvents(UCHAR* pucEventBuffer_)
{
   if(pucEventBuffer_)
   {
      // Response no error events
      if((pucEventBuffer_[BUFFER_INDEX_MESG_ID] == MESG_RESPONSE_EVENT_ID) && (pucEventBuffer_[BUFFER_INDEX_RESPONSE_CODE] == RESPONSE_NO_ERROR))
      {
         switch(pucEventBuffer_[BUFFER_INDEX_RESPONSE_MESG_ID])
         {
            case MESG_SYSTEM_RESET_ID:
            {
               printf("Opening Beacon...\n");
               ANTFS_OpenBeacon();   // Open ANT-FS beacon after system reset
               break;
            }
            case MESG_NETWORK_KEY_ID:
            {
               printf("Assigned Network Key\n");
               break;
            }
            case MESG_ASSIGN_CHANNEL_ID:
            {
               printf("Assigned Channel\n");
               break;
            }
            case MESG_CHANNEL_ID_ID:
            {
               printf("Assigned Channel ID\n");
               break;
            }
            case MESG_CHANNEL_RADIO_FREQ_ID:
            {
               printf("Assigned RF Frequency\n");
               break;
            }
            case MESG_CHANNEL_MESG_PERIOD_ID:
            {
               printf("Assigned Message Period\n");
               break;
            }
            case MESG_RADIO_TX_POWER_ID:
            {
               printf("Set Transmit Power\n");
               break;
            }
            case MESG_OPEN_CHANNEL_ID:
            {
               printf("Opened Channel\n");
               break;
            }
            case MESG_CLOSE_CHANNEL_ID:
            {
               printf("Closed Channel\n");
               break;
            }
            default:
            {
               break;
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////
// ProcessANTFSvents
//
// ANT-FS event processor  
//
// Processes events recieved from the ANT-FS client
//
// \return: N/A 
///////////////////////////////////////////////////////////////////////////
void ProcessANTFSEvents(ANTFS_EVENT_RETURN* pstEvent_)
{
   switch(pstEvent_->eEvent)
   {
      case ANTFS_EVENT_OPEN_COMPLETE:
      {
         printf("ANTFS_EVENT_OPEN_COMPLETE\n");
         break;
      }
      case ANTFS_EVENT_OPEN_FAIL:
      {
         printf("ANTFS_EVENT_OPEN_FAIL\n");
         break;
      }
      case ANTFS_EVENT_CLOSE_COMPLETE:
      {
         printf("ANTFS_EVENT_CLOSE_COMPLETE\n");
         break;
      }
      case ANTFS_EVENT_LINK:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;     // TURN OFF LED 3
         bPairingRequest = FALSE;
         printf("ANTFS_EVENT_LINK\n");
         break;
      }
      case ANTFS_EVENT_AUTH:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;     // TURN OFF LED 3
         bPairingRequest = FALSE;
         printf("ANTFS_EVENT_AUTH\n");
         break;
      }
      case ANTFS_EVENT_TRANS:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;     // TURN OFF LED 3
         bPairingRequest = FALSE;
         printf("ANTFS_EVENT_TRANS\n");
         break;
      }
      case ANTFS_EVENT_PAIRING_REQUEST:
      {
         char* pcName = ANTFS_GetHostName();       // Get friendly name
         IOBOARD_LED3_OUT &= ~IOBOARD_LED3_BIT;    // TURN ON LED 3
         bPairingRequest = TRUE;
         printf("ANTFS_EVENT_PAIRING_REQUEST\n");       
            if (pcName)
               printf("Host Name: %s\n", pcName);     // Request user to accept/reject pairing request
         break;
      }
      case ANTFS_EVENT_PAIRING_TIMEOUT:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;     // TURN OFF LED 3
         bPairingRequest = FALSE;
         printf("ANTFS_EVENT_PAIRING_TIMEOUT\n");
         break;
      }
      case ANTFS_EVENT_DOWNLOAD_REQUEST:
      {
         UCHAR ucResponse = RESPONSE_MESSAGE_OK;   // Initialize as accept
         IOBOARD_LED3_OUT &= ~IOBOARD_LED3_BIT;    // TURN ON LED 3  
         printf("ANTFS_EVENT_DOWNLOAD_REQUEST\n");

         // Grab request info
         usFileIndex = pstEvent_->usFileIndex;

         if (Mem_GetDir(usFileIndex, &stTempDirStruct))  // Read file information from directory
         {
            if (!(stTempDirStruct.ucGeneralFlags & ANTFS_DIR_READ_MASK))    // Check permissions
            {
               ucResponse = RESPONSE_MESSAGE_NOT_AVAILABLE;
               printf("Download request denied: file n/a for reading\n");
            }
            // Set response parameters
            stResponseInfo.uIndex.usData = usFileIndex;                 // File index
            stResponseInfo.uSize.ulData = stTempDirStruct.ulSize;       // File size (per directory)
            stResponseInfo.ulMaxSize = stTempDirStruct.ulSize;          // File is being read, so maximum size is the file size
            stResponseInfo.uBlockSize.ulData = stTempDirStruct.ulSize;  // Send the entire file in a single block if possible
         }
         else    // Index not found
         {
            ucResponse = RESPONSE_MESSAGE_NOT_EXIST;    // File does not exist
            stResponseInfo.uIndex.usData = 0;           // Set all response parameters to zero
            stResponseInfo.uSize.ulData = 0;
            stResponseInfo.ulMaxSize = 0;
            stResponseInfo.uBlockSize.ulData = 0;
            printf("Download request denied: file does not exist\n");
         }
         ANTFS_DownloadRequestResponse(ucResponse, &stResponseInfo); 
         usLowPowerMode = 0;
         break;
      }
      case ANTFS_EVENT_DOWNLOAD_START:
      {
         printf("ANTFS_EVENT_DOWNLOAD_START");
         break;
      }
      case ANTFS_EVENT_DOWNLOAD_REQUEST_DATA:
      {
      	 // !!! This reference design does not interact with a file system, and it does not account for latency
		 // for reading or writing a file from EEPROM/flash.
		 // Prefetching the file might be useful to be able to feed data to download in order to maintain the
		 // burst timing      	
         if (usFileIndex == pstEvent_->usFileIndex)     // Only send data for file index matching the download request
         {
            UCHAR aucTempBuffer[ANTFS_BURST_BLOCK_SIZE*8];  // Burst data block size * 8 bytes per burst packet
            ULONG ulOffset = pstEvent_->ulOffset;           // Offset specified by client
            ULONG ulDataBytes = pstEvent_->ulBytes;         // Size of requested block of data
            Mem_ReadFile(usFileIndex, ulOffset, aucTempBuffer, ulDataBytes);    // Read block of data from memory
            ANTFS_DownloadInputData(usFileIndex, ulOffset, ulDataBytes, aucTempBuffer);  // Download block of data
            usLowPowerMode = 0;
         }
         break;
      }
      case ANTFS_EVENT_DOWNLOAD_COMPLETE:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;        // TURN OFF LED 3
         printf("ANTFS_EVENT_DOWNLOAD_COMPLETE\n");
         break;
      }
      case ANTFS_EVENT_DOWNLOAD_FAIL:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;        // TURN OFF LED 3
         printf("ANTFS_EVENT_DOWNLOAD_FAIL\n");
         break;
      }
      case ANTFS_EVENT_UPLOAD_REQUEST:
      {
         UCHAR ucResponse = RESPONSE_MESSAGE_OK;      // Initialize as accept
         printf("ANTFS_EVENT_UPLOAD_REQUEST\n");
         if((pstEvent_->ulOffset == MAX_ULONG))       // If requesting to resume an upload
         {
            if(usFileIndex != pstEvent_->usFileIndex)
            {
               // We do not have a save point for this file
               ulFileOffset = 0;
               usCurrentCRC = 0;
            }
         }
         else    // This is a new upload
         {
            ulFileOffset = pstEvent_->ulOffset; // Use requested offset 
            usCurrentCRC = 0;                   // Reset CRC
         }

         usFileIndex = pstEvent_->usFileIndex;   // Get requested file index

         if (Mem_GetDir(usFileIndex, &stTempDirStruct))  // Read file information from directory
         { 
            if (!(stTempDirStruct.ucGeneralFlags & ANTFS_DIR_WRITE_MASK))    // Check permissions
            {
               ucResponse = RESPONSE_MESSAGE_NOT_AVAILABLE;
               printf("Upload request denied: file n/a for writing\n");
            }
            // Set response parameters
            stResponseInfo.uIndex.usData = usFileIndex;                 // File index
            stResponseInfo.uSize.ulData = ulFileOffset;                 // Current valid file size is the last offset written to the file
            stResponseInfo.ulMaxSize = stTempDirStruct.ulSize;          // Space available for writing is the file size, as specified on directory
            stResponseInfo.uBlockSize.ulData = stTempDirStruct.ulSize;  // Get the entire file in a single burst if possible
            stResponseInfo.usFileCRC = usCurrentCRC;                    // Last valid CRC
         }
         else    // Index not found
         {
            ucResponse = RESPONSE_MESSAGE_NOT_EXIST;        // File does not exist
            stResponseInfo.uIndex.usData = usFileIndex;
            stResponseInfo.uSize.ulData = 0;                // Set all response parameters to zero
            stResponseInfo.ulMaxSize = 0;
            stResponseInfo.uBlockSize.ulData = 0;
            stResponseInfo.usFileCRC = 0;
            printf("Upload request denied: file does not exist\n");
         }
         bUploadSuccess = TRUE;
         ANTFS_UploadRequestResponse(ucResponse, &stResponseInfo);
         usLowPowerMode = 0;
         break;
      }
      case ANTFS_EVENT_UPLOAD_START:
      {
         IOBOARD_LED3_OUT &= ~IOBOARD_LED3_BIT;    // TURN ON LED 3  
         printf("ANTFS_EVENT_UPLOAD_START\n");
         break;
      }
      case ANTFS_EVENT_UPLOAD_DATA:
      {
      	 // !!! This reference design does not interact with a file system, and it does not account for latency
		 // for reading or writing a file from EEPROM/flash.
		 // Buffering and other strategies might be useful to handle a received upload while maintaining the
		 // burst timing 
         if (bUploadSuccess && (usFileIndex == pstEvent_->usFileIndex))       // Check it is the right index
         {
            ULONG ulOffset = pstEvent_->ulOffset;         // Offset requested for upload
            ULONG ulDataBytes = pstEvent_->ulBytes;       // Size of current block of data    

            if (!Mem_WriteFile(usFileIndex, ulOffset, pstEvent_->aucData, ulDataBytes))    // Write data to file
            {
               bUploadSuccess = FALSE;     // Failed to write data to system; do not attempt to write any more data after this, and set upload response as FAIL
               printf("Failed to write file to system\n");
               printf("Current offset %i, ", ulOffset);
            }
            else
            {
               // If data was written successfully
               ulFileOffset = ulOffset + ulDataBytes;      // Update offset
               usCurrentCRC = pstEvent_->usCRC;            // Update current CRC
            }
         }
         break;
      }
      case ANTFS_EVENT_UPLOAD_FAIL:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;     // TURN OFF LED 3
         printf("ANTFS_EVENT_UPLOAD_FAIL\n");
         ANTFS_UploadResponse(FALSE);
         usLowPowerMode = 0;
         break;
      }
      case ANTFS_EVENT_UPLOAD_COMPLETE:
      {
         IOBOARD_LED3_OUT |= IOBOARD_LED3_BIT;     // TURN OFF LED 3 
         printf("ANTFS_EVENT_UPLOAD_COMPLETE\n");
         printf("Last offset written  %i bytes, ", ulFileOffset);
         printf("CRC %i\n", usCurrentCRC);
         ANTFS_UploadResponse(bUploadSuccess);
         if(bUploadSuccess)
            ulFileOffset = 0;
         usLowPowerMode = 0;
         break;
      }
      case ANTFS_EVENT_ERASE_REQUEST:
      {
         UCHAR ucResponse = RESPONSE_MESSAGE_OK;         // initialize as accept
         usFileIndex = pstEvent_->usFileIndex;           // File index
         printf("ANTFS_EVENT_ERASE_REQUEST\n"); 

         if(!usFileIndex)        // Should not delete the directory
         {
            ucResponse = RESPONSE_MESSAGE_FAIL;
            printf("Erase request denied: can not delete directory\n");
         }
         if (Mem_GetDir(usFileIndex, &stTempDirStruct))  // Read file information from directory
         {
            if (!(stTempDirStruct.ucGeneralFlags & ANTFS_DIR_ERASE_MASK))    // Check permissions
            {
               ucResponse = RESPONSE_MESSAGE_FAIL;
               printf("Erase request denied: file n/a for erasing\n");
            }
            else
            {
               // Erase file
               if(!Mem_EraseFile(usFileIndex))
                  ucResponse = RESPONSE_MESSAGE_FAIL;
            }
         }
         else    // Index not found
         {
            ucResponse = RESPONSE_MESSAGE_FAIL;    // File does not exist
            printf("Erase request denied: file does not exist\n");
         }
         ANTFS_EraseRequestResponse(ucResponse);
         usLowPowerMode = 0;
         break;
      }
      default:
      {
         break;
      }
   }
}


////////////////////////////////////////////////////////////////////////////
// ProcessButtonState
//
// IO Board button state processing
//
// Processes state of buttons in IO board
// Pressing Button 3 accepts a pairing request
// Pressing Button 1 rejects a pairing request
//
// \return: N/A 
///////////////////////////////////////////////////////////////////////////
void ProcessButtonState()
{
   UCHAR ucButtonStates = IOBoard_Button_Pressed();

   if(ucButtonStates & BUTTON3_STATE_PRESSED)
   {
      if(bPairingRequest)
      {
         ANTFS_PairingResponse(TRUE);               // Accept pairing request
         usLowPowerMode = 0;    
         bPairingRequest = FALSE;
         printf("Pairing request accepted\n");
      }
      IOBoard_Button_Clear(BUTTON3_STATE_OFFSET);
   }
   else if(ucButtonStates & BUTTON1_STATE_PRESSED)
   {
      if(bPairingRequest)
      {
         ANTFS_PairingResponse(FALSE);                  // Reject pairing request
         usLowPowerMode = 0;    
         bPairingRequest = FALSE;
         printf("Pairing request rejected\n");
         }
         IOBoard_Button_Clear(BUTTON1_STATE_OFFSET);
      }
}


