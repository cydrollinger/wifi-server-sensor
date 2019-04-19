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
#include "StdAfx.h"
#include "Auth.h"


using namespace ANTFSDemo;

////////////////////////////////////////////////////////////////////////////
// Auth
//
// Class Contructor. 
// Initalizes member variables. Takes options struct and translates it to
// display on GUI.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
Auth::Auth()
{
	InitializeComponent();
}


////////////////////////////////////////////////////////////////////////////
// ~Auth
//
// Destructor. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
Auth::~Auth()
{
	if (components)
	{
		delete components;
	}
}


////////////////////////////////////////////////////////////////////////////
// Auth Show Window
//
// Static show window function enabling Options window to be a singleton
//
////////////////////////////////////////////////////////////////////////////
void Auth::ShowWindow(OptionsStruct^ pstOptions_, System::Windows::Forms::Form^ form)
{
   if(instance == nullptr)
      instance = gcnew Auth();

   instance->Owner = form;
   instance->pstMyOptionsStruct = pstOptions_;
	
   // If struct exists, fill in the GUI
   if(instance->pstMyOptionsStruct)
      instance->ToGui(instance->pstMyOptionsStruct); 

   instance->Show();
}


////////////////////////////////////////////////////////////////////////////
// OKClick
//
// Runs when user presses the 'OK' button. The contents of the GUI are parsed
// and the options struct is updated with their contents. Options dialog is
// closed. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
System::Void Auth::OKClick() 
{
   FromGui(pstMyOptionsStruct);
   this->Close();
}

////////////////////////////////////////////////////////////////////////////
// ApplyClick
//
// Runs when user presses the 'Apply' button. The contents of the GUI are parsed
// and the options struct is updated with their contents. Options dialog is
// not closed.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
System::Void Auth::ApplyClick() 
{
   FromGui(pstMyOptionsStruct);
}


////////////////////////////////////////////////////////////////////////////
// ComboQuickOptionsChanged
//
// Called when user selects an item in the 'Quick Config' combo box. The options
// struct is populated with the options for selected device. The GUI is then updated
// to reflect these options.   
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Auth::ComboQuickOptionsChanged()
{
   switch(comboOptions->SelectedIndex)
   {
      case 1:  // FR-50
      {

         for (int i = 0; i < NETWORK_KEY_SIZE; i++)
         {
            pstMyOptionsStruct->aucNetworkKey[i] = aucAntFsKey[i];
         }

         pstMyOptionsStruct->usChannelDeviceID = FR_50_CHANNEL_DEVICE_ID;
         pstMyOptionsStruct->ucChannelDeviceType = FR_50_CHANNEL_DEVICE_TYPE;
         pstMyOptionsStruct->ucChannelTransmissionType = FR_50_CHANNEL_TRANS_TYPE;

         pstMyOptionsStruct->ucSearchFrequency = FR_50_SEARCH_FREQ;
         pstMyOptionsStruct->ucLinkFrequency = FR_50_LINK_FREQ;

         pstMyOptionsStruct->usMessagePeriod = FR_50_MESG_PERIOD;

         pstMyOptionsStruct->usClientDeviceType = FR_50_CLIENT_DEVICE_TYPE;
         pstMyOptionsStruct->usClientManufacturingID = FR_50_CLIENT_MFG_ID;
         pstMyOptionsStruct->bBroadcastSearch = FALSE;


         break;
      }
      case 2: // FR-60
      {
         for (int i = 0; i < NETWORK_KEY_SIZE; i++)
         {
            pstMyOptionsStruct->aucNetworkKey[i] = aucAntFsKey[i];
         }
        
         pstMyOptionsStruct->usChannelDeviceID = FR_60_CHANNEL_DEVICE_ID;
         pstMyOptionsStruct->ucChannelDeviceType = FR_60_CHANNEL_DEVICE_TYPE;
         pstMyOptionsStruct->ucChannelTransmissionType = FR_60_CHANNEL_TRANS_TYPE;

         pstMyOptionsStruct->ucSearchFrequency = FR_60_SEARCH_FREQ;
         pstMyOptionsStruct->ucLinkFrequency = FR_60_LINK_FREQ;

         pstMyOptionsStruct->usMessagePeriod = FR_60_MESG_PERIOD;

         pstMyOptionsStruct->usClientDeviceType = FR_60_CLIENT_DEVICE_TYPE;
         pstMyOptionsStruct->usClientManufacturingID = FR_60_CLIENT_MFG_ID;
         pstMyOptionsStruct->bBroadcastSearch = FALSE;     
         break;
      }
	  case 3: // FR-310XT
      {
         for (int i = 0; i < NETWORK_KEY_SIZE; i++)
         {
            pstMyOptionsStruct->aucNetworkKey[i] = aucAntFsKey[i];
         }
        
         pstMyOptionsStruct->usChannelDeviceID = FR_310_CHANNEL_DEVICE_ID;
         pstMyOptionsStruct->ucChannelDeviceType = FR_310_CHANNEL_DEVICE_TYPE;
         pstMyOptionsStruct->ucChannelTransmissionType = FR_310_CHANNEL_TRANS_TYPE;

         pstMyOptionsStruct->ucSearchFrequency = FR_310_SEARCH_FREQ;
         pstMyOptionsStruct->ucLinkFrequency = FR_310_LINK_FREQ;

         pstMyOptionsStruct->usMessagePeriod = FR_310_MESG_PERIOD;

         pstMyOptionsStruct->usClientDeviceType = FR_310_CLIENT_DEVICE_TYPE;
         pstMyOptionsStruct->usClientManufacturingID = FR_310_CLIENT_MFG_ID;
         pstMyOptionsStruct->bBroadcastSearch = FALSE;
         break;
      }
      case 4: // Fitness Equipment
      {
         for (int i = 0; i < NETWORK_KEY_SIZE; i++)
         {
            pstMyOptionsStruct->aucNetworkKey[i] = aucAntPlusKey[i];
         }
        
         pstMyOptionsStruct->usChannelDeviceID = FE_CHANNEL_DEVICE_ID;
         pstMyOptionsStruct->ucChannelDeviceType = FE_CHANNEL_DEVICE_TYPE;
         pstMyOptionsStruct->ucChannelTransmissionType = FE_CHANNEL_TRANS_TYPE;

         pstMyOptionsStruct->ucSearchFrequency = FE_SEARCH_FREQ;
         pstMyOptionsStruct->ucLinkFrequency = FE_LINK_FREQ;

         pstMyOptionsStruct->usMessagePeriod = FE_MESG_PERIOD;

         pstMyOptionsStruct->usClientDeviceType = FE_CLIENT_DEVICE_TYPE;
         pstMyOptionsStruct->usClientManufacturingID = FE_CLIENT_MFG_ID;
         pstMyOptionsStruct->bBroadcastSearch = FALSE;         
         break;
      }
      case 5: // Weight Scale
      {
         for (int i = 0; i < NETWORK_KEY_SIZE; i++)
         {
            pstMyOptionsStruct->aucNetworkKey[i] = aucAntPlusKey[i];
         }
        
         pstMyOptionsStruct->usChannelDeviceID = WGT_CHANNEL_DEVICE_ID;
         pstMyOptionsStruct->ucChannelDeviceType = WGT_CHANNEL_DEVICE_TYPE;
         pstMyOptionsStruct->ucChannelTransmissionType = WGT_CHANNEL_TRANS_TYPE;

         pstMyOptionsStruct->ucSearchFrequency = WGT_SEARCH_FREQ;
         pstMyOptionsStruct->ucLinkFrequency = WGT_LINK_FREQ;

         pstMyOptionsStruct->usMessagePeriod = WGT_MESG_PERIOD;

         pstMyOptionsStruct->usClientDeviceType = WGT_CLIENT_DEVICE_TYPE;
         pstMyOptionsStruct->usClientManufacturingID = WGT_CLIENT_MFG_ID;
         pstMyOptionsStruct->bBroadcastSearch = FALSE;
         break;
      }
      case 6: // Blood Pressure Monitor
      {
         for (int i = 0; i < NETWORK_KEY_SIZE; i++)
         {
            pstMyOptionsStruct->aucNetworkKey[i] = aucAntPlusKey[i];
         }
        
         pstMyOptionsStruct->usChannelDeviceID = BPM_CHANNEL_DEVICE_ID;
         pstMyOptionsStruct->ucChannelDeviceType = BPM_CHANNEL_DEVICE_TYPE;
         pstMyOptionsStruct->ucChannelTransmissionType = BPM_CHANNEL_TRANS_TYPE;

         pstMyOptionsStruct->ucSearchFrequency = BPM_SEARCH_FREQ;
         pstMyOptionsStruct->ucLinkFrequency = BPM_LINK_FREQ;

         pstMyOptionsStruct->usMessagePeriod = BPM_MESG_PERIOD;

         pstMyOptionsStruct->usClientDeviceType = BPM_CLIENT_DEVICE_TYPE;
         pstMyOptionsStruct->usClientManufacturingID = BPM_CLIENT_MFG_ID;
         pstMyOptionsStruct->bBroadcastSearch = FALSE;
         break;
      }
// NOTE: Please do not add any non public Quick Configs before this one.
      case 0:  // default
      default:
      {
         pstMyOptionsStruct->Reset();
         break;
      }

   }

   ToGui(pstMyOptionsStruct);

}



////////////////////////////////////////////////////////////////////////////
// ToGui
//
// Parses options struct and updates the GUI.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
BOOL Auth::ToGui(OptionsStruct^ pstOptions_)
{

   //************* 'General' TAB ************************
   
   // Debug?
   checkDebug->Checked = pstOptions_->bDebug == 1 ? TRUE:FALSE;
      
   // Baud Rate
   comboBaud->SelectedIndex = pstOptions_->usBaudRate == 50000 ? 0:1;
   
   // USB Port
   comboPort->SelectedIndex = pstOptions_->ucUSBPort;

   // Auto Init
   if(pstOptions_->bAutoInit == 1)
   {
      checkAutoInit->Checked = TRUE;
      comboBaud->Enabled = FALSE;
      comboPort->Enabled = FALSE;
   }
   else
   {
      checkAutoInit->Checked = FALSE;
      comboBaud->Enabled = TRUE;
      comboPort->Enabled = TRUE;
   }

   //************* 'ANT Channel' TAB ************************
   
   // Channel Device ID
   textChannelDeviceID->Text = pstOptions_->usChannelDeviceID.ToString();
   
   // Channel Device Type
   textChannelDeviceType->Text = pstOptions_->ucChannelDeviceType.ToString();
   
   // Channel Transmission Type
   textChannelTransType->Text = pstOptions_->ucChannelTransmissionType.ToString();

   // Channel Search Frequency
   textChannelSearchFreq->Text = pstOptions_->ucSearchFrequency.ToString();
   
   // Channel Link Frequency
   textChannelLinkFreq->Text = pstOptions_->ucLinkFrequency.ToString();

   // Use Random Link Frequency?
   if(pstMyOptionsStruct->ucLinkFrequency == 255)
   {
      checkRandom->Checked = TRUE;
      textChannelLinkFreq->Enabled = FALSE;
   }
   else
   {
      checkRandom->Checked = FALSE;
      textChannelLinkFreq->Enabled = TRUE;
   }
   
   // Message period 
   textChannelPeriodByPeriod->Text = pstOptions_->usMessagePeriod.ToString();
   textChannelPeriodByPeriod_TextChanged(this, nullptr);

   // Proximity search
   numProximityBin->Value = pstOptions_->ucProximitySearchBin;

   // Network Key
   textNetKey1->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[0]);
   textNetKey2->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[1]);
   textNetKey3->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[2]);
   textNetKey4->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[3]);
   textNetKey5->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[4]);
   textNetKey6->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[5]);
   textNetKey7->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[6]);
   textNetKey8->Text = String::Format("{0:X2}",pstOptions_->aucNetworkKey[7]);


   //************* 'Client' TAB ************************
   
   // Client Manufacturing ID
   textClientDeviceID->Text = pstOptions_->ulClientDeviceID.ToString();
   
   // Client Device Type
   textClientDeviceType->Text = pstOptions_->usClientDeviceType.ToString();
   
   // Client Device ID
   textClientManufID->Text = pstOptions_->usClientManufacturingID.ToString();

   // ANT-FS Broadcast
   checkBroadcastMode->Checked = pstOptions_->bBroadcastSearch == 1 ? TRUE:FALSE;

   // Ping Enabled
   checkPing->Checked = pstOptions_->bEnablePing == 1 ? TRUE : FALSE;

   // Request undiscoverability
   numUndiscoverableTime->Value = pstOptions_->ucUndiscoverableTimeDuration * (USHORT) 30;
   numUndiscoverableAppSpecific->Value = pstOptions_->ucUndiscoverableAppSpecificDuration;
   numBlockSize->Value = (UInt32) pstOptions_->ulBlockSize;

   // Host Friendly Name
   char acFriendlyName[255];
   memset(acFriendlyName, 0, sizeof(acFriendlyName));

   int iLengthName = pstOptions_->aucFriendlyName->Length;
   for(int i = 0; i < iLengthName; i++)
   {
      acFriendlyName[i] = (char) *pstOptions_->aucFriendlyName[i];
   }

   String^ strName = gcnew String(acFriendlyName);
   textFriendlyName->Text = strName;

   //************* 'Security' TAB ************************

   // Client Passcode
   int iLength = pstOptions_->pcArray->Length;
   String^ strPasscode = gcnew String("");

   for(int i = 0; i < iLength; i++)
   {
      if(i < (iLength-1))
         strPasscode += String::Format("{0:X2},",pstOptions_->pcArray[i]);
      else
         strPasscode += String::Format("{0:X2}",pstOptions_->pcArray[i]);      
   }
   textPasskey->Text = strPasscode;

   // Client Encryption Key
   String^ strEncryptionKey = gcnew String("");

   for(int i = 0; i < ENCRYPTION_KEY_LENGTH; i++)
   {
	  if(i < (ENCRYPTION_KEY_LENGTH-1))
         strEncryptionKey += String::Format("{0:X2},",pstOptions_->aucEncryptionKey[i]);
      else
         strEncryptionKey += String::Format("{0:X2}",pstOptions_->aucEncryptionKey[i]);
   }
   textBox_EncryptionKey1->Text = strEncryptionKey->Substring(0, ((ENCRYPTION_KEY_LENGTH*3)/2)-1);
   textBox_EncryptionKey2->Text = strEncryptionKey->Substring(((ENCRYPTION_KEY_LENGTH*3)/2));


   return(TRUE);
}



////////////////////////////////////////////////////////////////////////////
// FromGui
//
// Takes values from the GUI and updates the options struct.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
BOOL Auth::FromGui(OptionsStruct^ pstOptions_)
{

   //************* 'General' TAB ************************

   // Use debug?
   pstOptions_->bDebug = checkDebug->Checked;
   
   
   // Baud rate (AP1 = 50000, everything else is 57600
   if( comboBaud->SelectedIndex == 0)
      pstOptions_->usBaudRate = 50000;
   else
      pstOptions_->usBaudRate = 57600;

   
   // USB Port      
   pstOptions_->ucUSBPort = comboPort->SelectedIndex;
 
   // Auto USB stick initialization (for black USB stick only)  
   pstOptions_->bAutoInit = checkAutoInit->Checked;
   
   
   
   //************* 'ANT Channel' TAB ************************
   
   
   // Channel Device ID
   try
   {
      pstOptions_->usChannelDeviceID = System::Convert::ToUInt16(textChannelDeviceID->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Channel ID \"" + textChannelDeviceID->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Channel ID", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }
   
   // Channel Device Type
   try
   {
      pstOptions_->ucChannelDeviceType = System::Convert::ToByte(textChannelDeviceType->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Channel Type \"" + textChannelDeviceType->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Channel Type", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }
   
   // Channel Transmission Type
   try
   {
      pstOptions_->ucChannelTransmissionType = System::Convert::ToByte(textChannelTransType->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Transmission Tyoe \"" + textChannelTransType->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Transmission Type", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }
   
   // Search Frequency
   try
   {
      pstOptions_->ucSearchFrequency = System::Convert::ToByte(textChannelSearchFreq->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Search Frequency \"" + textChannelSearchFreq->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Search Frequency", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;   
   }
      
   // Link Frequency
   try
   {
      pstOptions_->ucLinkFrequency = System::Convert::ToByte(textChannelLinkFreq->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Link Frequency \"" + textChannelLinkFreq->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Link Frequency", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;   
   }
   
   
   // Use Random Link Frequency?
   if(checkRandom->Checked)
      pstOptions_->ucLinkFrequency = 255;
   
   
   // ANT Channel message period  
   try 
   {
      pstOptions_->usMessagePeriod = System::Convert::ToUInt16(textChannelPeriodByPeriod->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Message Period \"" + textChannelPeriodByPeriod->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Message Period", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;   
   }

   // Proximity bin
   pstOptions_->ucProximitySearchBin = System::Convert::ToByte(numProximityBin->Value);   // validation built into numeric input control
   
   // Network key 1
   try 
   {
      pstOptions_->aucNetworkKey[0] = System::Convert::ToByte(textNetKey1->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey1->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   // Network key 2
   try 
   {
      pstOptions_->aucNetworkKey[1] = System::Convert::ToByte(textNetKey2->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey2->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   // Netwokr key 3
   try 
   {
      pstOptions_->aucNetworkKey[2] = System::Convert::ToByte(textNetKey3->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey3->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   // Network key 4
   try 
   {
      pstOptions_->aucNetworkKey[3] = System::Convert::ToByte(textNetKey4->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey4->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   // Network key 5
   try 
   {
      pstOptions_->aucNetworkKey[4] = System::Convert::ToByte(textNetKey5->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey5->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   // Network key 6
   try 
   {
      pstOptions_->aucNetworkKey[5] = System::Convert::ToByte(textNetKey6->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey6->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   // Network key 7
   try 
   {
      pstOptions_->aucNetworkKey[6] = System::Convert::ToByte(textNetKey7->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey7->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   // Network key 8
   try 
   {
      pstOptions_->aucNetworkKey[7] = System::Convert::ToByte(textNetKey8->Text, 16);
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Invalid Hex Code - \"" + textNetKey8->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Network Key", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   //************* 'Client' TAB ************************


   // Client Manufacturing ID   
   try
   {
      pstOptions_->usClientManufacturingID = System::Convert::ToUInt16(textClientManufID->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Client Manufacturing ID \"" + textClientManufID->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Client Manufacturing ID", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;   
   }
   
   // Client Device Type
   try
   {
      pstOptions_->usClientDeviceType = System::Convert::ToUInt16(textClientDeviceType->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Client Device Type \"" + textClientDeviceType->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Client Device Type", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;   
   }
   
   // Client Device ID
   try
   {
      pstOptions_->ulClientDeviceID = System::Convert::ToUInt32(textClientDeviceID->Text);
   }
   catch(...)
   {
      String^ strMessage = gcnew String ("Invalid Client Device ID \"" + textClientDeviceID->Text + "\"");
      MessageBox::Show(strMessage, "Input Error - Client Device ID", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;   
   }

   // ANT-FS Broadcast
   pstOptions_->bBroadcastSearch = checkBroadcastMode->Checked;

   // Ping
   pstOptions_->bEnablePing = checkPing->Checked;

   // Request undiscoverability
   pstOptions_->ucUndiscoverableTimeDuration = (UCHAR) ((USHORT) numUndiscoverableTime->Value/30.0);
   pstOptions_->ucUndiscoverableAppSpecificDuration = (UCHAR) (numUndiscoverableAppSpecific->Value);
   pstOptions_->ulBlockSize = System::Convert::ToUInt32(numBlockSize->Value);

   // Host friendly name
   String^ strFriendlyName = textFriendlyName->Text;
   int iLengthName = strFriendlyName->Length;
   array<UCHAR,1>^ tempName = System::Text::Encoding::ASCII->GetBytes(strFriendlyName);
   array<UCHAR^>^ aucFriendlyName = gcnew array<UCHAR^>(iLengthName);
   for(int i = 0; i < iLengthName; i++)
      aucFriendlyName[i] = System::Convert::ToByte(tempName[i]);

   pstOptions_->aucFriendlyName = aucFriendlyName;


   //************* 'Security' TAB ************************
   
   // Client Passkey
   System::Text::RegularExpressions::Regex^ regEx = gcnew System::Text::RegularExpressions::Regex(",");
   array<String^>^ stDataArray;
   stDataArray = regEx->Split(textPasskey->Text);
   int iLength = stDataArray->Length;
   array<UCHAR^>^ acPassCode = gcnew array<UCHAR^>(iLength);
   
   // Check for valid input
   for (int i = 0; i < iLength; i++)
   {
      try
      {
         acPassCode[i] = System::Convert::ToByte(stDataArray[i],16);
      }
      catch (...)
      {
         String^ strMessage = gcnew String ("Invalid Hex Code - \"" + stDataArray[i]+ "\"");
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
         return FALSE;
      }
   }

   pstOptions_->pcArray = acPassCode;


   // Client Encryption Key
   array<UCHAR>^ aucNewEncKey = gcnew array<UCHAR>(ENCRYPTION_KEY_LENGTH);

   int i;
   try
   {
      int halfKey = ENCRYPTION_KEY_LENGTH/2;

      //Get the first half of the key
      stDataArray = regEx->Split(textBox_EncryptionKey1->Text);
      if(stDataArray->Length != halfKey)
      {
		   MessageBox::Show("Encryption Key Field 1 must be 16 bytes in length (hex seperated by commas)", "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
         return FALSE;
      }

      for (i = 0; i < halfKey; i++)
      {
        aucNewEncKey[i] = System::Convert::ToByte(stDataArray[i],16);
      }


      //Get the second half of the key
      stDataArray = regEx->Split(textBox_EncryptionKey2->Text);
      if(stDataArray->Length != halfKey)
      {
		   MessageBox::Show("Encryption Key Field 1 must be 16 bytes in length (hex seperated by commas)", "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
         return FALSE;
      }

      for (i = 0; i < halfKey; i++)
      {
        aucNewEncKey[i + halfKey] = System::Convert::ToByte(stDataArray[i],16);
      }
   }
   catch (...)
   {
      String^ strMessage = gcnew String ("Encryption Key Invalid Hex Number - \"" + stDataArray[i]+ "\"");
      MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      return FALSE;
   }

   pstOptions_->aucEncryptionKey = aucNewEncKey;

   return(TRUE);

}


// Handle the channel period display and conversion between message rate and period
void Auth::UpdateHz()
{
   try
   {
      USHORT usPeriod32768ths = UInt16::Parse(textChannelPeriodByPeriod->Text);
      double dPeriodHz = Math::Round((double) 32768 / usPeriod32768ths, 3);
      String^ rounded = Convert::ToString(dPeriodHz);

       // Handle rounding to make sure it fits in display
       if (rounded->Length > 5 && !rounded->Equals("Infinity"))
       {
           if (rounded[4] == '.')
               rounded = rounded->Remove(4);
           else
               rounded = rounded->Remove(5);                    
       }
       textChannelPeriodByHz->Text = rounded;
   }
   catch(...)
   {
       textChannelPeriodByHz->Text = "error";
   }   
}
        
void Auth::UpdatePeriod()
{
   try
   {
      double dPeriodHz = Double::Parse(textChannelPeriodByHz->Text);
       if (dPeriodHz < 0.5 || dPeriodHz > 32768)
           throw gcnew Exception("Period out of range");
       USHORT usPeriod32768ths = (USHORT) Math::Round(32768 / dPeriodHz);
       if (usPeriod32768ths == 0)    // we only get a zero if it is on a rollover past 65535
           usPeriod32768ths = 65535;
       textChannelPeriodByPeriod->Text = Convert::ToString(usPeriod32768ths);
   }
   catch (...)
   {
       textChannelPeriodByPeriod->Text = "error";
   }

} 

void Auth::ShowLogsDirectory()
{
   System::Diagnostics::Process::Start("explorer.exe", pstMyOptionsStruct->strPath);
}

void Auth::ComboDebugPathChanged()
{
   if(this->comboPath->SelectedIndex == 0)
      pstMyOptionsStruct->SetDefaultPath();
   else
   {
      if(selectFolderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
      {
         pstMyOptionsStruct->strPath = selectFolderDialog->SelectedPath;
      }
   }
}

void Auth::NumBoxUndiscoverableTimeChanged()
{
   USHORT currentValue = (USHORT) this->numUndiscoverableTime->Value;
   if(currentValue % 30 != 0)  // Only accept multiples of 30
   {
      this->numUndiscoverableTime->Value = 30 * (UCHAR) (currentValue/30.0);  // Truncate value
   }
}










