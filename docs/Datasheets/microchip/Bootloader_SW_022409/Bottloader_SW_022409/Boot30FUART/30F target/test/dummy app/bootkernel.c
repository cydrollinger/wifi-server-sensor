/********************************************************************
* FileName:		bootkernel.c
* Dependencies:    
* Processor:		dsPIC30F Family
* Hardware:		dsPICDEM 2
* Compiler:		C30 2.01
* Company:		Microchip Technology, Inc.
*
* Software License Agreement
*
* The software supplied herewith by Microchip Technology Incorporated
* (the “Company”) for its PICmicro® Microcontroller is intended and
* supplied to you, the Company’s customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
*********************************************************************/

#include "p30fxxxx.h"
//#include "DataEEPROM.h"



void _ToggleLED(void);


//_FGS(CODE_PROT_ON);
// _FOSC(CSW_FSCM_OFF & XT_PLL16);


int _EEDATA(32) BufferInDataEE[] = {0,1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF};

int main(void)
{
	
	TRISBbits.TRISB0 = 0;	//make I/O output
	
	TRISBbits.TRISB1 = 0;	//make I/O output



	while(1)
	{
	
		_ToggleLED();	
				
	}
 
}

void _ToggleLED(void)
{

static int count = 0;
	
	
		if(++count == 0)
		{
			LATBbits.LATB0 = ~LATBbits.LATB0;
		}

	
}
