/********************************************************************
 FileName:     	usb_descriptors.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM� FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PIC� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

*********************************************************************
-usb_descriptors.c-
-------------------------------------------------------------------
Filling in the descriptor values in the usb_descriptors.c file:
-------------------------------------------------------------------

[Device Descriptors]
The device descriptor is defined as a USB_DEVICE_DESCRIPTOR type.  
This type is defined in usb_ch9.h  Each entry into this structure
needs to be the correct length for the data type of the entry.

[Configuration Descriptors]
The configuration descriptor was changed in v2.x from a structure
to a BYTE array.  Given that the configuration is now a byte array
each byte of multi-byte fields must be listed individually.  This
means that for fields like the total size of the configuration where
the field is a 16-bit value "64,0," is the correct entry for a
configuration that is only 64 bytes long and not "64," which is one
too few bytes.

The configuration attribute must always have the _DEFAULT
definition at the minimum. Additional options can be ORed
to the _DEFAULT attribute. Available options are _SELF and _RWU.
These definitions are defined in the usb_device.h file. The
_SELF tells the USB host that this device is self-powered. The
_RWU tells the USB host that this device supports Remote Wakeup.

[Endpoint Descriptors]
Like the configuration descriptor, the endpoint descriptors were 
changed in v2.x of the stack from a structure to a BYTE array.  As
endpoint descriptors also has a field that are multi-byte entities,
please be sure to specify both bytes of the field.  For example, for
the endpoint size an endpoint that is 64 bytes needs to have the size
defined as "64,0," instead of "64,"

Take the following example:
    // Endpoint Descriptor //
    0x07,                       //the size of this descriptor //
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    _EP02_IN,                   //EndpointAddress
    _INT,                       //Attributes
    0x08,0x00,                  //size (note: 2 bytes)
    0x02,                       //Interval

The first two parameters are self-explanatory. They specify the
length of this endpoint descriptor (7) and the descriptor type.
The next parameter identifies the endpoint, the definitions are
defined in usb_device.h and has the following naming
convention:
_EP<##>_<dir>
where ## is the endpoint number and dir is the direction of
transfer. The dir has the value of either 'OUT' or 'IN'.
The next parameter identifies the type of the endpoint. Available
options are _BULK, _INT, _ISO, and _CTRL. The _CTRL is not
typically used because the default control transfer endpoint is
not defined in the USB descriptors. When _ISO option is used,
addition options can be ORed to _ISO. Example:
_ISO|_AD|_FE
This describes the endpoint as an isochronous pipe with adaptive
and feedback attributes. See usb_device.h and the USB
specification for details. The next parameter defines the size of
the endpoint. The last parameter in the polling interval.

-------------------------------------------------------------------
Adding a USB String
-------------------------------------------------------------------
A string descriptor array should have the following format:

rom struct{byte bLength;byte bDscType;word string[size];}sdxxx={
sizeof(sdxxx),DSC_STR,<text>};

The above structure provides a means for the C compiler to
calculate the length of string descriptor sdxxx, where xxx is the
index number. The first two bytes of the descriptor are descriptor
length and type. The rest <text> are string texts which must be
in the unicode format. The unicode format is achieved by declaring
each character as a word type. The whole text string is declared
as a word array with the number of characters equals to <size>.
<size> has to be manually counted and entered into the array
declaration. Let's study this through an example:
if the string is "USB" , then the string descriptor should be:
(Using index 02)
rom struct{byte bLength;byte bDscType;word string[3];}sd002={
sizeof(sd002),DSC_STR,'U','S','B'};

A USB project may have multiple strings and the firmware supports
the management of multiple strings through a look-up table.
The look-up table is defined as:
rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};

The above declaration has 3 strings, sd000, sd001, and sd002.
Strings can be removed or added. sd000 is a specialized string
descriptor. It defines the language code, usually this is
US English (0x0409). The index of the string must match the index
position of the USB_SD_Ptr array, &sd000 must be in position
USB_SD_Ptr[0], &sd001 must be in position USB_SD_Ptr[1] and so on.
The look-up table USB_SD_Ptr is used by the get string handler
function.

-------------------------------------------------------------------

The look-up table scheme also applies to the configuration
descriptor. A USB device may have multiple configuration
descriptors, i.e. CFG01, CFG02, etc. To add a configuration
descriptor, user must implement a structure similar to CFG01.
The next step is to add the configuration descriptor name, i.e.
cfg01, cfg02,.., to the look-up table USB_CD_Ptr. USB_CD_Ptr[0]
is a dummy place holder since configuration 0 is the un-configured
state according to the definition in the USB specification.

********************************************************************/
 
/*********************************************************************
 * Descriptor specific type definitions are defined in:
 * usb_device.h
 *
 * Configuration options are defined in:
 * usb_config.h
 ********************************************************************/

#ifndef __USB_DESCRIPTORS_C
#define __USB_DESCRIPTORS_C

/** INCLUDES *******************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "./USB/usb_device.h"

#include "./USB/usb_function_hid.h"

/** CONSTANTS ******************************************************/
#if defined(__18CXX)
#pragma romdata
#endif

/* Device Descriptor */
ROM USB_DEVICE_DESCRIPTOR device_dsc=
{
	0x12, // Size of this descriptor in bytes
	USB_DESCRIPTOR_DEVICE, // DEVICE descriptor type
	0x0110, // USB Spec Release Number in BCD format
	0x00, // Class Code
	0x00, // Subclass code
	0x00, // Protocol code
	USB_EP0_BUFF_SIZE, // Max packet size for EP0, see
	// usbcfg.h
	MY_VID, // Vendor ID
	MY_PID, // Product ID
	0x0003, // Device release number in BCD format
	0x01, // Manufacturer string index
	0x02, // Product string index
	0x00, // Device serial number string index
	0x01 // Number of possible configurations
};

/* Configuration 1 Descriptor */
	ROM BYTE configDescriptor1[]={
	    /* Configuration Descriptor */
	  0x09,//sizeof(USB_CFG_DSC), // Size of this descriptor in bytes
	USB_DESCRIPTOR_CONFIGURATION, // CONFIGURATION descriptor type
	DESC_CONFIG_WORD(0x0022), // Total length of data for this cfg
	1, // Number of interfaces in this cfg
	1, // Index value of this configuration
	0, // Configuration string index
	_DEFAULT|_SELF, // Attributes, see usbd.h
	50, // Max power consumption (2X mA)
	/* Interface Descriptor */
	0x09,//sizeof(USB_INTF_DSC), // Size of this descriptor
	// in bytes
	USB_DESCRIPTOR_INTERFACE, // INTERFACE descriptor type
	0, // Interface Number
	0, // Alternate Setting Number
	1, // Number of endpoints in this intf
	HID_INTF, // Class code
	BOOT_INTF_SUBCLASS, // Subclass code
	HID_PROTOCOL_MOUSE, // Protocol code
	0, // Interface string index
	/* HID Class-Specific Descriptor */
	0x09,//sizeof(USB_HID_DSC)+3, // Size of this descriptor in bytes
	DSC_HID, // HID descriptor type
	DESC_CONFIG_WORD(0x0111), // HID Spec Release Number in BCD
	// format(1.11)
	0x00, // Country Code (0x00 for Not
	// supported)
	HID_NUM_OF_DSC, // Number of class descriptors, see
	// usbcfg.h
	DSC_RPT, // Report descriptor type
	DESC_CONFIG_WORD(50), // sizeof(hid_rpt01),
	// Size of the report descriptor
	/* Endpoint Descriptor */
	0x07,/*sizeof(USB_EP_DSC)*/
	USB_DESCRIPTOR_ENDPOINT, // Endpoint Descriptor
	HID_EP | _EP_IN, // EndpointAddress
	_INT, // Attributes
	DESC_CONFIG_WORD(3), // size
	0x01 // Interval
};


//Language code string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[1];}sd000={
sizeof(sd000),USB_DESCRIPTOR_STRING,{0x0409
}};

//Manufacturer string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[25];}sd001={
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'M','i','c','r','o','c','h','i','p',' ',
'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'
}};

//Product string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[22];}sd002={
sizeof(sd002),USB_DESCRIPTOR_STRING,
{'M','o','u','s','e',
' ','E','n','u','m','e','r','a','t','i','o','n ',
' ','D','e','m','o'
}};

//Class specific descriptor - HID mouse
ROM struct{BYTE report[HID_RPT01_SIZE];}hid_rpt01={
    {0x05, 0x01, /* Usage Page (Generic Desktop)*/
		0x09, 0x02, /* Usage (Mouse)*/
		0xA1, 0x01, /* Collection (Application)*/
		0x09, 0x01, /* Usage (Pointer)*/
		0xA1, 0x00, /* Collection (Physical)*/
		0x05, 0x09, /* Usage Page (Buttons) */
		0x19, 0x01, /* Usage Minimum (01)*/
		0x29, 0x03, /* Usage Maximum (03)*/
		0x15, 0x00, /* Logical Minimum (0)*/
		0x25, 0x01, /* Logical Maximum (0)*/
		0x95, 0x03, /* Report Count (3)*/
		0x75, 0x01, /* Report Size (1)*/
		0x81, 0x02, /* Input (Data, Variable, Absolute)*/
		0x95, 0x01, /* Report Count (1)*/
		0x75, 0x05, /* Report Size (5)*/
		0x81, 0x01, /* Input (Constant) ;5 bit padding */
		0x05, 0x01, /* Usage Page (Generic Desktop)*/
		0x09, 0x30, /* Usage (X)*/
		0x09, 0x31, /* Usage (Y)*/
		0x15, 0x81, /* Logical Minimum (-127)*/
		0x25, 0x7F, /* Logical Maximum (127)*/
		0x75, 0x08, /* Report Size (8)*/
		0x95, 0x02, /* Report Count (2)*/
		0x81, 0x06, /* Input (Data, Variable, Relative)*/
		0xC0, 0xC0
    }
};/* End Collection,End Collection            */

//Array of configuration descriptors
ROM BYTE *ROM USB_CD_Ptr[]=
{
    (ROM BYTE *ROM)&configDescriptor1
};

//Array of string descriptors
ROM BYTE *ROM USB_SD_Ptr[]=
{
    (ROM BYTE *ROM)&sd000,
    (ROM BYTE *ROM)&sd001,
    (ROM BYTE *ROM)&sd002
};

/** EOF usb_descriptors.c ***************************************************/

#endif