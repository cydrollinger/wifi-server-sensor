#include "AN1069.h"
#ifdef DSPIC33F
#include <p33fxxxx.h>
#endif

#ifdef PIC24F
#include <p24fxxxx.h>
#endif

/********************************************************************
*     Function Name:    LDByteWriteSPI                              *
*     Parameters:       EE memory control, address and pointer 1    *
*     Description:      Writes Data Byte to SPI EE memory device    *
*                       This routine can be used for any SPI        * 
*                       EE memory device with 1 byte of address.    *
*                                                                   *
********************************************************************/
unsigned char LDByteWriteSPI(unsigned char LowAdd, unsigned char Data )
{
  WriteEnable();                      // Write Enable prior to Write
  ReadStatus();                       // Check for WEL bit set
  ChipSelect = 0;                             // Select Device
  WriteSPI1 ( 0x02 );                 // Send Write OpCode
  WriteSPI1 ( LowAdd );               // write address byte to EEPROM
  WriteSPI1 ( Data );                 // Write Byte to device
  ChipSelect = 1;                             // Deselect device and initiate Write
  SPIWIPPolling();                    // Wait for Write to complete
  SPI1STATbits.SPITBF = 0;
  return ( 0 );
}

/********************************************************************
*     Function Name:    LDByteReadSPI                               *
*     Parameters:       EE memory control, address, pointer and     *
*                       length bytes.                               *
*     Description:      Reads data Byte from SPI EE memory device.  *
*                       This routine can be used for any SPI        *
*                       EE memory device with 1 byte of address     *
*                                                                   *
********************************************************************/  
unsigned char LDByteReadSPI(unsigned char LowAdd, unsigned char *rdptr, unsigned char length )
{
  ChipSelect = 0;                             // Select Device
  WriteSPI1( 0x03 );                  // Send Read OpCode 
  WriteSPI1( LowAdd );                // WRITE word address to EEPROM
  getsSPI( rdptr, length );           // read in multiple bytes
  ChipSelect = 1;                             // Deselect Device
  return ( 0 );                                 
}

/********************************************************************
*     Function Name:    LDPageWriteSPI                              *
*     Parameters:       EE memory control, address and pointer 1    *
*     Description:      Writes data string to SPI EE memory         *
*                       device. This routine can be used for any SPI*
*                       EE memory device with 1 byte of address.    *
*                                                                   *  
********************************************************************/
unsigned char LDPageWriteSPI( unsigned char LowAdd, unsigned char *wrptr )
{
  WriteEnable();                      // Write Enable prior to Write
  ReadStatus();                       // Check for WEL bit set
  ChipSelect = 0;                             // Select Device
  WriteSPI1 ( 0x02 );                 // send Write OpCode
  WriteSPI1 ( LowAdd );               // write address byte to EEPROM
  PutStringSPI ( wrptr );             // Write Page to device
  ChipSelect = 1;                             // Deselect Device
  SPIWIPPolling();                    // Wait for Write to Complete
  SPI1STATbits.SPITBF = 0;
  return ( 0 );
}

/********************************************************************
*     Function Name:    LDSequentialReadSPI                         *
*     Parameters:       EE memory control, address, pointer and     *
*                       length bytes.                               *
*     Description:      Reads data string from SPI EE memory        *
*                       device. This routine can be used for any SPI*
*                       EE memory device with 1 byte of address.    *
*                                                                   *
********************************************************************/  
unsigned char LDSequentialReadSPI( unsigned char LowAdd, unsigned char *rdptr, unsigned char length )
{
  ChipSelect = 0;                             // Select Device
  WriteSPI1( 0x03 );                  // Send Read OpCode 
  WriteSPI1( LowAdd );                // WRITE word address to EEPROM
  getsSPI( rdptr, length );           // read in multiple bytes
  ChipSelect = 1;                             // Deselect Device    
  return ( 0 );                       
}

/********************************************************************
*     Function Name:    HDByteWriteSPI                              *
*     Parameters:       EE memory control, address and pointer 1    *
*     Description:      Writes data string to SPI EE memory         *
*                       device. This routine can be used for any SPI*
*                       EE memory device with two address bytes.    *
*                                                                   *  
********************************************************************/
unsigned char HDByteWriteSPI(unsigned char HighAdd, unsigned char LowAdd, unsigned char Data )
{
  WriteEnable();                      // Write Enable prior to Write
  ReadStatus();                       // Check for WEL bit set
  ChipSelect = 0;                             // Select Device
  WriteSPI1 ( 0x02 );                 // Send Write OpCode
  WriteSPI1 ( HighAdd);               // Send High Address Byte
  WriteSPI1 ( LowAdd );               // write address byte to EEPROM
  WriteSPI1 ( Data );                 // Write Byte to device
  ChipSelect = 1;                             // Deselect Device
  SPIWIPPolling();                    // Wait for Write to Complete
  return ( 0 );
}

/********************************************************************
*     Function Name:    HDByteReadSPI                               *
*     Parameters:       EE memory control, address, pointer and     *
*                       length bytes.                               *
*     Description:      Reads data string from SPI EE memory        *
*                       device. This routine can be used for any SPI*
*                       EE memory device with two address bytes.    *
*                                                                   *
********************************************************************/  
unsigned char HDByteReadSPI(unsigned char HighAdd, unsigned char LowAdd, unsigned char *rdptr, unsigned char length )
{
  ChipSelect = 0;                             // Select Device
  WriteSPI1( 0x03 );                  // Send Read OpCode 
  WriteSPI1( HighAdd);                // Send High Address Byte
  WriteSPI1( LowAdd );                // WRITE word address to EEPROM
  getsSPI( rdptr, length );           // read in multiple bytes
  ChipSelect = 1;                             // Deselect Device
  return ( 0 );                       // return with no error
}

/********************************************************************
*     Function Name:    HDPageWriteSPI                              *
*     Return Value:     error condition status                      *
*     Parameters:       EE memory control, address and pointer 1    *
*     Description:      Writes data string to SPI EE memory         *
*                       device. This routine can be used for any SPI*
*                       EE memory device with two address bytes.    *
*                                                                   *  
********************************************************************/
unsigned char HDPageWriteSPI( unsigned char HighAdd, unsigned char LowAdd, unsigned char *wrptr )
{
  WriteEnable();                      // Write Enable prior to Write
  ReadStatus();                       // Check for WEL bit set
  ChipSelect = 0;                             // Select Device
  WriteSPI1 ( 0x02 );                 // send Write OpCode
  WriteSPI1 ( HighAdd);               // Send High Address Byte
  WriteSPI1 ( LowAdd );               // write address byte to EEPROM
  PutStringSPI ( wrptr );             // Write Page to device
  ChipSelect = 1;                             // Deselect Device
  SPIWIPPolling();                    // Wait for Write to Complete
  return ( 0 );
}

unsigned char MegaHDPageWriteSPI( unsigned char HigherAdd, unsigned char HighAdd, unsigned char LowAdd, unsigned char *wrptr )
{
  WriteEnable();                      // Write Enable prior to Write
  ReadStatus();                       // Check for WEL bit set
  ChipSelect = 0;                             // Select Device
  WriteSPI1 ( 0x02 );                 // send Write OpCode
  WriteSPI1 ( HigherAdd );
  WriteSPI1 ( HighAdd);               // Send High Address Byte
  WriteSPI1 ( LowAdd );               // write address byte to EEPROM
  PutStringSPI ( wrptr );             // Write Page to device
  ChipSelect = 1;                             // Deselect Device
  SPIWIPPolling();                    // Wait for Write to Complete
  return ( 0 );
}

/********************************************************************
*     Function Name:    HDSequentialReadSPI                         *
*     Parameters:       EE memory control, address, pointer and     *
*                       length bytes.                               *
*     Description:      Reads data string from SPI EE memory        *
*                       device. This routine can be used for any SPI*
*                       EE memory device with two address bytes.    *
*                                                                   *
********************************************************************/  
unsigned char HDSequentialReadSPI( unsigned char HighAdd, unsigned char LowAdd, unsigned char *rdptr, unsigned char length )
{
  ChipSelect = 0;                             // Select Device
  WriteSPI1( 0x03 );                  // Send Read OpCode 
  WriteSPI1 (HighAdd);                // Send High Address Byte
  WriteSPI1( LowAdd );                // WRITE word address to EEPROM
  getsSPI( rdptr, length );           // read in multiple bytes
  ChipSelect = 1;                             // Deselect Device
  return ( 0 );                       
}

/********************************************************************
*     Function Name:    PutStringSPI                                *
*     Return Value:     void                                        *
*     Parameters:       address of write string storage location    *
*     Description:      This routine writes a string to the SPI bus.*  
********************************************************************/

unsigned char PutStringSPI( unsigned char *wrptr )
{
  unsigned int x;
  for (x = 0; x < PageSize; x++ )     // transmit data until PageSize
  {
     SPI1BUF = *wrptr++;               // initiate SPI bus cycle
     while( !SPI1STATbits.SPITBF );        // wait until 'BF' bit is set
  }
  return ( 0 );
}


/********************************************************************
*     Function Name:    SPIWIPPolling                               *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      This routine loops until WIP = 0            *  
********************************************************************/

void SPIWIPPolling(void)
{
  unsigned char Status;
  do
  {
    ChipSelect = 0;                           //Select Device
    WriteSPI1 ( 0x05 );               //Read Status Reg OpCode
    Status = ReadSPI1();              //Read Status Reg
    ChipSelect = 1;                           //Deselect Device
  } while (Status & 0x01);            //Check for WIP bit Set
}

/********************************************************************
*     Function Name:    WriteEnable                                 *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      This routine sets the Write Enable Latch    *  
********************************************************************/

void WriteEnable(void)
{
    ChipSelect = 0;                           //Select Device
    WriteSPI1 ( 0x06 );               //Write Enable OpCode
    ChipSelect = 1;                           //Deselect Device
}
/********************************************************************
*     Function Name:    Read Status                                 *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      This routine reads the Status Register      *  
********************************************************************/

void ReadStatus(void)
{
    ChipSelect = 0;                           //Select Device
    WriteSPI1 ( 0x05 );               //Read Status Register OpCode
    ReadSPI1();                       //Read Status Register
    ChipSelect = 1;                           //Deselect Device
}


/********************************************************************
*     Function Name : WriteSPI1                                     *
*     Description   : This routine writes a single byte/word to     * 
*                     the SPI bus.                                  *
*     Parameters    : Single data byte/word for SPI bus             *
*     Return Value  : None                                          *
********************************************************************/

void WriteSPI1(unsigned int data_out)
{   
    if (SPI1CON1bits.MODE16)          /* word write */
        SPI1BUF = data_out;
    else 
        SPI1BUF = data_out & 0xff;    /*  byte write  */
    while(SPI1STATbits.SPITBF);
    data_out = SPI1BUF;               //Avoiding overflow when reading
}


/********************************************************************
*     Function Name:    getsSPI                                     *
*     Return Value:     void                                        *
*     Parameters:       address of read string storage location and *
*                       length of string bytes to read              *
*     Description:      This routine reads a string from the SPI    *
*                       bus.  The number of bytes to read is deter- *
*                       mined by parameter 'length'.                *
********************************************************************/
void getsSPI( unsigned char *rdptr, unsigned char length )
{
  while ( length )                  // stay in loop until length = 0
  {
    *rdptr++ = ReadSPI1();          // read a single byte
    length--;                       // reduce string length count by 1
  }
}


/******************************************************************************
*     Function Name :   ReadSPI1                                              *
*     Description   :   This function will read single byte/ word  from SPI   *
*                       bus. If SPI is configured for byte  communication     *
*                       then upper byte of SPIBUF is masked.                  *         
*     Parameters    :   None                                                  *
*     Return Value  :   contents of SPIBUF register                           *
******************************************************************************/

unsigned int ReadSPI1()
{         
  SPI1STATbits.SPIROV = 0;
  SPI1BUF = 0x00;                  // initiate bus cycle 
  while(!SPI1STATbits.SPIRBF);
   /* Check for Receive buffer full status bit of status register*/
  if (SPI1STATbits.SPIRBF)
  { 
      SPI1STATbits.SPIROV = 0;
              
      if (SPI1CON1bits.MODE16)
          return (SPI1BUF);           /* return word read */
      else
          return (SPI1BUF & 0xff);    /* return byte read */
  }
  return -1;                  		/* RBF bit is not set return error*/
}



