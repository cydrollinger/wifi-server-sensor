/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;                                                                     
;                     Software License Agreement                      
;                                                                     
;     ©2007 Microchip Technology Inc
;     Mirochip Technology Inc. ("Microchip") licenses this software to 
;     you solely for the use with Microchip Products. The software is
;     owned by Microchip and is protected under applicable copyright
;     laws. All rights reserved.
;
;     SOFTWARE IS PROVIDED "AS IS." MICROCHIP EXPRESSLY DISCLAIMS ANY
;     WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
;     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
;     PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
;     BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
;     DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
;     PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
;     BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
;     ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
;     
;                                                                
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;	Filename:			    AN1069.c            
;	Date:				      November 26, 2007          
;	File Version:		  2.0                             
;	Assembled using:	MPLAB IDE 7.50.00.0               
; Author:		  	    Martin Kvasnicka              
;	Company:			    Microchip Technology, Inc.
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#include "AN1069.h"

#ifdef DSPIC33F
#include <p33fxxxx.h>
#endif

#ifdef PIC24F
#include <p24fxxxx.h>
#endif

//config word
#ifdef PIC24F
_CONFIG2(IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS);
                                                // Two speed start up off, Prim. Osc (XT,HS, EC) w/PLL,
                                                // Clock switch & monitor off, OSC0/RC15 function off, HS Osc  
_CONFIG1(GCP_OFF & GWRP_OFF & FWDTEN_OFF);      // Code Protect off, Write protect off, Watchdog timer off
#endif

#ifdef DSPIC33F
_FGS(GSS_OFF & GWRP_OFF);                       //Code Protect off, Write protect disabled
_FOSCSEL(FNOSC_PRIPLL & IESO_OFF & TEMP_OFF);   //Prim. Osc (XT, HS, EC) w/ PLL, Two speed osc off, Temp protect off
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_HS); //Clock Switch & Clock Monitor off, Osc 2 clock, High Speed
_FWDT(FWDTEN_OFF);                              //Watchdog Timer Disabled
#endif

//********************************************************************
// Global Variable declarations
//********************************************************************

int main(void)
{
  unsigned char HighAdd;                    //High Order Address Byte
  unsigned char LowAdd;                     //Low Order Address Byte
  unsigned char Data;                       //Data Byte
  unsigned char Length;                     //Length of bytes to read
  unsigned char PageString[128];            //Holds the device page data to/from EEPROM

  unsigned int n;
  HighAdd = 0x00;                         //HighAdd is first byte transmitted for High Density devices
  LowAdd = 0x10;                          //LowAdd is only Address byte for Low Density, second byte to HD
  Data = 0xA5;                            //Data can be whatever user would like
  Length = 0x01;                          //Length param set to 1 for single byte read
  
  for(n = 0; n < PageSize; n++)
  {
    PageString[n] = Data;                 //Fill PageString with data to be written
  }
  InitPIC();
  ChipSelect = 1;                                 // Deselect device at beginning of code
   
//  LDByteWriteSPI( LowAdd, Data );                                 //Low Density Byte Write
//  LDByteReadSPI(LowAdd, &Data, Length );                          //Low Density Byte Read
  LDPageWriteSPI(LowAdd, PageString );                            //Low Density Page Write
  LDSequentialReadSPI(LowAdd, PageString, PageSize );             //Low Density Page Read

//  HDByteWriteSPI(HighAdd, LowAdd, Data );                         //High Density Byte Write
//  HDByteReadSPI(HighAdd, LowAdd , &Data, Length) ;                //High Density Byte Read
//  HDPageWriteSPI(HighAdd, LowAdd, PageString );                   //High Density Page Write
//  HDSequentialReadSPI(HighAdd, LowAdd, PageString, PageSize );    //High Density Page Read  

  while(1){};                             // Loop here forever
}

void InitPIC(void)

#ifdef DSPIC33F         //here is the SPI setup for 10 MHz operation, Mode 0,0.
{
  TRISF = 0x0080;             
  PLLFBD = 38;                
  CLKDIV = 0x0040;            
	SPI1CON1 = 0x013B;          
  SPI1CON2 = 0x0000;
  SPI1STAT = 0x8000;        
}
#endif

#ifdef PIC24F           //here is the SPI setup for 8 MHz operation, Mode 0,0.
{
  TRISF = 0x0080;
  CLKDIV = 0x0040;            
	SPI1CON1 = 0x013B;          
  SPI1CON2 = 0x0000;
  SPI1STAT = 0x8000;        
}
#endif
