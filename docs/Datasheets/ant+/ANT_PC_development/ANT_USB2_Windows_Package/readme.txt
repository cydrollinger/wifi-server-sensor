---------------------------------------------------------------------------------
Dynastream Innovations Inc.
Cochrane, AB, CANADA

Copyright © 1998-2010 Dynastream Innovations Inc.
All rights reserved. This software may not be reproduced by
any means without express written approval of Dynastream
Innovations Inc.
---------------------------------------------------------------------------------

!!!!!!!!!!!!!!!!! PLEASE CHECK IMPORTANT NOTES AT THE BOTTOM !!!!!!!!!!!!!!!!!!!!

---------------------------------------------------------------------------------

Description of projects ...

DEMO_DLL (C++)

A simple command line application built on top of the ANT DLL that demonstrates how
to import the ANT DLL and setup ANT channels and data messages.

DEMO_NET (C#)

A simple command line application built on top of the ANT_NET DLL that demonstrates how
to import the managed library and setup ANT channels and data messages.

DEMO_ANTFS (C#)

A simple command line application built on top of the ANT_NET DLL that demonstrates how to
import the managed library and setup an ANT-FS host device.


---------------------------------------------------------------------------------
** Important ** Setting the network key.

Before the DEMO_ANTFS will compile, the ANT-FS network key must be set. Inside 
the file DEMO_ANTFS\demo_host.cs, set the key to the ANT-FS network
key or to all 0's to use the public key.

---------------------------------------------------------------------------------
** Important ** Using the ANT Managed Library

The ANT Managed Library was designed for ease of integration of ANT in applications
using the .NET framework.  

When using the ANT Managed Library, BOTH the unmanaged and managed wrapper
need to be included.  The two DLLs required  are:

ANT_WrappedLib.dll
ANT_NET.dll

Binary versions of these are available in the Release directory.

Additionally, applications developed using the ANT Managed Library require the 
Microsoft .NET Framework v3.5 or higher
http://www.microsoft.com/downloads/details.aspx?FamilyId=333325FD-AE52-4E35-B531-508D977D32A6&displaylang=en














