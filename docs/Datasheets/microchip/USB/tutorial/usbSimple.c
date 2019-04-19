/*
** This program is designed to give an introduction to
** USB programming on a PIC. It is a barebones, stripped-
** down script that demonstrates the bare essentials 
** of getting USB working on a PIC. It's written with
** the intent of showing every instruction necessary,
** in order, with no confusing loops and endless embedded
** function paths to follow. In other words, it's
** inefficent, despicable code. But I think it serves its
** purpose well.
**
** It comes with several hard assumptions:
**
** 1. I'm assuming you have a basic understanding of C.
**    I'm also assuming you have a basic understanding
**    of the USB protocol. I recommend "USB In A Nutshell":
**    http://www.beyondlogic.org/usbnutshell/usb1.shtml
**    if you need a quick crash course.
**
** 2. This code is heavily 18F14K50-specific. It should 
**    port easily to any other USB PIC by just updating
**    the memory address specified for the USB data. I'm
**    not familiar enough with the entire family of USB
**    enabled PICs, so I'm can't promise the register
**    names are consistent throughout all PICs.
**
** 3. I work exclusively in Linux, so this code is tailored
**    for a Linux system. Specifically, I developed this on 
**    kernel 2.6.33.5 on a Fedora Core 13 machine. I'm 
**    guessing this should work on any 2.6 kernel, but I
**    haven't verified.  No idea what will happen if you
**    plug this into a Windows machine. I'm sure Windows
**    enumerates devices slightly differently than Linux,
**    so this code would not work on Windows without some
**    tweaking (but it shouldn't take much).
**
** 4. I include a header file called serialUART_18f.h. This
**    is a home-brew serial port library I wrote that has
**    some very basic print routines. If you have not yet
**    done basic serial-port communication on your PIC,
**    it's highly recommended you start with that. The
**    ability to print data from your PIC directly to a
**    terminal screen is infinitely valuable. I don't use
**    a store-bought debugger, so this is a  must for me.
**    See the top of serialUART_18f.h for some useful
**    websites and some hints to set up the hardware. The
**    functions have comments that explain their usage.
**    I use minicom on Linux as terminal screen.
**
** 4a. If you choose not to use my serial communication file,
**     you'll need to comment out the #include statement, the
**     call to setupUART(), the contents of the printError 
**     function, and any random calls to print8bitVal,
**     print16bitVal, or printString throughout the code.
**
** 5. This code is for a USB 2.0 compatible low-speed device.
**    That means you need a >=6MHz (see your datasheet for 
**    which values are allowed) crystal and data transfers
**    are limited to 8 bytes max. If you need more, you'll
**    need to go full-speed USB and make the necessary
**    changes to the code.
**
** 6. This code compiles successfully on sdcc v2.9.7. If you
**    use a different compiler, you may have to tweak a bit.
**    The compile command I use is:
**        > sdcc -mpic16 -p18f14k50 usbSimple.c
**        
** For Linux users: 
**     minicom is a great terminal program for viewing
**     serial communication. It's similar to hyperterminal
**     for Windows. 
**
**     usbmon is a function that prints USB transaction
**     data to stdout. It takes a few minutes to figure
**     out what's useful and what's junk, but it's very
**     helpful for debugging botched transactions.
**
**     Monitor /var/log/messages when you plug your device
**     in to see an instant report of success or any 
**     difficulties the kernel had enumerating your device.
**     A successful enumeration should look like this:
**         Oct  1 23:33:57 localhost kernel: usb 2-3: new low speed USB device using ohci_hcd and address 28
**         Oct  1 23:33:57 localhost kernel: usb 2-3: New USB device found, idVendor=1234, idProduct=4321
**         Oct  1 23:33:57 localhost kernel: usb 2-3: New USB device strings: Mfr=1, Product=2, SerialNumber=0
**         Oct  1 23:33:57 localhost kernel: usb 2-3: Product: MY PRO
**         Oct  1 23:33:57 localhost kernel: usb 2-3: Manufacturer: MY VEN
*/

#define __pic18f14k50
#include "pic18fregs.h"
#include "config_18f14k50_usbSimple.h"
#include "serialUART_18f.h"
#include "usbSimple.h"

void main() {
    unsigned char pid;
    unsigned char descriptorType;
    unsigned char addrWillBe = 0;

    setupUART();

    initDescriptors();

    UCON = 0;

    // PING-POING disabled
    UCFGbits.PPB0 = 0;
    UCFGbits.PPB1 = 0;

    // Eye-pattern test disabled
    UCFGbits.UTEYE = 0;

    // On-chip pull-up enabled
    UCFGbits.UPUEN = 1;

    // Low speed device
    UCFGbits.FSEN = 0;

    printString("Start-up...");

    // Enable USB
    UCONbits.USBEN = 1;

    // Disable interrupts. Flags will still be
    // set in UIR, but won't interrupt processor.
    UIE = 0;
    UIR = 0;
    UEIE = 0;
    UEIR = 0;

    // The host-assigned address. This should be zero
    // until the host assigns a unique address during
    // enumeration.
    UADDR = 0x00;
    
    UEP0bits.EPOUTEN = 1;  //Enable OUT on ep0
    UEP0bits.EPINEN = 1;   //Enable IN on ep0
    UEP0bits.EPCONDIS = 0; //Allow SETUP transactions on ep0
    UEP0bits.EPHSHK = 1;   //Enable handshaking on ep0

    // Prevent endpoint 0 IN transactions for the moment 
    BD0_IN.STAT = 0;

    // Enable SETUP packet transfers
    UCONbits.PKTDIS = 0;

    /*
    ** Now that the initial USB setup stuff is taken care
    ** of, we're ready for the first actual transaction.
    ** As soon as the host detects your device, it will
    ** send a serious of SETUP requests asking for various
    ** information about your device. 
    **
    ** The first SETUP transaction will always be in the
    ** OUT direction containing a request for a specific 
    ** descriptor. This will be followed by an IN 
    ** transaction that will carry back the requested
    ** descriptor. All of this will be done on endpoint 0.
    */ 

    /*
    ** This is the first transaction. It will have a SETUP
    ** token and be in the OUT direction. It will request
    ** the device descriptor.
    **
    ** In anticipation of this transaction, the maximum
    ** number of bytes in one transaction is set to 8,
    ** the address to write the incoming data is set to 
    ** 0x0228 (this is the address of hostSetupPkg), and
    ** buffer ownership of the OUT direction of endpoint 0 
    ** is given to the SIE.
    **
    ** DTSEN is the data toggle syncronization enable. The DTS
    ** bit must be toggled on and off when sending or receiving
    ** data that is split between multiple transactions. DATA 1
    ** indicates DTS is set, DATA 0 indicates DTS is clear. OUT
    ** direction transactions always begin with DTS clear.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;             // Max number of bytes to receive in one transaction (low-speed device)
    BD0_OUT.ADRH = 0x02;         // Upper two bytes of memory address to write incoming data
    BD0_OUT.ADRL = 0x28;         // Lower two bytes of memory address to write incoming data
    BD0_OUT.STAT = UOWN | DTSEN; // Give ownership to the SIE

    /*
    ** Obviously in a real program, you'll set the address registers 
    ** as a pointer to a variable. But I decided to manually type
    ** out the addresses for complete transparency.
    */

    /*
    ** Now wait for the transaction to complete. When that happens,
    ** the TRNIF flag will be set. At that point, the data from the
    ** host will have been written to the address specified (0x0228  
    ** in this case) and will be accessible as regular RAM. The buffer
    ** ownership bit will be automatically cleared, which prevents
    ** further transactions from happening until the ownership is given
    ** back to the SIE.
    */
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;

    /*
    ** The Packet ID (PID) indicates whether the completed transaction was
    ** a SETUP, IN, or OUT transaction. Here, it will have been a SETUP
    ** transaction. The PID is written to bits 2-5 of BD0_OUT.STAT.
    */
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if ( pid != SETUP_TOKEN )printError(1);

    /*
    ** Of the 8 bytes of data written, the first and second byte are
    ** bmRequestType and bRequest, respectively. The expected values of 
    ** 0x80 and 0x06 indicate this is a GET_DESCRIPTOR request. The 3rd and
    ** 4th byte, wValue, specify which descriptor (the device descriptor here).
    */
    if ( hostSetupPkg.bmRequestType != 0x80 )printError(2); //Just to double check things are
    if ( hostSetupPkg.bRequest      != 0x06 )printError(3); //happening the way we expect.
    descriptorType = hostSetupPkg.wValueH;
    if ( descriptorType != descriptorType_device )printError(4);

    /*
    ** PKTDIS must be cleared after every SETUP transaction to allow
    ** the next SETUP transaction to be processed. This is not necessary
    ** after IN or OUT transactions.
    */
    UCONbits.PKTDIS = 0;

    /**************************/
    /* Send Device Descriptor */
    /**************************/
    sendDeviceDescriptor();  //See comments for this function in usbSimple.h

    /*
    ** Once the device descriptor has been sent, the host will assign a
    ** unique address to your device. The address will be retrievable in
    ** the wValue byte that comes in with the SETUP transaction. This
    ** address will populate the UADDR register.
    ** NOTE: You must complete the subsequent Status transaction BEFORE
    ** assigning the UADDR register. This is because the host will be 
    ** listening for the Status transaction at address 0, not the newly
    ** assigned address.
    **
    ** A bmRequestType and bRequest values of 0x00 and 0x05 indicate
    ** the SET_ADDRESS request.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(5);
    if ( hostSetupPkg.bmRequestType != 0x00 )printError(6);
    if ( hostSetupPkg.bRequest      != 0x05 )printError(7);
    addrWillBe = hostSetupPkg.wValueL;
    UCONbits.PKTDIS = 0;

    /*
    ** No data was requested, thus there was no IN transaction.
    ** The last transaction was a SETUP OUT direction, so this
    ** Status will be IN.
    */

    /*********************/
    /* Status IN DATA 1 */
    /*********************/
    BD0_IN.CNT = 0;
    BD0_IN.STAT = UOWN | DTS | DTSEN;
    while(!UIRbits.TRNIF){}
    if (USTATbits.DIR!=1)printError(8);
    UIRbits.TRNIF = 0;

    /*
    ** Status complete, so now it's safe to set the UADDR register.
    */
    UADDR = addrWillBe;

    /* 
    ** Now that the device address has been set, the host will ask for 
    ** the device descriptor again on the new address. The PIC SIE
    ** takes care of all the USB device address particulars, so
    ** nothing really changes from a coding standpoint.
    */
 
    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(9);
    if ( hostSetupPkg.bmRequestType != 0x80 )printError(10);
    if ( hostSetupPkg.bRequest      != 0x06 )printError(11);
    descriptorType = hostSetupPkg.wValueH;
    if ( descriptorType != descriptorType_device )printError(12);
    UCONbits.PKTDIS = 0;

    /*************************************/
    /* Send Device Descriptor (2nd time) */
    /*************************************/
    sendDeviceDescriptor();

    /*
    ** And now the configuration descriptor.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(13);
    if ( hostSetupPkg.bmRequestType != 0x80 )printError(14);
    if ( hostSetupPkg.bRequest      != 0x06 )printError(15);
    descriptorType = hostSetupPkg.wValueH;
    if ( descriptorType != descriptorType_config )printError(16);
    UCONbits.PKTDIS = 0;

    /*********************************/
    /* Send Configuration Descriptor */
    /*********************************/
    sendConfigDescriptor();

    /*
    ** The config descriptor told the host how many bytes total are
    ** in the config, interface, and endpoint descriptors. With that
    ** information, it'll ask for all three descriptors at once. The
    ** value of wValue indicates how many bytes the host wants. This
    ** time wValue should equal the value defined by wTotalLength in
    ** the config descriptor.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(17);
    if ( hostSetupPkg.bmRequestType != 0x80 )printError(18);
    if ( hostSetupPkg.bRequest      != 0x06 )printError(19);
    descriptorType = hostSetupPkg.wValueH;
    if ( descriptorType != descriptorType_config )printError(20);
    if ( hostSetupPkg.wLengthL != configDescr.wTotalLength )printError(21);
    UCONbits.PKTDIS = 0;

   /**********************************************************/
   /* Send Configuration, Interface, and Endpoint Descriptors*/
   /**********************************************************/
    sendConfigDescriptorAndOthers();

    /*
    ** Finally, the string descriptors. These are personalized strings
    ** associated with your USB device. The first string descriptor
    ** just sets the language ID. After that, the host will request
    ** string descriptors populated with your personalized strings.
    ** For this program, we only defined a string for the vendor
    ** and product as examples. The rest are disabled.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(22);
    if ( hostSetupPkg.bmRequestType != 0x80 )printError(23);
    if ( hostSetupPkg.bRequest      != 0x06 )printError(24);
    descriptorType = hostSetupPkg.wValueH;
    if ( descriptorType != descriptorType_string )printError(25);
    if ( hostSetupPkg.wValueL != 0x00 )printError(26);
    UCONbits.PKTDIS = 0;

    /***************************/
    /* Send String Descriptor0 */
    /***************************/
    sendStringDescriptor0();

    /*
    ** String descriptor request for the product string.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(27);
    if ( hostSetupPkg.bmRequestType != 0x80 )printError(28);
    if ( hostSetupPkg.bRequest      != 0x06 )printError(29);
    descriptorType = hostSetupPkg.wValueH;
    if ( descriptorType != descriptorType_string )printError(30);
    if ( hostSetupPkg.wValueL != 0x02 )printError(31);
    UCONbits.PKTDIS = 0;

    /***************************/
    /* Send String Descriptor1 */
    /***************************/
    sendStringDescriptor1();

    /*
    ** Now the vendor string.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    if (USTATbits.DIR!=0)printError(32);
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(33);
    if ( hostSetupPkg.bmRequestType != 0x80 )printError(34);
    if ( hostSetupPkg.bRequest      != 0x06 )printError(35);
    descriptorType = hostSetupPkg.wValueH;
    if ( descriptorType != descriptorType_string )printError(36);
    if ( hostSetupPkg.wValueL != 0x01 )printError(37);
    UCONbits.PKTDIS = 0;

    /***************************/
    /* Send String Descriptor2 */
    /***************************/
    sendStringDescriptor2();

    /*
    ** After sending the last string descriptor, the host will send
    ** back one last SETUP command to enable the device. That means
    ** we're done! The device is fully enumerated and the host is
    ** satisfied with the information it recieved.
    */

    /**********************/
    /* SETUP Token DATA 0 */
    /**********************/
    BD0_OUT.CNT = 8;
    BD0_OUT.ADRH = 0x02;
    BD0_OUT.ADRL = 0x28;
    BD0_OUT.STAT = UOWN | DTSEN;
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;
    pid = (BD0_OUT.STAT & 0x3C) >> 2;
    if (pid!=SETUP_TOKEN)printError(38);
    if ( hostSetupPkg.bmRequestType != 0x00 )printError(39);
    if ( hostSetupPkg.bRequest      != 0x09 )printError(40);
    UCONbits.PKTDIS = 0;

    /*********************/
    /* Status IN DATA 1 */
    /*********************/
    BD0_IN.CNT = 0;
    BD0_IN.STAT = UOWN | DTS | DTSEN;
    while(!UIRbits.TRNIF){}
    if (USTATbits.DIR!=1)printError(63);
    UIRbits.TRNIF = 0;

    /*
    ** Now, just for giggles, let's set up an OUT transaction. 
    ** This will wait forever for an OUT token, which can only
    ** come from a driver running on the host (look into libusb, 
    ** for example).
    **
    ** The basic set up is very similar to a SETUP transaction.
    ** Except now we'll be talking over endpoint 1 in the OUT
    ** direction, so we'll use the appropriate buffer descriptor.
    ** A structure was created in this program at 0x02A0 called
    ** outPkgEp1. This will contain the 8 bytes of data transmitted
    ** from the host.
    */

    /**************/
    /* OUT DATA 0 */
    /**************/
    BD1_OUT.CNT = 8;
    BD1_OUT.ADRH = 0x02;  //address of outPkgEp1
    BD1_OUT.ADRL = 0xA0;
    BD1_OUT.STAT = UOWN | DTSEN; 
    while(!UIRbits.TRNIF){}
    UIRbits.TRNIF = 0;

    print8bitVal(outPkgEp1.BYTE0);
    print8bitVal(outPkgEp1.BYTE1);
    print8bitVal(outPkgEp1.BYTE2);
    print8bitVal(outPkgEp1.BYTE3);
    print8bitVal(outPkgEp1.BYTE4);
    print8bitVal(outPkgEp1.BYTE5);
    print8bitVal(outPkgEp1.BYTE6);
    print8bitVal(outPkgEp1.BYTE7);

    /*
    ** The end
    */
    while(1){} 
}
