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
// ©2009 Dynastream Innovations Inc. All Rights Reserved
// This software may not be reproduced by
// any means without express written approval of Dynastream
// Innovations Inc.
//
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "types.h"

#define OPTION_FILE_VERSION_STRING "AAA"              // Config file "Version"

#define NETWORK_KEY_SIZE            ((UCHAR) 8)

#define DEFAULT_BAUD_RATE           ((USHORT) 57600)
#define DEFAULT_USB_PORT            ((UCHAR) 0)
#define DEFAULT_USB_AUTOINIT        ((UCHAR) 1)

#define DEFAULT_CHANNEL_DEVICE_ID   ((UCHAR) 0)
#define DEFAULT_CHANNEL_DEVICE_TYPE ((UCHAR) 1)
#define DEFAULT_CHANNEL_TRANS_TYPE  ((UCHAR) 0)       // Wild card

#define DEFAULT_SEARCH_FREQ         ((UCHAR) 50)      // 2450 MHz
#define DEFAULT_LINK_FREQ           ((UCHAR) 255)     // Use random link freq
#define DEFAULT_SEARCH_MESG_PERIOD  ((USHORT) 8192)   // 4Hz

#define DEFAULT_CLIENT_MFG_ID       ((USHORT) 2)
#define DEFAULT_CLIENT_DEVICE_TYPE  ((USHORT) 416)
#define DEFAULT_CLIENT_DEVICE_ID    ((ULONG) 0)
#define DEFAULT_CLIENT_KEY_LENGTH   ((UCHAR) 16)

#define DEFAULT_PROXIMITY_BIN_DISABLED ((UCHAR) 0)

#define ENCRYPTION_KEY_LENGTH		   ((UCHAR) 32)

// FR-50 Specific
#define FR_50_CHANNEL_DEVICE_ID     ((USHORT) 0)      // Wild card
#define FR_50_CHANNEL_DEVICE_TYPE   ((UCHAR) 1)       // Watch
#define FR_50_CHANNEL_TRANS_TYPE    ((UCHAR) 0)       // Wild card
#define FR_50_SEARCH_FREQ           ((UCHAR) 50)      // ANT-FS default
#define FR_50_LINK_FREQ             ((UCHAR) 255)     // Random
#define FR_50_MESG_PERIOD           ((USHORT) 4096)   // 8Hz
#define FR_50_CLIENT_MFG_ID         ((USHORT) 1)      // Garmin
#define FR_50_CLIENT_DEVICE_TYPE    ((USHORT) 782)    // FR-50

// FR-60 Specific
#define FR_60_CHANNEL_DEVICE_ID     ((USHORT) 0)      // Wild card
#define FR_60_CHANNEL_DEVICE_TYPE   ((UCHAR) 1)       // Watch
#define FR_60_CHANNEL_TRANS_TYPE    ((UCHAR) 0)       // Wild card
#define FR_60_SEARCH_FREQ           ((UCHAR) 50)      // ANT-FS default
#define FR_60_LINK_FREQ             ((UCHAR) 255)     // Random
#define FR_60_MESG_PERIOD           ((USHORT) 4096)   // 8Hz
#define FR_60_CLIENT_MFG_ID         ((USHORT) 1)      // Garmin
#define FR_60_CLIENT_DEVICE_TYPE    ((USHORT) 988)    // FR-60

// FR-310XT Specific
#define FR_310_CHANNEL_DEVICE_ID     ((USHORT) 0)      // Wild card
#define FR_310_CHANNEL_DEVICE_TYPE   ((UCHAR) 1)       // Watch
#define FR_310_CHANNEL_TRANS_TYPE    ((UCHAR) 0)       // Wild card
#define FR_310_SEARCH_FREQ           ((UCHAR) 50)      // ANT-FS default
#define FR_310_LINK_FREQ             ((UCHAR) 255)       // Random
#define FR_310_MESG_PERIOD           ((USHORT) 4096)   // 8Hz
#define FR_310_CLIENT_MFG_ID         ((USHORT) 1)      // Garmin
#define FR_310_CLIENT_DEVICE_TYPE    ((USHORT) 1018)   // FR-310XT

// Fitness Equipment
#define FE_CHANNEL_DEVICE_ID        ((USHORT) 0)      // Wild card
#define FE_CHANNEL_DEVICE_TYPE      ((UCHAR) 1)       // Fitness Equipment
#define FE_CHANNEL_TRANS_TYPE       ((UCHAR) 0)       // Wild card
#define FE_SEARCH_FREQ              ((UCHAR) 57)      // ANT+
#define FE_LINK_FREQ                ((UCHAR) 255)     // Random
#define FE_MESG_PERIOD              ((USHORT) 4096)   // 8Hz
#define FE_CLIENT_MFG_ID            ((USHORT) 0)	  // Wild card
#define FE_CLIENT_DEVICE_TYPE       ((USHORT) 0)      // Wild card


// Weight Scale
#define WGT_CHANNEL_DEVICE_ID       ((USHORT) 0)      // Wild card
#define WGT_CHANNEL_DEVICE_TYPE     ((UCHAR) 119)     // Weight scale
#define WGT_CHANNEL_TRANS_TYPE      ((UCHAR) 0)       // Wild card
#define WGT_SEARCH_FREQ             ((UCHAR) 57)      // ANT+
#define WGT_LINK_FREQ               ((UCHAR) 255)     // Random
#define WGT_MESG_PERIOD             ((USHORT) 8192)   // 4Hz
#define WGT_CLIENT_MFG_ID           ((USHORT) 0)      // Wild card
#define WGT_CLIENT_DEVICE_TYPE      ((USHORT) 0)      // Wild card


// Blood Pressure Monitor
#define BPM_CHANNEL_DEVICE_ID       ((USHORT) 0)      // Wild card
#define BPM_CHANNEL_DEVICE_TYPE     ((UCHAR) 18)      // Blood Pressure monitor
#define BPM_CHANNEL_TRANS_TYPE      ((UCHAR) 0)       // Wild card
#define BPM_SEARCH_FREQ             ((UCHAR) 57)      // ANT+
#define BPM_LINK_FREQ               ((UCHAR) 255)     // Random
#define BPM_MESG_PERIOD             ((USHORT) 8192)   // 4Hz
#define BPM_CLIENT_MFG_ID           ((USHORT) 0)      // Wild card
#define BPM_CLIENT_DEVICE_TYPE      ((USHORT) 0)	  // Wild card



//#error "MUST TYPE ANT-FS & ANT+ NETWORK KEYS HERE, OR SET TO ALL 0'S IF USING THE PUBLIC KEY" 
const UCHAR aucAntFsKey[NETWORK_KEY_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   // INSERT ANT-FS NETWORK KEY HERE !!!!
const UCHAR aucAntPlusKey[NETWORK_KEY_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   // INSERT ANT+ NETWORK KEY HERE !!!!


using namespace System;
using namespace System::Windows::Forms;

public ref struct OptionsStruct
{

public:

   OptionsStruct()
   {
      Reset();
   }


   void Reset()
   {
      bDebug = FALSE;
      bEnablePing = TRUE;
      bBroadcastSearch = FALSE;
      ucUndiscoverableTimeDuration = 0;
      ucUndiscoverableAppSpecificDuration = 0;
      ulBlockSize = 0;
   
      usBaudRate = DEFAULT_BAUD_RATE;
      ucUSBPort = DEFAULT_USB_PORT;
      bAutoInit = DEFAULT_USB_AUTOINIT;
   
      usChannelDeviceID = DEFAULT_CHANNEL_DEVICE_ID;
      ucChannelDeviceType = DEFAULT_CHANNEL_DEVICE_TYPE;
      ucChannelTransmissionType = DEFAULT_CHANNEL_TRANS_TYPE;
      usMessagePeriod = DEFAULT_SEARCH_MESG_PERIOD;          
   
      ucSearchFrequency = DEFAULT_SEARCH_FREQ;                
      ucLinkFrequency = DEFAULT_LINK_FREQ;             
   
      usClientManufacturingID = DEFAULT_CLIENT_MFG_ID;     
      usClientDeviceType = DEFAULT_CLIENT_DEVICE_TYPE;
      ulClientDeviceID = DEFAULT_CLIENT_DEVICE_ID;

      // Proximity search bin (not stored in config)
      ucProximitySearchBin = DEFAULT_PROXIMITY_BIN_DISABLED;

      // Friendly name
      aucFriendlyName = gcnew array<UCHAR^>(1);
      aucFriendlyName[0] = (UCHAR)0x00;
    
      // Network key
      aucNetworkKey = gcnew array<UCHAR^>(NETWORK_KEY_SIZE);

      for (int i = 0; i < NETWORK_KEY_SIZE; i++)
      {
         aucNetworkKey[i] = aucAntFsKey[i];
      }
      
      // Passkey
      pcArray = gcnew array<UCHAR^>(DEFAULT_CLIENT_KEY_LENGTH);

      for (int i = 0; i< DEFAULT_CLIENT_KEY_LENGTH; i++)
         pcArray[i] = (UCHAR)0x00;

	  // Encryption Key
	  aucEncryptionKey = gcnew array<UCHAR>(ENCRYPTION_KEY_LENGTH);

	  for (int i = 0; i< ENCRYPTION_KEY_LENGTH; ++i)
         aucEncryptionKey[i] = (Byte)i;

     // Path (not stored in config)
     SetDefaultPath();

   }

   void SetDefaultPath()
   {
      strPath = System::IO::Path::Combine(Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData), "ANTFS_PC_Host\\logs");
   }



   BOOL SaveConfigFile(String^ strFileName_)
   {
      System::IO::StreamWriter^ pWriter = gcnew System::IO::StreamWriter(strFileName_);

      pWriter->WriteLine(OPTION_FILE_VERSION_STRING);      


      pWriter->WriteLine(bDebug.ToString());      
      pWriter->WriteLine(usBaudRate.ToString());
      pWriter->WriteLine(ucUSBPort.ToString());
      pWriter->WriteLine(bAutoInit.ToString());
      pWriter->WriteLine(usChannelDeviceID.ToString());
      pWriter->WriteLine(ucChannelDeviceType.ToString());
      pWriter->WriteLine(ucChannelTransmissionType.ToString());
      pWriter->WriteLine(usMessagePeriod.ToString());
      pWriter->WriteLine(ucSearchFrequency.ToString());
      pWriter->WriteLine(ucLinkFrequency.ToString());
      pWriter->WriteLine(usClientManufacturingID.ToString());
      pWriter->WriteLine(usClientDeviceType.ToString());
      pWriter->WriteLine(ulClientDeviceID.ToString());


      // Write out network key
      for (int iKeyInd = 0; iKeyInd < 8; iKeyInd++)
         pWriter->WriteLine(aucNetworkKey[iKeyInd]->ToString());

      // Write out passkey, length first
      if(pcArray)
      {
         pWriter->WriteLine(pcArray->Length.ToString());

         for (int i = 0; i < pcArray->Length; i++)
         {
            if(pcArray[i])
               pWriter->WriteLine(pcArray[i]->ToString());
            else
               pWriter->WriteLine("0");
         }
      }  

      // Write out friendly name, length first
      if(aucFriendlyName)
         pWriter->WriteLine(aucFriendlyName->Length.ToString());

      for(int i = 0; i < aucFriendlyName->Length; i++)
      {
         if(aucFriendlyName[i])
            pWriter->WriteLine(aucFriendlyName[i]->ToString());
         else
            pWriter->WriteLine("0");
      }
      
      // Write out encryption key
      for (int iKeyInd = 0; iKeyInd < ENCRYPTION_KEY_LENGTH; iKeyInd++)
         pWriter->WriteLine(aucEncryptionKey[iKeyInd]);

      // Write out ping/bcast config
      pWriter->WriteLine(bEnablePing.ToString());
      pWriter->WriteLine(bBroadcastSearch.ToString());


      pWriter->Close();

      return(TRUE);
   }
   
   
   BOOL LoadConfigFile(String^ strFileName_)
   {
      try
      {
	      System::IO::StreamReader^ psrdReader = gcnew System::IO::StreamReader(strFileName_);  // Load file
	
         try
	      {

            String^ strVersion = gcnew String(psrdReader->ReadLine());

            if(System::String::Compare(strVersion, OPTION_FILE_VERSION_STRING))
            {
               String^ strMessage = gcnew String ("Invalid File Format \"" + strFileName_ + "\"");
               MessageBox::Show(strMessage, "Invalid File Format", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
               return FALSE;

            }
            bDebug = System::Convert::ToUInt16(psrdReader->ReadLine());
            usBaudRate = System::Convert::ToUInt16(psrdReader->ReadLine());
            ucUSBPort = System::Convert::ToByte(psrdReader->ReadLine());
            bAutoInit = System::Convert::ToByte(psrdReader->ReadLine());
            usChannelDeviceID = System::Convert::ToInt16(psrdReader->ReadLine());
            ucChannelDeviceType = System::Convert::ToByte(psrdReader->ReadLine());
            ucChannelTransmissionType = System::Convert::ToByte(psrdReader->ReadLine());
            usMessagePeriod = System::Convert::ToInt16(psrdReader->ReadLine());
            ucSearchFrequency = System::Convert::ToByte(psrdReader->ReadLine());
            ucLinkFrequency = System::Convert::ToByte(psrdReader->ReadLine());
            usClientManufacturingID = System::Convert::ToInt16(psrdReader->ReadLine());
            usClientDeviceType = System::Convert::ToInt16(psrdReader->ReadLine());
            ulClientDeviceID = System::Convert::ToInt16(psrdReader->ReadLine());


            // Get network key
            for (int iKeyInd = 0; iKeyInd < NETWORK_KEY_SIZE; iKeyInd++)
               aucNetworkKey[iKeyInd] = System::Convert::ToByte(psrdReader->ReadLine());


            // Get passkey
            UCHAR ucLength = System::Convert::ToByte(psrdReader->ReadLine());


            pcArray = gcnew array<UCHAR^>(ucLength);

            for (int i = 0; i < ucLength; i++)
            {
                 pcArray[i] = System::Convert::ToByte(psrdReader->ReadLine());
            }

            // Get friendly name
            if(psrdReader->Peek() > -1)    // first check if there is a name in the config
            {
               UCHAR ucLengthName = System::Convert::ToByte(psrdReader->ReadLine());

               aucFriendlyName = gcnew array<UCHAR^>(ucLengthName);

               for (int i = 0; i < ucLengthName; i++)
               {
                  aucFriendlyName[i] = System::Convert::ToByte(psrdReader->ReadLine());
               }
            }
            else
            {
               aucFriendlyName = gcnew array<UCHAR^>(1);
               aucFriendlyName[0] = (UCHAR)0x00;
            }

			// Get encryption key
			for (int iKeyInd = 0; iKeyInd < ENCRYPTION_KEY_LENGTH; iKeyInd++)
               aucEncryptionKey[iKeyInd] = System::Convert::ToByte(psrdReader->ReadLine());

         // Get ping/search parameters
          bEnablePing = System::Convert::ToUInt16(psrdReader->ReadLine());
          bBroadcastSearch = System::Convert::ToUInt16(psrdReader->ReadLine());

	      }
         finally
         {
		   if (psrdReader)
			   psrdReader->Close();
         }
      }
      catch(...) 
      {
         return(FALSE);  
      }

      return(TRUE);
   }
   


   // General Options
   BOOL bDebug;
   BOOL bBroadcastSearch;
   BOOL bEnablePing;
   UCHAR ucUndiscoverableTimeDuration;
   UCHAR ucUndiscoverableAppSpecificDuration;
   ULONG ulBlockSize;
   
   // USB Options
   USHORT usBaudRate;
   UCHAR ucUSBPort;
   BOOL bAutoInit;
   
   // ANT Channel options
   USHORT usChannelDeviceID;
   UCHAR ucChannelDeviceType;
   UCHAR ucChannelTransmissionType;
   USHORT usMessagePeriod;
   
   // RF Frequency
   UCHAR ucSearchFrequency;
   UCHAR ucLinkFrequency;

   // Advanced Channel Config
   UCHAR ucProximitySearchBin;
   
   // Client options
   USHORT usClientManufacturingID;
   USHORT usClientDeviceType;
   ULONG ulClientDeviceID;

   // Host options
   array<UCHAR^>^ aucFriendlyName;

   // Network key
   array<UCHAR^>^ aucNetworkKey;

   // Passkey
   array<UCHAR^>^ pcArray;

   //Encryption Key
   array<UCHAR>^ aucEncryptionKey;

   // Debug path
   System::String^ strPath;
};








