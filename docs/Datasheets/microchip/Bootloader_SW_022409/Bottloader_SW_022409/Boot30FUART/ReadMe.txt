This is Beta version of Bootloader for 30F devices.
It was developed on dsPICDEM 2 board running 30F4011 processor.


****************************************************************************
Procedure:

1.	Compile and Load target side bootloader application onto dsPICDEM2 board
3.	Start/Run target side bootloader application
4.	Start/Run host side bootloader application with appropriate arguments from the command line window.
	Executable is located in the "x86 host\Debug" directory
5.	User application must be modified to start at address 0x600 (see below)

When bootloader runs on the target, it waits N numbers of seconds for UART communication before
transferring control to the user application at address 0x602. The delay of N seconds must be specified
at address 0x600. if the delay is 0 seconds, bootloader jumps to user application on the start-up.
If the delay is 255 seconds (oxFF), bootloader will wait for UART communication indefinitely.


****************************************************************************
Usage: "16-Bit Flash Programmer.exe" -i interface [-bpe] hexfile

Options:

  -i
       specifies serial interface name such as COM1, COM2, etc

  -b
       specifies baud rate for serial interface. Default is 115200

  -p
       read program flash. Must provide address to read in HEX format: -p 0x000100

  -e
       read EEPROM. Must provide address to read in HEX format: -e 0x7FFC00


****************************************************************************
GLD file modifications to place application at address 0x600 and to provide bootloader timeout:

-	Modify the following lines of of GLD code:

  program (xr) : ORIGIN = 0x600,    LENGTH = ((16K * 2) - 0x600)
  __CODE_BASE   = 0x600;    /* Handles, User Code, Library Code */


  /*
  ** User Code and Library Code
  */
  .text __CODE_BASE :
  {
        SHORT(0x0A); /* Bootloader timeout in sec */


        *(.handle);
        *(.libc) *(.libm) *(.libdsp);  /* keep together in this order */
        *(.lib*);
        *(.text);
  } >program


****************************************************************************
Notes for Beta release:

-	Added bootloader timeout
