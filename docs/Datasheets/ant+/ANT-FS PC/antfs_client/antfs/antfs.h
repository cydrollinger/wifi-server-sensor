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

#ifndef __ANTFS_H__
#define __ANTFS_H__

#include "config.h"
#include "types.h"
#include "antinterface.h"   

#define ANTFS_VERSION_MAJOR         ((USHORT) 1)
#define ANTFS_VERSION_MINOR         ((USHORT) 0)
#define ANTFS_VERSION_ITERATION     ((USHORT) 0)
#define ANTFS_VERSION_TYPE          ((char) 'R')      // Release
#define ANTFS_VERSION_SPEC          ((char) '0.AK')   // Version of the ANT-FS Technology Specification
#define ANTFS_DIR_STRUCT_VERSION    ((UCHAR) 1)       // Version of the directory file structure
#define ANTFS_VERSION_DATE          ((ULONG) 20090522)

// Compile switches
#define ANTFS_AUTH_TYPE_PAIRING        // Use pairing and key exchange authentication
#define ANTFS_AUTH_TYPE_PASSKEY        // Use passkey authentication
#define ANTFS_AUTH_TYPE_PASSTHROUGH    // Allow host to bypass authentication
#define ANTFS_INCLUDE_UPLOAD           // Support upload operation

// ANT FS Options
#define ANTFS_NETWORK_NUMBER           ((UCHAR) 0)             // ANT-FS Network Number
#define ANTFS_CHANNEL                  ((UCHAR) 0)             // ANT Channel Number
#define ANTFS_LINK_FREQ                ((UCHAR) 50)            // RF Frequency (+2400MHz)
#define ANTFS_PAIRING_TIMEOUT          ((USHORT) 120)           // Pairing timeout (time the UI will wait for a response to the pairing request before switching to the link layer, in seconds)
#define ANTFS_LINK_COMMAND_TIMEOUT     ((UCHAR) 60)            // Command timeout (time the client will wait without receiving any commands before switching to the link layer, in seconds)
#define ANTFS_TRANS_TYPE               ((UCHAR) 5)             // ANT-FS Transmission Type
#define ANTFS_DEVICE_TYPE              ((UCHAR) 1)             // ANT-FS Device Type
#define ANTFS_CHANNEL_TYPE             CHANNEL_TYPE_MASTER     // ANT-FS Client Channel Type
#define ANTFS_BEACON_PERIOD_STATUS     BEACON_PERIOD_8_HZ      // ANT-FS Beacon Message Period

#define ANTFS_TRANSMIT_POWER           ((UCHAR) 0)             // ANT Transmit Power (0dBm)

#define ANTFS_NETWORK_KEY              { 0xA8,0xA4,0x23,0xB9,0xF5,0x5E,0x63,0xC1 }
#define ANTFS_AUTH_STRING_MAX          ((UCHAR) 255)           // Maximum size of authentication strings (passkey/friendly name)
#define ANTFS_PASSKEY_SIZE             ((UCHAR) 16)            // Passkey size
#define ANTFS_FRIENDLY_NAME_MAX        ((UCHAR) 16)            // Maximum size of friendly name received from host
#define ANTFS_REMOTE_FRIENDLY_NAME_MAX ((UCHAR) 16)            // Maximum size of client's friendly name

//////////////////////////////////////////////////////////////////////////////////
// Public Definitions
//////////////////////////////////////////////////////////////////////////////////

// Beacon definitions
#define BEACON_PERIOD_SHIFT            ((UCHAR) 0x00)
#define BEACON_PERIOD_MASK             ((UCHAR) (0x07 << BEACON_PERIOD_SHIFT))
#define BEACON_PERIOD_0_5_HZ           ((UCHAR) (0x00 << BEACON_PERIOD_SHIFT))
#define BEACON_PERIOD_1_HZ             ((UCHAR) (0x01 << BEACON_PERIOD_SHIFT))
#define BEACON_PERIOD_2_HZ             ((UCHAR) (0x02 << BEACON_PERIOD_SHIFT))
#define BEACON_PERIOD_4_HZ             ((UCHAR) (0x03 << BEACON_PERIOD_SHIFT))
#define BEACON_PERIOD_8_HZ             ((UCHAR) (0x04 << BEACON_PERIOD_SHIFT))
#define PAIRING_AVAILABLE_FLAG_SHIFT   ((UCHAR) 0x03)
#define PAIRING_AVAILABLE_FLAG_MASK    ((UCHAR) (0x01 << PAIRING_AVAILABLE_FLAG_SHIFT))
#define UPLOAD_ENABLED_FLAG_SHIFT      ((UCHAR) 0x04)
#define UPLOAD_ENABLED_FLAG_MASK       ((UCHAR) (0x01 << UPLOAD_ENABLED_FLAG_SHIFT))
#define DATA_AVAILABLE_FLAG_SHIFT      ((UCHAR) 0x05)
#define DATA_AVAILABLE_FLAG_MASK       ((UCHAR) (0x01 << DATA_AVAILABLE_FLAG_SHIFT))
#define REQUEST_TO_SEND_FLAG_SHIFT     ((UCHAR) 0x06)
#define REQUEST_TO_SEND_FLAG_MASK      ((UCHAR) (0x01 << REQUEST_TO_SEND_FLAG_SHIFT))

// Build the default beacon settings
#if defined (ANTFS_AUTH_TYPE_PAIRING)
   #define ANTFS_PAIRING_BIT           PAIRING_AVAILABLE_FLAG_MASK
#else
   #define ANTFS_PAIRING_BIT           ((UCHAR) 0x00)
#endif // ANTFS_INCLUDE_PAIRING   
#if defined (ANTFS_INCLUDE_UPLOAD)
   #define ANTFS_UPLOAD_BIT            UPLOAD_ENABLED_FLAG_MASK
#else                   
   #define ANTFS_UPLOAD_BIT            ((UCHAR) 0x00)
#endif // ANTFS_INCLUDE_UPLOAD  

#define ANTFS_DEFAULT_BEACON           (ANTFS_BEACON_PERIOD_STATUS | ANTFS_UPLOAD_BIT | ANTFS_PAIRING_BIT | DATA_AVAILABLE_FLAG_MASK)


// Download/Upload/Erase Responses
// Download/Upload
#define RESPONSE_MESSAGE_OK            ((UCHAR) 0x00)
#define RESPONSE_MESSAGE_NOT_EXIST     ((UCHAR) 0x01)          // File does not exist
#define RESPONSE_MESSAGE_NOT_AVAILABLE ((UCHAR) 0x02)          // File can not be read/written to (download/upload respectively)
// Download
#define RESPONSE_MESSAGE_NOT_READY     ((UCHAR) 0x03)
#define RESPONSE_INVALID_OPERATION     ((UCHAR) 0x04)
#define RESPONSE_INVALID_CRC           ((UCHAR) 0x05)
// Upload 
#define RESPONSE_MESSAGE_NOT_ENOUGH_SPACE    ((UCHAR) 0x03)
// Upload/Erase
#define RESPONSE_MESSAGE_FAIL          ((UCHAR) 0x01)

// Directory
// General File Flags
#define ANTFS_DIR_READ_MASK            ((UCHAR) 0x80)          // Read (can download)
#define ANTFS_DIR_WRITE_MASK           ((UCHAR) 0x40)          // Write (can upload)
#define ANTFS_DIR_ERASE_MASK           ((UCHAR) 0x20)          // Erase (can erase)
#define ANTFS_DIR_ARCHIVE_MASK         ((UCHAR) 0x10)          // Archive (been downloaded)
#define ANTFS_DIR_APPEND_MASK          ((UCHAR) 0x08)          // Append (can append to file only)

#define ANTFS_MAX_FILE_SIZE            ((ULONG) 0xFFFFFFFF)    // Maximum file size, as specified by directory structure
#define ANTFS_BURST_BLOCK_SIZE         ((UCHAR) 8)             // Size of each block of burst data that the client attempts to send when it processes a data request event

//////////////////////////////////////////////////////////////////////////////////
// Public structs and enums
//////////////////////////////////////////////////////////////////////////////////

// Beacon Status
typedef union
{
   UCHAR ucBeaconStatus1;
   struct
   {
      UCHAR ucLinkPeriod      : 3;     // Beacon period (0.5 - 8 Hz)
      BOOL bPairingEnabled    : 1;     // Pairing is enabled/disabled
      BOOL bUploadEnabled     : 1;     // Upload is enabled/disabled
      BOOL bDataAvailable     : 1;     // Data is available for download / no data available
      BOOL                    : 2;
   } stParameters;
} ANTFS_BEACON_STATUS1;       // Beacon Status Byte 1

// ANT-FS States
typedef enum
{
   ANTFS_STATE_OFF = 0,
   ANTFS_STATE_INIT,
   ANTFS_STATE_LINK,
   ANTFS_STATE_AUTH,
   ANTFS_STATE_TRANS
} ANTFS_STATE;

typedef enum
{
   ANTFS_LINK_SUBSTATE_NONE
} ANTFS_LINK_SUBSTATE;

typedef enum
{
   ANTFS_AUTH_SUBSTATE_NONE,
   ANTFS_AUTH_SUBSTATE_PAIR,
   ANTFS_AUTH_SUBSTATE_PASSKEY,
   ANTFS_AUTH_SUBSTATE_ACCEPT,
   ANTFS_AUTH_SUBSTATE_REJECT
} ANTFS_AUTH_SUBSTATE;

typedef enum
{
   ANTFS_TRANS_SUBSTATE_NONE,
   ANTFS_TRANS_SUBSTATE_VERIFY_CRC,
   ANTFS_TRANS_SUBSTATE_DOWNLOADING,
   ANTFS_TRANS_SUBSTATE_UPLOAD_WAIT_FOR_DATA,
   ANTFS_TRANS_SUBSTATE_UPLOADING,
   ANTFS_TRANS_SUBSTATE_UPLOAD_RESUME
} ANTFS_TRANS_SUBSTATE;

// ANT-FS Events
typedef enum {
   ANTFS_EVENT_PAIRING_REQUEST =       (UCHAR) 0xB0,
   ANTFS_EVENT_PAIRING_TIMEOUT =       (UCHAR) 0xB1,
   ANTFS_EVENT_OPEN_COMPLETE =         (UCHAR) 0xB2,  // Serial Int case only
   ANTFS_EVENT_OPEN_FAIL =             (UCHAR) 0xB3,  // Serial Int case only
   ANTFS_EVENT_CLOSE_COMPLETE =        (UCHAR) 0xB4,  // Serial Int case only
   ANTFS_EVENT_CLOSE_FAIL =            (UCHAR) 0xB5,  // Serial Int case only
   ANTFS_EVENT_LINK =                  (UCHAR) 0xB6,
   ANTFS_EVENT_AUTH =                  (UCHAR) 0xB7,
   ANTFS_EVENT_TRANS =                 (UCHAR) 0xB8,
   ANTFS_EVENT_DOWNLOAD_REQUEST =      (UCHAR) 0xB9,
   ANTFS_EVENT_DOWNLOAD_REQUEST_DATA = (UCHAR) 0xBA,
   ANTFS_EVENT_DOWNLOAD_START  =       (UCHAR) 0xBB,
   ANTFS_EVENT_DOWNLOAD_COMPLETE =     (UCHAR) 0xBC,
   ANTFS_EVENT_DOWNLOAD_FAIL =         (UCHAR) 0xBD,
   ANTFS_EVENT_UPLOAD_REQUEST =        (UCHAR) 0xBE,
   ANTFS_EVENT_UPLOAD_DATA =           (UCHAR) 0xBF,
   ANTFS_EVENT_UPLOAD_START  =         (UCHAR) 0xC0,
   ANTFS_EVENT_UPLOAD_COMPLETE =       (UCHAR) 0xC1,
   ANTFS_EVENT_UPLOAD_FAIL =           (UCHAR) 0xC2,
   ANTFS_EVENT_ERASE_REQUEST =         (UCHAR) 0xC3
} ANTFS_EVENT;

typedef struct
{
   ANTFS_EVENT eEvent;                 // Event
   USHORT usFileIndex;                 // File index (download/upload/erase)
   ULONG ulOffset;                     // Current offset (download/upload)
   ULONG ulBytes;                      // Number of bytes in block (download/upload)
   USHORT usCRC;                       // Current CRC (upload)
   UCHAR aucData[8];                   // Block of data (upload)
} ANTFS_EVENT_RETURN;

// ANT-FS Parameters
typedef struct
{
   ULONG ulESN;                        // Client serial number
   USHORT usBeaconDeviceType;          // Client device type
   USHORT usBeaconDeviceManufID;       // Client manufacturing ID
   UCHAR ucBeaconFrequency;            // Beacon RF Frequency
   ANTFS_BEACON_STATUS1 stBeaconStatus1;   // Status Byte 1
   const UCHAR *pucPassKey;            // Pass Key
   const UCHAR *pucRemoteFriendlyName; // Friendly Name
} ANTFS_PARAMS;

// ANT-FS Directory Header Structure
// !!! Beware of endianness when porting to a different architecture
typedef struct
{
   UCHAR ucVersion;                    // Version of the directory file structure
   UCHAR ucLength;                     // Length of each structure, in bytes
   UCHAR ucTimeFormat;                 // Defines how system keeps track of date/time stamps
   UCHAR ucReserved01;
   UCHAR ucReserved02;
   UCHAR ucReserved03;
   UCHAR ucReserved04;
   UCHAR ucReserved05;
   ULONG ulSystemTime;                 // Number of seconds elapsed since system power up
   ULONG ulDate;                       // Number of seconds elapsed since 00:00 hrs Dec 31, 1989. If system time is unknown, used as counter.
} ANTFS_DIR_HEADER;

// ANT-FS Directory Entry Structure
// !!! Beware of endianness when porting to a different architecture
typedef struct
{
   USHORT usIndex;                     // Data file index
   UCHAR ucDataType;                   // File data type
   UCHAR ucUserDefined1;               // Identifier, first byte (structure defined by data type)
   USHORT usUserDefined2;              // Identifier, last two bytes (structure defined by data type)
   UCHAR ucUserFlags;                  // File data type specific flags (bits defined by data type)     
   UCHAR ucGeneralFlags;               // Bit mapped flags of flag permissions
   ULONG ulSize;                       // File size, in bytes
   ULONG ulDate;                       // Number of seconds elapsed since 00:00 hrs Dec 31, 1980, if supported
} ANTFS_DIR_STRUCT;

typedef struct
{
   ULONG_UNION uSize;                  // Size of a file to download when reading, or the size of a partially completed upload when writing
   ULONG ulMaxSize;                    // The maximum size of the file specified, this is the file size when reading, and the maximum allowed file size when writing
   ULONG_UNION uBlockSize;             // Maximum burst block size
   USHORT_UNION uIndex;                // File index
   USHORT usFileCRC;                   // CRC (uploads)
} ANTFS_REQUEST_INFO;

//////////////////////////////////////////////////////////////////////////////////
// Public Function Prototypes
//////////////////////////////////////////////////////////////////////////////////

void 
ANTFS_Init(
   ANTFS_PARAMS* pstInitParams_);

BOOL 
ANTFS_OpenBeacon();

BOOL 
ANTFS_CloseBeacon(); 

ANTFS_STATE 
ANTFS_GetState();

char* 
ANTFS_GetHostName();

BOOL 
ANTFS_PairingResponse(
   BOOL bAccept_);

BOOL 
ANTFS_DownloadRequestResponse(
   UCHAR ucResponse_, 
   ANTFS_REQUEST_INFO* stRequestInfo_);

ULONG 
ANTFS_DownloadInputData(
   USHORT usIndex_, 
   ULONG ulOffset_, 
   ULONG ulNumBytes, 
   UCHAR *pucMesg);

BOOL 
ANTFS_UploadRequestResponse(
   UCHAR ucResponse_, 
   ANTFS_REQUEST_INFO* stRequestInfo_);

BOOL 
ANTFS_UploadResponse(
   BOOL bAccept_);

BOOL 
ANTFS_EraseRequestResponse(
   UCHAR ucResponse_);

BOOL 
ANTFS_CheckEvents(
   ANTFS_EVENT_RETURN* pstEvent_);

BOOL 
ANTFS_ProcessMessage(
   UCHAR* pucRxMesg_);

#endif // ANTFS_H
