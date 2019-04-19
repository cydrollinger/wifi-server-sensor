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
#include "antinterface.h"
#include "antfs.h"
#include "crc.h"
#include "timer.h"
#include "string.h"


//////////////////////////////////////////////////////////////////////////////////
// Private Definitions
//////////////////////////////////////////////////////////////////////////////////

#define ANTFS_CONNECTION_TYPE_OFFSET         ((UCHAR) 0x00)
#define ANTFS_COMMAND_OFFSET                 ((UCHAR) 0x01)
#define ANTFS_RESPONSE_OFFSET                ((UCHAR) 0x01)
#define ANTFS_CONTROL_OFFSET                 ((UCHAR) 0x02)
#define ANTFS_DATA_OFFSET                     MESG_DATA_OFFSET + 1    // (3) start of actual data

#define ANTFS_BEACON_ID                      ((UCHAR) 0x43)
#define ANTFS_COMMAND_ID                     ((UCHAR) 0x44)

// Beacon definitions
#define STATUS1_OFFSET                       ((UCHAR) 0x01)
#define STATUS2_OFFSET                       ((UCHAR) 0x02)
   #define DEVICE_STATE_SHIFT                ((UCHAR) 0x00)
   #define DEVICE_STATE_MASK                 ((UCHAR) (0x0F << DEVICE_STATE_SHIFT))
   #define DEVICE_STATE_LINK                 ((UCHAR) (0x00 << DEVICE_STATE_SHIFT))
   #define DEVICE_STATE_AUTHENTICATE         ((UCHAR) (0x01 << DEVICE_STATE_SHIFT))
   #define DEVICE_STATE_TRANSPORT            ((UCHAR) (0x02 << DEVICE_STATE_SHIFT))
   #define DEVICE_STATE_BUSY                 ((UCHAR) (0x03 << DEVICE_STATE_SHIFT))
#define AUTHENTICATION_TYPE_OFFSET           ((UCHAR) 0x03)
#define DEVICE_DESCRIPTOR_OFFSET_0           ((UCHAR) 0x04)
#define DEVICE_DESCRIPTOR_OFFSET_1           ((UCHAR) 0x05)
#define DEVICE_DESCRIPTOR_OFFSET_2           ((UCHAR) 0x06)
#define DEVICE_DESCRIPTOR_OFFSET_3           ((UCHAR) 0x07)

// Commands
#define ANTFS_CMD_NONE                       ((UCHAR) 0x00)
#define ANTFS_CMD_LINK_ID                    ((UCHAR) 0x02)
#define ANTFS_CMD_DISCONNECT_ID              ((UCHAR) 0x03)
#define ANTFS_CMD_AUTHENTICATE_ID            ((UCHAR) 0x04)
#define ANTFS_CMD_PING_ID                    ((UCHAR) 0x05)
#define ANTFS_CMD_DOWNLOAD_ID                ((UCHAR) 0x09)
#define ANTFS_CMD_UPLOAD_REQUEST_ID          ((UCHAR) 0x0A)
#define ANTFS_CMD_ERASE_ID                   ((UCHAR) 0x0B)
#define ANTFS_CMD_UPLOAD_DATA_ID             ((UCHAR) 0x0C)
#define ANTFS_CMD_SEND_DIRECT_ID             ((UCHAR) 0x0D)

// Responses
#define ANTFS_RSP_AUTHENTICATE_ID            ((UCHAR) 0x84)
#define ANTFS_RSP_PING_ID                    ((UCHAR) 0x85)
#define ANTFS_RSP_DOWNLOAD_ID                ((UCHAR) 0x89)
#define ANTFS_RSP_UPLOAD_REQ_ID              ((UCHAR) 0x8A)
#define ANTFS_RSP_ERASE_ID                   ((UCHAR) 0x8B)
#define ANTFS_RSP_UPLOAD_DATA_ID             ((UCHAR) 0x8C)

// Link Command
#define TRANSPORT_CHANNEL_FREQUENCY_OFFSET   ((UCHAR) 0x02)
#define TRANSPORT_MESSAGE_PERIOD_OFFSET      ((UCHAR) 0x03)
#define HOST_ID_OFFSET_0                     ((UCHAR) 0x04)
#define HOST_ID_OFFSET_1                     ((UCHAR) 0x05)
#define HOST_ID_OFFSET_2                     ((UCHAR) 0x06)
#define HOST_ID_OFFSET_3                     ((UCHAR) 0x07)

// Authenticate Command
#define COMMAND_TYPE_OFFSET                  ((UCHAR) 0x02)
   #define COMMAND_TYPE_PROCEED              ((UCHAR) 0x00)
   #define COMMAND_TYPE_REQUEST_SERIAL       ((UCHAR) 0x01)
   #define COMMAND_TYPE_REQUEST_PAIR         ((UCHAR) 0x02)
   #define COMMAND_TYPE_REQUEST_PASSKEY      ((UCHAR) 0x03)

// Authenticate Response
#define RESPONSE_TYPE_OFFSET                 ((UCHAR) 0x02)
   #define AUTH_RESPONSE_N_A                 ((UCHAR) 0x00)
   #define AUTH_RESPONSE_ACCEPT              ((UCHAR) 0x01)
   #define AUTH_RESPONSE_REJECT              ((UCHAR) 0x02)

// Authenticate Command/Response
#define AUTH_STRING_LENGTH_OFFSET            ((UCHAR) 0x03)
#define SERIAL_NUMBER_OFFSET_0               ((UCHAR) 0x04)
#define SERIAL_NUMBER_OFFSET_1               ((UCHAR) 0x05)
#define SERIAL_NUMBER_OFFSET_2               ((UCHAR) 0x06)
#define SERIAL_NUMBER_OFFSET_3               ((UCHAR) 0x07)

// Download/Upload/Erase Commands
#define INITIAL_REQUEST_OFFSET               ((UCHAR) 0x01)
#define DATA_INDEX_OFFSET_LOW                ((UCHAR) 0x02)
#define DATA_INDEX_OFFSET_HIGH               ((UCHAR) 0x03)

// Download/Upload/Erase Responses
#define RESPONSE_MESSAGE_OFFSET              ((UCHAR) 0x02)
#define ADDRESS_PARAMETER_OFFSET_0           ((UCHAR) 0x04)
#define ADDRESS_PARAMETER_OFFSET_1           ((UCHAR) 0x05)
#define ADDRESS_PARAMETER_OFFSET_2           ((UCHAR) 0x06)
#define ADDRESS_PARAMETER_OFFSET_3           ((UCHAR) 0x07)
#define UPLOAD_CRC_OFFSET_LOW                ((UCHAR) 0x06)
#define UPLOAD_CRC_OFFSET_HIGH               ((UCHAR) 0x07)

// Authentication type
// The highest level of authentication available is included in the beacon
#if defined (ANTFS_AUTH_TYPE_PASSKEY)
   #define AUTHENTICATION_TYPE               COMMAND_TYPE_REQUEST_PASSKEY
#elif defined (ANTFS_AUTH_TYPE_PAIRING)
   #define AUTHENTICATION_TYPE               COMMAND_TYPE_REQUEST_PAIR
#elif defined (ANTFS_AUTH_TYPE_PASSTHROUGH)
   #define AUTHENTICATION_TYPE               COMMAND_TYPE_PROCEED
#else
   #error "No valid auth type defined"
#endif 

#define ANTFS_BURST_PARTIAL                  FALSE     // Burst block does not contain the end of the burst sequence
#define ANTFS_BURST_COMPLETE                 TRUE      // Burst block contains the end of the burst sequence

#define ANTFS_EVENT_QUEUE_SIZE               ((UCHAR) 0x04)  // ANT-FS event queue size

//////////////////////////////////////////////////////////////////////////////////
// Private Structs
//////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   char acFriendlyName[ANTFS_FRIENDLY_NAME_MAX];      // Friendly Name
   BOOL bNameSet;                           // Is the name set
   UCHAR ucIndex;                           // Current index (for reading the friendly name)
   UCHAR ucFNSize;                          // Friendly name size
} FriendlyName;


typedef union
{
   ANTFS_LINK_SUBSTATE eLinkSubState;       // Sub-state (Link layer)
   ANTFS_AUTH_SUBSTATE eAuthSubState;       // Sub-state (Authentication layer)
   ANTFS_TRANS_SUBSTATE eTransSubState;     // Sub-state (Transport layer)
} ANTFS_SUBSTATE;

typedef struct
{
   ANTFS_STATE eState;                      // ANT-FS state
   ANTFS_SUBSTATE eSubState;                // ANT-FS sub-state
} ANTFS_STATES;

typedef struct
{
   ANTFS_EVENT_RETURN* astQueue;
   UCHAR ucHead;
   UCHAR ucTail;
} ANTFS_EventQueue;

//////////////////////////////////////////////////////////////////////////////////
// Private Constants
//////////////////////////////////////////////////////////////////////////////////
static const UCHAR aucNetworkKey[] = ANTFS_NETWORK_KEY;  // ANT-FS Network Key

//////////////////////////////////////////////////////////////////////////////////
// Private Variables
//////////////////////////////////////////////////////////////////////////////////

static ANTFS_PARAMS stInitParams;           // Initial parameters
static ANTFS_BEACON_STATUS1 stActiveBeaconStatus1;    // Status 1 field in beacon
static UCHAR ucActiveBeaconFrequency;       // Active beacon frequency
static ANTFS_STATES stState;                // Current state

static FriendlyName stFriendlyName;         // Host's friendly name
static ULONG_UNION stLinkHostSerialNumber;  // Host's serial number

#if defined (ANTFS_AUTH_TYPE_PASSKEY)
   static UCHAR ucPassKeyIndex;             // Current location of Tx block (auth string)
#endif // ANTFS_AUTH_TYPE_PASSKEY

static UCHAR ucAlarmNumber;                 // Timer alarm
static UCHAR ucLinkCommandInProgress;       // ANT-FS command in progress

// Download/upload 
static BOOL bDataRequestPending;            // Requested data pending
static BOOL bCRCPending;                    // CRC for data packets pending
static USHORT_UNION uFileIndex;             // File index of current upload/download
static ULONG_UNION uFileSize;               // File size of current upload/download (bytes)
static ULONG_UNION uMaxBlockSize;           // Maximum number of bytes expected to be downloaded in a single burst block of data (set by host)
static ULONG_UNION uLinkBurstIndex;         // Current location of Tx block (bytes)
static ULONG_UNION uBytesRemaining;         // Total remaining data length (bytes)
static ULONG_UNION uMaxTransferIndex;       // Upper limit of the current Tx burst block (bytes)
static ULONG_UNION uBlockSize;              // Number of bytes the client can receive in a single burst

static ULONG ulCurrentPacketNum;            // Number of packet within a burst sequence
static UCHAR ucBytesToWrite;                // Number of bytes to write to file (upload)

// CRC Verification
static ULONG ulSavedCRCOffset;              // CRC data offset (bytes) saved at last CRC update (save point)
static ULONG ulTempCRCOffset;
static USHORT usComparedCRC;                // 16 bit CRC for all data packets in the block (provided by download request)
static USHORT usTransferCRC;                // 16 bit CRC for all data packets in the block (calculated by client)
static USHORT usSavedTransferCRC;           // 16 bit CRC saved at last CRC update (save point)

static UCHAR ucCommandType;                 // Auth command type in progress

// ANT-FS Event handling
static UCHAR aucUploadBuffer[8];            // Buffer to hold data received from upload
static ANTFS_EVENT_RETURN astEventQueue[ANTFS_EVENT_QUEUE_SIZE];
static ANTFS_EventQueue stTheEventQueue;    // Event queue


//////////////////////////////////////////////////////////////////////////////////
// Private Function Prototypes
//////////////////////////////////////////////////////////////////////////////////
static BOOL HandleResponseEvents(UCHAR* pucBuffer_);
static void ANTFS_Reset(void);
static void DecodeLinkCommand(UCHAR *pucLinkCommand_);
static void DecodeAuthenticateCommand(UCHAR ucControlByte_, UCHAR *pucAuthCommand_);
static void DecodeTransportCommand(UCHAR ucControlByte_, UCHAR *pucTransCommand_);
static void UploadInputData(UCHAR ucControlByte_, UCHAR* pucMesg_);
static BOOL LinkLoadBeacon(UCHAR ucMessageType_);
static void AuthenticateResponse(UCHAR ucResponseType_, UCHAR ucPasswordLength_, const UCHAR* pucPassword_);
static BOOL DownloadResponse(UCHAR ucResponse_);
static void SwitchToLink(void);
static void SwitchToAuthenticate(void);
static void SwitchToTransport(void);
static void ANTFS_SetChannelPeriod(UCHAR ucLinkPeriod_);
static void ANTFS_Tick(USHORT usTimeStamp_, UCHAR ucAlarmNumber_);
static BOOL SetTimeout(USHORT usTimeout);
static void DisableTimeout(void);
static BOOL ANTFS_SetEvent(UCHAR ucEventCode_);

//////////////////////////////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// ANTFS_Init
//
// Get initial ANT-FS configuration parameters
//
//////////////////////////////////////////////////////////////////////////////////
void ANTFS_Init(ANTFS_PARAMS* pstInitParams_)
{
   stInitParams = *pstInitParams_;
   stActiveBeaconStatus1 = stInitParams.stBeaconStatus1;
   ANTFS_Reset();
}

///////////////////////////////////////////////////////////////////////
// ANTFS_OpenBeacon
//
// Returns: FALSE if it fails to start channel configuration
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_OpenBeacon(void)
{
   // Initialize ANTFS
     ANTFS_Init(&stInitParams);

   // Start channel configuration
   if (!ANT_NetworkKey(ANTFS_NETWORK_NUMBER, aucNetworkKey))
      return FALSE;

   return TRUE;
}

///////////////////////////////////////////////////////////////////////
// ANTFS_CloseBeacon
//
// Returns: FALSE if unable to close the beacon
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_CloseBeacon(void)
{
   if (stState.eState != ANTFS_STATE_LINK)         // Do not close if not in LINK layer.
      return FALSE;

   if (!ANT_CloseChannel(ANTFS_CHANNEL))           // Close channel                     
      return FALSE;
   
   ANTFS_Reset();                                  // Reset ANTFS state variables
   return TRUE;
}


///////////////////////////////////////////////////////////////////////
// ANTFS_GetState
//
// Returns: current ANT-FS state
///////////////////////////////////////////////////////////////////////
ANTFS_STATE ANTFS_GetState(void)
{
   return stState.eState; 
}

///////////////////////////////////////////////////////////////////////
// ANTFS_GetHostName
// 
// Returns: pointer to host name buffer if a host name was recieved.
//
///////////////////////////////////////////////////////////////////////
char* ANTFS_GetHostName()
{
   if(stFriendlyName.bNameSet)
      return( stFriendlyName.acFriendlyName);

   return(NULL);
}

///////////////////////////////////////////////////////////////////////
// ANTFS_PairingResponse
// 
// Sends response to a pairing request.
//
// Returns: FALSE if not in pairing mode or pairing not supported
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_PairingResponse(BOOL bAccept_)
{
#if defined(ANTFS_AUTH_TYPE_PAIRING)
   if(stState.eState != ANTFS_STATE_AUTH || stState.eSubState.eAuthSubState != ANTFS_AUTH_SUBSTATE_PAIR)   // This function should only ever be called while ANTFS is in PAIRING mode.
      return FALSE;

   ucLinkCommandInProgress = ANTFS_CMD_AUTHENTICATE_ID;
   
   if(bAccept_)
   {
      AuthenticateResponse(AUTH_RESPONSE_ACCEPT, ANTFS_PASSKEY_SIZE, stInitParams.pucPassKey);     // Accept request and send passkey if authentication passed
   }
   else
   {
      AuthenticateResponse(AUTH_RESPONSE_REJECT, 0, NULL);     // Reject authentication request
   }

   return TRUE;
#else
   return FALSE;
#endif   // ANTFS_AUTH_TYPE_PAIRING
}

///////////////////////////////////////////////////////////////////////
// ANTFS_DownloadRequestResponse
//
// Sends a response to a download request and requests data to the application
// if needed
//
// Returns: FALSE if not processing a download request
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_DownloadRequestResponse(UCHAR ucResponse_, ANTFS_REQUEST_INFO* stRequestInfo_)
{ 
   UCHAR ucResponse = ucResponse_;

   if(stState.eState != ANTFS_STATE_TRANS || ucLinkCommandInProgress != ANTFS_CMD_DOWNLOAD_ID)    // This function should only be called after receiving a download request
      return FALSE;

   if(!ucResponse)  // Download Request OK
   {
      uFileSize.ulData = stRequestInfo_->uSize.ulData;      // File size of the requested download

      if(uLinkBurstIndex.ulData > uFileSize.ulData) 
         uLinkBurstIndex.ulData = uFileSize.ulData;         // Offset should not exceed file size
            
      if(uMaxBlockSize.ulData == 0 || uFileSize.ulData < uMaxBlockSize.ulData)    // If the host is not limiting download size or the file size does not exceed the host's download size limit
         uBytesRemaining.ulData = uFileSize.ulData;         //  Number of bytes remaining to be downloaded in this block is the file size

      if((uFileSize.ulData - uLinkBurstIndex.ulData) < uBytesRemaining.ulData)
         uBytesRemaining.ulData = uFileSize.ulData - uLinkBurstIndex.ulData;  // Calculate number of remaining bytes in this block based on the offset

      if(uBytesRemaining.ulData > stRequestInfo_->uBlockSize.ulData)    // If the application is limiting the Tx block size
         uBytesRemaining.ulData = stRequestInfo_->uBlockSize.ulData;   // Number of remaining bytes in this block is the application defined block size     

      // Find upper limit of the burst Tx
      uMaxTransferIndex.ulData = uLinkBurstIndex.ulData + uBytesRemaining.ulData;     // upper limit of next block

      if(ulSavedCRCOffset == ANTFS_MAX_FILE_SIZE)          // CRC checking was set as invalid
         ucResponse = RESPONSE_INVALID_OPERATION;           // An invalid download was requested, so reject it
   }

   if(ucResponse || uFileSize.ulData == 0)  // Send the response right away if the download request was rejected or there is no data to send
   {
      DownloadResponse(ucResponse);
   }
   else
   {
      // Proceed to download data  
      if(uLinkBurstIndex.ulData != ulSavedCRCOffset)  // If resuming
      {
         ULONG ulTemp;
         if(uLinkBurstIndex.ulData == ulTempCRCOffset)  // If requesting to resume exactly where we left off, we can start from the same block
         {
            ulSavedCRCOffset = uLinkBurstIndex.ulData;      // Move last save point to end of last block sent
            usSavedTransferCRC = usTransferCRC;
         }
         // To resume the download, request a block of data starting from the last save point     
         // Update the remaining number of bytes per the last save point
         uBytesRemaining.ulData += (uLinkBurstIndex.ulData - ulSavedCRCOffset);
         // Swap the current burst Tx index with the saved CRC index, to make sure we do not start updating the CRC until we get to the requested index
         ulTemp = uLinkBurstIndex.ulData;
         uLinkBurstIndex.ulData = ulSavedCRCOffset;
         ulSavedCRCOffset = ulTemp;
         // Set CRC to previous save point, to check the CRC provided by the host
         usTransferCRC = usSavedTransferCRC;
      }

      ulTempCRCOffset = ulSavedCRCOffset;

      bDataRequestPending = TRUE;
      ANTFS_SetEvent(ANTFS_EVENT_DOWNLOAD_REQUEST_DATA);    // Request data from application
      stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_VERIFY_CRC;
   }

   return TRUE;
}

///////////////////////////////////////////////////////////////////////
// ANTFS_DownloadInputData
//
// Sends response to a download data request, and requests data to the
// application as needed
//
// Returns: number of download data bytes transmitted
// 
///////////////////////////////////////////////////////////////////////
ULONG ANTFS_DownloadInputData(USHORT usIndex_, ULONG ulOffset_, ULONG ulNumBytes_, UCHAR *pucMesg_)
{ 
   if((ulOffset_ != uLinkBurstIndex.ulData) || (usIndex_ != uFileIndex.usData))     // this is not the data that was requested
      return 0;
   
   if(ulOffset_ > uMaxTransferIndex.ulData)    // file offset is greater than the upper limit, so this is not data we need
      return 0;

   if (stState.eState == ANTFS_STATE_TRANS && 
      (stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_VERIFY_CRC || 
      stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_DOWNLOADING))  // Only send data if we were processing a download request
   {
      ULONG ulBlockOffset = 0;
      ULONG ulNumBytes = ulNumBytes_;

      if (stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_VERIFY_CRC)
      {
         UCHAR ucResponse = RESPONSE_MESSAGE_OK;
         // Check CRC
         if (uLinkBurstIndex.ulData == ulSavedCRCOffset)     // If indexes match, we can compare CRC directly
         {
            if (usTransferCRC != usComparedCRC)     // CRC seed provided by host does not match CRC calculated at client
            {
               ucResponse = RESPONSE_INVALID_CRC;
            }
            else
            {
               usSavedTransferCRC = usTransferCRC;
               bCRCPending = TRUE;
            }
         }
         else if(uLinkBurstIndex.ulData < ulSavedCRCOffset && (ulSavedCRCOffset - uLinkBurstIndex.ulData) < ulNumBytes)   // If our save point and requested offset are in the same data block, we can calculate the CRC and compare
         {
            ulBlockOffset = ulSavedCRCOffset - uLinkBurstIndex.ulData;    // Update the offset within this block for the requested transmission                
            ulNumBytes -= ulBlockOffset;   // Update the number of bytes that will actually be transmitted
            usTransferCRC = CRC_UpdateCRC16(usTransferCRC, pucMesg_, ulBlockOffset);   // Update CRC calculation up to requested index
            uBytesRemaining.ulData -= ulBlockOffset;    // Update the remaining number of bytes

            if(usTransferCRC != usComparedCRC)
            {
               ucResponse = RESPONSE_INVALID_CRC;  // Check CRC
            }
            else
            {
               uLinkBurstIndex.ulData = ulSavedCRCOffset;    // Move index back to point where transmission will resume 
               usSavedTransferCRC = usTransferCRC;
               bCRCPending = TRUE;
            }
         }
         else
         {
            ucResponse = RESPONSE_INVALID_CRC;      // Unable to check CRC, can not resume download
         }
         DownloadResponse(ucResponse);   // Start bursting beacon and the download response (3 burst packets)
      }

      // Append data
      if (stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_DOWNLOADING)
      {
         UCHAR aucTemp[ANTFS_BURST_BLOCK_SIZE * 8];
         USHORT usRemainingPackets;
         ULONG ulNumDataPackets = ulNumBytes / 8;   // Number of packets in this block of data
         if(ulNumBytes % 8) 
            ulNumDataPackets++;

         memset(aucTemp, 0, ANTFS_BURST_BLOCK_SIZE * 8);  // Pad with zeroes
         memcpy(aucTemp, &pucMesg_[ulBlockOffset], ulNumBytes);   // Grab data

         usRemainingPackets = ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, ulNumDataPackets, ulCurrentPacketNum, ANTFS_BURST_PARTIAL);  // Send block of data 
         if(usRemainingPackets) // If we could only transmit part of the burst 
         {
            ulNumDataPackets -= usRemainingPackets;     // Update the number of packets that was actually transmitted
            ulNumBytes = ulNumDataPackets * 8;          // Update the number of bytes actually transmitted
         }

         ulCurrentPacketNum += ulNumDataPackets;         // Update the current packet Tx count, for next Tx            
         uLinkBurstIndex.ulData += ulNumBytes;           // Update current burst index   
         uBytesRemaining.ulData -= ulNumBytes;           // Update remaining bytes

         bDataRequestPending = FALSE;

         usTransferCRC = CRC_UpdateCRC16(usTransferCRC, aucTemp, ulNumBytes);   // Update CRC
         usSavedTransferCRC = usComparedCRC;             // Set save point                 
         usComparedCRC = usTransferCRC;    
         ulSavedCRCOffset = ulTempCRCOffset;          
         ulTempCRCOffset = uLinkBurstIndex.ulData;

         if(!bDataRequestPending && uLinkBurstIndex.ulData < uMaxTransferIndex.ulData) // If we have not finished the download
         {
            ANTFS_SetEvent(ANTFS_EVENT_DOWNLOAD_REQUEST_DATA);      // Request more data
            bDataRequestPending = TRUE; 
         }
         else if(uLinkBurstIndex.ulData >= uMaxTransferIndex.ulData && bCRCPending)  // We are done, send CRC footer
         {
            aucTemp[0] = 0;
            aucTemp[1] = 0;
            aucTemp[2] = 0;
            aucTemp[3] = 0;
            aucTemp[4] = 0;
            aucTemp[5] = 0;
            aucTemp[6] = (UCHAR) usTransferCRC;
            aucTemp[7] = (UCHAR) (usTransferCRC >> 8);  
            ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, ulCurrentPacketNum, ANTFS_BURST_COMPLETE);
            bCRCPending = FALSE;                
            uMaxTransferIndex.ulData = 0;
         }
         return (ulNumBytes);    // return the number of bytes we accepted
      }
   }

   return 0;   // No bytes were accepted
}

///////////////////////////////////////////////////////////////////////
// ANTFS_UploadRequestResponse

// Sends a response to an upload request.
//
// Returns: FALSE if unable to send the upload request response
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_UploadRequestResponse(UCHAR ucResponse_, ANTFS_REQUEST_INFO* stRequestInfo_)
{
#if defined(ANTFS_INCLUDE_UPLOAD)
   UCHAR aucTemp[8];
   ULONG_UNION uMaxMemSize;
   USHORT usPacketNum = 1;
   
   if(stState.eState != ANTFS_STATE_TRANS || ucLinkCommandInProgress != ANTFS_CMD_UPLOAD_REQUEST_ID)   // Only send the response if we were processing an upload request
      return FALSE;

   if(stRequestInfo_->uIndex.usData != uFileIndex.usData)
   {
      ANTFS_SetEvent(ANTFS_EVENT_UPLOAD_FAIL);    // If the application is sending a response for a different file than requested, the upload will fail
      return FALSE;
   }

   uMaxMemSize.ulData = stRequestInfo_->ulMaxSize;       // Maximum number of bytes that can be written to the file
    
   if(stRequestInfo_->uBlockSize.ulData)                 // If the client is limiting the block size
      uBlockSize.ulData = stRequestInfo_->uBlockSize.ulData;  // Set the block size requested by the client
   else
      uBlockSize.ulData = uMaxMemSize.ulData;                 // Otherwise, try to get the entire file in a single block    

   if(!ucResponse_)
   {
      if(uMaxTransferIndex.ulData > uMaxMemSize.ulData)
      {
         ucResponse_ = RESPONSE_MESSAGE_NOT_ENOUGH_SPACE;    // Not enough space to write file, reject download
      }
   }

   usTransferCRC = stRequestInfo_->usFileCRC;                  // Get last valid CRC
   uLinkBurstIndex.ulData = stRequestInfo_->uSize.ulData;      // Get last valid offset

   // Beacon
   LinkLoadBeacon(MESG_BURST_DATA_ID);    // First packet is beacon
   usPacketNum++;

   // Second packet 
   aucTemp[0] = ANTFS_COMMAND_ID;
   aucTemp[1] = ANTFS_RSP_UPLOAD_REQ_ID;
   aucTemp[2] = ucResponse_;                              // Response code
   aucTemp[3] = 0;
   aucTemp[4] = uLinkBurstIndex.stBytes.ucByte0;          // Last valid data offset written to the file
   aucTemp[5] = uLinkBurstIndex.stBytes.ucByte1;
   aucTemp[6] = uLinkBurstIndex.stBytes.ucByte2;
   aucTemp[7] = uLinkBurstIndex.stBytes.ucByte3;  

   ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_PARTIAL);
   usPacketNum++;

   // Third packet
   aucTemp[0] = uMaxMemSize.stBytes.ucByte0;             // Maximum number of bytes that can be written to the file
   aucTemp[1] = uMaxMemSize.stBytes.ucByte1;
   aucTemp[2] = uMaxMemSize.stBytes.ucByte2;
   aucTemp[3] = uMaxMemSize.stBytes.ucByte3;
   aucTemp[4] = uBlockSize.stBytes.ucByte0;              // Maxium upload block size
   aucTemp[5] = uBlockSize.stBytes.ucByte1;
   aucTemp[6] = uBlockSize.stBytes.ucByte2;
   aucTemp[7] = uBlockSize.stBytes.ucByte3;  
   
   ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_PARTIAL);
   usPacketNum++;

   // Fourth packet
   aucTemp[0] = 0;
   aucTemp[1] = 0;
   aucTemp[2] = 0;
   aucTemp[3] = 0;
   aucTemp[4] = 0;
   aucTemp[5] = 0;
   aucTemp[6] = (UCHAR) usTransferCRC;                   // Value of CRC at last data offset
   aucTemp[7] = (UCHAR)(usTransferCRC >> 8);

   ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_COMPLETE); // Send last packet

   ucLinkCommandInProgress = ANTFS_CMD_UPLOAD_REQUEST_ID;

   if(ucResponse_)                           // Failed upload request
      uMaxTransferIndex.ulData = 0;          // Reset max transfer index to 0 (do not accept any data if the host sends it anyway)
   else
      stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_UPLOAD_WAIT_FOR_DATA;   // Wait for Upload Data Request

   return TRUE;
#else
   return FALSE;
#endif   // ANTFS_INCLUDE_UPLOAD
}

///////////////////////////////////////////////////////////////////////
// ANTFS_UploadResponse
//
// Sends upload response, after a download has been completed
//
// Returns: FALSE if unable to send the response
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_UploadResponse(BOOL bAccept_)
{
#if defined(ANTFS_INCLUDE_UPLOAD)
   UCHAR aucTemp[8];
   UCHAR ucResponse = RESPONSE_MESSAGE_OK;
   USHORT usPacketNum = 1;
   
   if(stState.eState != ANTFS_STATE_TRANS)   // Should be in TRANSPORT layer to send this response
      return FALSE;
   
   if (!bAccept_)
      ucResponse = RESPONSE_MESSAGE_FAIL;
   
   // Beacon
   LinkLoadBeacon(MESG_BURST_DATA_ID);    // First packet is beacon
   usPacketNum++; 

   // Response 
   aucTemp[0] = ANTFS_COMMAND_ID;
   aucTemp[1] = ANTFS_RSP_UPLOAD_DATA_ID;
   aucTemp[2] = ucResponse;
   aucTemp[3] = 0;
   aucTemp[4] = 0;
   aucTemp[5] = 0;
   aucTemp[6] = 0;
   aucTemp[7] = 0;  

   ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_COMPLETE);    // Send last packet
   usPacketNum++;
   ucLinkCommandInProgress = ANTFS_CMD_UPLOAD_REQUEST_ID;

   uMaxTransferIndex.ulData = 0;    // Reset maximum index

   return TRUE;
#else
   return FALSE;
#endif   // ANTFS_INCLUDE_UPLOAD
}

///////////////////////////////////////////////////////////////////////
// ANTFS_EraseRequestResponse
//
// Sends a response to an erase request
//
// Returns: FALSE if in incorrect state
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_EraseRequestResponse(UCHAR ucResponse_)
{
   UCHAR aucTemp[8];
   USHORT usPacketNum = 1;

   if(stState.eState != ANTFS_STATE_TRANS || ucLinkCommandInProgress != ANTFS_CMD_ERASE_ID)    // This function should only be called after receiving an erase request
      return FALSE;

   LinkLoadBeacon(MESG_BURST_DATA_ID);      // Beacon
   usPacketNum++;

   // Erase response (single burst page message)
   aucTemp[0] = ANTFS_COMMAND_ID;
   aucTemp[1] = ANTFS_RSP_ERASE_ID;
   aucTemp[2] = ucResponse_;
   aucTemp[3] = 0;
   aucTemp[4] = 0;
   aucTemp[5] = 0;
   aucTemp[6] = 0;
   aucTemp[7] = 0;

   ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_COMPLETE);

   return TRUE;
}

///////////////////////////////////////////////////////////////////////
// ANTFS_CheckEvents
//
// Check for pending ANT-FS events
//
// Returns TRUE if there is an event to process, and copies the event
// parameters in the return event
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_CheckEvents(ANTFS_EVENT_RETURN* pstEvent_)
{
   if(stTheEventQueue.ucHead != stTheEventQueue.ucTail)    // Check for pending events
   {
      // Copy event parameters into return event
      pstEvent_->eEvent = stTheEventQueue.astQueue[stTheEventQueue.ucTail].eEvent;
      pstEvent_->usFileIndex = stTheEventQueue.astQueue[stTheEventQueue.ucTail].usFileIndex;
      pstEvent_->ulOffset = stTheEventQueue.astQueue[stTheEventQueue.ucTail].ulOffset;
      pstEvent_->ulBytes = stTheEventQueue.astQueue[stTheEventQueue.ucTail].ulBytes;
      pstEvent_->usCRC = stTheEventQueue.astQueue[stTheEventQueue.ucTail].usCRC;
      memcpy(pstEvent_->aucData, stTheEventQueue.astQueue[stTheEventQueue.ucTail].aucData, 8);

      // Release event queue
      stTheEventQueue.ucTail = ((stTheEventQueue.ucTail + 1) & (ANTFS_EVENT_QUEUE_SIZE - 1));    
      return TRUE;
   }

   return FALSE;
}

///////////////////////////////////////////////////////////////////////
// ANTFS_ProcessMessage
// 
// Process ANT Events/Data for the ANTFS Channel
//
// Returns: FALSE if not processing the message
//
///////////////////////////////////////////////////////////////////////
BOOL ANTFS_ProcessMessage(UCHAR* pucRxMesg_)
{
   BOOL bTransmit = TRUE;

   if(pucRxMesg_)
   {
      if((pucRxMesg_[BUFFER_INDEX_CHANNEL_NUM] & CHANNEL_NUMBER_MASK) != ANTFS_CHANNEL)       // Only process messages corresponding to the ANT-FS channel here
         return FALSE;

      if (stState.eState == ANTFS_STATE_OFF && 
         !((pucRxMesg_[BUFFER_INDEX_MESG_ID] == MESG_RESPONSE_EVENT_ID)&& (pucRxMesg_[BUFFER_INDEX_RESPONSE_CODE] == RESPONSE_NO_ERROR)))
            return FALSE;

      switch (pucRxMesg_[BUFFER_INDEX_MESG_ID])
      {
         case MESG_BROADCAST_DATA_ID:  // we're not going to process broadcast messages or pass them to the app to handle
            break;
         case MESG_ACKNOWLEDGED_DATA_ID:
            pucRxMesg_[ANTFS_CONTROL_OFFSET] |= SEQUENCE_LAST_MESSAGE;       // mark it as being the last message if it's an ack message
         case MESG_BURST_DATA_ID:   // fall thru (process both burst/ack messages)
         {
            switch (stState.eState)
            {
               case ANTFS_STATE_LINK:
               {
                  DecodeLinkCommand(&pucRxMesg_[ANTFS_DATA_OFFSET]); 
                  break;
               }
               case ANTFS_STATE_AUTH:
               {
                  DecodeAuthenticateCommand(pucRxMesg_[ANTFS_CONTROL_OFFSET], &pucRxMesg_[ANTFS_DATA_OFFSET]);
                  break;
               }
               case ANTFS_STATE_TRANS:
               {
                  if(stState.eSubState.eTransSubState != ANTFS_TRANS_SUBSTATE_UPLOADING)
                  {
                     DecodeTransportCommand(pucRxMesg_[ANTFS_CONTROL_OFFSET], &pucRxMesg_[ANTFS_DATA_OFFSET]);
                  }
                  else
                  {
                     UploadInputData(pucRxMesg_[ANTFS_CONTROL_OFFSET], &pucRxMesg_[ANTFS_DATA_OFFSET]);
                  }
                  break;
               }
               default:
               {
                  // If in any other state or sub-state, do nothing
                  break;
               }
            }
            break;
         }
         case MESG_RESPONSE_EVENT_ID:
         {
            switch (pucRxMesg_[BUFFER_INDEX_RESPONSE_CODE])           // Event ID
            {
               case EVENT_TRANSFER_TX_FAILED:
               {
                  ucLinkCommandInProgress = ANTFS_CMD_NONE;

                  // Switch into the appropriate state after the failure. 
                  // Must be ready for the host to do a retry.
                  switch (stState.eState)
                  {
                     case ANTFS_STATE_LINK:
                     {
                        SwitchToLink();
                        break;
                     }
                     case ANTFS_STATE_AUTH:
                     {
                        SwitchToAuthenticate();
                        break;
                     }
                     case ANTFS_STATE_TRANS:
                     { 
                        if(stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_DOWNLOADING) 
                        {
                           ANTFS_SetEvent(ANTFS_EVENT_DOWNLOAD_FAIL);
                        }
                        SwitchToTransport();
                        break;
                     }
                     case ANTFS_STATE_OFF:
                     case ANTFS_STATE_INIT:
                     default:
                     {
                        // Do Nothing
                        break;
                     }
                  }
                  break;
               }
               case EVENT_TRANSFER_RX_FAILED:
               {
                  ucLinkCommandInProgress = ANTFS_CMD_NONE;
                  if(stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_UPLOADING) 
                  {
                     ANTFS_SetEvent(ANTFS_EVENT_UPLOAD_FAIL);
                     stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_UPLOAD_RESUME;
                  }
                  break;
               }
               case EVENT_TRANSFER_TX_COMPLETED:
               {
                  ucLinkCommandInProgress = ANTFS_CMD_NONE;

                  // Switch into appropiate state after successful command
                  switch (stState.eState)
                  {
                     case ANTFS_STATE_AUTH:
                     {
                        if(stState.eSubState.eAuthSubState == ANTFS_AUTH_SUBSTATE_ACCEPT)
                        {
                           SwitchToTransport();         // We passed auth, so go to transport state
                        }
                        else if(stState.eSubState.eAuthSubState == ANTFS_AUTH_SUBSTATE_REJECT)
                        {
                           SwitchToLink();                 // We failed auth, so go to link state
                        }
                        else
                        {
                           SwitchToAuthenticate();       // Reload beacon 
                        }
                        break;
                     }
                     case ANTFS_STATE_TRANS:
                     {
                        if(stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_DOWNLOADING)
                        {
                           ANTFS_SetEvent(ANTFS_EVENT_DOWNLOAD_COMPLETE);
                        }

                        if (stState.eSubState.eTransSubState != ANTFS_TRANS_SUBSTATE_UPLOAD_WAIT_FOR_DATA) 
                           SwitchToTransport();            // Reload beacon
                        break;
                     }
                     case ANTFS_STATE_OFF:
                     case ANTFS_STATE_INIT:
                     case ANTFS_STATE_LINK:
                     default:
                     {
                        SwitchToLink();         // Reload beacon
                        break;
                     }
                  }
                  break;
               }
               case EVENT_TX:
               {
                  LinkLoadBeacon(MESG_BROADCAST_DATA_ID);   // Load beacon
                  break;
               }
               case EVENT_CHANNEL_CLOSED:
               {
                  ANTFS_SetEvent(ANTFS_EVENT_CLOSE_COMPLETE);
                  break;
               }
               case RESPONSE_NO_ERROR:
               {
                  bTransmit = HandleResponseEvents(pucRxMesg_);
                  break;
               }
               default:
               {
                  return FALSE;   // we did not handle the event
               }
            }
            break;
         }
         default:
         {
            return FALSE;   // we did not handle the event
         }
      }
   }

   return bTransmit;
}

//////////////////////////////////////////////////////////////////////////////////
// Private Functions
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Handle RESPONSE NO ERROR events
/////////////////////////////////////////////////////////////////////////
static BOOL HandleResponseEvents(UCHAR* pucBuffer_)
{
   if(pucBuffer_ && (pucBuffer_[BUFFER_INDEX_RESPONSE_CODE] == RESPONSE_NO_ERROR))
   {
      switch(pucBuffer_[BUFFER_INDEX_RESPONSE_MESG_ID])
      {
         case MESG_NETWORK_KEY_ID:
         {
            if(stState.eState == ANTFS_STATE_OFF)
            ANT_AssignChannel(ANTFS_CHANNEL, ANTFS_CHANNEL_TYPE, ANTFS_NETWORK_NUMBER);
            break;
         }
         case MESG_ASSIGN_CHANNEL_ID:
         {
            if(stState.eState == ANTFS_STATE_OFF)
            {
               USHORT usDeviceNumber = (USHORT)(stInitParams.ulESN & 0x0000FFFF);   // Use the lower 2 bytes of the ESN for device number
               if(usDeviceNumber == 0)                                              // Device number of 0 is not allowed. 
                  usDeviceNumber = 2;
               ANT_ChannelId(ANTFS_CHANNEL, usDeviceNumber, ANTFS_DEVICE_TYPE, ANTFS_TRANS_TYPE);
            }
            break;
         }
         case MESG_CHANNEL_ID_ID:
         {
            if(stState.eState == ANTFS_STATE_OFF)
            {
               stState.eState = ANTFS_STATE_INIT;
               SwitchToLink();
               stState.eState = ANTFS_STATE_INIT;      // Remain in initialization state until channel is open
            }
            break;
         }
         case MESG_CHANNEL_RADIO_FREQ_ID:
         {
            if((stState.eState == ANTFS_STATE_LINK) || (stState.eState == ANTFS_STATE_INIT) || (stState.eState == ANTFS_STATE_AUTH))
            {
               ANTFS_SetChannelPeriod(stActiveBeaconStatus1.stParameters.ucLinkPeriod);  
            }
            break;
         }
         case MESG_CHANNEL_MESG_PERIOD_ID:
         {
            if((stState.eState == ANTFS_STATE_LINK) || (stState.eState == ANTFS_STATE_INIT))
            {
               ANTFS_SetEvent(ANTFS_EVENT_LINK);
               if(stState.eState == ANTFS_STATE_INIT)
                  ANT_OpenChannel(ANTFS_CHANNEL);
               LinkLoadBeacon(MESG_BROADCAST_DATA_ID);
            }
            if(stState.eState == ANTFS_STATE_AUTH)
            {
               ANTFS_SetEvent(ANTFS_EVENT_AUTH);
               LinkLoadBeacon(MESG_BROADCAST_DATA_ID);
            }
            break;
         } 
         case MESG_OPEN_CHANNEL_ID:
         {
            if(stState.eState == ANTFS_STATE_INIT)
            {
               ANT_ChannelPower(ANTFS_CHANNEL, ANTFS_TRANSMIT_POWER);
               stState.eState = ANTFS_STATE_LINK; 
               stState.eSubState.eLinkSubState = ANTFS_LINK_SUBSTATE_NONE;
               ANTFS_SetEvent(ANTFS_EVENT_OPEN_COMPLETE);
            }
            break;
         }
         case MESG_CLOSE_CHANNEL_ID:
         {
            ANTFS_SetEvent(ANTFS_EVENT_CLOSE_COMPLETE);
            break;
         }
         default:
         {
            return FALSE;
         }
      }
   }
   // ANT command timeout events
   else if(pucBuffer_ && (pucBuffer_[BUFFER_INDEX_RESPONSE_CODE] == EVENT_COMMAND_TIMEOUT))
   {
      if(stState.eState == ANTFS_STATE_INIT)
         {
            ANTFS_Init(&stInitParams);                  // Retry ANT-FS initialization
            ANTFS_SetEvent(ANTFS_EVENT_OPEN_FAIL);
         }
         else if(pucBuffer_[BUFFER_INDEX_RESPONSE_MESG_ID] == MESG_CLOSE_CHANNEL_ID)
         {
            ANTFS_SetEvent(ANTFS_EVENT_CLOSE_FAIL);
         }
   }

   return TRUE;
}

///////////////////////////////////////////////////////////////////////
// Resets the ANT-FS State Machine
///////////////////////////////////////////////////////////////////////
static void ANTFS_Reset(void)
{
   stState.eState = ANTFS_STATE_OFF;
   ucLinkCommandInProgress = ANTFS_CMD_NONE;

   if(ucAlarmNumber)
   {
      DisableTimeout();
      Timer_UnRegisterAlarm(ucAlarmNumber);   // Unregister previously set alarm
   }
   ucAlarmNumber = Timer_RegisterAlarm(ANTFS_Tick, ALARM_FLAG_ONESHOT);      // Register timer alarm

   stTheEventQueue.astQueue = astEventQueue;   // Reset ANT-FS event queue
   stTheEventQueue.ucHead = 0;  
   stTheEventQueue.ucTail = 0;

   ucCommandType = 0xFF;  // Set it to invalid (defined as 0-3)

   ulSavedCRCOffset = 0xFFFFFFFF;
   uMaxTransferIndex.ulData = 0;
   bCRCPending = FALSE;
   bDataRequestPending = FALSE;

   stFriendlyName.bNameSet = FALSE;
   stFriendlyName.ucIndex = 0;

   memset(stFriendlyName.acFriendlyName, 0, ANTFS_FRIENDLY_NAME_MAX);
}

////////////////////////////////////////////////////////////////////////
// Decode Link Commands
///////////////////////////////////////////////////////////////////////
static void DecodeLinkCommand(UCHAR *pucLinkCommand_)
{
   if(pucLinkCommand_[ANTFS_CONNECTION_TYPE_OFFSET] != ANTFS_COMMAND_ID)
      return;

   switch (pucLinkCommand_[ANTFS_COMMAND_OFFSET])
   {
      case ANTFS_CMD_LINK_ID:
      {
         ucActiveBeaconFrequency = pucLinkCommand_[TRANSPORT_CHANNEL_FREQUENCY_OFFSET];                         // Channel frequency
         stActiveBeaconStatus1.stParameters.ucLinkPeriod = pucLinkCommand_[TRANSPORT_MESSAGE_PERIOD_OFFSET];    // Channel message period
         stLinkHostSerialNumber.stBytes.ucByte0 = pucLinkCommand_[HOST_ID_OFFSET_0];                            // Host serial Number
         stLinkHostSerialNumber.stBytes.ucByte1 = pucLinkCommand_[HOST_ID_OFFSET_1];
         stLinkHostSerialNumber.stBytes.ucByte2 = pucLinkCommand_[HOST_ID_OFFSET_2];
         stLinkHostSerialNumber.stBytes.ucByte3 = pucLinkCommand_[HOST_ID_OFFSET_3];

         SwitchToAuthenticate();
         break;
      }
      default:
      {
         break;
      }
   }
}


/////////////////////////////////////////////////////////////////////////
// Decode Authenticate Commands
/////////////////////////////////////////////////////////////////////////
static void DecodeAuthenticateCommand(UCHAR ucControlByte_, UCHAR *pucAuthCommand_)
{
   if(pucAuthCommand_[ANTFS_CONNECTION_TYPE_OFFSET] == ANTFS_COMMAND_ID)
   {
      if(pucAuthCommand_[ANTFS_COMMAND_OFFSET] == ANTFS_CMD_AUTHENTICATE_ID)
      {
         ucLinkCommandInProgress = ANTFS_CMD_AUTHENTICATE_ID;
         ucCommandType = pucAuthCommand_[COMMAND_TYPE_OFFSET];
      }
   }
      
   if(ucLinkCommandInProgress == ANTFS_CMD_AUTHENTICATE_ID)
   {
      switch(ucCommandType)
      {
         case COMMAND_TYPE_REQUEST_SERIAL:
         {
            if(ucControlByte_ & SEQUENCE_LAST_MESSAGE) // don't do anything unless the burst completes
            {
               SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);
               AuthenticateResponse(AUTH_RESPONSE_N_A, ANTFS_REMOTE_FRIENDLY_NAME_MAX, stInitParams.pucRemoteFriendlyName);  // send my device friendly name if it exists
            }
            break;
         }
      #if defined (ANTFS_AUTH_TYPE_PASSTHROUGH)
         case COMMAND_TYPE_PROCEED:
         {
            if(ucControlByte_ & SEQUENCE_LAST_MESSAGE) // don't do anything unless the burst completes
            {
               SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);
               AuthenticateResponse(AUTH_RESPONSE_ACCEPT, 0, NULL);   // proceed directly to transport layer (no authentication required)
            }
            break;
         }
      #endif // ANTFS_AUTH_TYPE_PASSTHROUGH
      #if defined (ANTFS_AUTH_TYPE_PAIRING)
         case COMMAND_TYPE_REQUEST_PAIR:
         {
            if((ucControlByte_ & SEQUENCE_NUMBER_ROLLOVER) == 0) // first packet
            {
               stFriendlyName.ucFNSize = pucAuthCommand_[AUTH_STRING_LENGTH_OFFSET];   // friendly name length

               if (stFriendlyName.ucFNSize > 0)
               {
                  if (stFriendlyName.ucFNSize > ANTFS_FRIENDLY_NAME_MAX)
                     stFriendlyName.ucFNSize = ANTFS_FRIENDLY_NAME_MAX;
                     stFriendlyName.ucIndex = 0;
               }
            }
            else   // next packets: read host friendly name
            {
               if (stFriendlyName.ucIndex < ANTFS_FRIENDLY_NAME_MAX)
               {
                  UCHAR ucNumBytes = ANTFS_FRIENDLY_NAME_MAX - stFriendlyName.ucIndex;
                  if(ucNumBytes > 8)
                     ucNumBytes = 8;
                  memcpy((UCHAR*)&stFriendlyName.acFriendlyName[stFriendlyName.ucIndex], pucAuthCommand_, ucNumBytes);
                  stFriendlyName.ucIndex += ucNumBytes;
               }
            }

            if(ucControlByte_ & SEQUENCE_LAST_MESSAGE)    // last packet
            {
               if(PAIRING_AVAILABLE_FLAG_MASK)
               {
                  SetTimeout(ANTFS_PAIRING_TIMEOUT);
                  if(stFriendlyName.ucFNSize > 0)
                     stFriendlyName.bNameSet = TRUE;

                  stState.eSubState.eAuthSubState = ANTFS_AUTH_SUBSTATE_PAIR;
                  ANTFS_SetEvent(ANTFS_EVENT_PAIRING_REQUEST);   // If pairing is supported, send request to UI
               }
               else
               {
                  SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);
                  AuthenticateResponse(AUTH_RESPONSE_REJECT, 0, NULL);  // Pairing not supported, authentication rejected
               }
            }
            break;
         }
      #endif // ANTFS_AUTH_TYPE_PAIRING
      #if defined (ANTFS_AUTH_TYPE_PASSKEY)
         case COMMAND_TYPE_REQUEST_PASSKEY:
         {
            static UCHAR ucResponse;
            if ((ucControlByte_ & SEQUENCE_NUMBER_ROLLOVER) == 0) // initial packet
            {
               UCHAR ucPasswordSize = pucAuthCommand_[AUTH_STRING_LENGTH_OFFSET];   // Passkey length

               ucResponse = AUTH_RESPONSE_ACCEPT;

               // Check if the passkey length is valid
               if(ucPasswordSize != ANTFS_PASSKEY_SIZE)
               {
                  ucResponse = AUTH_RESPONSE_REJECT;                        
               }
               else
               {
                  ucPassKeyIndex = 0;
               }
            }
            else
            {
               UCHAR ucCounter;

               for (ucCounter = 0; ucCounter < 8; ucCounter++)
               {
                  if (ucPassKeyIndex >= ANTFS_PASSKEY_SIZE)
                     break;
                  if (stInitParams.pucPassKey[ucPassKeyIndex++] != pucAuthCommand_[ucCounter])
                  {
                     ucResponse = AUTH_RESPONSE_REJECT;      // reject authentication if passkeys are different
                  }
               }
            }

            if (ucControlByte_ & SEQUENCE_LAST_MESSAGE)   // last packet
            {
               if (ucPassKeyIndex < ANTFS_PASSKEY_SIZE)         // we did not get the complete passkey
                  ucResponse = AUTH_RESPONSE_REJECT;            // reject authentication request

               SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);
               stState.eSubState.eAuthSubState = ANTFS_AUTH_SUBSTATE_PASSKEY;
               AuthenticateResponse(ucResponse, 0, NULL);
            }
            break;
         }
      #endif // ANTFS_AUTH_TYPE_PASSKEY
         default:
         {
      #if !defined (ANTFS_AUTH_TYPE_CUSTOM)
            if (ucControlByte_ & SEQUENCE_LAST_MESSAGE)  
               AuthenticateResponse(AUTH_RESPONSE_REJECT, 0, NULL);       // reject if no other authentication method has been defined     
      #endif // ANTFS_CUSTOM_AUTH
            break;
         }
      }
   }
   else if (pucAuthCommand_[ANTFS_COMMAND_OFFSET] == ANTFS_CMD_DISCONNECT_ID)
   {
      if (ucControlByte_ & SEQUENCE_LAST_MESSAGE) // don't do anything until the burst completes
         SwitchToLink();
   }
   else if (pucAuthCommand_[ANTFS_COMMAND_OFFSET] == ANTFS_CMD_PING_ID)
   {
      SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);      // reset timeout
      ucLinkCommandInProgress = ANTFS_CMD_NONE;
   }
}

///////////////////////////////////////////////////////////////////////
// Decode Transport Commands
///////////////////////////////////////////////////////////////////////
static void DecodeTransportCommand(UCHAR ucControlByte_, UCHAR *pucTransCommand_)
{
   if(pucTransCommand_[ANTFS_CONNECTION_TYPE_OFFSET] == ANTFS_COMMAND_ID)
   {
      ucLinkCommandInProgress = pucTransCommand_[ANTFS_COMMAND_OFFSET];
   }
    
   switch (ucLinkCommandInProgress)
   {
      case ANTFS_CMD_PING_ID:     // Ping
      {
         SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);     // reset timeout
         ucLinkCommandInProgress = ANTFS_CMD_NONE;
         break;
      }
      case ANTFS_CMD_DISCONNECT_ID:       // Disconnect
      {
         if(ucControlByte_ & SEQUENCE_LAST_MESSAGE) // don't do anything unless the burst completes
            SwitchToLink();
         break;
      }
      case ANTFS_CMD_ERASE_ID:        // Erase request
      {
         if(ucControlByte_ & SEQUENCE_LAST_MESSAGE) // don't do anything unless the burst completes
         {
            uFileIndex.stBytes.ucLow = pucTransCommand_[DATA_INDEX_OFFSET_LOW];          // Requested index 
            uFileIndex.stBytes.ucHigh = pucTransCommand_[DATA_INDEX_OFFSET_HIGH];
            ANTFS_SetEvent(ANTFS_EVENT_ERASE_REQUEST);      // Send erase request to the application
            SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);     // reset timeout
            ucLinkCommandInProgress = ANTFS_CMD_ERASE_ID;
         }
         break;
      }
      case ANTFS_CMD_DOWNLOAD_ID:     // Download request
      {
         if(stState.eSubState.eTransSubState != ANTFS_TRANS_SUBSTATE_NONE)  // ignore command if we are busy
            break;
            
         if((ucControlByte_ & ~SEQUENCE_LAST_MESSAGE) == 0x00)    // first packet
         {
            if (uFileIndex.stBytes.ucLow != pucTransCommand_[DATA_INDEX_OFFSET_LOW] ||  
               uFileIndex.stBytes.ucHigh != pucTransCommand_[DATA_INDEX_OFFSET_HIGH])
               {
                  // This is a new index, so we can not check the CRC against the previous saved CRC
                  ulSavedCRCOffset = ANTFS_MAX_FILE_SIZE;      // CRC seed checking is made invalid by setting the last saved offset to the maximum file size 
               }
            uFileIndex.stBytes.ucLow = pucTransCommand_[DATA_INDEX_OFFSET_LOW];         // Requested data file index
            uFileIndex.stBytes.ucHigh = pucTransCommand_[DATA_INDEX_OFFSET_HIGH];

            uLinkBurstIndex.stBytes.ucByte0 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_0];     // Initialize current position in the Tx burst to the requested offset
            uLinkBurstIndex.stBytes.ucByte1 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_1];
            uLinkBurstIndex.stBytes.ucByte2 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_2];
            uLinkBurstIndex.stBytes.ucByte3 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_3];
         }
         else if (ucControlByte_ & SEQUENCE_LAST_MESSAGE) // last packet (download command should be two packets long)
         {
            usComparedCRC = (USHORT) pucTransCommand_[DATA_INDEX_OFFSET_LOW];           // Get CRC seed from host
            usComparedCRC |= ((USHORT) pucTransCommand_[DATA_INDEX_OFFSET_HIGH] << 8);

            uMaxBlockSize.stBytes.ucByte0 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_0];     // Maximum block size allowed by host
            uMaxBlockSize.stBytes.ucByte1 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_1]; 
            uMaxBlockSize.stBytes.ucByte2 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_2];
            uMaxBlockSize.stBytes.ucByte3 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_3];

            uBytesRemaining.ulData = uMaxBlockSize.ulData;       // Initialize number of remaining bytes for this block to the maximum block size

            if(pucTransCommand_[INITIAL_REQUEST_OFFSET])   // This request is the start of a new transfer
            {
               ulSavedCRCOffset = uLinkBurstIndex.ulData;  // initialize data offset for CRC calculation to the requested data offset
               usTransferCRC = usComparedCRC;              // use CRC seed provided by host for CRC checking of the data
               usSavedTransferCRC = usComparedCRC;         
               stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_VERIFY_CRC;
            }
            else     // This is a request to resume a partially completed transfer
            {
               if(ulSavedCRCOffset > uLinkBurstIndex.ulData)
               {
                  // We can not check the received CRC seed as the requested offset is before our last save point
                  ulSavedCRCOffset = ANTFS_MAX_FILE_SIZE;      // Set CRC checking as invalid
               }
               else 
               {
                  stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_VERIFY_CRC;
               }
            }

            bDataRequestPending = FALSE;  // clear data request pending flag

            ANTFS_SetEvent(ANTFS_EVENT_DOWNLOAD_REQUEST);  // Send download request to the application for further handling
            SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);
            ucLinkCommandInProgress = ANTFS_CMD_DOWNLOAD_ID;
         }
         break;
      }
      case ANTFS_CMD_UPLOAD_REQUEST_ID:   // Upload request
      {
   #if defined(ANTFS_INCLUDE_UPLOAD)
         if ((ucControlByte_ & ~SEQUENCE_LAST_MESSAGE) == 0x00) // first packet
         {
            if (uFileIndex.stBytes.ucLow != pucTransCommand_[DATA_INDEX_OFFSET_LOW] ||     // If it is a new index or we completed the last upload
               uFileIndex.stBytes.ucHigh != pucTransCommand_[DATA_INDEX_OFFSET_HIGH] ||
               stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_NONE)
               {
                  uFileIndex.stBytes.ucLow = pucTransCommand_[DATA_INDEX_OFFSET_LOW];        // Get file index
                  uFileIndex.stBytes.ucHigh = pucTransCommand_[DATA_INDEX_OFFSET_HIGH];
                  ulSavedCRCOffset = 0;           // As this is a new upload, reset save point to the beginning of the file
                  usSavedTransferCRC = 0;         // Set CRC to zero
               }

            uMaxTransferIndex.stBytes.ucByte0 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_0];    // Get upper limit of upload from request message
            uMaxTransferIndex.stBytes.ucByte1 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_1];
            uMaxTransferIndex.stBytes.ucByte2 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_2];
            uMaxTransferIndex.stBytes.ucByte3 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_3];
         }
         else if (ucControlByte_ & SEQUENCE_LAST_MESSAGE)   // last (second) packet
         {
            uLinkBurstIndex.stBytes.ucByte0 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_0];     // Get data offset the requested upload will start at
            uLinkBurstIndex.stBytes.ucByte1 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_1];
            uLinkBurstIndex.stBytes.ucByte2 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_2];
            uLinkBurstIndex.stBytes.ucByte3 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_3];

            if(uLinkBurstIndex.ulData != ANTFS_MAX_FILE_SIZE)   // If this is a new upload
            {
               ulSavedCRCOffset = uLinkBurstIndex.ulData;  // The data offset specified in the upload request will be used
               usSavedTransferCRC = 0;                     // Reset CRC to zero               
            }
            usTransferCRC = usSavedTransferCRC;
                
            ANTFS_SetEvent(ANTFS_EVENT_UPLOAD_REQUEST);     // Send upload request to the application for further handling
            SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);
            ucLinkCommandInProgress = ANTFS_CMD_UPLOAD_REQUEST_ID;

         }
   #endif   // ANTFS_INCLUDE_UPLOAD
         break;
      }
      case ANTFS_CMD_UPLOAD_DATA_ID:  // Upload data
      {
   #if defined(ANTFS_INCLUDE_UPLOAD)
         if ((ucControlByte_ & ~SEQUENCE_LAST_MESSAGE) == 0x00) // Decode the first packet of the burst
         {
            if (stState.eSubState.eTransSubState == ANTFS_TRANS_SUBSTATE_UPLOAD_WAIT_FOR_DATA)
            {
               UCHAR ucEvent;
               
               usComparedCRC = (USHORT) pucTransCommand_[DATA_INDEX_OFFSET_LOW];           // Get CRC Seed from host
               usComparedCRC |= ((USHORT) pucTransCommand_[DATA_INDEX_OFFSET_HIGH] << 8);

               uLinkBurstIndex.stBytes.ucByte0 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_0];      // Set download offset
               uLinkBurstIndex.stBytes.ucByte1 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_1];
               uLinkBurstIndex.stBytes.ucByte2 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_2];
               uLinkBurstIndex.stBytes.ucByte3 = pucTransCommand_[ADDRESS_PARAMETER_OFFSET_3];

               if ((uLinkBurstIndex.ulData + uBlockSize.ulData) < uMaxTransferIndex.ulData)   // Adjust block size as set by client
                  uMaxTransferIndex.ulData = uLinkBurstIndex.ulData + uBlockSize.ulData;
               
               if(usComparedCRC != usTransferCRC)	// Check request matches the CRC sent on the Upload Response
               {
               	  uMaxTransferIndex.ulData = 0;	    // Do not accept any data
                  ucEvent = 0;                      // Failure will be reported when upload is done
               }

               stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_UPLOADING;              // Ready to receive file
               ucEvent = ANTFS_EVENT_UPLOAD_START;
               usTransferCRC = usComparedCRC;

               if (uLinkBurstIndex.ulData > uMaxTransferIndex.ulData)                      // If the requested offset is too high
               {
                  uMaxTransferIndex.ulData = 0;                                           // Clear the max transfer index, so we'll report fail when the transfer finishes
                  ucEvent = 0;                                                            // Clear the event because we normally would not send an event at this point in this case
               }

               if (ucControlByte_ & SEQUENCE_LAST_MESSAGE)     // if this upload contains no data
               {
                  stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_NONE;           // leave the upload state

                  if (!ucEvent)                            // if it was a valid index, report it as a successful upload, otherwise report it as a failure
                     ucEvent = ANTFS_EVENT_UPLOAD_FAIL;
                  else
                     ucEvent = ANTFS_EVENT_UPLOAD_COMPLETE;
               }

               if (ucEvent)
               {
                  ANTFS_SetEvent(ucEvent);
               }
            }
         }
   #endif // ANTFS_INCLUDE_UPLOAD
         break;
      }
      case ANTFS_CMD_LINK_ID:
      {
         ULONG_UNION uHostSerialNumber;
         
         uHostSerialNumber.stBytes.ucByte0 = pucTransCommand_[HOST_ID_OFFSET_0];
         uHostSerialNumber.stBytes.ucByte1 = pucTransCommand_[HOST_ID_OFFSET_1];
         uHostSerialNumber.stBytes.ucByte2 = pucTransCommand_[HOST_ID_OFFSET_2];
         uHostSerialNumber.stBytes.ucByte3 = pucTransCommand_[HOST_ID_OFFSET_3];
 
         if (stLinkHostSerialNumber.ulData == uHostSerialNumber.ulData)                 
         {
            ucActiveBeaconFrequency = pucTransCommand_[TRANSPORT_CHANNEL_FREQUENCY_OFFSET];
            stActiveBeaconStatus1.stParameters.ucLinkPeriod = pucTransCommand_[TRANSPORT_MESSAGE_PERIOD_OFFSET];

            ANT_ChannelRFFreq(ANTFS_CHANNEL, ucActiveBeaconFrequency);
            ANTFS_SetChannelPeriod(stActiveBeaconStatus1.stParameters.ucLinkPeriod);
         }
         ucLinkCommandInProgress = 0;
         break;
      }
      default:
      {
         ucLinkCommandInProgress = 0;                 // don't do anything, this is an invalid message
         break;
      }  
   }
}

///////////////////////////////////////////////////////////////////////
// Handle uploads (data)
///////////////////////////////////////////////////////////////////////
static void UploadInputData(UCHAR ucControlByte_, UCHAR* pucMesg_)
{
#if defined(ANTFS_INCLUDE_UPLOAD)
   if (ucControlByte_ & SEQUENCE_LAST_MESSAGE)  // Upload complete
   {
      stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_NONE;          
      usComparedCRC = (USHORT) pucMesg_[UPLOAD_CRC_OFFSET_LOW ];         // CRC for data packets contained in this upload block
      usComparedCRC |= ((USHORT) pucMesg_[UPLOAD_CRC_OFFSET_HIGH] << 8);

      if (uMaxTransferIndex.ulData && usComparedCRC == usTransferCRC)    // Compare CRC
         ANTFS_SetEvent(ANTFS_EVENT_UPLOAD_COMPLETE);                    // CRC OK, upload was completed successfully
      else
         ANTFS_SetEvent(ANTFS_EVENT_UPLOAD_FAIL);                        // CRC mismatch, upload failed

      uMaxTransferIndex.ulData = 0;                                      // clear max upload index
   }
   else    // Handle uploaded data packets
   {
      ucBytesToWrite = 8;                                                 // set initial number of bytes to 8 (size of burst packet)
      SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);                             // reset timeout

      if (uLinkBurstIndex.ulData > uMaxTransferIndex.ulData)              // if we're past the main index
      {
         ucBytesToWrite = 0;                                             // We do not need to write any more data
      }
      else
      {
         if((ucBytesToWrite + uLinkBurstIndex.ulData) > uMaxTransferIndex.ulData)   // if we're less than 8 bytes away from the end
         {
            ucBytesToWrite = uMaxTransferIndex.ulData - uLinkBurstIndex.ulData;     // adjust the number of bytes to write in this block
         }
      }
      if (ucBytesToWrite) // If there is data to write
      {
         memcpy(aucUploadBuffer, pucMesg_, ucBytesToWrite);              //  Get block of data      
         usTransferCRC = CRC_UpdateCRC16(usTransferCRC, pucMesg_, ucBytesToWrite);   // Update CRC
         ANTFS_SetEvent(ANTFS_EVENT_UPLOAD_DATA);                        // Send data to application
         uLinkBurstIndex.ulData += ucBytesToWrite;                       // Update current offset
         ulSavedCRCOffset = uLinkBurstIndex.ulData;                      // Store save point
         usSavedTransferCRC = usTransferCRC;
      }
   }
#endif // ANTFS_INCLUDE_UPLOAD
}

///////////////////////////////////////////////////////////////////////
// Load Beacon
///////////////////////////////////////////////////////////////////////
static BOOL LinkLoadBeacon(UCHAR ucMessageType_)
{
   UCHAR ucLinkStatus2;
   UCHAR aucTemp[8];
   BOOL bSuccess = FALSE;

   // Status Byte 2
   if (ucLinkCommandInProgress == ANTFS_CMD_NONE)
   {
      switch (stState.eState)
      {
         case ANTFS_STATE_AUTH:
         {
            ucLinkStatus2 = 1;
            break;
         }
         case ANTFS_STATE_TRANS:
         {
            ucLinkStatus2 = 2; 
            break;
         }
         case ANTFS_STATE_INIT:
         case ANTFS_STATE_LINK:
         default:
         {
            ucLinkStatus2 = 0;
            break;
         }
      }
   }
   else
   {
      ucLinkStatus2 = DEVICE_STATE_BUSY;
   }

   aucTemp[0] = ANTFS_BEACON_ID;                            // ANT-FS Beacon ID
   aucTemp[1] = stActiveBeaconStatus1.ucBeaconStatus1;      // Status Byte 1
   aucTemp[2] = ucLinkStatus2;                              // Status Byte 2
   aucTemp[3] = AUTHENTICATION_TYPE;                        // Authentication Type

   if ((stState.eState == ANTFS_STATE_AUTH) || (stState.eState == ANTFS_STATE_TRANS))
   {
      aucTemp[4]  = stLinkHostSerialNumber.stBytes.ucByte0;     // Host serial number
      aucTemp[5]  = stLinkHostSerialNumber.stBytes.ucByte1;
      aucTemp[6]  = stLinkHostSerialNumber.stBytes.ucByte2;
      aucTemp[7]  = stLinkHostSerialNumber.stBytes.ucByte3;
   }
   else
   {
      aucTemp[4]  = (UCHAR)(stInitParams.usBeaconDeviceType);           // Device type
      aucTemp[5]  = (UCHAR)(stInitParams.usBeaconDeviceType >> 8);
      aucTemp[6]  = (UCHAR)(stInitParams.usBeaconDeviceManufID);        // Manufacturer
      aucTemp[7]  = (UCHAR)(stInitParams.usBeaconDeviceManufID >> 8);
   }
   
   if (ucMessageType_ == MESG_BROADCAST_DATA_ID)
   {
      bSuccess = ANT_Broadcast(ANTFS_CHANNEL, aucTemp); // Send as broadcast

   }
   else if(ucMessageType_ == MESG_BURST_DATA_ID)
   {
      bSuccess = !ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, 1, ANTFS_BURST_PARTIAL);   // Send as the first packet of a burst
      DisableTimeout();     // This is the first packet of a burst response, disable command timeout while bursting
   }

   return(bSuccess);
}


///////////////////////////////////////////////////////////////////////
// Send Auth Response
///////////////////////////////////////////////////////////////////////
static void AuthenticateResponse(UCHAR ucResponseType_, UCHAR ucPasswordLength_, const UCHAR* pucPassword_)
{
   ULONG_UNION stESN;
   UCHAR aucTemp[8];
   USHORT usPacketNum = 1;

   stESN.ulData = stInitParams.ulESN;

   // Beacon
   LinkLoadBeacon(MESG_BURST_DATA_ID);    // First packet is beacon
   usPacketNum++;

   // Response 
   aucTemp[ANTFS_CONNECTION_TYPE_OFFSET] = ANTFS_COMMAND_ID;
   aucTemp[ANTFS_RESPONSE_OFFSET] = ANTFS_RSP_AUTHENTICATE_ID;
   aucTemp[RESPONSE_TYPE_OFFSET]  = ucResponseType_;               // Authentication response
   aucTemp[AUTH_STRING_LENGTH_OFFSET] = ucPasswordLength_;         // Length of authentication string
   aucTemp[SERIAL_NUMBER_OFFSET_0] = stESN.stBytes.ucByte0;        // Client serial number
   aucTemp[SERIAL_NUMBER_OFFSET_1] = stESN.stBytes.ucByte1;
   aucTemp[SERIAL_NUMBER_OFFSET_2] = stESN.stBytes.ucByte2;
   aucTemp[SERIAL_NUMBER_OFFSET_3] = stESN.stBytes.ucByte3;   

   if( stState.eState == ANTFS_STATE_AUTH && ucResponseType_ != AUTH_RESPONSE_REJECT && ucPasswordLength_ && ucPasswordLength_ <= ANTFS_AUTH_STRING_MAX)
   {
      UCHAR aucAuthTemp[ANTFS_AUTH_STRING_MAX + 1];   // round to a multiple of 8 bytes
      USHORT usNumPassPackets = ucPasswordLength_ / 8;       // Number of packets for authentication string (8 byte packets)

      ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_PARTIAL);    // Send second packet (auth response)

      // Append authentication string
      if(ucPasswordLength_ % 8)
         usNumPassPackets++;
      usPacketNum++;
      memset(aucAuthTemp, 0, ANTFS_AUTH_STRING_MAX + 1);      // pad with zeroes
      memcpy(aucAuthTemp, pucPassword_, ucPasswordLength_);
      ANT_SendPartialBurst(ANTFS_CHANNEL, aucAuthTemp, usNumPassPackets, usPacketNum, ANTFS_BURST_COMPLETE);  // Send auth string (last packets of the burst)

      ucLinkCommandInProgress = ANTFS_RSP_AUTHENTICATE_ID;
   }
   else // If the authorization is rejected or there is no valid password, the auth response is the last packet
   {
      ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_COMPLETE);
   }

   // Switch to appropiate state
   if(ucResponseType_ == AUTH_RESPONSE_REJECT)
      stState.eSubState.eAuthSubState = ANTFS_AUTH_SUBSTATE_REJECT;
   else if(ucResponseType_ == AUTH_RESPONSE_ACCEPT)
      stState.eSubState.eAuthSubState = ANTFS_AUTH_SUBSTATE_ACCEPT;
}

///////////////////////////////////////////////////////////////////////
// Send Download Response
///////////////////////////////////////////////////////////////////////
static BOOL DownloadResponse(UCHAR ucResponse_)
{
   UCHAR aucTemp[8];
   USHORT usPacketNum = 1;

   LinkLoadBeacon(MESG_BURST_DATA_ID);     // First packet is beacon
   usPacketNum++;

   // Next send the first part of the download response
   aucTemp[0] = ANTFS_COMMAND_ID;
   aucTemp[1] = ANTFS_RSP_DOWNLOAD_ID;
   aucTemp[2] = ucResponse_;                           // Response code
   aucTemp[3] = 0;                                     // Set to 0
   aucTemp[4] = uBytesRemaining.stBytes.ucByte0;       // Total number of bytes remaining in the data block
   aucTemp[5] = uBytesRemaining.stBytes.ucByte1;
   aucTemp[6] = uBytesRemaining.stBytes.ucByte2;
   aucTemp[7] = uBytesRemaining.stBytes.ucByte3;  

   ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_PARTIAL);   
   usPacketNum++;

   // Second byte of the download response
   aucTemp[0] = uLinkBurstIndex.stBytes.ucByte0;     // The offset the data will start from in this block
   aucTemp[1] = uLinkBurstIndex.stBytes.ucByte1;
   aucTemp[2] = uLinkBurstIndex.stBytes.ucByte2;
   aucTemp[3] = uLinkBurstIndex.stBytes.ucByte3;
   aucTemp[4] = uFileSize.stBytes.ucByte0;             // The file size in the client device
   aucTemp[5] = uFileSize.stBytes.ucByte1;
   aucTemp[6] = uFileSize.stBytes.ucByte2;
   aucTemp[7] = uFileSize.stBytes.ucByte3;  

   if(ucResponse_ || uBytesRemaining.ulData == 0)   // if the download was rejected or there is no data to send
      ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_COMPLETE);  // set response to end since we're not downloading any data  
   else 
      ANT_SendPartialBurst(ANTFS_CHANNEL, aucTemp, 1, usPacketNum, ANTFS_BURST_PARTIAL);   // response will continue (data packets + CRC footer to follow)

   usPacketNum++;
   ulCurrentPacketNum = usPacketNum;    // set first sequence number for the download data
   ucLinkCommandInProgress = ANTFS_CMD_DOWNLOAD_ID;

   if(!ucResponse_)    // if we are going to download (response OK), enter the downloading substate
   {
      stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_DOWNLOADING;
      ANTFS_SetEvent(ANTFS_EVENT_DOWNLOAD_START);
   }
   else
   {
      ANTFS_SetEvent(ANTFS_EVENT_DOWNLOAD_FAIL);  // Download rejected
   }

   return TRUE;

}

///////////////////////////////////////////////////////////////////////
// Switch to LINK layer
///////////////////////////////////////////////////////////////////////
static void SwitchToLink(void)
{
   if (stState.eState != ANTFS_STATE_OFF)
   {
      stState.eState = ANTFS_STATE_LINK;
      stState.eSubState.eLinkSubState = ANTFS_LINK_SUBSTATE_NONE;
      ucLinkCommandInProgress = ANTFS_CMD_NONE;
      stActiveBeaconStatus1 = stInitParams.stBeaconStatus1;
      ucActiveBeaconFrequency = stInitParams.ucBeaconFrequency;
      DisableTimeout();    // Disable timeout
      ANT_ChannelRFFreq(ANTFS_CHANNEL, ucActiveBeaconFrequency);
      ANTFS_SetEvent(ANTFS_EVENT_LINK);
   }
}

/////////////////////////////////////////////////////////////////////////
// Switch to AUTHENTICATION layer
/////////////////////////////////////////////////////////////////////////
static void SwitchToAuthenticate(void)
{
   if (stState.eState != ANTFS_STATE_OFF)
   {
      stState.eState = ANTFS_STATE_AUTH;
      stState.eSubState.eAuthSubState = ANTFS_AUTH_SUBSTATE_NONE;
      ucLinkCommandInProgress = ANTFS_CMD_NONE;     // No command in progress
      SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);       // Reset timeout
      ANT_ChannelRFFreq(ANTFS_CHANNEL, ucActiveBeaconFrequency);
      ANTFS_SetEvent(ANTFS_EVENT_AUTH);
   }
}

/////////////////////////////////////////////////////////////////////////
// Switch to TRANSPORT layer
/////////////////////////////////////////////////////////////////////////
static void SwitchToTransport(void)
{
   if (stState.eState != ANTFS_STATE_OFF)
   {
      stState.eState = ANTFS_STATE_TRANS;
      stState.eSubState.eTransSubState = ANTFS_TRANS_SUBSTATE_NONE;
      SetTimeout(ANTFS_LINK_COMMAND_TIMEOUT);      // Reset timeout
      LinkLoadBeacon(MESG_BROADCAST_DATA_ID);      // Reload beacon
      ANTFS_SetEvent(ANTFS_EVENT_TRANS);
   }
}

/////////////////////////////////////////////////////////////////////////
// Set the beacon period
// The only allowed frequencies are 0.5, 1, 2, 4 and 8 Hz
/////////////////////////////////////////////////////////////////////////
static void ANTFS_SetChannelPeriod(UCHAR ucLinkPeriod_)
{
   USHORT usPeriod;

   switch (ucLinkPeriod_)
   {
      default: // Shouldn't happen.
      case BEACON_PERIOD_0_5_HZ:
         usPeriod = 65535;
         break;
      case BEACON_PERIOD_1_HZ:
         usPeriod = 32768;
         break;
      case BEACON_PERIOD_2_HZ:
         usPeriod = 16384;
         break;
      case BEACON_PERIOD_4_HZ:
         usPeriod = 8192;
         break;
      case BEACON_PERIOD_8_HZ:
         usPeriod = 4096;
         break;
   }
   ANT_ChannelPeriod(ANTFS_CHANNEL, usPeriod);
}

/////////////////////////////////////////////////////////////////////////
// ANTFS Timer Event
// Handles command timeouts
// usTimeStamp is time of event in 1/1024 s
/////////////////////////////////////////////////////////////////////////
void ANTFS_Tick(USHORT usTimeStamp_, UCHAR ucAlarmNumber_)      
{
   if (stState.eState == ANTFS_STATE_OFF)
      return;
         
   if(stState.eState == ANTFS_STATE_AUTH && stState.eSubState.eAuthSubState == ANTFS_AUTH_SUBSTATE_PAIR)   // Pairing timeout
   {
      AuthenticateResponse(AUTH_RESPONSE_REJECT, 0, NULL);     // Reject authentication request
      ANTFS_SetEvent(ANTFS_EVENT_PAIRING_TIMEOUT);   // Set pairing timeout event
   }
   SwitchToLink();    // Fall back to link layer if an ANT-FS event times out
}

/////////////////////////////////////////////////////////////////////////
// Set Timeout
// usTimeout: timeout, in seconds
/////////////////////////////////////////////////////////////////////////
BOOL SetTimeout(USHORT usTimeout)
{
   USHORT usTicks = usTimeout * ALARM_TIMER_PERIOD;
   if(Timer_Start(ucAlarmNumber, usTicks))
      return TRUE;
   
   return FALSE;
}

/////////////////////////////////////////////////////////////////////////
// Clear Timeout
/////////////////////////////////////////////////////////////////////////
void DisableTimeout(void)
{
   ucLinkCommandInProgress = ANTFS_CMD_NONE;
   Timer_Stop(ucAlarmNumber);
}

/////////////////////////////////////////////////////////////////////////
// ANT-FS Event Generator
// Adds an ANT-FS event to the queue
// Returns TRUE if the event was successfully added, and FALSE if the 
// queue was full
/////////////////////////////////////////////////////////////////////////
BOOL ANTFS_SetEvent(UCHAR ucEventCode_)
{
   ANTFS_EVENT_RETURN* pstEvent = NULL;
   
   // Check if there is room in the queue for a new event
   if(((stTheEventQueue.ucHead+1) & (ANTFS_EVENT_QUEUE_SIZE -1)) != stTheEventQueue.ucTail)
      pstEvent = &(stTheEventQueue.astQueue[stTheEventQueue.ucHead]);

   if(pstEvent)
   {
      // Initialize event parameters
      pstEvent->eEvent = (ANTFS_EVENT) ucEventCode_;

      // Set parameters depending on event type
      switch (ucEventCode_)
      {
         case ANTFS_EVENT_ERASE_REQUEST: 
         {
            pstEvent->usFileIndex = uFileIndex.usData;              // File index
            pstEvent->ulOffset = 0;
            pstEvent->ulBytes = 0;
            pstEvent->usCRC = 0;
            break;
         }
         case ANTFS_EVENT_DOWNLOAD_REQUEST:
         {
            pstEvent->usFileIndex = uFileIndex.usData;              // File index
            pstEvent->ulOffset = uLinkBurstIndex.ulData;            // Requested offset for the download
            pstEvent->ulBytes = 0;
            pstEvent->usCRC = 0;
            break;
         }
         case ANTFS_EVENT_DOWNLOAD_REQUEST_DATA:
         {
            pstEvent->usFileIndex = uFileIndex.usData;              // File index
            pstEvent->ulOffset = uLinkBurstIndex.ulData;            // Current offset
            if(uBytesRemaining.ulData > ANTFS_BURST_BLOCK_SIZE*8)   // If remaining bytes > burst block size
               pstEvent->ulBytes = ANTFS_BURST_BLOCK_SIZE*8;       // Grab one block at a time
            else
               pstEvent->ulBytes = uBytesRemaining.ulData;         // Remaining bytes
            pstEvent->usCRC = 0;
            break;
         } 
         case ANTFS_EVENT_UPLOAD_REQUEST:
         {
            pstEvent->usFileIndex = uFileIndex.usData;              // File index
            pstEvent->ulOffset = uLinkBurstIndex.ulData;            // Requested offset for the upload
            pstEvent->ulBytes = uMaxTransferIndex.ulData;           // Upper limit of the download (offset + remaining bytes)
            pstEvent->usCRC = usTransferCRC;                        // CRC Seed (from last save point if resuming)
            break;
         }
         case ANTFS_EVENT_UPLOAD_DATA:
         {
            pstEvent->usFileIndex = uFileIndex.usData;              // File index
            pstEvent->ulOffset = uLinkBurstIndex.ulData;            // Current offset
            pstEvent->usCRC = usTransferCRC;                        // Current CRC
            pstEvent->ulBytes = ucBytesToWrite;                     // Number of bytes to write
            memcpy(pstEvent->aucData, aucUploadBuffer, ucBytesToWrite);    // Upload data buffer
            break;
         }
         default:
         {
            // No parameters need to be set
            pstEvent->usFileIndex = 0;
            pstEvent->ulOffset = 0;
            pstEvent->ulBytes = 0;
            pstEvent->usCRC = 0;
            break;
         }
      }

      stTheEventQueue.ucHead = ((stTheEventQueue.ucHead + 1) & (ANTFS_EVENT_QUEUE_SIZE - 1)); // put the event in the queue
      return TRUE;
   }

   return FALSE;
}

