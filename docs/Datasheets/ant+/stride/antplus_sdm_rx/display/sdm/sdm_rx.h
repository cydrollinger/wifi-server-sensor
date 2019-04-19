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
#ifndef __SDM_RX__
#define __SDM_RX__

#include "config.h"
#include "types.h"
#include "antinterface.h" 
#include "antplus.h"


// ANT Channel Settings
typedef struct
{
   UCHAR  ucLastTime;                                // Page 1
   UCHAR  ucLastTime200;                             // Page 1
   UCHAR  ucDistance;                                // Page 1
   UCHAR  ucDistance16;                              // Page 1
   UCHAR  ucInstSpeed;                               // Page 1
   UCHAR  ucInstSpeed256;                            // Page 1
   UCHAR  ucStrideCount;                             // Page 1
   UCHAR  ucLatency;                                 // Page 1
} SDMPage1_Data;

typedef struct
{
   UCHAR  ucCadence;                                 // Page 2
   UCHAR  ucCadence16;                               // Page 2
   UCHAR  ucInstSpeed;                               // Page 2
   UCHAR  ucInstSpeed256;                            // Page 2
   UCHAR  ucModuleLocation;                          // Page 2
   UCHAR  ucBatteryStatus;                           // Page 2
   UCHAR  ucUnitHealth;                              // Page 2
   UCHAR  ucUseState;                                // Page 2
} SDMPage2to13_Data;

typedef struct
{
   UCHAR  ucHwVersion;                               // Page 80
   USHORT usManId;                                   // Page 80
   USHORT usModelNumber;                             // Page 80
} CommonPage80_Data;


typedef struct
{
   UCHAR  ucSwVersion;                               // Page 81
   ULONG  ulSerialNumber;                            // Page 81
} CommonPage81_Data;

typedef struct
{
   ULONG  ulCumOperatingTime;                        // Page 82
   UCHAR  ucBattVoltage256;                          // Page 82
   UCHAR  ucBattVoltage;                             // Page 82
   UCHAR  ucBattStatus;                              // Page 82
   UCHAR  ucCumOperatingTimeRes;                     // Page 82
} CommonPage82_Data;

typedef struct
{
   ULONG  ulAccumDistance;                           // Accumulated data               
   ULONG  ulAccumStride;                             // Accumulated stride
} AccumulatedDistance;              


// Public Functions
void 
SDMRX_Init();

BOOL                                                 // TRUE is commands queued succesfully
SDMRX_Open(
   UCHAR ucANTChannel_,                              // ANT Channel
   USHORT usSearchDeviceNumber_,                     // ANT Channel device number
   UCHAR ucTransType_);                              // ANT Channel transmission type

BOOL                                                 // TRUE if command queued succesfully 
SDMRX_Close();

BOOL                                                 // TRUE if commands queued to transmit (don't sleep transmit thread)
SDMRX_ChannelEvent(
   UCHAR* pucEventBuffer_,                           // Pointer to buffer containing response from ANT
   ANTPLUS_EVENT_RETURN* pstEventStruct_);           // Pointer to event structure set by this function
   
SDMPage1_Data*                                       // Pointer to Page 1 data
SDMRX_GetPage1();
                                                     
SDMPage2to13_Data*                                   // Pointer to Page 2-13 data
SDMRX_GetPage2to13();
                                                     
CommonPage80_Data*                                   // Pointer to Page 80 data
Common_GetPage80();

CommonPage81_Data*                                   // Pointer to Page 81 data
Common_GetPage81();

CommonPage82_Data*                                   // Pointer to Page 81 data
Common_GetPage82();

AccumulatedDistance*                                 // Pointer to accumulated data
SDMRX_GetAccum();

#endif
