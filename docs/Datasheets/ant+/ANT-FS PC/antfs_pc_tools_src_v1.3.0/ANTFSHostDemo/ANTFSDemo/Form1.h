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
#pragma warning(disable: 4368)

#include "config.h"
#include "types.h"
#include "macros.h"
#include "antfs_host.hpp"
#include "antfs_directory.h"
#include "ThreadCreater.h"
#include "Auth.h"

#include <vcclr.h>
#include <stdio.h>

#define USER_RADIO_CHANNEL	0           // ANT channel 
//#define REFERENCE_DESIGN

static BOOL bResponseThreadExit;
static DSI_THREAD_ID hResponseThread;
static DSI_CONDITION_VAR stCondResponseThreadExit;
static DSI_MUTEX stMutexResponseThreadExit;
static BOOL bMyDownloadDir;

static UCHAR aucFriendlyName[FRIENDLY_NAME_MAX_LENGTH];
static UCHAR ucFriendlyNameLength = FRIENDLY_NAME_MAX_LENGTH;
static UCHAR aucDevicePassword[255];
static UCHAR ucDevicePasswordLength = 8;
static ANTFS_DEVICE_PARAMETERS stDeviceParameters;


namespace ANTFSDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;



	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void);
      
      
	   void ResponseThread();
	   void SetUIState();
      void UpdatePasskey();
      
      
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1();
      
	private:
		/// <summary>
      void OpenClick();
      void CloseClick();
      void CancelClick();
      void SearchClick();
      void PairClick();
      void AuthenticateClick();
      void SkipAuthClick();
      void DirectoryClick();
      void DownloadClick(int iIndex_);
      void UploadClick(int iIndex_);
      void EraseClick();
      void DisconnectClick();
      void ReturnToBroadcastClick();
      void ClickSetTime();
      void Directory(void);
	   void Download(void);
	   void AppendText(RichTextBox^ phTextBox_, char* pcText_);
	   void AppendText(RichTextBox^ phTextBox_, String^ strText_);
	   void AppendText_CLI(RichTextBox^ phTextBox_, System::String^ strText_);
	   void SetText(Label^ phLabel_, System::String^ strText_);
      String^ ParseDirectoryFlags(UCHAR ucFlags_, UCHAR ucSpecificFlags_);
      void SaveDirectory(UCHAR* pucBuffer_, ULONG ulLength_);
     
      
      OptionsStruct^ strOptions;
	   USHORT usLastDownloadIndex;
      DateTime^ dateReference;

      String^ strPath;

	   private: System::Windows::Forms::Label^  labelStatus;

	   private: System::Windows::Forms::Button^  buttonOpen;
	   private: System::Windows::Forms::Button^  buttonClose;
	   private: System::Windows::Forms::Button^  buttonSearch;
	   private: System::Windows::Forms::Button^  buttonPair;
	   private: System::Windows::Forms::Button^  buttonAuthenticate;
	   private: System::Windows::Forms::RichTextBox^  richTextBox1;
	   private: System::Windows::Forms::Button^  buttonSkipAuth;
	   private: System::Windows::Forms::Button^  btnDirectory;
	   private: System::Windows::Forms::Button^  buttonDownload;
	   private: System::Windows::Forms::Button^  buttonUpload;
	   private: System::Windows::Forms::Button^  buttonErase;
	   private: System::Windows::Forms::Button^  buttonDisconnect;
	   private: System::Windows::Forms::Label^  label9;
	   private: System::Windows::Forms::TextBox^  textBoxOutputFile;
	   private: System::Windows::Forms::Button^  buttonBrowse;
	   private: System::Windows::Forms::Label^  label2;
	   private: System::Windows::Forms::TextBox^  textBoxInputFile;
	   private: System::Windows::Forms::Button^  buttonBrowseOpen;

	   private: System::Windows::Forms::ListBox^  listDirectory;
	   private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	   private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	   private: System::Windows::Forms::Button^  buttonCancel;
      private: System::Windows::Forms::Button^  buttonOptions;
	   private: System::Windows::Forms::CheckBox^  checkBox_EnableDecrypt;
      private: System::Windows::Forms::Button^  buttonReturnBroadcast;
      private: System::Windows::Forms::CheckBox^  checkBox_SaveDirectory;
      private: System::Windows::Forms::Button^  buttonSetTime;
   private: System::Windows::Forms::TextBox^  txtCustomIndex;


   private: System::Windows::Forms::ComboBox^  comboDirectory;



























            /// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
         System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
         this->labelStatus = (gcnew System::Windows::Forms::Label());
         this->buttonOpen = (gcnew System::Windows::Forms::Button());
         this->buttonClose = (gcnew System::Windows::Forms::Button());
         this->buttonSearch = (gcnew System::Windows::Forms::Button());
         this->buttonPair = (gcnew System::Windows::Forms::Button());
         this->buttonAuthenticate = (gcnew System::Windows::Forms::Button());
         this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
         this->buttonSkipAuth = (gcnew System::Windows::Forms::Button());
         this->btnDirectory = (gcnew System::Windows::Forms::Button());
         this->buttonDownload = (gcnew System::Windows::Forms::Button());
         this->buttonUpload = (gcnew System::Windows::Forms::Button());
         this->buttonErase = (gcnew System::Windows::Forms::Button());
         this->buttonDisconnect = (gcnew System::Windows::Forms::Button());
         this->label9 = (gcnew System::Windows::Forms::Label());
         this->textBoxOutputFile = (gcnew System::Windows::Forms::TextBox());
         this->buttonBrowse = (gcnew System::Windows::Forms::Button());
         this->label2 = (gcnew System::Windows::Forms::Label());
         this->textBoxInputFile = (gcnew System::Windows::Forms::TextBox());
         this->buttonBrowseOpen = (gcnew System::Windows::Forms::Button());
         this->listDirectory = (gcnew System::Windows::Forms::ListBox());
         this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
         this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
         this->buttonCancel = (gcnew System::Windows::Forms::Button());
         this->buttonOptions = (gcnew System::Windows::Forms::Button());
         this->checkBox_EnableDecrypt = (gcnew System::Windows::Forms::CheckBox());
         this->buttonReturnBroadcast = (gcnew System::Windows::Forms::Button());
         this->checkBox_SaveDirectory = (gcnew System::Windows::Forms::CheckBox());
         this->buttonSetTime = (gcnew System::Windows::Forms::Button());
         this->txtCustomIndex = (gcnew System::Windows::Forms::TextBox());
         this->comboDirectory = (gcnew System::Windows::Forms::ComboBox());
         this->SuspendLayout();
         // 
         // labelStatus
         // 
         this->labelStatus->AutoSize = true;
         this->labelStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
            static_cast<System::Byte>(0)));
         this->labelStatus->Location = System::Drawing::Point(87, 23);
         this->labelStatus->Name = L"labelStatus";
         this->labelStatus->Size = System::Drawing::Size(138, 25);
         this->labelStatus->TabIndex = 0;
         this->labelStatus->Text = L"Status: None";
         // 
         // buttonOpen
         // 
         this->buttonOpen->Location = System::Drawing::Point(12, 83);
         this->buttonOpen->Name = L"buttonOpen";
         this->buttonOpen->Size = System::Drawing::Size(75, 23);
         this->buttonOpen->TabIndex = 2;
         this->buttonOpen->Text = L"&Open";
         this->buttonOpen->Click += gcnew System::EventHandler(this, &Form1::buttonOpen_Click);
         // 
         // buttonClose
         // 
         this->buttonClose->Enabled = false;
         this->buttonClose->Location = System::Drawing::Point(12, 111);
         this->buttonClose->Name = L"buttonClose";
         this->buttonClose->Size = System::Drawing::Size(75, 23);
         this->buttonClose->TabIndex = 3;
         this->buttonClose->Text = L"&Close";
         this->buttonClose->Click += gcnew System::EventHandler(this, &Form1::buttonClose_Click);
         // 
         // buttonSearch
         // 
         this->buttonSearch->Location = System::Drawing::Point(12, 167);
         this->buttonSearch->Name = L"buttonSearch";
         this->buttonSearch->Size = System::Drawing::Size(75, 23);
         this->buttonSearch->TabIndex = 5;
         this->buttonSearch->Text = L"&Search";
         this->buttonSearch->UseVisualStyleBackColor = true;
         this->buttonSearch->Click += gcnew System::EventHandler(this, &Form1::buttonSearch_Click);
         // 
         // buttonPair
         // 
         this->buttonPair->Location = System::Drawing::Point(12, 195);
         this->buttonPair->Name = L"buttonPair";
         this->buttonPair->Size = System::Drawing::Size(75, 23);
         this->buttonPair->TabIndex = 6;
         this->buttonPair->Text = L"&Pair";
         this->buttonPair->UseVisualStyleBackColor = true;
         this->buttonPair->Click += gcnew System::EventHandler(this, &Form1::buttonPair_Click);
         // 
         // buttonAuthenticate
         // 
         this->buttonAuthenticate->Location = System::Drawing::Point(12, 223);
         this->buttonAuthenticate->Name = L"buttonAuthenticate";
         this->buttonAuthenticate->Size = System::Drawing::Size(75, 23);
         this->buttonAuthenticate->TabIndex = 7;
         this->buttonAuthenticate->Text = L"P&asskey";
         this->buttonAuthenticate->UseVisualStyleBackColor = true;
         this->buttonAuthenticate->Click += gcnew System::EventHandler(this, &Form1::buttonAuthenticate_Click);
         // 
         // richTextBox1
         // 
         this->richTextBox1->HideSelection = false;
         this->richTextBox1->Location = System::Drawing::Point(92, 57);
         this->richTextBox1->Name = L"richTextBox1";
         this->richTextBox1->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
         this->richTextBox1->Size = System::Drawing::Size(302, 218);
         this->richTextBox1->TabIndex = 14;
         this->richTextBox1->Text = L"";
         // 
         // buttonSkipAuth
         // 
         this->buttonSkipAuth->Location = System::Drawing::Point(11, 252);
         this->buttonSkipAuth->Name = L"buttonSkipAuth";
         this->buttonSkipAuth->Size = System::Drawing::Size(75, 23);
         this->buttonSkipAuth->TabIndex = 8;
         this->buttonSkipAuth->Text = L"Pass &Thru";
         this->buttonSkipAuth->UseVisualStyleBackColor = true;
         this->buttonSkipAuth->Click += gcnew System::EventHandler(this, &Form1::buttonSkipAuth_Click);
         // 
         // btnDirectory
         // 
         this->btnDirectory->Location = System::Drawing::Point(400, 55);
         this->btnDirectory->Name = L"btnDirectory";
         this->btnDirectory->Size = System::Drawing::Size(75, 23);
         this->btnDirectory->TabIndex = 9;
         this->btnDirectory->Text = L"Dire&ctory";
         this->btnDirectory->UseVisualStyleBackColor = true;
         this->btnDirectory->Click += gcnew System::EventHandler(this, &Form1::btnDirectory_Click);
         // 
         // buttonDownload
         // 
         this->buttonDownload->Location = System::Drawing::Point(400, 83);
         this->buttonDownload->Name = L"buttonDownload";
         this->buttonDownload->Size = System::Drawing::Size(75, 23);
         this->buttonDownload->TabIndex = 10;
         this->buttonDownload->Text = L"&Download";
         this->buttonDownload->UseVisualStyleBackColor = true;
         this->buttonDownload->Click += gcnew System::EventHandler(this, &Form1::buttonDownload_Click);
         // 
         // buttonUpload
         // 
         this->buttonUpload->Enabled = false;
         this->buttonUpload->Location = System::Drawing::Point(400, 111);
         this->buttonUpload->Name = L"buttonUpload";
         this->buttonUpload->Size = System::Drawing::Size(75, 23);
         this->buttonUpload->TabIndex = 11;
         this->buttonUpload->Text = L"&Upload";
         this->buttonUpload->UseVisualStyleBackColor = true;
         this->buttonUpload->Click += gcnew System::EventHandler(this, &Form1::buttonUpload_Click);
         // 
         // buttonErase
         // 
         this->buttonErase->Location = System::Drawing::Point(400, 139);
         this->buttonErase->Name = L"buttonErase";
         this->buttonErase->Size = System::Drawing::Size(75, 23);
         this->buttonErase->TabIndex = 12;
         this->buttonErase->Text = L"&Erase";
         this->buttonErase->UseVisualStyleBackColor = true;
         this->buttonErase->Click += gcnew System::EventHandler(this, &Form1::buttonErase_Click);
         // 
         // buttonDisconnect
         // 
         this->buttonDisconnect->Location = System::Drawing::Point(400, 167);
         this->buttonDisconnect->Name = L"buttonDisconnect";
         this->buttonDisconnect->Size = System::Drawing::Size(75, 23);
         this->buttonDisconnect->TabIndex = 13;
         this->buttonDisconnect->Text = L"D&isconnect";
         this->buttonDisconnect->UseVisualStyleBackColor = true;
         this->buttonDisconnect->Click += gcnew System::EventHandler(this, &Form1::buttonDisconnect_Click);
         // 
         // label9
         // 
         this->label9->Location = System::Drawing::Point(17, 294);
         this->label9->Name = L"label9";
         this->label9->Size = System::Drawing::Size(88, 16);
         this->label9->TabIndex = 40;
         this->label9->Text = L"Download file:";
         // 
         // textBoxOutputFile
         // 
         this->textBoxOutputFile->Location = System::Drawing::Point(15, 311);
         this->textBoxOutputFile->Name = L"textBoxOutputFile";
         this->textBoxOutputFile->Size = System::Drawing::Size(379, 20);
         this->textBoxOutputFile->TabIndex = 41;
         // 
         // buttonBrowse
         // 
         this->buttonBrowse->Location = System::Drawing::Point(400, 308);
         this->buttonBrowse->Name = L"buttonBrowse";
         this->buttonBrowse->Size = System::Drawing::Size(75, 23);
         this->buttonBrowse->TabIndex = 42;
         this->buttonBrowse->Text = L"&Browse";
         this->buttonBrowse->Click += gcnew System::EventHandler(this, &Form1::buttonBrowse_Click);
         // 
         // label2
         // 
         this->label2->Location = System::Drawing::Point(17, 337);
         this->label2->Name = L"label2";
         this->label2->Size = System::Drawing::Size(88, 16);
         this->label2->TabIndex = 43;
         this->label2->Text = L"Upload File:";
         // 
         // textBoxInputFile
         // 
         this->textBoxInputFile->Location = System::Drawing::Point(15, 353);
         this->textBoxInputFile->Name = L"textBoxInputFile";
         this->textBoxInputFile->Size = System::Drawing::Size(379, 20);
         this->textBoxInputFile->TabIndex = 44;
         // 
         // buttonBrowseOpen
         // 
         this->buttonBrowseOpen->Location = System::Drawing::Point(400, 353);
         this->buttonBrowseOpen->Name = L"buttonBrowseOpen";
         this->buttonBrowseOpen->Size = System::Drawing::Size(75, 23);
         this->buttonBrowseOpen->TabIndex = 45;
         this->buttonBrowseOpen->Text = L"B&rowse";
         this->buttonBrowseOpen->Click += gcnew System::EventHandler(this, &Form1::buttonBrowseOpen_Click);
         // 
         // listDirectory
         // 
         this->listDirectory->BackColor = System::Drawing::Color::White;
         this->listDirectory->Cursor = System::Windows::Forms::Cursors::Default;
         this->listDirectory->FormattingEnabled = true;
         this->listDirectory->HorizontalScrollbar = true;
         this->listDirectory->Location = System::Drawing::Point(16, 405);
         this->listDirectory->Name = L"listDirectory";
         this->listDirectory->Size = System::Drawing::Size(460, 82);
         this->listDirectory->TabIndex = 47;
         this->listDirectory->DoubleClick += gcnew System::EventHandler(this, &Form1::listDirectory_DoubleClick);
         // 
         // saveFileDialog1
         // 
         this->saveFileDialog1->OverwritePrompt = false;
         this->saveFileDialog1->RestoreDirectory = true;
         this->saveFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::saveFileDialog1_FileOk);
         // 
         // openFileDialog1
         // 
         this->openFileDialog1->InitialDirectory = L"C:\\Documents and Settings\\pink\\My Documents";
         this->openFileDialog1->RestoreDirectory = true;
         this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
         // 
         // buttonCancel
         // 
         this->buttonCancel->Location = System::Drawing::Point(12, 139);
         this->buttonCancel->Name = L"buttonCancel";
         this->buttonCancel->Size = System::Drawing::Size(75, 23);
         this->buttonCancel->TabIndex = 4;
         this->buttonCancel->Text = L"Ca&ncel";
         this->buttonCancel->Click += gcnew System::EventHandler(this, &Form1::buttonCancel_Click);
         // 
         // buttonOptions
         // 
         this->buttonOptions->Location = System::Drawing::Point(12, 55);
         this->buttonOptions->Name = L"buttonOptions";
         this->buttonOptions->Size = System::Drawing::Size(75, 23);
         this->buttonOptions->TabIndex = 58;
         this->buttonOptions->Text = L"&Options";
         this->buttonOptions->UseVisualStyleBackColor = true;
         this->buttonOptions->Click += gcnew System::EventHandler(this, &Form1::buttonOptions_Click);
         // 
         // checkBox_EnableDecrypt
         // 
         this->checkBox_EnableDecrypt->AutoSize = true;
         this->checkBox_EnableDecrypt->Checked = true;
         this->checkBox_EnableDecrypt->CheckState = System::Windows::Forms::CheckState::Checked;
         this->checkBox_EnableDecrypt->Location = System::Drawing::Point(202, 382);
         this->checkBox_EnableDecrypt->Name = L"checkBox_EnableDecrypt";
         this->checkBox_EnableDecrypt->Size = System::Drawing::Size(134, 17);
         this->checkBox_EnableDecrypt->TabIndex = 59;
         this->checkBox_EnableDecrypt->Text = L"Decrypt encrypted files";
         this->checkBox_EnableDecrypt->UseVisualStyleBackColor = true;
         // 
         // buttonReturnBroadcast
         // 
         this->buttonReturnBroadcast->Location = System::Drawing::Point(400, 195);
         this->buttonReturnBroadcast->Name = L"buttonReturnBroadcast";
         this->buttonReturnBroadcast->Size = System::Drawing::Size(75, 23);
         this->buttonReturnBroadcast->TabIndex = 60;
         this->buttonReturnBroadcast->Text = L"&Ret Bcast";
         this->buttonReturnBroadcast->UseVisualStyleBackColor = true;
         this->buttonReturnBroadcast->Click += gcnew System::EventHandler(this, &Form1::buttonReturnBroadcast_Click);
         // 
         // checkBox_SaveDirectory
         // 
         this->checkBox_SaveDirectory->AutoSize = true;
         this->checkBox_SaveDirectory->Location = System::Drawing::Point(353, 382);
         this->checkBox_SaveDirectory->Name = L"checkBox_SaveDirectory";
         this->checkBox_SaveDirectory->Size = System::Drawing::Size(122, 17);
         this->checkBox_SaveDirectory->TabIndex = 61;
         this->checkBox_SaveDirectory->Text = L"Save directory to file";
         this->checkBox_SaveDirectory->UseVisualStyleBackColor = true;
         // 
         // buttonSetTime
         // 
         this->buttonSetTime->Location = System::Drawing::Point(400, 224);
         this->buttonSetTime->Name = L"buttonSetTime";
         this->buttonSetTime->Size = System::Drawing::Size(75, 23);
         this->buttonSetTime->TabIndex = 62;
         this->buttonSetTime->Text = L"Set Time";
         this->buttonSetTime->UseVisualStyleBackColor = true;
         this->buttonSetTime->Click += gcnew System::EventHandler(this, &Form1::buttonSetTime_Click);
         // 
         // txtCustomIndex
         // 
         this->txtCustomIndex->Location = System::Drawing::Point(16, 405);
         this->txtCustomIndex->Name = L"txtCustomIndex";
         this->txtCustomIndex->Size = System::Drawing::Size(460, 20);
         this->txtCustomIndex->TabIndex = 63;
         this->txtCustomIndex->Text = L"-1";
         this->txtCustomIndex->Visible = false;
         this->txtCustomIndex->TextChanged += gcnew System::EventHandler(this, &Form1::txtCustomIndex_TextChanged);
         // 
         // comboDirectory
         // 
         this->comboDirectory->FormattingEnabled = true;
         this->comboDirectory->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Directory"});
         this->comboDirectory->Location = System::Drawing::Point(16, 382);
         this->comboDirectory->Name = L"comboDirectory";
         this->comboDirectory->Size = System::Drawing::Size(121, 21);
         this->comboDirectory->TabIndex = 66;
         this->comboDirectory->Text = L"Directory";
         this->comboDirectory->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboDirectory_SelectedIndexChanged);
         // 
         // Form1
         // 
         this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
         this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
         this->ClientSize = System::Drawing::Size(490, 502);
         this->Controls->Add(this->comboDirectory);
         this->Controls->Add(this->txtCustomIndex);
         this->Controls->Add(this->buttonSetTime);
         this->Controls->Add(this->checkBox_SaveDirectory);
         this->Controls->Add(this->buttonReturnBroadcast);
         this->Controls->Add(this->checkBox_EnableDecrypt);
         this->Controls->Add(this->buttonOptions);
         this->Controls->Add(this->buttonCancel);
         this->Controls->Add(this->listDirectory);
         this->Controls->Add(this->buttonBrowseOpen);
         this->Controls->Add(this->textBoxInputFile);
         this->Controls->Add(this->label2);
         this->Controls->Add(this->buttonBrowse);
         this->Controls->Add(this->textBoxOutputFile);
         this->Controls->Add(this->label9);
         this->Controls->Add(this->buttonDisconnect);
         this->Controls->Add(this->buttonErase);
         this->Controls->Add(this->buttonUpload);
         this->Controls->Add(this->buttonDownload);
         this->Controls->Add(this->btnDirectory);
         this->Controls->Add(this->buttonSkipAuth);
         this->Controls->Add(this->richTextBox1);
         this->Controls->Add(this->buttonAuthenticate);
         this->Controls->Add(this->buttonPair);
         this->Controls->Add(this->buttonSearch);
         this->Controls->Add(this->buttonClose);
         this->Controls->Add(this->buttonOpen);
         this->Controls->Add(this->labelStatus);
         this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
         this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
         this->MaximizeBox = false;
         this->Name = L"Form1";
         this->Text = L"ANT-FS PC Host";
         this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
         this->ResumeLayout(false);
         this->PerformLayout();

      }
#pragma endregion

	/////////////////////////////////////////////////////////////////////////////////
	//	User-defined variables
	/////////////////////////////////////////////////////////////////////////////////
	private:
		ThreadCreater* pclThread;
		ANTFSHost* pclANTFSHost;
		ANTFSP_DIRECTORY* pstDirectory;
		UCHAR* pucUploadData;
		System::IO::FileInfo^ stOutputFileInfo;
		

	//Make this do general objects.
	//Delegates
	private:
	delegate System::Void RichTextBoxDelegate(RichTextBox^ phTextBox_, System::String^ strText_);
	delegate System::Void LabelDelegate(Label^ phLabel, System::String^ strText_);
	delegate int AddDelegate(System::Object^);
	
	/////////////////////////////////////////////////////////////////////////////////
	//	GUI Functions
	/////////////////////////////////////////////////////////////////////////////////
	private:
	System::Void buttonOpen_Click(System::Object^  sender, System::EventArgs^  e)
	{
      OpenClick();		
	}


	private:
	System::Void buttonClose_Click(System::Object^  sender, System::EventArgs^  e)
	{
      CloseClick();
	}


	System::Void buttonSearch_Click(System::Object^  sender, System::EventArgs^  e)
	{     
      SearchClick();		
	}


	private:
	System::Void buttonPair_Click(System::Object^  sender, System::EventArgs^  e)
	{
      PairClick();
	}


	private:
	System::Void buttonAuthenticate_Click(System::Object^  sender, System::EventArgs^  e)
	{
      AuthenticateClick();
	}


	private:
	System::Void buttonSkipAuth_Click(System::Object^  sender, System::EventArgs^  e)
	{
      SkipAuthClick();
	}
	

	private:
	System::Void btnDirectory_Click(System::Object^  sender, System::EventArgs^  e)
	{
      DirectoryClick();
		
	}


	private:
	System::Void buttonDownload_Click(System::Object^  sender, System::EventArgs^  e)
	{
      int iIndex;

      try{
         iIndex = int::Parse(txtCustomIndex->Text);
      }
      catch(OverflowException^g){
         String^ strMessage = gcnew String ("Error in Selected Index. Error " + g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }
      catch(ArgumentNullException^g){
         String^ strMessage = gcnew String ("String is null. Error "+ g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }
      catch(FormatException^g){
         String^ strMessage = gcnew String ("String does not consist of an optional sign followed by a series of digits. Error "+ g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }



      DownloadClick(iIndex);
	}


	private:
	System::Void buttonUpload_Click(System::Object^  sender, System::EventArgs^  e)
	{
      int iIndex;

      try{
         iIndex = int::Parse(txtCustomIndex->Text);
      }
      catch(OverflowException^g){
         String^ strMessage = gcnew String ("Overflow in Double to Int32 conversion. Error " + g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }
      catch(ArgumentNullException^g){
         String^ strMessage = gcnew String ("String is null. Error " + g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }
      catch(FormatException^g){
         String^ strMessage = gcnew String ("String does not consist of an optional sign followed by a series of digits. Error " + g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }
      UploadClick(iIndex);
	}


	private:
	System::Void buttonErase_Click(System::Object^  sender, System::EventArgs^  e)
	{
      EraseClick();
	}


	private:
	System::Void buttonDisconnect_Click(System::Object^  sender, System::EventArgs^  e)
	{
      DisconnectClick();
		
	}
   
	private:	//cancel/ignore/disconnect
	System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e)
	{
      CancelClick();		
	}
   

	private:
	System::Void buttonBrowse_Click(System::Object^  sender, System::EventArgs^  e)
	{
      this->saveFileDialog1->ShowDialog(this);
	}


	private:
	System::Void buttonBrowseOpen_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->openFileDialog1->ShowDialog(this);
	}
	
	/////////////////////////////////////////////////////////////////////////////////
	//	Private Functions
	/////////////////////////////////////////////////////////////////////////////////
	private:
	System::Void listDirectory_DoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		this->buttonDownload_Click(nullptr, nullptr);
	}
	
	private:
	System::Void saveFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		textBoxOutputFile->Text = this->saveFileDialog1->FileName;
	}

	private:
	System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		textBoxInputFile->Text = this->openFileDialog1->FileName;
	}


   private: System::Void buttonOptions_Click(System::Object^  sender, System::EventArgs^  e) 
   {
      Auth::ShowWindow(strOptions, (System::Windows::Forms::Form^)this);
   }

   private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
   {
      e->Cancel=false; //Ensure the main window can always close at user command
   }

   private: System::Void buttonReturnBroadcast_Click(System::Object^  sender, System::EventArgs^  e) 
   {
      ReturnToBroadcastClick();
   }

private: System::Void buttonSetTime_Click(System::Object^  sender, System::EventArgs^  e) 
   {
      ClickSetTime();
   }
private: System::Void buttonTest_Click(System::Object^  sender, System::EventArgs^  e) {
      int iIndex;


      try{
         iIndex = int::Parse(txtCustomIndex->Text);
      }
      catch(OverflowException^g){
         String^ strMessage = gcnew String ("Overflow in Double to Int32 conversion. Error " + g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }
      catch(ArgumentNullException^g){
         String^ strMessage = gcnew String ("String is null. Error " + g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }
      catch(FormatException^g){
         String^ strMessage = gcnew String ("String does not consist of an optional sign followed by a series of digits. Error " + g->ToString());
         MessageBox::Show(strMessage, "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
      }

      UploadClick(iIndex);


         }
private: System::Void txtCustomIndex_TextChanged(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void comboDirectory_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
            
            if(String::Compare(comboDirectory->Text,gcnew String("Directory")) == 0) // Directory
            {
               listDirectory->Visible = true;
               txtCustomIndex->Visible = false;
               txtCustomIndex->Text = gcnew String("-1");
               checkBox_EnableDecrypt->Enabled = true;
               checkBox_EnableDecrypt->Checked = true;
            }
            else // Custom
            {
               listDirectory->Visible = false;
               txtCustomIndex->Visible = true;
               txtCustomIndex->Text = gcnew String("-1");
               checkBox_EnableDecrypt->Enabled = false;
               checkBox_EnableDecrypt->Checked = false;
               // Can't reliably decide whether to decrypt if using custom indexes, so disable this option

            }
         }

};
}
