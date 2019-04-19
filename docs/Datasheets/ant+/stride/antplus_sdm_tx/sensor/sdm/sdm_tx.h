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
#ifndef __SDM_TX__
#define __SDM_TX__

#include "config.h"
#include "types.h"
#include "antinterface.h" 
#include "antplus.h"                  

// Public Channel Settings 

// Global Page Settings
#define SDMTX_MFG_ID							     ((USHORT) 2)			   // Manufacturer ID
#define SDMTX_SERIAL_NUMBER					  ((ULONG) 0xABCDEF12)  // Serial Number
#define SDMTX_HW_VERSION						  ((UCHAR) 5)			   // HW Version
#define SDMTX_SW_VERSION						  ((UCHAR) 0)           // SW Version
#define SDMTX_MODEL_NUMBER					     ((USHORT) 2)			   // Model Number


// Sim only
#define STRIDE_LENGTH                       ((SHORT) 24)			   // In 1/16 meters

void 
SDMTX_Init();

BOOL                                                              // TRUE is commands queued succesfully
SDMTX_Open(
   UCHAR ucAntChannel_,                                           // ANT Channelm
   USHORT usDeviceNumber_,                                        // ANT Channel device number
   UCHAR ucTransType_);                                           // ANT Channel transmission type

BOOL                                                              // TRUE if command queued succesfully 
SDMTX_Close();

BOOL                                                              // TRUE if commands queued to transmit (don't sleep transmit thread)
SDMTX_ChannelEvent(                          
   UCHAR* pucEventBuffer_,                                        // Pointer to buffer containing response from ANT
   ANTPLUS_EVENT_RETURN* pstEventStruct_);                        // Pointer to event structure set by this function

void                                                              // Returns TRUE
SDMTX_PulseEvent(                                                 // Time of pulse event in (1/1024)s
   USHORT ulTime1024,
   UCHAR ucAlarmNumber_);

#endif
