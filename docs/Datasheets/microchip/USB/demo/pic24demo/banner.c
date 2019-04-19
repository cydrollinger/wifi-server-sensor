/*****************************************************************************
 *
 * Show Start Banners
 *
 *****************************************************************************
 * FileName:        banner.c
 * Dependencies:    system.h
 * Processor:       PIC24
 * Compiler:        MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company. 
 *
 * The software is owned by the Company and/or its supplier, and is 
 * protected under applicable copyright laws. All rights are reserved. 
 * Any use in violation of the foregoing restrictions may subject the 
 * user to criminal sanctions under applicable laws, as well as to 
 * civil liability for the breach of the terms and conditions of this 
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, 
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * State mashine to display  PIC24 Features. 
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * XXXX                 XXX         ...
 * Anton Alkhimenok     10/18/05	Several additions
 *****************************************************************************/
#include "system.h"

/*****************************************************************************
*  Banners strings.
 *****************************************************************************/
//                  "XXXXXXXXXXXXXXXX";
#if defined(__PIC24FJ64GA004__)
    #define PART_NUMBER "PIC24FJ64GA004  "
    #define __NUMBER_OF_SPI_MODULES__ '2'
    #define __NUMBER_OF_I2C_MODULES__ '2'
    #define __NUMBER_OF_UART_WITH_IRDA_MODULES__ '2'
    #define __NUMBER_OF_OC_MODULES__ '5'
    #define __NUMBER_OF_IC_MODULES__ '5'
    #define __NUMBER_OF_TIMER_MODULES__ '5'
    #define __DEVICE_BANNERS__  _MCHP1,_MCHP2,              \
                                _PART1,_PART2,              \
                                _COPYRIGHT1,_COPYRIGHT2,    \
                                _16BIT1,_16BIT2,            \
                                _OP_SPEED,_OP_VOLTAGE,      \
                                _FEATURES,_WDT,             \
                                _SPI,_I2C,                  \
                                _UART,_IC,                  \
                                _OC1,_OC2,                  \
                                _PMP1,_PMP2,                \
                                _RTCC1,_RTCC2,              \
                                _TMR1,_TMR2,                \
                                _A2D1,_A2D2,                \
                                _VREG1,_VREG2,              \
                                _OSC1,_OSC2,                \
                                _FRC1,_FRC2

#elif defined(__PIC24FJ128GA010__)
    #define PART_NUMBER "PIC24FJ128GA010 "
    #define __NUMBER_OF_SPI_MODULES__ '2'
    #define __NUMBER_OF_I2C_MODULES__ '2'
    #define __NUMBER_OF_UART_WITH_IRDA_MODULES__ '2'
    #define __NUMBER_OF_OC_MODULES__ '5'
    #define __NUMBER_OF_IC_MODULES__ '5'
    #define __NUMBER_OF_TIMER_MODULES__ '5'
    #define __DEVICE_BANNERS__  _MCHP1,_MCHP2,              \
                                _PART1,_PART2,              \
                                _COPYRIGHT1,_COPYRIGHT2,    \
                                _16BIT1,_16BIT2,            \
                                _OP_SPEED,_OP_VOLTAGE,      \
                                _FEATURES,_WDT,             \
                                _SPI,_I2C,                  \
                                _UART,_IC,                  \
                                _OC1,_OC2,                  \
                                _PMP1,_PMP2,                \
                                _RTCC1,_RTCC2,              \
                                _TMR1,_TMR2,                \
                                _A2D1,_A2D2,                \
                                _VREG1,_VREG2,              \
                                _OSC1,_OSC2,                \
                                _FRC1,_FRC2

#elif defined(__PIC24FJ256GB110__)
    #define PART_NUMBER "PIC24FJ256GB110 "
    #define __NUMBER_OF_SPI_MODULES__ '3'
    #define __NUMBER_OF_I2C_MODULES__ '3'
    #define __NUMBER_OF_UART_WITH_IRDA_MODULES__ '4'
    #define __NUMBER_OF_OC_MODULES__ '9'
    #define __NUMBER_OF_IC_MODULES__ '9'
    #define __NUMBER_OF_TIMER_MODULES__ '5'

    #define __DEVICE_BANNERS__  _MCHP1,_MCHP2,              \
                                _PART1,_PART2,              \
                                _COPYRIGHT1,_COPYRIGHT2,    \
                                _16BIT1,_16BIT2,            \
                                _OP_SPEED,_OP_VOLTAGE,      \
                                _FEATURES,_BLANK,           \
                                _USBOTG1,_USBOTG2,          \
                                _PPS1,_PPS2,                \
                                _SPI,_I2C,                  \
                                _UART,_IC,                  \
                                _OC1,_OC2,                  \
                                _PMP1,_PMP2,                \
                                _RTCC1,_RTCC2,              \
                                _TMR1,_TMR2,                \
                                _A2D1,_A2D2,                \
                                _WDT,_BLANK,                \
                                _VREG1,_VREG2,              \
                                _OSC1,_OSC2,                \
                                _FRC1,_FRC2

#elif defined(__PIC24FJ256GA110__)
    #define PART_NUMBER "PIC24FJ256GA110 "
    #define __NUMBER_OF_SPI_MODULES__ '3'
    #define __NUMBER_OF_I2C_MODULES__ '3'
    #define __NUMBER_OF_UART_WITH_IRDA_MODULES__ '4'
    #define __NUMBER_OF_OC_MODULES__ '9'
    #define __NUMBER_OF_IC_MODULES__ '9'
    #define __NUMBER_OF_TIMER_MODULES__ '5'

    #define __DEVICE_BANNERS__  _MCHP1,_MCHP2,              \
                                _PART1,_PART2,              \
                                _COPYRIGHT1,_COPYRIGHT2,    \
                                _16BIT1,_16BIT2,            \
                                _OP_SPEED,_OP_VOLTAGE,      \
                                _FEATURES,_WDT,           \
                                _PPS1,_PPS2,                \
                                _SPI,_I2C,                  \
                                _UART,_IC,                  \
                                _OC1,_OC2,                  \
                                _PMP1,_PMP2,                \
                                _RTCC1,_RTCC2,              \
                                _TMR1,_TMR2,                \
                                _A2D1,_A2D2,                \
                                _VREG1,_VREG2,              \
                                _OSC1,_OSC2,                \
                                _FRC1,_FRC2

#else
    #error "Device not supported in this demo.  Please add code here."
#endif

const char _BLANK[16] =    "                ";

const char _MCHP1[16]  = "Microchip       ";
const char _MCHP2[16]  = "Technology, Inc ";

const char _PART1[16]  = "Presenting the  ";
const char _PART2[16]  = PART_NUMBER;

const char _COPYRIGHT1[16]  = "Copyright 2007  ";
const char _COPYRIGHT2[16]  = "                ";

const char _16BIT1[16]  = "16-bit          ";
const char _16BIT2[16]  = "Microcontroller ";

const char _OP_SPEED[16]  = "16MIPS / 32MHz  ";
const char _OP_VOLTAGE[16] = "2.0V - 3.6V     ";

const char _FEATURES[16] = "Features:       ";

const char _SPI[16] = {__NUMBER_OF_SPI_MODULES__,' ','S','P','I',' ','m','o','d','u','l','e','s',' ',' ',' '};

const char _I2C[16] = {__NUMBER_OF_I2C_MODULES__,' ','I','2','C',' ','m','o','d','u','l','e','s',' ',' ',' '};   

const char _UART[16] = {__NUMBER_OF_UART_WITH_IRDA_MODULES__,' ','U','A','R','T','s',' ','w','/',' ','I','r','D','A',' '};

const char _PMP1[16] = "Parallel        ";
const char _PMP2[16] = "Master Port     ";

const char _A2D1[16] = "500k sample     ";
const char _A2D2[16] = "10-bit A/D      ";

const char _OC1[16] = {__NUMBER_OF_OC_MODULES__,' ','P','W','M',' ','o','r',' ',' ',' ',' ',' ',' ',' ',' '};
const char _OC2[16] = "Output compare  ";

const char _IC[16] = {__NUMBER_OF_IC_MODULES__,' ','I','n','p','u','t',' ','C','a','p','t','u','r','e',' '};

const char _RTCC1[16] = "Real-time clock ";
const char _RTCC2[16] = "and calendar    ";

const char _WDT[16] = "Watchdog Timer  ";

const char _VREG1[16] = "On-chip voltage ";
const char _VREG2[16] = "regulator       ";

const char _TMR1[16] = {__NUMBER_OF_TIMER_MODULES__,' ','1','6','-','b','i','t',' ','t','i','m','e','r','s',' '};
const char _TMR2[16] = "32-bit options  ";

const char _OSC1[16] = "Many oscillator ";
const char _OSC2[16] = "modes           ";

const char _FRC1[16] = "8MHz internal   ";
const char _FRC2[16] = "oscillator      ";

 const char _PPS1[16] = "Peripheral Pin  ";
const char _PPS2[16] = "Select          ";

const char _USBOTG1[16] = "USB On-The-Go,  ";
const char _USBOTG2[16] = "Device, Emb Host";

// Last banner is showed at start only
const char _INTRO1[] = "  Explorer 16   ";
const char _INTRO2[] = "Development Brd ";


/*****************************************************************************
*  Array of pointers to banners strings
******************************************************************************/
const char* _pBannersArray[] =
{__DEVICE_BANNERS__,_INTRO1,_INTRO2};

// Specify delay between banners
#define BNR_CHANGE_DELAY 2000
// Quantity of Banners - 2 (last banner is showed at start only and never counted again)
#define BNR_COUNT   ((sizeof(_pBannersArray)/sizeof(const char*))-2)

unsigned char 	_uBannerNum;
unsigned char 	_uBannerState;
const char * 	_pBanner;
unsigned char 	_uBannerLen;
unsigned int	_uBannerWait;
unsigned int 	_uBannerCharWait;

/*****************************************************************************
* Function: BannerStart
*
* Preconditions: None.
*
* Overview: The function starts to show banners from the last one. The banner
* is displayed once.
*
* Input: None.
*
* Output: None.
*
******************************************************************************/
void BannerStart(void){
	_uBannerState = 3;
	_uBannerNum = BNR_COUNT;
	_uBannerWait = BNR_CHANGE_DELAY;	
}

/*****************************************************************************
* Function: BannerInit
*
* Preconditions: None.
*
* Overview: The function starts to show banners  from the first one.
*
* Input: None.
*
* Output: None.
*
******************************************************************************/
void BannerInit(void){
	_uBannerState = 9;
	_uBannerNum = 0;
	_uBannerWait = 20;	
}

/*****************************************************************************
* Function: BannerProcessEvents
*
* Preconditions: BannerInit or BannerStart must be called before.
*
* Overview: The function implements a state mashine to display banners sequence.
* Must be called periodically to output the strings.
*
* Input: None.
*
* Output: None.
*
******************************************************************************/
void BannerProcessEvents(void)
{

	switch(_uBannerState){
        case 4: 			// Wait to put a char
        case 7:
			if (_uBannerCharWait)
                 _uBannerCharWait--;
			else {
				_uBannerState++;
				_uBannerCharWait = 1;
			}
			break;

        case 5:				// Put a char on the LCD
        case 8:
			if (!mLCDIsBusy()) {
				mLCDPutChar(*_pBanner);
				_pBanner++;
				_uBannerLen--;
				if (!_uBannerLen)
                     _uBannerState++;
			}
			break;

		case 3:             // Put the first line
			if (!mLCDIsBusy()) {
            	mLCDHome();
				_pBanner = _pBannersArray[_uBannerNum];
                _uBannerNum++;
				_uBannerLen = LCD_DISPLAY_LEN;
				_uBannerState++;
			}
			break;

		case 6:				// Put the second line
            if (!mLCDIsBusy()){
			    mLCDPutCmd(0xC0);
    			_pBanner = _pBannersArray[_uBannerNum];
                _uBannerNum++;
	    		_uBannerLen = LCD_DISPLAY_LEN;
    			_uBannerState++;
            }
			break;

		case 9:				// Wait at the end of each banner
			if(_uBannerWait--)
                break;
			if(_uBannerNum >= BNR_COUNT)
				_uBannerNum = 0;
   			_uBannerWait = BNR_CHANGE_DELAY;
    		_uBannerState = 3;
            break;

		default:
			_uBannerState = 3;
	}	
}
/*****************************************************************************
* EOF
******************************************************************************/
