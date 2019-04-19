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
#include "system.h"
#include "config.h"
#include "ioboard.h"

volatile USHORT usLowPowerMode;                 // low power mode control

//----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////
// main
//
// main function  
//
// Configures device simulator
//
// \return: This function does not return. 
////////////////////////////////////////////////////////////////////////////
void main()
{
   System_DisableWatchdog();                    // Disable the watchdog
   System_Init();                               // Setup the clock 
   Timer_Init();                                // Setup the timer
   
   Timer_DelayTime(50000);                      // delay for 500ms to allow DCO to settle

#if defined (USE_EXTERNAL_32KHZ)
   EXT_CLOCK_32KHZ_SEL |= EXT_CLOCK_32KHZ_BIT;  // Select ACLK out on P2.0
   EXT_CLOCK_32KHZ_DIR |= EXT_CLOCK_32KHZ_BIT;  // Set as output
#endif

   __enable_interrupt();                        // Enable global interrupts
   
   ANTInterface_Init();                         // Initialize ANT to serial interface
   

   main_sdm();                                  // Run SDM RX main thread - does not return.









#if defined(USE_WGTRX)
   main_wgtrx();                                // Run weight scale RX thread - does not return.
#endif




#if defined(USE_TEST_AT3)
   main_test_at3();                             // Run test (reset AT3)
#endif
} 
