/*
 * Dynastream Innovations Inc.
 * Cochrane, AB, CANADA
 *
 * Copyright � 1998-2008 Dynastream Innovations Inc.
 * All rights reserved. This software may not be reproduced by
 * any means without express written approval of Dynastream
 * Innovations Inc.
 */ 
/* 
 * VCS Information - autogenerated - do not modify
 * 
 * $Header: /repository/Products/ANT_Reference_Designs/ANT\040CPU\040Board\040Reference\040Designs/ANTPlus/base/crc.h,v 1.1 2009/04/06 17:25:52 canotinoco Exp $
 *
 **********************************************************************
 *
 * $Log: crc.h,v $
 * Revision 1.1  2009/04/06 17:25:52  canotinoco
 * Moved to base directory, for use for ANT-FS client
 *
 * Revision 1.1  2009/02/24 18:05:21  barnowski
 * Add UART driver, FIT1E, some test demos.
 *
 * Revision 1.3  2008/11/28 17:00:39  reddekop
 * *** empty log message ***
 *
 * Revision 1.2  2008/11/20 22:57:47  reddekop
 * Added 32-bit CRC functions.
 *
 * Revision 1.1  2008/10/06 17:57:05  glund
 * Move to common folder.
 *
 **********************************************************************
 */ 

#if !defined(CRC_H)
#define CRC_H

#include "types.h"

//////////////////////////////////////////////////////////////////////////////////
// Public Function Prototypes
//////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
   extern "C" {
#endif

USHORT CRC_Calc16(const volatile void *pvDataPtr_, ULONG ulSize_);
USHORT CRC_UpdateCRC16(USHORT usCRC_, const volatile void *pvDataPtr_, ULONG ulSize_);
USHORT CRC_Get16(USHORT usCRC_, UCHAR ucByte_);
ULONG CRC_Calc32(const volatile void *pvDataPtr_, ULONG ulSize_);
ULONG CRC_UpdateCRC32(ULONG ulCRC_, const volatile void *pvDataPtr_, ULONG ulSize_);
UCHAR CRC_Get8(UCHAR ucCRC_, UCHAR ucByte_);

#if defined(__cplusplus)
   }
#endif

#endif // !defined(CRC_H)
