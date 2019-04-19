//Global variables 



//Function Prototypes

void InitPIC(void);         // Initializes the PIC to POR defaults
unsigned char PutStringSPI( unsigned char * );
void SPIWIPPolling( void );
void WriteEnable( void );
void ReadStatus( void );

void WriteSPI1(unsigned int data_out) ;
void getsSPI( unsigned char *, unsigned char );
unsigned int ReadSPI1( void );

unsigned char LDByteWriteSPI ( unsigned char, unsigned char);
unsigned char LDByteReadSPI ( unsigned char, unsigned char * , unsigned char);
unsigned char LDPageWriteSPI ( unsigned char, unsigned char *);
unsigned char LDSequentialReadSPI ( unsigned char, unsigned char *, unsigned char );

unsigned char HDByteWriteSPI( unsigned char, unsigned char, unsigned char );
unsigned char HDByteReadSPI( unsigned char, unsigned char, unsigned char * , unsigned char );
unsigned char HDPageWriteSPI( unsigned char, unsigned char, unsigned char * );
unsigned char HDSequentialReadSPI( unsigned char, unsigned char, unsigned char *, unsigned char );
unsigned char MegaHDSequentialReadSPI( unsigned char, unsigned char, unsigned char, unsigned char *, unsigned char );
//********************************************************************
//Constant Definitions
//********************************************************************
#define DSPIC33F
//#define PIC24F
#define ChipSelect PORTFbits.RF2    // ChipSelect for SPI
#define getcSPI1    ReadSPI1
#define PageSize 0x80       //PageSize is Physical page boundry from Datasheet. (0x10 = 16, 0x20 = 32)
