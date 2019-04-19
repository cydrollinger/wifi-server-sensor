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
#include "stdafx.h"
#include "Form1.h"
#include "antfs_host.hpp"
#include "antfsmessage.h"
#include "version.h"

using namespace ANTFSDemo;
using namespace System::Runtime::InteropServices;

////////////////////////////////////////////////////////////////////////////
// Form1
//
// Class Contructor. 
// Creates threads and the ANTFSHost class. Loads session options from 
// config.txt file. Initializes all relevant member variables.  
//
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
Form1::Form1(void)
{
   InitializeComponent();

   this->comboDirectory->Items->Add(L"Custom");

   //Initialize condition variable and mutex
   DSIThread_CondInit(&stCondResponseThreadExit);
   DSIThread_MutexInit(&stMutexResponseThreadExit);

   // Try to load the configuration file. If it is missing, 
   // default options are used.
   strOptions =  gcnew OptionsStruct;
   
   strPath = System::IO::Path::Combine(Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData), "ANTFS_PC_Host\\config.txt");
   strOptions->LoadConfigFile(strPath);

   pstDirectory = NULL;
   pucUploadData = NULL;
   pclThread = new ThreadCreater(this);
   pclANTFSHost = new ANTFSHost();
   dateReference = gcnew DateTime(1989,12,31,0,0,0);   // Dec 31, 1989, 00:00
   this->SetUIState();
   
   this->AppendText(richTextBox1, gcnew String("SW Version: " + APP_SW_VER + "\n"));
   
}

////////////////////////////////////////////////////////////////////////////
// ~Form1
//
// Class Destructor. 
// Saves current session options to "config.txt" file. Destroys threads and 
// deletes unmanaged resources.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
Form1::~Form1()
{
	this->buttonClose_Click(nullptr, nullptr);  //make sure that it is okay for this to happen when close button has already been clicked

   try
   {
       // Create app directory if it does not exist
      if (!System::IO::Directory::Exists(System::IO::Path::GetDirectoryName(strPath)))
         System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(strPath));
       
       // Save config
       strOptions->SaveConfigFile(strPath);
   }
   catch (...)
   {
      MessageBox::Show("Unable to save configuration", "Error");
   }

	//Destroy condition variable and mutex
	DSIThread_MutexDestroy(&stMutexResponseThreadExit);
	DSIThread_CondDestroy(&stCondResponseThreadExit);

	if(pclANTFSHost)
	{
		delete pclANTFSHost;
		pclANTFSHost = NULL;
	}
	
	if(pclThread)
	{
		delete pclThread;
		pclThread = NULL;
	}

	if (components)
	{
		delete components;
		components = nullptr;
	}
}


////////////////////////////////////////////////////////////////////////////
// OpenClick
//
// Called when user presses the 'Open' button. Initializes the ANTFSHost class
// and response threads. Tries to connect to USB stick. ANT-FS session is not
// yet started (host does not search).
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::OpenClick()
{
	UCHAR aucLibVersion[255];        // Version of ANT-FS Library
   char* pcPath = (char*) NULL;     // debug path


   // Setup the response thread.
	hResponseThread = pclThread->NewResponseThread();
   
	if(hResponseThread == NULL)
	{
		this->AppendText(richTextBox1, "Error: Unable to initialize.\n");
		CloseClick();
		return;
	}

   // Get and display the ANT-FS Library version
	pclANTFSHost->GetVersion(aucLibVersion, sizeof(aucLibVersion));

	this->AppendText(richTextBox1, "Library version:  ");
	this->AppendText(richTextBox1, (char*)aucLibVersion);
	this->AppendText(richTextBox1, "\n");

	#if defined(DEBUG_FILE)
   try
   {
     if(strOptions->bDebug == TRUE)
     {
        // Create log directory if it does not exist
        if (!System::IO::Directory::Exists(strOptions->strPath))
           System::IO::Directory::CreateDirectory(strOptions->strPath);
        pcPath = (char*) Marshal::StringToHGlobalAnsi(strOptions->strPath + "\\").ToPointer();
     }
   }
   catch (...) {/* Save to working directory */}

   pclANTFSHost->SetDebug(strOptions->bDebug, pcPath);

   if(pcPath != (char*) NULL)
      Marshal::FreeHGlobal((IntPtr) pcPath);
	#endif


   BOOL bInitSuccess = FALSE;
   
   
   //Initialize the library and connect to the USB stick
   if(strOptions->bAutoInit)
   {
      bInitSuccess = pclANTFSHost->Init();
   }
   else
   {
      bInitSuccess = pclANTFSHost->Init(strOptions->ucUSBPort,strOptions->usBaudRate);
   }

	if(!bInitSuccess)  
	{
		this->AppendText(richTextBox1, "Error: Unable to initialize.\n");
		MessageBox::Show("Unable to initialize", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		this->buttonClose_Click(nullptr, nullptr);
		return;
	}



}

////////////////////////////////////////////////////////////////////////////
// CloseClick
//
// Called when user presses the 'Close' button. Kills response thread,
// and disconnectes from the library (and subsequently the USB stick).
// Deletes unmanaged resources. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::CloseClick()
{
	//Exit Response Thread
	if(hResponseThread)
	{
		DSIThread_MutexLock(&stMutexResponseThreadExit);
			bResponseThreadExit = TRUE;
			if (DSIThread_CondTimedWait(&stCondResponseThreadExit, &stMutexResponseThreadExit, 3000) != DSI_THREAD_ENONE)  //will timeout if bResponseThreadExit was already true
         {
             DSIThread_DestroyThread(hResponseThread);
         }
         DSIThread_ReleaseThreadID(hResponseThread);
         hResponseThread = (DSI_THREAD_ID)NULL;
			
		DSIThread_MutexUnlock(&stMutexResponseThreadExit);
	}
	

	//Close library
	pclANTFSHost->Close();
	
	if(pstDirectory)
	{
		delete[] pstDirectory;
		pstDirectory = NULL;
	}

	if(pucUploadData)
	{
		delete[] pucUploadData;
		pucUploadData = NULL;
	}


}

////////////////////////////////////////////////////////////////////////////
// CancelClick
//
// Called when user presses the 'Cancel' button. This will immediately 
// disconnect the host from the Client and blacklist the client for 
// 60 seconds. Can be called while session is in any layer. Client and Host 
// will return to LINK layer.  
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::CancelClick()
{
	if(pclANTFSHost->GetStatus() == ANTFS_STATE_CONNECTED)	//ignore
	{
		ANTFS_RETURN eReturn = pclANTFSHost->Disconnect(60);
		switch(eReturn)
		{
		case ANTFS_RETURN_PASS:
			this->AppendText(richTextBox1, "Disconnecting...\n");
			break;
		
		case ANTFS_RETURN_BUSY:
			this->AppendText(richTextBox1, "Error: Library is busy with another request\n");
			break;

		case ANTFS_RETURN_FAIL:
			this->AppendText(richTextBox1, "Error: Ignoring failed\n");
			break;
		}
	}
	else	//cancel
	{
		pclANTFSHost->Cancel();
	}

	
}


////////////////////////////////////////////////////////////////////////////
// SearchClick
//
// Called when user presses the 'Search' button. This effectively starts the 
// ANT-FS session. Host will open ANT channel and begin looking for a Client.
// Once the Client is found, the host will send a link command to connect to 
// the device. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::SearchClick()
{
	ANTFS_RETURN eReturn;
	ANTFS_DEVICE_PARAMETERS stSearchMask;
	ANTFS_DEVICE_PARAMETERS stSearchParameters;		
	
	//Get search device parameters
   ULONG ulDeviceID = strOptions->ulClientDeviceID;
   USHORT usManufacturerID = strOptions->usClientManufacturingID;
   USHORT usDeviceType = strOptions->usClientDeviceType;

	//Search Mask
	stSearchMask.ulDeviceID = ulDeviceID ? 0xFFFFFFFF : 0;
	stSearchMask.usManufacturerID = usManufacturerID ? 0xFFFF : 0;
	stSearchMask.usDeviceType = usDeviceType ? 0xFFFF : 0;
	stSearchMask.ucAuthenticationType = 0;
	stSearchMask.ucStatusByte1 = 0;//ANTFS_STATUS1_DEFAULT_SEARCH_MASK;
	stSearchMask.ucStatusByte2 = 0;//ANTFS_STATUS2_DEFAULT_SEARCH_MASK;

	//Search Parameters
	stSearchParameters.ulDeviceID = ulDeviceID;
	stSearchParameters.usManufacturerID = usManufacturerID;
	stSearchParameters.usDeviceType = usDeviceType;
	stSearchParameters.ucAuthenticationType = 0;
	stSearchParameters.ucStatusByte1 = 0;
	stSearchParameters.ucStatusByte2 = 0;

   // Configure ping
   pclANTFSHost->EnablePing(strOptions->bEnablePing);
   
   // Set channel options
   pclANTFSHost->SetChannelID(strOptions->ucChannelDeviceType, strOptions->ucChannelTransmissionType);
   pclANTFSHost->SetMessagePeriod(strOptions->usMessagePeriod);
   pclANTFSHost->SetProximitySearch(strOptions->ucProximitySearchBin);  // The library will ignore this if using a part that does not support proximity search

   UCHAR aucNetworkkey[8];

   for (int i = 0; i < 8; i++)
      aucNetworkkey[i] = *strOptions->aucNetworkKey[i];

   pclANTFSHost->SetNetworkkey(aucNetworkkey);   

	pclANTFSHost->ClearSearchDeviceList();
	if(pclANTFSHost->AddSearchDevice(&stSearchMask, &stSearchParameters) == 0)
	{
		this->AppendText(richTextBox1, "Error: Could not add search device.\n");
		return;
	}

   eReturn = pclANTFSHost->SearchForDevice(strOptions->ucSearchFrequency, strOptions->ucLinkFrequency, strOptions->usChannelDeviceID, strOptions->bBroadcastSearch);

	switch(eReturn)
	{
	case ANTFS_RETURN_PASS:
		this->AppendText(richTextBox1, "Searching...\n");
		break;
	case ANTFS_RETURN_BUSY:
		this->AppendText(richTextBox1, "Error: Busy with another request.\n");
		break;
	case ANTFS_RETURN_FAIL:
		this->AppendText(richTextBox1, "Error: Search button pushed out of sequence.\n");
		break;
	}


}

////////////////////////////////////////////////////////////////////////////
// PairClick
//
// Called when user presses the 'Pair' button. This may only be done when the
// Client and Host are in Auth state. This action will initiate a "Pairing"
// method of Authentication with the Client. If pairing was suscccessful, 
// the Client will send a passkey to the host and both will move to the 
// transport state.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::PairClick()
{
	ANTFS_RETURN eReturn;
   UCHAR ucFriendlyNameLength = FRIENDLY_NAME_MAX_LENGTH;

   ucFriendlyNameLength = 0;

	this->AppendText(richTextBox1, "Pairing...\n");
   ucDevicePasswordLength = sizeof(aucDevicePassword);
   if(strOptions->aucFriendlyName)
   {
      ucFriendlyNameLength = strOptions->aucFriendlyName->Length;
      for(int i = 0; i < ucFriendlyNameLength; i++)
         aucFriendlyName[i] = *strOptions->aucFriendlyName[i];
   }

	eReturn = pclANTFSHost->Authenticate(AUTH_COMMAND_PAIR, aucFriendlyName, ucFriendlyNameLength, aucDevicePassword, &ucDevicePasswordLength, 9999);
	
   switch(eReturn)
	{
	   case ANTFS_RETURN_PASS:
      {

		   break;
	   
      }
      case ANTFS_RETURN_BUSY:
      {
         this->AppendText(richTextBox1, "Error: Busy with another request.\n");
		   break;
      }
      case ANTFS_RETURN_FAIL:
      {
         this->AppendText(richTextBox1, "Error: Pair button pushed out of sequence.\n");
		   break;
	
      }
   }
}


////////////////////////////////////////////////////////////////////////////
// AuthenticateClick
//
// Called when user presses the 'Passkey' button. This may only be done when the
// Client and Host are in Auth state. This action will initiate a "Passkey"
// method of Authentication with the Client. The passkey set in the Options
// dialog is used, unless a new passkey was obtained during a pairing operation.
// If authentication is successful, the host and the client will move to the 
// transport state.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::AuthenticateClick()
{
	this->AppendText(richTextBox1, (char*)aucDevicePassword);
	this->AppendText(richTextBox1, "\nAuthenticating...\n");

   // Get the password from Options struct
   ucDevicePasswordLength = strOptions->pcArray->Length;


   for (int i = 0; i < ucDevicePasswordLength; i++)
      aucDevicePassword[i] = *strOptions->pcArray[i];

	ANTFS_RETURN eReturn = pclANTFSHost->Authenticate(AUTH_COMMAND_PASSKEY, aucDevicePassword, ucDevicePasswordLength, NULL, NULL, 9999);
	switch(eReturn)
	{
	case ANTFS_RETURN_PASS:
		break;
	case ANTFS_RETURN_BUSY:
		this->AppendText(richTextBox1, "Error: Busy with another request.\n");
		break;
	case ANTFS_RETURN_FAIL:
		this->AppendText(richTextBox1, "Error: Authentication failed\n");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////
// SkipAuthClick
//
// Called when user presses the 'Pass Thru' button. This may only be done when the
// Client and Host are in Auth state. This action will initiate a "Pass Through"
// method of Authentication with the Client. If the Client supports this 
// method of authentication then the client and the host will move to the transport
// state.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::SkipAuthClick()
{
   // Switches directly into Transport, for testing
   this->AppendText(richTextBox1, "Switching to transport layer.\n");

   ANTFS_RETURN eReturn = pclANTFSHost->Authenticate(AUTH_COMMAND_GOTO_TRANSPORT, NULL, 0, NULL, NULL, 9999);
   switch(eReturn)
   {
   case ANTFS_RETURN_PASS:
	   break;
   case ANTFS_RETURN_BUSY:
	   this->AppendText(richTextBox1, "Error: Busy with another request.\n");
	   break;
   case ANTFS_RETURN_FAIL:
	   this->AppendText(richTextBox1, "Error: Could not switch to transport layer\n");
	   break;
   }

}



////////////////////////////////////////////////////////////////////////////
// DirectoryClick
//
// Called when user presses the 'Directory' button. This may only be done when the
// Client and Host are in Transport state. This action will attempt to download
// a file from the client at index 0. The contents of this file will be displayed in 
// the Directory list box on the GUI. This button should only be called for client 
// devices that support a directory structure, as defined in the specification.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::DirectoryClick()
{
	bMyDownloadDir = TRUE;
	ANTFS_RETURN eReturn = pclANTFSHost->Download(0, 0, MAX_DATA_SIZE); // Directory size not yet known
	switch(eReturn)
	{
	case ANTFS_RETURN_PASS:
		break;
	case ANTFS_RETURN_BUSY:
		this->AppendText(richTextBox1, "Error: Busy with another request.\n");
		bMyDownloadDir = FALSE;
		break;
	case ANTFS_RETURN_FAIL:
		this->AppendText(richTextBox1, "Error: Directory button pushed out of sequence\n");
		bMyDownloadDir = FALSE;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////
// DownloadClick
//
// Called when user presses the 'Download' button, or double clicks an item 
// in the directory list box. This may only be done when the Client and Host 
// are in Transport state. This action will attempt to download the file from 
// the client that was specified in the directory list box. If no file is 
// specified, the file at index location 0 is downloaded. The contents of this
// file will be stored in the file specifed in the Output File text box. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::DownloadClick(int iIndex)
{

   ULONG ulFileSize = MAX_DATA_SIZE;
   USHORT usFileIndex = 0xFFFF;

   if(iIndex == -1)
   {
      iIndex = listDirectory->SelectedIndex;

      if(iIndex >= 0xFFFF || iIndex < 0)
	   {
		   System::Windows::Forms::MessageBox::Show("Please Select a file.","File not selected.",MessageBoxButtons::OK,MessageBoxIcon::Exclamation );
		   return;
	   }

      if(pstDirectory == NULL)
      {
         System::Windows::Forms::MessageBox::Show("Please request directory first.","File not selected.",MessageBoxButtons::OK,MessageBoxIcon::Exclamation );
		   return;
      }

      ulFileSize = pstDirectory[iIndex].ulFileSize;
      usFileIndex = pstDirectory[iIndex].usFileIndex;


   }
   else
   {
      usFileIndex = iIndex;      
   }



   ANTFS_RETURN eReturn = ANTFS_RETURN_FAIL;

   try{
	   this->stOutputFileInfo = gcnew System::IO::FileInfo(this->textBoxOutputFile->Text); //Make a new output file info object
   }catch(...){
      MessageBox::Show(this, "The file path you have entered is empty or contains errors.\nChange the path to an valid directory and filename before attempting to download a file.",
								"File Download - Invalid File Path",MessageBoxButtons::OK, MessageBoxIcon::Error);
      return;
   }
	if(!this->stOutputFileInfo->Directory->Exists)
	{
		MessageBox::Show(this, "The file path you have entered does not exist.\nChange the path to an existing directory before attempting to download a file.",
								"File Download - Directory Does Not Exist",MessageBoxButtons::OK, MessageBoxIcon::Error);
      return;
	}
	else
	{
		if(iIndex >= 0xFFFF)
		{
			bMyDownloadDir = FALSE;
			eReturn = pclANTFSHost->Download(0, 0, MAX_DATA_SIZE);   // Directory
		}
		else
		{
			//Download the file
			bMyDownloadDir = FALSE;
         eReturn = pclANTFSHost->Download((USHORT)usFileIndex, 0, ulFileSize, strOptions->ulBlockSize);
		}
	}
   switch(eReturn)
   {
	case ANTFS_RETURN_PASS:
		usLastDownloadIndex = (USHORT)iIndex;
		break;
	case ANTFS_RETURN_BUSY:
		this->AppendText(richTextBox1, "Error: Busy with another request.\n");
		break;
	case ANTFS_RETURN_FAIL:
		this->AppendText(richTextBox1, "Error: Download Button Pushed out of sequence\n");
		break;
	}


}



////////////////////////////////////////////////////////////////////////////
// UploadClick
//
// Called when user presses the 'Upload' button. This may only be done when 
// the Client and Host are in Transport state. This action will attempt to 
// upload the file specified in the Input File text box to the Client, using 
// the index specified in the directory list box. Client must support uploads.  
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::UploadClick(int iIndex_)
{

   USHORT usFileIndex;
   ULONG ulBlockSize = 0;
   BOOL bDoCrypto = false;
   		
   if(iIndex_ == -1)
   {
	   iIndex_ = listDirectory->SelectedIndex;
	   if(iIndex_ >= 0xFFFF)
	   {
		   System::Windows::Forms::MessageBox::Show("Please Select a file.","File not selcted.",MessageBoxButtons::OK,MessageBoxIcon::Exclamation );
		   return;
	   }
      usFileIndex = pstDirectory[iIndex_].usFileIndex;
    	if(pstDirectory[iIndex_].ucGeneralFlags & ANTFS_GENERAL_FLAG_CRYPTO)
         bDoCrypto = true;

   }
   else
   {
      usFileIndex = iIndex_;
   }

   char szFileName[4096];
	ULONG ulUploadSize;
	size_t readsize;
	FILE* fpUploadFile;

	SNPRINTF(szFileName, sizeof(szFileName), "%s", this->textBoxInputFile->Text);
	fpUploadFile = FOPEN(szFileName, "r + b");

	if (!fpUploadFile)
	{
		openFileDialog1->ShowDialog();
		SNPRINTF(szFileName, sizeof(szFileName), "%s", this->textBoxInputFile->Text);
		fpUploadFile = FOPEN(szFileName, "r + b");
		if(!fpUploadFile)
		{
			this->AppendText(richTextBox1, "Error opening input file\n");
			return;
		}
	}

	//Get upload size.
	fseek(fpUploadFile, 0, SEEK_END);
	ulUploadSize = ftell(fpUploadFile);
	rewind(fpUploadFile);

	if(pucUploadData)
	{
		delete[] pucUploadData;
		pucUploadData = NULL;
	}
	pucUploadData = new UCHAR[ulUploadSize + 32];   //!! watch out for memory leak, padded extra bytes here for appended CRC and crypto IV
	readsize = fread((void *)&pucUploadData[0], 1, ulUploadSize, fpUploadFile);
	
	fclose(fpUploadFile);

	if (readsize != ulUploadSize)
	{
		delete[] pucUploadData;
		pucUploadData = NULL;
		this->AppendText(richTextBox1, "Error reading input file\n");
		return;
	}

	//Note: This encrypts the data assuming this is the entire file, generating a new IV. Non-zero offsets or any partial write will be messed up - they need to read the existing IV
	//Encrypt the file if enabled and the file is encrypted
	if(bDoCrypto)
	{
		array<UCHAR>^ encryptionBuf = gcnew array<UCHAR>(ulUploadSize);
		for(ULONG i=0; i<ulUploadSize; ++i)	//Copy unmanaged array to managed
			encryptionBuf[i] = pucUploadData[i];
		encryptionBuf = AES_CTR::AES_CTR::FS_encrypt_bytes(encryptionBuf, strOptions->aucEncryptionKey);
      ulUploadSize += 16; //Add the crypto vector size to the upload
		for(ULONG i=0; i<ulUploadSize; ++i)	//Copy managed result back to unmanaged
			pucUploadData[i] = encryptionBuf[i];      
	}

   
   
   this->AppendText(richTextBox1, gcnew String("Uploading to index: " + usFileIndex.ToString() + "\n"));

   ANTFS_RETURN eReturn = pclANTFSHost->Upload(usFileIndex, 0, ulUploadSize, pucUploadData, TRUE, strOptions->ulBlockSize);
	switch(eReturn)
	{
	case ANTFS_RETURN_PASS:
		this->AppendText(richTextBox1, "Uploading...\n");
		break;
	
	case ANTFS_RETURN_BUSY:
		this->AppendText(richTextBox1, "Error: Library is busy with another request\n");
		break;

	case ANTFS_RETURN_FAIL:
		this->AppendText(richTextBox1, "Error: Upload Button Pushed out of sequence\n");
		break;

	}



}

////////////////////////////////////////////////////////////////////////////
// EraseClick
//
// Called when user presses the 'Erase' button. This may only be done when 
// the Client and Host are in Transport state. This action will attempt to 
// erase the file specified in the directory list box.  
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::EraseClick()
{
	USHORT usIndex = listDirectory->SelectedIndex;
	if(usIndex >= 0xFFFF)
	{
		System::Windows::Forms::MessageBox::Show("Please Select a file.","File not selcted.",MessageBoxButtons::OK,MessageBoxIcon::Exclamation );
		return;
	}

	ANTFS_RETURN eReturn = pclANTFSHost->EraseData(pstDirectory[usIndex].usFileIndex);
	switch(eReturn)
	{
	case ANTFS_RETURN_PASS:
		this->AppendText(richTextBox1, "Erasing...\n");
		break;
	
	case ANTFS_RETURN_BUSY:
		this->AppendText(richTextBox1, "Error: Library is busy with another request\n");
		break;

	case ANTFS_RETURN_FAIL:
		this->AppendText(richTextBox1, "Error: Erase failed\n");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////
// DisconnectClick
//
// Called when user presses the 'Disconnect' button. This may only be done when 
// the Client and Host are in ant ANT-FS state. This action will disconnect 
// the host from the client, putting both into the LINK state. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::DisconnectClick()
{
   ANTFS_RETURN eReturn = pclANTFSHost->Disconnect(0, 0, strOptions->ucUndiscoverableTimeDuration, strOptions->ucUndiscoverableAppSpecificDuration);
   switch(eReturn)
   {
   case ANTFS_RETURN_PASS:
	   this->AppendText(richTextBox1, "Disconnecting...\n");
	   break;

   case ANTFS_RETURN_BUSY:
	   this->AppendText(richTextBox1, "Error: Library is busy with another request\n");
	   break;

   case ANTFS_RETURN_FAIL:
	   this->AppendText(richTextBox1, "Error: Disconnect failed\n");
	   break;
	}
}


////////////////////////////////////////////////////////////////////////////
// ReturnToBroadcastClick
//
// Called when user presses the 'Ret Bcast' button. This may only be done when 
// the Client and Host are in ant ANT-FS state. This action will disconnect 
// the host from the client, putting the client back in broadcast mode, if
// supported
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::ReturnToBroadcastClick()
{
   ANTFS_RETURN eReturn = pclANTFSHost->Disconnect(0, 1, strOptions->ucUndiscoverableTimeDuration, strOptions->ucUndiscoverableAppSpecificDuration);
   switch(eReturn)
   {
   case ANTFS_RETURN_PASS:
	   this->AppendText(richTextBox1, "Disconnecting - Return to broadcast...\n");
	   break;

   case ANTFS_RETURN_BUSY:
	   this->AppendText(richTextBox1, "Error: Library is busy with another request\n");
	   break;

   case ANTFS_RETURN_FAIL:
	   this->AppendText(richTextBox1, "Error: Disconnect failed\n");
	   break;
	}
}


////////////////////////////////////////////////////////////////////////////
// SetUIState
//
// Updates the GUI based on the current ANT-FS state.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::SetUIState()
{
	//Make sure that we are calling the GUI from the same thread it was created with
	if(this->buttonOpen->InvokeRequired)
	{    
		MethodInvoker^ phSetUIState = gcnew MethodInvoker(this, &Form1::SetUIState);
		this->BeginInvoke(phSetUIState);
		return;
	}
	
	ANTFS_STATE eState = pclANTFSHost->GetStatus();
	switch (eState)
	{
	case ANTFS_STATE_IDLE:
		this->buttonOpen->Enabled = TRUE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = FALSE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
		this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Idle";
		break;

	case ANTFS_STATE_IDLE_POLLING_USB:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = TRUE;
		this->buttonCancel->Enabled = FALSE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
		this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Waiting on USB";
		break;

	case ANTFS_STATE_OPEN:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = TRUE;
		this->buttonCancel->Enabled = FALSE;
		this->buttonSearch->Enabled = TRUE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = TRUE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Open";
		break;

	case ANTFS_STATE_DISCONNECTING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = FALSE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = TRUE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Disconnecting...";
		break;

	case ANTFS_STATE_SEARCHING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = TRUE;
		this->buttonCancel->Enabled = TRUE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Searching...";
		break;

	case ANTFS_STATE_CONNECTED:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = TRUE;
		this->buttonCancel->Enabled = TRUE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = TRUE;
		this->buttonAuthenticate->Enabled = TRUE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = TRUE;
		this->buttonErase->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = TRUE;
      this->buttonReturnBroadcast->Enabled = TRUE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Connected";
		break;

	case ANTFS_STATE_AUTHENTICATING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = TRUE;
		this->buttonCancel->Enabled = FALSE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Authenticating...";
		break;

	case ANTFS_STATE_TRANSPORT:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = FALSE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = TRUE;
		this->btnDirectory->Enabled = TRUE;
		this->listDirectory->Enabled = TRUE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = TRUE;
		this->buttonDisconnect->Enabled = TRUE;
		this->buttonErase->Enabled = TRUE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = TRUE;
      this->buttonSetTime->Enabled = TRUE;
		this->labelStatus->Text = "Status:  Transport";
		break;

	case ANTFS_STATE_DOWNLOADING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = TRUE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Downloading...";
		break;

	case ANTFS_STATE_UPLOADING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = TRUE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Uploading...";
		break;

	case ANTFS_STATE_ERASING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = TRUE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = TRUE;
		this->btnDirectory->Enabled = TRUE;
		this->listDirectory->Enabled = TRUE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = TRUE;
		this->buttonErase->Enabled = TRUE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Erasing...";
		break;

	case ANTFS_STATE_SENDING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = TRUE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Sending...";
		break;

	case ANTFS_STATE_RECEIVING:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = FALSE;
		this->buttonCancel->Enabled = TRUE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Receiving...";
		break;

	default:
		this->buttonOpen->Enabled = FALSE;
		this->buttonClose->Enabled = TRUE;
		this->buttonCancel->Enabled = FALSE;
		this->buttonSearch->Enabled = FALSE;
		this->buttonDownload->Enabled = FALSE;
		this->btnDirectory->Enabled = FALSE;
		this->listDirectory->Enabled = FALSE;
		this->buttonPair->Enabled = FALSE;
		this->buttonAuthenticate->Enabled = FALSE;
		this->buttonUpload->Enabled = FALSE;
		this->buttonDisconnect->Enabled = FALSE;
		this->buttonErase->Enabled = FALSE;
		this->buttonSkipAuth->Enabled = FALSE;
      this->buttonReturnBroadcast->Enabled = FALSE;
      this->buttonSetTime->Enabled = FALSE;
		this->labelStatus->Text = "Status:  Unknown";
		break;
	}
}




////////////////////////////////////////////////////////////////////////////
// ResponseThread
//
// This function runs the response thread which is used to handle responses 
// from the ANT-FS library.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::ResponseThread()
{   
	bResponseThreadExit = FALSE;
   ANTFS_RESPONSE eResponse;
	while(!bResponseThreadExit)
	{

      eResponse = pclANTFSHost->WaitForResponse(999);
      
		switch(eResponse)
		{
		case ANTFS_RESPONSE_OPEN_PASS:
			this->AppendText(richTextBox1, "Open Success\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_SERIAL_FAIL:
			this->AppendText(richTextBox1, "Error: Serial Failure\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_CONNECT_PASS:
      {	
         USHORT usFoundDeviceNumber = 0;
			
			if(!pclANTFSHost->GetFoundDeviceParameters(&stDeviceParameters, aucFriendlyName, &ucFriendlyNameLength)) 
			{
				this->AppendText(richTextBox1, "Error: Could not get device parameters.\n");
				return;
			}

         if(!pclANTFSHost->GetFoundDeviceChannelID(&usFoundDeviceNumber))
         {
            this->AppendText(richTextBox1, "Error: Could not get device ANT Channel ID.\n");
            return;
         }

			this->AppendText(richTextBox1, "Found remote device.\n");
         if(ucFriendlyNameLength != 0)
         {
            this->AppendText(richTextBox1, "   Friendly Name:  ");
            this->AppendText(richTextBox1, (char*) aucFriendlyName);
            this->AppendText(richTextBox1, "\n");
         }
			this->AppendText(richTextBox1, "   ANT Device Number:  ");
			this->AppendText(richTextBox1, usFoundDeviceNumber.ToString());
			this->AppendText(richTextBox1, "\n");

			this->AppendText(richTextBox1, "   Device ID:  ");
         
			this->AppendText(richTextBox1, stDeviceParameters.ulDeviceID.ToString());

			this->AppendText(richTextBox1, "   Manufacturer ID:  ");
			this->AppendText(richTextBox1, stDeviceParameters.usManufacturerID.ToString());
			this->AppendText(richTextBox1, "\n");

			this->AppendText(richTextBox1, "   Device Type: ");
			this->AppendText(richTextBox1, stDeviceParameters.usDeviceType.ToString());
			this->AppendText(richTextBox1, "\n");

			this->AppendText(richTextBox1, "   Authentication Type: ");
			this->AppendText(richTextBox1, stDeviceParameters.ucAuthenticationType.ToString());
			this->AppendText(richTextBox1, "\n");

			if (stDeviceParameters.ucStatusByte1 & ANTFS_STATUS1_DATA_AVAILABLE_BIT)
				this->AppendText(richTextBox1, "   Device has new data.\n");
			else
				this->AppendText(richTextBox1, "   Device does not have new data.\n");

			if (stDeviceParameters.ucStatusByte1 & ANTFS_STATUS1_PAIRING_ENABLED_BIT)
				this->AppendText(richTextBox1, "   Device is in Pairing Mode.\n");
			else
				this->AppendText(richTextBox1, "   Device is not in Pairing Mode.\n");

			if (stDeviceParameters.ucStatusByte1 & ANTFS_STATUS1_UPLOAD_ENABLED_BIT)
				this->AppendText(richTextBox1, "   Device has upload enabled.\n");
			else
				this->AppendText(richTextBox1, "   Device does not have upload enabled.\n");
		
			this->SetUIState();
			break;
      }

		case ANTFS_RESPONSE_DISCONNECT_PASS:
			this->AppendText(richTextBox1, "Disconnected Successfully\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_CONNECTION_LOST:
			this->AppendText(richTextBox1, "Lost Connection\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_AUTHENTICATE_NA:
			this->AppendText(richTextBox1, "Authenticate NA\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_AUTHENTICATE_PASS:
         UpdatePasskey();
			this->AppendText(richTextBox1, "Authenticate Passed!\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_AUTHENTICATE_REJECT:
			this->AppendText(richTextBox1, "Error: Authenticate Rejected.\n");
			this->SetUIState();
		   this->BeginInvoke(gcnew EventHandler(this, &Form1::buttonDisconnect_Click), this, nullptr); // go to LINK
			break;

		case ANTFS_RESPONSE_AUTHENTICATE_FAIL:
			this->AppendText(richTextBox1, "Error: Authenticate Failed.\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_DOWNLOAD_PASS:
			this->AppendText(richTextBox1, "Getting downloaded data...\n");
			if(bMyDownloadDir)
			{
				Directory();
				this->AppendText(richTextBox1, "Directory Success\n");
			}
			else
			{
				Download();
				this->AppendText(richTextBox1, "Download Passed!\n");
			}
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_DOWNLOAD_REJECT:
			this->AppendText(richTextBox1, "Error: Download Rejected.\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_DOWNLOAD_FAIL:
			this->AppendText(richTextBox1, "Error: Download Failed\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_UPLOAD_PASS:
			this->AppendText(richTextBox1, "Upload Complete!\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_UPLOAD_REJECT:
			this->AppendText(richTextBox1, "Error: Upload Rejected\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_UPLOAD_FAIL:
			this->AppendText(richTextBox1, "Error: Upload Failed\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_ERASE_PASS:
			this->AppendText(richTextBox1, "Erase Complete!\n");
			this->SetUIState();
			this->DirectoryClick();
			break;

		case ANTFS_RESPONSE_ERASE_FAIL:
			this->AppendText(richTextBox1, "Error: Erase Failed\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_MANUAL_TRANSFER_PASS:
			this->AppendText(richTextBox1, "Manual Transfer Complete!\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_MANUAL_TRANSFER_TRANSMIT_FAIL:
			this->AppendText(richTextBox1, "Error: Manual Transmit Failed\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_MANUAL_TRANSFER_RESPONSE_FAIL:
			this->AppendText(richTextBox1, "Error: Manual Response Failed\n");
			this->SetUIState();
			break;

		case ANTFS_RESPONSE_CANCEL_DONE:
			this->AppendText(richTextBox1, "Event Canceled\n");
			this->SetUIState();
			break;
         
         
      case ANTFS_RESPONSE_DOWNLOAD_INVALID_INDEX:
      {
			this->AppendText(richTextBox1, "Download Response: Invalid Index\n");
			this->SetUIState();
         break;
      }
      case ANTFS_RESPONSE_DOWNLOAD_FILE_NOT_READABLE:
      {
			this->AppendText(richTextBox1, "Download Response: File not Readable\n");
			this->SetUIState();
         break;
      }
      case ANTFS_RESPONSE_DOWNLOAD_NOT_READY:
      {
			this->AppendText(richTextBox1, "Download Response: Not Ready\n");
			this->SetUIState();
         break;
      }
      case ANTFS_RESPONSE_UPLOAD_INVALID_INDEX:
      {
			this->AppendText(richTextBox1, "Upload Response: Invalid Index\n");
			this->SetUIState();
         break;
      }
      case ANTFS_RESPONSE_UPLOAD_FILE_NOT_WRITEABLE:
      {
			this->AppendText(richTextBox1, "Upload Response: File not Writeable\n");
			this->SetUIState();
         break;
      }
      case ANTFS_RESPONSE_UPLOAD_INSUFFICIENT_SPACE:
      {
			this->AppendText(richTextBox1, "Upload Response: Insufficient Space\n");
			this->SetUIState();
         break;
      }
         

		case ANTFS_RESPONSE_NONE:
			this->SetUIState();

			if(pclANTFSHost->GetStatus() == ANTFS_STATE_DOWNLOADING)
			{
				ULONG ulBytesDone;
				ULONG ulBytesTotal;
				ULONG ulPercentage;
				char acBuffer[32];

				pclANTFSHost->GetDownloadStatus(&ulBytesDone, &ulBytesTotal);
				if(ulBytesTotal == 0) ulBytesTotal = 1;
				ulPercentage = (ulBytesDone*100/ulBytesTotal);
				SNPRINTF(acBuffer, 31, "Status: Downloading... %lu%%", ulPercentage);
				this->SetText(labelStatus, gcnew String(acBuffer));
			}
			else if(pclANTFSHost->GetStatus() == ANTFS_STATE_UPLOADING)
			{
				ULONG ulBytesDone;
				ULONG ulBytesTotal;
				ULONG ulPercentage;
				char acBuffer[32];

				pclANTFSHost->GetUploadStatus(&ulBytesDone, &ulBytesTotal);
				if(ulBytesTotal == 0) ulBytesTotal = 1;
				ulPercentage = (ulBytesDone*100/ulBytesTotal);
				SNPRINTF(acBuffer, 31, "Status: Uploading... %lu%%", ulPercentage);
				this->SetText(labelStatus, gcnew String(acBuffer));
			}
			break;

		default:
      
         int i = eResponse;
         String^ temp = gcnew String("Error: Unknown Response " + i.ToString() +"\n");
			this->AppendText(richTextBox1, temp);
			this->SetUIState();
			break;
		}
	}

	
	DSIThread_MutexLock(&stMutexResponseThreadExit);
		DSIThread_CondSignal(&stCondResponseThreadExit);
	DSIThread_MutexUnlock(&stMutexResponseThreadExit);
	
	return;
}



////////////////////////////////////////////////////////////////////////////
// Directory
//
// On a successful download of a directory (initiated by the DirectoryClick
// function), the contents recieved from the client are parsed and displayed 
// in the directory list box. Note that this function assumes that the contents
// recieved from the client are properly formatted directory. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::Directory(void)
{
	ULONG ulLength;
   ULONG ulNumberOfEntries;

	if(!pclANTFSHost->GetTransferData(&ulLength))
	{
		this->AppendText(richTextBox1, "Error: Unable to get length of dir file.\n");
		return;
	}
	
	if(listDirectory->InvokeRequired)
	{
		MethodInvoker^ hFunc = gcnew MethodInvoker(listDirectory->Items, &ListBox::ObjectCollection::Clear);
		this->BeginInvoke(hFunc);
	}
	else
	{
		listDirectory->Items->Clear();
	}
	
	UCHAR* pucBuffer = new UCHAR[ulLength];
	if(!pclANTFSHost->GetTransferData(&ulLength, pucBuffer))
	{
		delete[] pucBuffer;
		pucBuffer = NULL;
		this->AppendText(richTextBox1, "Error: Unable to get dir file.\n");
		return;
	}
	
	//Make sure data is at least the size of the header
	if(ulLength < sizeof(ANTFS_DIRECTORY_HEADER) )
	{
		delete[] pucBuffer;
		pucBuffer = NULL;
		this->AppendText(richTextBox1, "Error: Invalid directory length.\n");
		return;
	}
	
	ANTFS_DIRECTORY_HEADER* stHeader = (ANTFS_DIRECTORY_HEADER*)pucBuffer;  //!! endianness dependent!

	this->AppendText(richTextBox1, "Directory Version:  ");
   this->AppendText(richTextBox1, stHeader->ucVersion.ToString());
	this->AppendText(richTextBox1, "\n");


	ulNumberOfEntries = (ulLength - sizeof(ANTFS_DIRECTORY_HEADER))/sizeof(ANTFSP_DIRECTORY);
	
	
	if(pstDirectory)
	{
		delete[] pstDirectory;
		pstDirectory = NULL;
	}
	pstDirectory = new ANTFSP_DIRECTORY[ulNumberOfEntries]; //!! watch out for memory leak
	

	
	ANTFSP_DIRECTORY* pstDir;
	for(ULONG i=0; i<ulNumberOfEntries; i++)
	{
		pstDir = (ANTFSP_DIRECTORY*)&pucBuffer[sizeof(ANTFS_DIRECTORY_HEADER) + (i*sizeof(ANTFSP_DIRECTORY))];	//!! endianness dependent!

		if(listDirectory->InvokeRequired)
		{
			AddDelegate^ hFunc = gcnew AddDelegate(listDirectory->Items, &ListBox::ObjectCollection::Add);
			this->BeginInvoke(hFunc, System::String::Concat( "Index: ", System::Convert::ToString(pstDir->usFileIndex),
											  " Data Type: ", System::Convert::ToString(pstDir->ucFileDataType),
											  " Identifier: ", System::Convert::ToString(pstDir->ucFileSubType), " ", System::Convert::ToString(pstDir->usFileNumber),
                                   " Flags: ", 
                                   (pstDir->ucFileDataType == 0x80 ? ParseDirectoryFlags(pstDir->ucGeneralFlags, pstDir->ucSpecificFlags) : ParseDirectoryFlags(pstDir->ucGeneralFlags, 0)),
											  " File Size: ", System::Convert::ToString((__int64)pstDir->ulFileSize),
											  " Date: ", 
                                   (pstDir->ulTimeStamp  >= (ULONG) 0x0FFFFFFF? dateReference->AddSeconds(pstDir->ulTimeStamp).ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss") : System::Convert::ToString((__int64)pstDir->ulTimeStamp))
											  ));
		}
		else
		{
			listDirectory->Items->Add(System::String::Concat( "Index: ", System::Convert::ToString(pstDir->usFileIndex),
											  " Data Type: ", System::Convert::ToString(pstDir->ucFileDataType),
											  " Identifier: ", System::Convert::ToString(pstDir->ucFileSubType), " ", System::Convert::ToString(pstDir->usFileNumber),
                                   " Flags: ", 
                                   (pstDir->ucFileDataType == 0x80 ? ParseDirectoryFlags(pstDir->ucGeneralFlags, pstDir->ucSpecificFlags) : ParseDirectoryFlags(pstDir->ucGeneralFlags, 0)),
											  " File Size: ", System::Convert::ToString((__int64)pstDir->ulFileSize),
											  " Date: ", 
                                   (pstDir->ulTimeStamp  >= (ULONG) 0x0FFFFFFF? dateReference->AddSeconds(pstDir->ulTimeStamp).ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss") : System::Convert::ToString((__int64)pstDir->ulTimeStamp))
											  ));
		}
										  
		pstDirectory[i] = *pstDir;
	}

   // Save the last downloaded directory to disk
   SaveDirectory(pucBuffer, ulLength);
	
	delete[] pucBuffer;
	pucBuffer = NULL;
	
}

////////////////////////////////////////////////////////////////////////////
// SaveDirectory
//
// Store downloaded directory into a file
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::SaveDirectory(UCHAR* pucBuffer_, ULONG ulBufferLength_)
{
	char szFileName[4096];
	DateTime stLocalTime = DateTime::Now;
	ULONG i;
	FILE *pFile;

   System::IO::FileInfo^ stOutputDirectoryInfo;

   if(this->checkBox_SaveDirectory->Checked == FALSE)
   {
      return;  // Saving disabled
   }

   if(ulBufferLength_ == 0)
   {
      this->AppendText(richTextBox1, "No directory data to save.\n");
		return;
   }

   stOutputDirectoryInfo = gcnew System::IO::FileInfo(Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "\\ANTFS_PC_Host\\antfs-" +  stDeviceParameters.ulDeviceID.ToString()  + ".dir");
	
   if(stOutputDirectoryInfo->Exists
      && MessageBox::Show("A directory file from this client already exists. Would you like to overwrite it? (Pressing 'No' will create a new file","File Exists", MessageBoxButtons::YesNo, MessageBoxIcon::Exclamation) == ::DialogResult::No)
	{
			SNPRINTF(szFileName, sizeof(szFileName), "%s%s%s%s%s%s", stOutputDirectoryInfo->DirectoryName, "\\", (stOutputDirectoryInfo->Name->Replace(stOutputDirectoryInfo->Extension, "")),
																						"-NewFileAt-",	stLocalTime.ToString("hh-mm-ss"), 
																						stOutputDirectoryInfo->Extension);
	}
	else
	{
		SNPRINTF(szFileName, sizeof(szFileName), "%s", stOutputDirectoryInfo->FullName);
	}

	pFile = FOPEN(szFileName, "w+b");
	if (pFile == NULL)
	{
		this->AppendText(richTextBox1, "Error: Unable to create output file to store directory data.\n");
		return;
	}

   for (i = 0; i < ulBufferLength_; i++)
	{
		if (fputc(pucBuffer_[i], pFile) == EOF)
		{
			this->AppendText(richTextBox1, "Error: Failed to write directory to file.\n");
			fclose(pFile);
			return;
		}
	}
	fclose(pFile);
}


////////////////////////////////////////////////////////////////////////////
// ParseDirectoryFlags
//
// Parses the flag field of a directory entry and returns a reader friendly 
// string.
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
String^ 
Form1::ParseDirectoryFlags(UCHAR ucFlags_, UCHAR ucSpecificFlags_)
{
   BOOL bFirst = TRUE;
   String^ str = gcnew String("");   
   
   if(ucFlags_ & ANTFS_GENERAL_FLAG_READ)
   {
      if(!bFirst)
      {
         str += "|";
      }
      else
      {
         bFirst = FALSE;
      }

      str += "Re"; 
   }
   
   if(ucFlags_ & ANTFS_GENERAL_FLAG_WRITE)
   {
      if(!bFirst)
      {
         str += "|";
      }
      else
      {
         bFirst = FALSE;
      }

      str += "Wr"; 
   }
   if(ucFlags_ & ANTFS_GENERAL_FLAG_ERASE)
   {
      if(!bFirst)
      {
         str += "|";
      }
      else
      {
         bFirst = FALSE;
      }

      str += "Er"; 
   }
   if(ucFlags_ & ANTFS_GENERAL_FLAG_ARCHIVE)
   {
      if(!bFirst)
      {
         str += "|";
      }
      else
      {
         bFirst = FALSE;
      }

      str += "Ar"; 
   }
   if(ucFlags_ & ANTFS_GENERAL_FLAG_APPEND)
   {
      if(!bFirst)
      {
         str += "|";
      }
      else
      {
         bFirst = FALSE;
      }

      str += "Ap"; 
   }
   if(ucFlags_ & ANTFS_GENERAL_FLAG_CRYPTO)
   {
      if(!bFirst)
      {
         str += "|";
      }
      else
      {
         bFirst = FALSE;
      }

      str += "Cr"; 
   }
   if(ucSpecificFlags_ & ANTFS_SPECIFIC_FLAG_SELECTED)
   {
      if(!bFirst)
      {
         str += "|";
      }
      else
      {
         bFirst = FALSE;
      }

      str += "Sel";
   }
   
  return(str); 




}

////////////////////////////////////////////////////////////////////////////
// Download
//
// On a successful download of a file (initiated by the DownloadClick
// function), the contents recieved from the client are stored to the file
// specified in the Output File text box. 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::Download(void)
{
	UCHAR* pucDownload;
	char szFileName[4096];
	DateTime stLocalTime = DateTime::Now;

	ULONG ulLength;
	ULONG i;
	FILE *pFile;
	
	if(this->stOutputFileInfo->Exists
      && MessageBox::Show("The destination file already exists. Would you like to overwrite it? (Pressing 'No' will create a new file","File Exists", MessageBoxButtons::YesNo, MessageBoxIcon::Exclamation) == ::DialogResult::No)
	{
			SNPRINTF(szFileName, sizeof(szFileName), "%s%s%s%s%s%s", this->stOutputFileInfo->DirectoryName, "\\", (this->stOutputFileInfo->Name->Replace(this->stOutputFileInfo->Extension, "")),
																						"-NewFileAt-",	stLocalTime.ToString("hh-mm-ss"), 
																						this->stOutputFileInfo->Extension);
	}
	else
	{
		SNPRINTF(szFileName, sizeof(szFileName), "%s", this->stOutputFileInfo->FullName);
	}

	pFile = FOPEN(szFileName, "w+b");
	if (pFile == NULL)
	{
		this->AppendText(richTextBox1, "Error: Unable to open output file.\n");
		return;
	}

	//Get download length
	if(!pclANTFSHost->GetTransferData(&ulLength))
	{
		this->AppendText(richTextBox1, "Error: Failed to retrieve data length.\n");
		fclose(pFile);
		return;
	}

	pucDownload = new UCHAR[ulLength];

	if(!pclANTFSHost->GetTransferData(&ulLength, pucDownload))
	{
		this->AppendText(richTextBox1, "Error: Failed to retrieve downloaded data.\n");
		fclose(pFile);
		delete[] pucDownload;
		pucDownload = NULL;
		return;
	}

	//Encrypt the file if enabled and the file is encrypted, as indicated in the directory
   // !!! This will severely break if using custom indexes
	if(checkBox_EnableDecrypt->Checked && pstDirectory &&
      (pstDirectory[usLastDownloadIndex].ucGeneralFlags & ANTFS_GENERAL_FLAG_CRYPTO))
   {
      this->AppendText(richTextBox1, "Decrypting downloaded data...\n");
	   array<UCHAR>^ encryptionBuf = gcnew array<UCHAR>(ulLength);
	   for(ULONG i=0; i<ulLength; ++i)	//Copy unmanaged array to managed
		   encryptionBuf[i] = pucDownload[i];
	   encryptionBuf = AES_CTR::AES_CTR::FS_decrypt_bytes(encryptionBuf, strOptions->aucEncryptionKey);
	   ulLength -= 16;	//The first 16 bytes were the encryption paramaters
	   for(ULONG i=0; i<ulLength; ++i)	//Copy managed result back to unmanaged
		   pucDownload[i] = encryptionBuf[i];
	}

	for (i = 0; i < ulLength; i++)
	{
		if (fputc(pucDownload[i], pFile) == EOF)
		{
			this->AppendText(richTextBox1, "Error: Failed to write to output file.\n");
			fclose(pFile);
			delete[] pucDownload;
			pucDownload = NULL;
			return;
		}
	}
	fclose(pFile);
	delete[] pucDownload;
	pucDownload = NULL;
	
}


////////////////////////////////////////////////////////////////////////////
// UpdatePasskey
//
// Upon getting a passkey during the pairing operation, the passkey stored
// in the options struct is updated (over-writing the previously entered
// passkey). 
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::UpdatePasskey()
{

	if(this->buttonOpen->InvokeRequired)
	{    
		MethodInvoker^ phUpdatePasskey = gcnew MethodInvoker(this, &Form1::UpdatePasskey);
		this->BeginInvoke(phUpdatePasskey);
		return;
	}


   // Copy password over for future use
   strOptions->pcArray = gcnew array<UCHAR^>(ucDevicePasswordLength);

   for (int i = 0; i < ucDevicePasswordLength; i++)
      strOptions->pcArray[i] = aucDevicePassword[i];
}


////////////////////////////////////////////////////////////////////////////
// AppendText
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::AppendText(RichTextBox^ phTextBox_,String^ strText_)
{

	// InvokeRequired required compares the thread ID of the
	// calling thread to the thread ID of the creating thread.
	// If these threads are different, it returns true.
	if(phTextBox_->InvokeRequired)
	{    
		RichTextBoxDelegate^ phCallback = gcnew RichTextBoxDelegate(this, &Form1::AppendText_CLI);
		this->BeginInvoke(phCallback, phTextBox_, strText_);
	}
	else
	{
		this->AppendText_CLI(phTextBox_, strText_);
	}


   
}

////////////////////////////////////////////////////////////////////////////
// AppendText
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::AppendText(RichTextBox^ phTextBox_, char* pcText_)
{
	// InvokeRequired required compares the thread ID of the
	// calling thread to the thread ID of the creating thread.
	// If these threads are different, it returns true.
	if(phTextBox_->InvokeRequired)
	{    
		RichTextBoxDelegate^ phCallback = gcnew RichTextBoxDelegate(this, &Form1::AppendText_CLI);
		this->BeginInvoke(phCallback, phTextBox_, gcnew String(pcText_));
	}
	else
	{
		this->AppendText_CLI(phTextBox_, gcnew String(pcText_));
	}
}

////////////////////////////////////////////////////////////////////////////
// AppendText_CLI
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::AppendText_CLI(RichTextBox^ phTextBox_, System::String^ strText_)
{
	phTextBox_->AppendText(strText_);
}

////////////////////////////////////////////////////////////////////////////
// SetText
//
// \return: N/A
////////////////////////////////////////////////////////////////////////////
void Form1::SetText(Label^ phLabel_, System::String^ strText_)
{
	if(phLabel_->InvokeRequired)
	{
		LabelDelegate^ phSetTextFunc = gcnew LabelDelegate(this, &Form1::SetText);
		this->BeginInvoke(phSetTextFunc, phLabel_, strText_);
	}
	else
	{
		phLabel_->Text = strText_;
	}
}

void Form1::ClickSetTime(void)
{
   static UCHAR ucSequence = 0;
   ULONG_UNION ulTime;
   static UCHAR aucBuffer[16];
   
   DateTime^ date2 = System::DateTime::UtcNow;           //UTC Time
   DateTime^ localdate = System::DateTime::Now;          //Local Time

   TimeSpan^ span = date2->Subtract(dateReference->Date);
   ulTime.ulData = static_cast<ULONG>(span->TotalSeconds);

   this->AppendText(richTextBox1, "Setting UTC Time to:  [" + date2->ToLongDateString() + "][" + date2->ToLongTimeString() + "]\n");
   this->AppendText(richTextBox1, "Local Time is: [" + localdate->ToLongDateString() + "][" + localdate->ToLongTimeString() + "]\n");
   this->AppendText(richTextBox1, "Raw  [" + ulTime.ulData.ToString() + "]\n"); 
   
   aucBuffer[0] = 3;
   aucBuffer[1] = 0;
   aucBuffer[2] = 0;
   aucBuffer[3] = ucSequence++;
   aucBuffer[4] = ulTime.stBytes.ucByte0;
   aucBuffer[5] = ulTime.stBytes.ucByte1;
   aucBuffer[6] = ulTime.stBytes.ucByte2;
   aucBuffer[7] = ulTime.stBytes.ucByte3;
   aucBuffer[8] = 0;
   aucBuffer[9] = 0;
   aucBuffer[10] = 0;
   aucBuffer[11] = 0;
   aucBuffer[12] = 0;
   aucBuffer[13] = 0;
   aucBuffer[14] = 0;
   aucBuffer[15] = 0;   
   
	USHORT usIndex = 0xFFFE; // Command Pipe Index

	ANTFS_RETURN eReturn = pclANTFSHost->Upload(usIndex, 0, sizeof(aucBuffer), aucBuffer);
	switch(eReturn)
	{
	case ANTFS_RETURN_PASS:
		this->AppendText(richTextBox1, "Uploading...\n");
		break;
	
	case ANTFS_RETURN_BUSY:
		this->AppendText(richTextBox1, "Error: Library is busy with another request\n");
		break;

	case ANTFS_RETURN_FAIL:
		this->AppendText(richTextBox1, "Error: Set Time Button Pushed out of sequence\n");
		break;

	}
}




















