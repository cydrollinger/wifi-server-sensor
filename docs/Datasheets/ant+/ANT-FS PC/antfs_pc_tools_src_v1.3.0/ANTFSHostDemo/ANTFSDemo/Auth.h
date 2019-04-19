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
#include "options.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace ANTFSDemo {

	/// <summary>
	/// Summary for Auth
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Auth : public System::Windows::Forms::Form
	{
	public:
	static void ShowWindow(OptionsStruct^ pstOptions_, System::Windows::Forms::Form^ form);
   private: System::Windows::Forms::CheckBox^  checkBroadcastMode;
   private: System::Windows::Forms::CheckBox^  checkPing;
   private: System::Windows::Forms::TabPage^  tabPageSecurity;
   private: System::Windows::Forms::GroupBox^  groupBox11;
   private: System::Windows::Forms::Label^  label16;
   private: System::Windows::Forms::Label^  label17;
   private: System::Windows::Forms::TextBox^  textBox_EncryptionKey2;

   private: System::Windows::Forms::TextBox^  textBox_EncryptionKey1;

   private: System::Windows::Forms::GroupBox^  groupBox9;
   private: System::Windows::Forms::TextBox^  textPasskey;

   private: System::Windows::Forms::Label^  label15;
   private: System::Windows::Forms::Label^  label11;
   private: System::Windows::Forms::GroupBox^  groupBox12;
   private: System::Windows::Forms::Label^  label12;
   private: System::Windows::Forms::TextBox^  textChannelPeriodByHz;
   private: System::Windows::Forms::Label^  label18;
   private: System::Windows::Forms::Button^  buttonLogs;
   private: System::Windows::Forms::ComboBox^  comboPath;
   private: System::Windows::Forms::Label^  labelLogs;
   private: System::Windows::Forms::FolderBrowserDialog^  selectFolderDialog;
   private: System::Windows::Forms::GroupBox^  groupBox13;
   private: System::Windows::Forms::NumericUpDown^  numProximityBin;
   private: System::Windows::Forms::Label^  labelProximity;
   private: System::Windows::Forms::Label^  label19;
   private: System::Windows::Forms::Label^  labelAppSpecific;

   private: System::Windows::Forms::NumericUpDown^  numUndiscoverableAppSpecific;

   private: System::Windows::Forms::Label^  label20;
   private: System::Windows::Forms::NumericUpDown^  numUndiscoverableTime;
   private: System::Windows::Forms::NumericUpDown^  numBlockSize;

   private: System::Windows::Forms::CheckBox^  checkBlockSize;
   private: System::Windows::Forms::Label^  label21;
   private: System::Windows::Forms::Button^  btnApply;




   public: 
      static Auth^ instance;
      


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Auth();
      
   private: Auth();
   private: System::Windows::Forms::Button^  btnOK;

   protected: 



   private: System::Windows::Forms::Button^  btnCancel;
   private: System::Windows::Forms::GroupBox^  groupBox8;
   private: System::Windows::Forms::TextBox^  textNetKey8;
   private: System::Windows::Forms::TextBox^  textNetKey7;
   private: System::Windows::Forms::TextBox^  textNetKey6;
   private: System::Windows::Forms::TextBox^  textNetKey5;
   private: System::Windows::Forms::TextBox^  textNetKey4;
   private: System::Windows::Forms::TextBox^  textNetKey3;
   private: System::Windows::Forms::TextBox^  textNetKey2;
   private: System::Windows::Forms::TextBox^  textNetKey1;
   private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
   private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
   private: System::Windows::Forms::GroupBox^  groupBox10;
   private: System::Windows::Forms::TextBox^  textFriendlyName;



   private: System::Windows::Forms::Label^  label1;














   protected: 

	private:
		/// <summary>
      OptionsStruct^ pstMyOptionsStruct;
      
      void OKClick();
      void ApplyClick();
      
      void ComboQuickOptionsChanged();
      void ComboDebugPathChanged();
      void NumBoxUndiscoverableTimeChanged();
      BOOL ToGui(OptionsStruct^ pstOptions_);
      BOOL FromGui(OptionsStruct^ pstOptions_);


      void UpdateHz();
      void UpdatePeriod();
      void ShowLogsDirectory();
      
      private: System::Windows::Forms::Label^  label2;
      private: System::Windows::Forms::Label^  label3;
      private: System::Windows::Forms::Label^  label4;
      private: System::Windows::Forms::TextBox^  textChannelDeviceID;
      private: System::Windows::Forms::TextBox^  textChannelDeviceType;
      private: System::Windows::Forms::TextBox^  textChannelTransType;
      private: System::Windows::Forms::Label^  label5;
      private: System::Windows::Forms::Label^  label6;
      private: System::Windows::Forms::TextBox^  textChannelSearchFreq;
      private: System::Windows::Forms::TextBox^  textChannelLinkFreq;
      private: System::Windows::Forms::Label^  label7;
      private: System::Windows::Forms::Label^  label8;
      private: System::Windows::Forms::Label^  label9;
      private: System::Windows::Forms::TextBox^  textClientManufID;
      private: System::Windows::Forms::TextBox^  textClientDeviceType;
      private: System::Windows::Forms::TextBox^  textClientDeviceID;
      private: System::Windows::Forms::CheckBox^  checkAutoInit;
      private: System::Windows::Forms::GroupBox^  groupBox1;
      private: System::Windows::Forms::ComboBox^  comboOptions;
      private: System::Windows::Forms::TabControl^  tabControl1;
      private: System::Windows::Forms::TabPage^  tabPage1;
      private: System::Windows::Forms::TabPage^  tabPage2;
      private: System::Windows::Forms::TabPage^  tabPage3;
      private: System::Windows::Forms::GroupBox^  groupBox5;
      private: System::Windows::Forms::GroupBox^  groupBox4;
      private: System::Windows::Forms::GroupBox^  groupBox2;
      private: System::Windows::Forms::TextBox^  textChannelPeriodByPeriod;

      private: System::Windows::Forms::GroupBox^  groupBox3;
      private: System::Windows::Forms::GroupBox^  groupBox6;
      private: System::Windows::Forms::Button^  buttonSave;
      private: System::Windows::Forms::Button^  buttonLoad;
      private: System::Windows::Forms::GroupBox^  groupBox7;
      private: System::Windows::Forms::ComboBox^  comboPort;
      private: System::Windows::Forms::ComboBox^  comboBaud;
      private: System::Windows::Forms::Label^  label13;
      private: System::Windows::Forms::Label^  label10;
      private: System::Windows::Forms::CheckBox^  checkDebug;
      private: System::Windows::Forms::Label^  label14;
      private: System::Windows::Forms::CheckBox^  checkRandom;
      
      /// Required designer variable.
	   /// </summary>
	   System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
         System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Auth::typeid));
         this->btnOK = (gcnew System::Windows::Forms::Button());
         this->btnCancel = (gcnew System::Windows::Forms::Button());
         this->label2 = (gcnew System::Windows::Forms::Label());
         this->label3 = (gcnew System::Windows::Forms::Label());
         this->label4 = (gcnew System::Windows::Forms::Label());
         this->textChannelDeviceID = (gcnew System::Windows::Forms::TextBox());
         this->textChannelDeviceType = (gcnew System::Windows::Forms::TextBox());
         this->textChannelTransType = (gcnew System::Windows::Forms::TextBox());
         this->label5 = (gcnew System::Windows::Forms::Label());
         this->label6 = (gcnew System::Windows::Forms::Label());
         this->textChannelSearchFreq = (gcnew System::Windows::Forms::TextBox());
         this->textChannelLinkFreq = (gcnew System::Windows::Forms::TextBox());
         this->label7 = (gcnew System::Windows::Forms::Label());
         this->label8 = (gcnew System::Windows::Forms::Label());
         this->label9 = (gcnew System::Windows::Forms::Label());
         this->textClientManufID = (gcnew System::Windows::Forms::TextBox());
         this->textClientDeviceType = (gcnew System::Windows::Forms::TextBox());
         this->textClientDeviceID = (gcnew System::Windows::Forms::TextBox());
         this->checkAutoInit = (gcnew System::Windows::Forms::CheckBox());
         this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
         this->comboPath = (gcnew System::Windows::Forms::ComboBox());
         this->labelLogs = (gcnew System::Windows::Forms::Label());
         this->buttonLogs = (gcnew System::Windows::Forms::Button());
         this->checkDebug = (gcnew System::Windows::Forms::CheckBox());
         this->comboOptions = (gcnew System::Windows::Forms::ComboBox());
         this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
         this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
         this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
         this->label13 = (gcnew System::Windows::Forms::Label());
         this->label10 = (gcnew System::Windows::Forms::Label());
         this->comboPort = (gcnew System::Windows::Forms::ComboBox());
         this->comboBaud = (gcnew System::Windows::Forms::ComboBox());
         this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
         this->label1 = (gcnew System::Windows::Forms::Label());
         this->buttonSave = (gcnew System::Windows::Forms::Button());
         this->buttonLoad = (gcnew System::Windows::Forms::Button());
         this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
         this->groupBox13 = (gcnew System::Windows::Forms::GroupBox());
         this->labelProximity = (gcnew System::Windows::Forms::Label());
         this->numProximityBin = (gcnew System::Windows::Forms::NumericUpDown());
         this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
         this->textNetKey8 = (gcnew System::Windows::Forms::TextBox());
         this->textNetKey7 = (gcnew System::Windows::Forms::TextBox());
         this->textNetKey6 = (gcnew System::Windows::Forms::TextBox());
         this->textNetKey5 = (gcnew System::Windows::Forms::TextBox());
         this->textNetKey4 = (gcnew System::Windows::Forms::TextBox());
         this->textNetKey3 = (gcnew System::Windows::Forms::TextBox());
         this->textNetKey2 = (gcnew System::Windows::Forms::TextBox());
         this->textNetKey1 = (gcnew System::Windows::Forms::TextBox());
         this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
         this->textChannelPeriodByHz = (gcnew System::Windows::Forms::TextBox());
         this->label18 = (gcnew System::Windows::Forms::Label());
         this->label12 = (gcnew System::Windows::Forms::Label());
         this->textChannelPeriodByPeriod = (gcnew System::Windows::Forms::TextBox());
         this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
         this->label14 = (gcnew System::Windows::Forms::Label());
         this->checkRandom = (gcnew System::Windows::Forms::CheckBox());
         this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
         this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
         this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
         this->label21 = (gcnew System::Windows::Forms::Label());
         this->numBlockSize = (gcnew System::Windows::Forms::NumericUpDown());
         this->checkBlockSize = (gcnew System::Windows::Forms::CheckBox());
         this->labelAppSpecific = (gcnew System::Windows::Forms::Label());
         this->numUndiscoverableAppSpecific = (gcnew System::Windows::Forms::NumericUpDown());
         this->label20 = (gcnew System::Windows::Forms::Label());
         this->numUndiscoverableTime = (gcnew System::Windows::Forms::NumericUpDown());
         this->label19 = (gcnew System::Windows::Forms::Label());
         this->checkBroadcastMode = (gcnew System::Windows::Forms::CheckBox());
         this->checkPing = (gcnew System::Windows::Forms::CheckBox());
         this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
         this->textFriendlyName = (gcnew System::Windows::Forms::TextBox());
         this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
         this->tabPageSecurity = (gcnew System::Windows::Forms::TabPage());
         this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
         this->label16 = (gcnew System::Windows::Forms::Label());
         this->label17 = (gcnew System::Windows::Forms::Label());
         this->textBox_EncryptionKey2 = (gcnew System::Windows::Forms::TextBox());
         this->textBox_EncryptionKey1 = (gcnew System::Windows::Forms::TextBox());
         this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
         this->textPasskey = (gcnew System::Windows::Forms::TextBox());
         this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
         this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
         this->label15 = (gcnew System::Windows::Forms::Label());
         this->label11 = (gcnew System::Windows::Forms::Label());
         this->selectFolderDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
         this->btnApply = (gcnew System::Windows::Forms::Button());
         this->groupBox1->SuspendLayout();
         this->tabControl1->SuspendLayout();
         this->tabPage1->SuspendLayout();
         this->groupBox7->SuspendLayout();
         this->groupBox6->SuspendLayout();
         this->tabPage2->SuspendLayout();
         this->groupBox13->SuspendLayout();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numProximityBin))->BeginInit();
         this->groupBox8->SuspendLayout();
         this->groupBox5->SuspendLayout();
         this->groupBox4->SuspendLayout();
         this->groupBox2->SuspendLayout();
         this->tabPage3->SuspendLayout();
         this->groupBox12->SuspendLayout();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numBlockSize))->BeginInit();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUndiscoverableAppSpecific))->BeginInit();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUndiscoverableTime))->BeginInit();
         this->groupBox10->SuspendLayout();
         this->groupBox3->SuspendLayout();
         this->tabPageSecurity->SuspendLayout();
         this->groupBox11->SuspendLayout();
         this->groupBox9->SuspendLayout();
         this->SuspendLayout();
         // 
         // btnOK
         // 
         this->btnOK->Location = System::Drawing::Point(116, 413);
         this->btnOK->Name = L"btnOK";
         this->btnOK->Size = System::Drawing::Size(75, 23);
         this->btnOK->TabIndex = 0;
         this->btnOK->Text = L"&OK";
         this->btnOK->UseVisualStyleBackColor = true;
         this->btnOK->Click += gcnew System::EventHandler(this, &Auth::btnOK_Click);
         // 
         // btnCancel
         // 
         this->btnCancel->Location = System::Drawing::Point(197, 413);
         this->btnCancel->Name = L"btnCancel";
         this->btnCancel->Size = System::Drawing::Size(75, 23);
         this->btnCancel->TabIndex = 3;
         this->btnCancel->Text = L"&Cancel";
         this->btnCancel->UseVisualStyleBackColor = true;
         this->btnCancel->Click += gcnew System::EventHandler(this, &Auth::btnCancel_Click);
         // 
         // label2
         // 
         this->label2->AutoSize = true;
         this->label2->Location = System::Drawing::Point(17, 24);
         this->label2->Name = L"label2";
         this->label2->Size = System::Drawing::Size(55, 13);
         this->label2->TabIndex = 4;
         this->label2->Text = L"Device ID";
         // 
         // label3
         // 
         this->label3->AutoSize = true;
         this->label3->Location = System::Drawing::Point(17, 51);
         this->label3->Name = L"label3";
         this->label3->Size = System::Drawing::Size(68, 13);
         this->label3->TabIndex = 5;
         this->label3->Text = L"Device Type";
         // 
         // label4
         // 
         this->label4->AutoSize = true;
         this->label4->Location = System::Drawing::Point(17, 75);
         this->label4->Name = L"label4";
         this->label4->Size = System::Drawing::Size(95, 13);
         this->label4->TabIndex = 6;
         this->label4->Text = L"Transmission Type";
         // 
         // textChannelDeviceID
         // 
         this->textChannelDeviceID->Location = System::Drawing::Point(199, 19);
         this->textChannelDeviceID->Name = L"textChannelDeviceID";
         this->textChannelDeviceID->Size = System::Drawing::Size(75, 20);
         this->textChannelDeviceID->TabIndex = 7;
         // 
         // textChannelDeviceType
         // 
         this->textChannelDeviceType->Location = System::Drawing::Point(199, 44);
         this->textChannelDeviceType->Name = L"textChannelDeviceType";
         this->textChannelDeviceType->Size = System::Drawing::Size(75, 20);
         this->textChannelDeviceType->TabIndex = 8;
         // 
         // textChannelTransType
         // 
         this->textChannelTransType->Location = System::Drawing::Point(199, 70);
         this->textChannelTransType->Name = L"textChannelTransType";
         this->textChannelTransType->Size = System::Drawing::Size(75, 20);
         this->textChannelTransType->TabIndex = 9;
         // 
         // label5
         // 
         this->label5->AutoSize = true;
         this->label5->Location = System::Drawing::Point(16, 26);
         this->label5->Name = L"label5";
         this->label5->Size = System::Drawing::Size(94, 13);
         this->label5->TabIndex = 10;
         this->label5->Text = L"Search Frequency";
         // 
         // label6
         // 
         this->label6->AutoSize = true;
         this->label6->Location = System::Drawing::Point(17, 52);
         this->label6->Name = L"label6";
         this->label6->Size = System::Drawing::Size(100, 13);
         this->label6->TabIndex = 11;
         this->label6->Text = L"Connect Frequency";
         // 
         // textChannelSearchFreq
         // 
         this->textChannelSearchFreq->Location = System::Drawing::Point(199, 19);
         this->textChannelSearchFreq->Name = L"textChannelSearchFreq";
         this->textChannelSearchFreq->Size = System::Drawing::Size(75, 20);
         this->textChannelSearchFreq->TabIndex = 12;
         // 
         // textChannelLinkFreq
         // 
         this->textChannelLinkFreq->Location = System::Drawing::Point(199, 45);
         this->textChannelLinkFreq->Name = L"textChannelLinkFreq";
         this->textChannelLinkFreq->Size = System::Drawing::Size(75, 20);
         this->textChannelLinkFreq->TabIndex = 13;
         // 
         // label7
         // 
         this->label7->AutoSize = true;
         this->label7->Location = System::Drawing::Point(17, 25);
         this->label7->Name = L"label7";
         this->label7->Size = System::Drawing::Size(89, 13);
         this->label7->TabIndex = 14;
         this->label7->Text = L"Manufacturing ID";
         // 
         // label8
         // 
         this->label8->AutoSize = true;
         this->label8->Location = System::Drawing::Point(17, 51);
         this->label8->Name = L"label8";
         this->label8->Size = System::Drawing::Size(68, 13);
         this->label8->TabIndex = 15;
         this->label8->Text = L"Device Type";
         // 
         // label9
         // 
         this->label9->AutoSize = true;
         this->label9->Location = System::Drawing::Point(17, 77);
         this->label9->Name = L"label9";
         this->label9->Size = System::Drawing::Size(55, 13);
         this->label9->TabIndex = 16;
         this->label9->Text = L"Device ID";
         // 
         // textClientManufID
         // 
         this->textClientManufID->Location = System::Drawing::Point(191, 22);
         this->textClientManufID->Name = L"textClientManufID";
         this->textClientManufID->Size = System::Drawing::Size(75, 20);
         this->textClientManufID->TabIndex = 17;
         // 
         // textClientDeviceType
         // 
         this->textClientDeviceType->Location = System::Drawing::Point(191, 48);
         this->textClientDeviceType->Name = L"textClientDeviceType";
         this->textClientDeviceType->Size = System::Drawing::Size(75, 20);
         this->textClientDeviceType->TabIndex = 18;
         // 
         // textClientDeviceID
         // 
         this->textClientDeviceID->Location = System::Drawing::Point(191, 74);
         this->textClientDeviceID->Name = L"textClientDeviceID";
         this->textClientDeviceID->Size = System::Drawing::Size(75, 20);
         this->textClientDeviceID->TabIndex = 19;
         // 
         // checkAutoInit
         // 
         this->checkAutoInit->AutoSize = true;
         this->checkAutoInit->Location = System::Drawing::Point(19, 104);
         this->checkAutoInit->Name = L"checkAutoInit";
         this->checkAutoInit->Size = System::Drawing::Size(178, 17);
         this->checkAutoInit->TabIndex = 20;
         this->checkAutoInit->Text = L"Auto Init (ANT USB Stick Only)\?";
         this->checkAutoInit->UseVisualStyleBackColor = true;
         this->checkAutoInit->CheckedChanged += gcnew System::EventHandler(this, &Auth::checkBox1_CheckedChanged);
         // 
         // groupBox1
         // 
         this->groupBox1->Controls->Add(this->comboPath);
         this->groupBox1->Controls->Add(this->labelLogs);
         this->groupBox1->Controls->Add(this->buttonLogs);
         this->groupBox1->Controls->Add(this->checkDebug);
         this->groupBox1->Location = System::Drawing::Point(6, 17);
         this->groupBox1->Name = L"groupBox1";
         this->groupBox1->Size = System::Drawing::Size(280, 78);
         this->groupBox1->TabIndex = 21;
         this->groupBox1->TabStop = false;
         this->groupBox1->Text = L"Run Mode";
         // 
         // comboPath
         // 
         this->comboPath->FormattingEnabled = true;
         this->comboPath->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"User\'s Application Data", L"Select..."});
         this->comboPath->Location = System::Drawing::Point(95, 45);
         this->comboPath->Name = L"comboPath";
         this->comboPath->Size = System::Drawing::Size(164, 21);
         this->comboPath->TabIndex = 30;
         this->comboPath->SelectedIndexChanged += gcnew System::EventHandler(this, &Auth::comboPath_SelectedIndexChanged);
         // 
         // labelLogs
         // 
         this->labelLogs->AutoSize = true;
         this->labelLogs->Location = System::Drawing::Point(17, 48);
         this->labelLogs->Name = L"labelLogs";
         this->labelLogs->Size = System::Drawing::Size(68, 13);
         this->labelLogs->TabIndex = 29;
         this->labelLogs->Text = L"Save logs in:";
         // 
         // buttonLogs
         // 
         this->buttonLogs->Location = System::Drawing::Point(182, 15);
         this->buttonLogs->Name = L"buttonLogs";
         this->buttonLogs->Size = System::Drawing::Size(75, 23);
         this->buttonLogs->TabIndex = 25;
         this->buttonLogs->Text = L"View Logs";
         this->buttonLogs->UseVisualStyleBackColor = true;
         this->buttonLogs->Click += gcnew System::EventHandler(this, &Auth::buttonLogs_Click);
         // 
         // checkDebug
         // 
         this->checkDebug->AutoSize = true;
         this->checkDebug->Location = System::Drawing::Point(20, 19);
         this->checkDebug->Name = L"checkDebug";
         this->checkDebug->Size = System::Drawing::Size(58, 17);
         this->checkDebug->TabIndex = 24;
         this->checkDebug->Text = L"Debug";
         this->checkDebug->UseVisualStyleBackColor = true;
         // 
         // comboOptions
         // 
         this->comboOptions->FormattingEnabled = true;
         this->comboOptions->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"Default", L"FR-50", L"FR-60", L"FR-310XT", 
            L"Fitness Equipment", L"Weight Scale", L"Blood Pressure Monitor"});
         this->comboOptions->Location = System::Drawing::Point(102, 95);
         this->comboOptions->Name = L"comboOptions";
         this->comboOptions->Size = System::Drawing::Size(155, 21);
         this->comboOptions->TabIndex = 23;
         this->comboOptions->SelectedIndexChanged += gcnew System::EventHandler(this, &Auth::comboOptions_SelectedIndexChanged);
         // 
         // tabControl1
         // 
         this->tabControl1->Controls->Add(this->tabPage1);
         this->tabControl1->Controls->Add(this->tabPage2);
         this->tabControl1->Controls->Add(this->tabPage3);
         this->tabControl1->Controls->Add(this->tabPageSecurity);
         this->tabControl1->Location = System::Drawing::Point(4, 3);
         this->tabControl1->Name = L"tabControl1";
         this->tabControl1->SelectedIndex = 0;
         this->tabControl1->Size = System::Drawing::Size(300, 404);
         this->tabControl1->TabIndex = 26;
         // 
         // tabPage1
         // 
         this->tabPage1->Controls->Add(this->groupBox7);
         this->tabPage1->Controls->Add(this->groupBox6);
         this->tabPage1->Controls->Add(this->groupBox1);
         this->tabPage1->Location = System::Drawing::Point(4, 22);
         this->tabPage1->Name = L"tabPage1";
         this->tabPage1->Padding = System::Windows::Forms::Padding(3);
         this->tabPage1->Size = System::Drawing::Size(292, 378);
         this->tabPage1->TabIndex = 0;
         this->tabPage1->Text = L"General";
         this->tabPage1->UseVisualStyleBackColor = true;
         // 
         // groupBox7
         // 
         this->groupBox7->Controls->Add(this->label13);
         this->groupBox7->Controls->Add(this->label10);
         this->groupBox7->Controls->Add(this->comboPort);
         this->groupBox7->Controls->Add(this->comboBaud);
         this->groupBox7->Controls->Add(this->checkAutoInit);
         this->groupBox7->Location = System::Drawing::Point(7, 242);
         this->groupBox7->Name = L"groupBox7";
         this->groupBox7->Size = System::Drawing::Size(279, 130);
         this->groupBox7->TabIndex = 25;
         this->groupBox7->TabStop = false;
         this->groupBox7->Text = L"USB";
         // 
         // label13
         // 
         this->label13->AutoSize = true;
         this->label13->Location = System::Drawing::Point(43, 62);
         this->label13->Name = L"label13";
         this->label13->Size = System::Drawing::Size(50, 13);
         this->label13->TabIndex = 24;
         this->label13->Text = L"USB port";
         // 
         // label10
         // 
         this->label10->AutoSize = true;
         this->label10->Location = System::Drawing::Point(43, 26);
         this->label10->Name = L"label10";
         this->label10->Size = System::Drawing::Size(58, 13);
         this->label10->TabIndex = 23;
         this->label10->Text = L"Baud Rate";
         // 
         // comboPort
         // 
         this->comboPort->FormattingEnabled = true;
         this->comboPort->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", 
            L"8", L"9"});
         this->comboPort->Location = System::Drawing::Point(135, 57);
         this->comboPort->Name = L"comboPort";
         this->comboPort->Size = System::Drawing::Size(121, 21);
         this->comboPort->TabIndex = 22;
         // 
         // comboBaud
         // 
         this->comboBaud->FormattingEnabled = true;
         this->comboBaud->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"50000(AP1)", L"57600(AP2)", L"57600(AT3)"});
         this->comboBaud->Location = System::Drawing::Point(135, 21);
         this->comboBaud->Name = L"comboBaud";
         this->comboBaud->Size = System::Drawing::Size(121, 21);
         this->comboBaud->TabIndex = 21;
         // 
         // groupBox6
         // 
         this->groupBox6->Controls->Add(this->label1);
         this->groupBox6->Controls->Add(this->buttonSave);
         this->groupBox6->Controls->Add(this->comboOptions);
         this->groupBox6->Controls->Add(this->buttonLoad);
         this->groupBox6->Location = System::Drawing::Point(6, 101);
         this->groupBox6->Name = L"groupBox6";
         this->groupBox6->Size = System::Drawing::Size(280, 135);
         this->groupBox6->TabIndex = 24;
         this->groupBox6->TabStop = false;
         this->groupBox6->Text = L"Configuration";
         // 
         // label1
         // 
         this->label1->AutoSize = true;
         this->label1->Location = System::Drawing::Point(26, 98);
         this->label1->Name = L"label1";
         this->label1->Size = System::Drawing::Size(68, 13);
         this->label1->TabIndex = 24;
         this->label1->Text = L"Quick Config";
         // 
         // buttonSave
         // 
         this->buttonSave->Location = System::Drawing::Point(27, 58);
         this->buttonSave->Name = L"buttonSave";
         this->buttonSave->Size = System::Drawing::Size(75, 23);
         this->buttonSave->TabIndex = 23;
         this->buttonSave->Text = L"&Save";
         this->buttonSave->UseVisualStyleBackColor = true;
         this->buttonSave->Click += gcnew System::EventHandler(this, &Auth::buttonSave_Click);
         // 
         // buttonLoad
         // 
         this->buttonLoad->Location = System::Drawing::Point(27, 29);
         this->buttonLoad->Name = L"buttonLoad";
         this->buttonLoad->Size = System::Drawing::Size(75, 23);
         this->buttonLoad->TabIndex = 22;
         this->buttonLoad->Text = L"&Load";
         this->buttonLoad->UseVisualStyleBackColor = true;
         this->buttonLoad->Click += gcnew System::EventHandler(this, &Auth::buttonLoad_Click);
         // 
         // tabPage2
         // 
         this->tabPage2->Controls->Add(this->groupBox13);
         this->tabPage2->Controls->Add(this->groupBox8);
         this->tabPage2->Controls->Add(this->groupBox5);
         this->tabPage2->Controls->Add(this->groupBox4);
         this->tabPage2->Controls->Add(this->groupBox2);
         this->tabPage2->Location = System::Drawing::Point(4, 22);
         this->tabPage2->Name = L"tabPage2";
         this->tabPage2->Padding = System::Windows::Forms::Padding(3);
         this->tabPage2->Size = System::Drawing::Size(292, 378);
         this->tabPage2->TabIndex = 1;
         this->tabPage2->Text = L"ANT Channel";
         this->tabPage2->UseVisualStyleBackColor = true;
         // 
         // groupBox13
         // 
         this->groupBox13->Controls->Add(this->labelProximity);
         this->groupBox13->Controls->Add(this->numProximityBin);
         this->groupBox13->Location = System::Drawing::Point(7, 264);
         this->groupBox13->Name = L"groupBox13";
         this->groupBox13->Size = System::Drawing::Size(279, 52);
         this->groupBox13->TabIndex = 14;
         this->groupBox13->TabStop = false;
         this->groupBox13->Text = L"Advanced";
         // 
         // labelProximity
         // 
         this->labelProximity->AutoSize = true;
         this->labelProximity->Location = System::Drawing::Point(7, 20);
         this->labelProximity->Name = L"labelProximity";
         this->labelProximity->Size = System::Drawing::Size(103, 13);
         this->labelProximity->TabIndex = 1;
         this->labelProximity->Text = L"Proximity Search Bin";
         // 
         // numProximityBin
         // 
         this->numProximityBin->Location = System::Drawing::Point(115, 18);
         this->numProximityBin->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
         this->numProximityBin->Name = L"numProximityBin";
         this->numProximityBin->Size = System::Drawing::Size(45, 20);
         this->numProximityBin->TabIndex = 0;
         // 
         // groupBox8
         // 
         this->groupBox8->Controls->Add(this->textNetKey8);
         this->groupBox8->Controls->Add(this->textNetKey7);
         this->groupBox8->Controls->Add(this->textNetKey6);
         this->groupBox8->Controls->Add(this->textNetKey5);
         this->groupBox8->Controls->Add(this->textNetKey4);
         this->groupBox8->Controls->Add(this->textNetKey3);
         this->groupBox8->Controls->Add(this->textNetKey2);
         this->groupBox8->Controls->Add(this->textNetKey1);
         this->groupBox8->Location = System::Drawing::Point(7, 322);
         this->groupBox8->Name = L"groupBox8";
         this->groupBox8->Size = System::Drawing::Size(279, 50);
         this->groupBox8->TabIndex = 13;
         this->groupBox8->TabStop = false;
         this->groupBox8->Text = L"Network Key";
         // 
         // textNetKey8
         // 
         this->textNetKey8->Location = System::Drawing::Point(191, 19);
         this->textNetKey8->MaxLength = 2;
         this->textNetKey8->Name = L"textNetKey8";
         this->textNetKey8->Size = System::Drawing::Size(22, 20);
         this->textNetKey8->TabIndex = 7;
         this->textNetKey8->Text = L"00";
         this->textNetKey8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // textNetKey7
         // 
         this->textNetKey7->Location = System::Drawing::Point(166, 19);
         this->textNetKey7->MaxLength = 2;
         this->textNetKey7->Name = L"textNetKey7";
         this->textNetKey7->Size = System::Drawing::Size(22, 20);
         this->textNetKey7->TabIndex = 6;
         this->textNetKey7->Text = L"00";
         this->textNetKey7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // textNetKey6
         // 
         this->textNetKey6->Location = System::Drawing::Point(141, 19);
         this->textNetKey6->MaxLength = 2;
         this->textNetKey6->Name = L"textNetKey6";
         this->textNetKey6->Size = System::Drawing::Size(22, 20);
         this->textNetKey6->TabIndex = 5;
         this->textNetKey6->Text = L"00";
         this->textNetKey6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // textNetKey5
         // 
         this->textNetKey5->Location = System::Drawing::Point(117, 19);
         this->textNetKey5->MaxLength = 2;
         this->textNetKey5->Name = L"textNetKey5";
         this->textNetKey5->Size = System::Drawing::Size(22, 20);
         this->textNetKey5->TabIndex = 4;
         this->textNetKey5->Text = L"00";
         this->textNetKey5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // textNetKey4
         // 
         this->textNetKey4->Location = System::Drawing::Point(92, 19);
         this->textNetKey4->MaxLength = 2;
         this->textNetKey4->Name = L"textNetKey4";
         this->textNetKey4->Size = System::Drawing::Size(22, 20);
         this->textNetKey4->TabIndex = 3;
         this->textNetKey4->Text = L"00";
         this->textNetKey4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // textNetKey3
         // 
         this->textNetKey3->Location = System::Drawing::Point(67, 19);
         this->textNetKey3->MaxLength = 2;
         this->textNetKey3->Name = L"textNetKey3";
         this->textNetKey3->Size = System::Drawing::Size(22, 20);
         this->textNetKey3->TabIndex = 2;
         this->textNetKey3->Text = L"00";
         this->textNetKey3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // textNetKey2
         // 
         this->textNetKey2->Location = System::Drawing::Point(42, 19);
         this->textNetKey2->MaxLength = 2;
         this->textNetKey2->Name = L"textNetKey2";
         this->textNetKey2->Size = System::Drawing::Size(22, 20);
         this->textNetKey2->TabIndex = 1;
         this->textNetKey2->Text = L"00";
         this->textNetKey2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // textNetKey1
         // 
         this->textNetKey1->Location = System::Drawing::Point(17, 19);
         this->textNetKey1->MaxLength = 2;
         this->textNetKey1->Name = L"textNetKey1";
         this->textNetKey1->Size = System::Drawing::Size(22, 20);
         this->textNetKey1->TabIndex = 0;
         this->textNetKey1->Text = L"00";
         this->textNetKey1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // groupBox5
         // 
         this->groupBox5->Controls->Add(this->textChannelPeriodByHz);
         this->groupBox5->Controls->Add(this->label18);
         this->groupBox5->Controls->Add(this->label12);
         this->groupBox5->Controls->Add(this->textChannelPeriodByPeriod);
         this->groupBox5->Location = System::Drawing::Point(7, 210);
         this->groupBox5->Name = L"groupBox5";
         this->groupBox5->Size = System::Drawing::Size(279, 48);
         this->groupBox5->TabIndex = 12;
         this->groupBox5->TabStop = false;
         this->groupBox5->Text = L"Channel Period";
         // 
         // textChannelPeriodByHz
         // 
         this->textChannelPeriodByHz->Location = System::Drawing::Point(191, 18);
         this->textChannelPeriodByHz->Name = L"textChannelPeriodByHz";
         this->textChannelPeriodByHz->Size = System::Drawing::Size(49, 20);
         this->textChannelPeriodByHz->TabIndex = 19;
         this->textChannelPeriodByHz->Text = L"4";
         this->textChannelPeriodByHz->TextChanged += gcnew System::EventHandler(this, &Auth::textChannelPeriodByHz_TextChanged);
         // 
         // label18
         // 
         this->label18->AutoSize = true;
         this->label18->Location = System::Drawing::Point(240, 21);
         this->label18->Name = L"label18";
         this->label18->Size = System::Drawing::Size(20, 13);
         this->label18->TabIndex = 18;
         this->label18->Text = L"Hz";
         // 
         // label12
         // 
         this->label12->AutoSize = true;
         this->label12->Location = System::Drawing::Point(89, 21);
         this->label12->Name = L"label12";
         this->label12->Size = System::Drawing::Size(97, 13);
         this->label12->TabIndex = 17;
         this->label12->Text = L"/32768[s] period or";
         // 
         // textChannelPeriodByPeriod
         // 
         this->textChannelPeriodByPeriod->Location = System::Drawing::Point(19, 18);
         this->textChannelPeriodByPeriod->Name = L"textChannelPeriodByPeriod";
         this->textChannelPeriodByPeriod->Size = System::Drawing::Size(70, 20);
         this->textChannelPeriodByPeriod->TabIndex = 16;
         this->textChannelPeriodByPeriod->Text = L"8192";
         this->textChannelPeriodByPeriod->TextChanged += gcnew System::EventHandler(this, &Auth::textChannelPeriodByPeriod_TextChanged);
         // 
         // groupBox4
         // 
         this->groupBox4->Controls->Add(this->label14);
         this->groupBox4->Controls->Add(this->checkRandom);
         this->groupBox4->Controls->Add(this->textChannelSearchFreq);
         this->groupBox4->Controls->Add(this->label5);
         this->groupBox4->Controls->Add(this->textChannelLinkFreq);
         this->groupBox4->Controls->Add(this->label6);
         this->groupBox4->Location = System::Drawing::Point(7, 107);
         this->groupBox4->Name = L"groupBox4";
         this->groupBox4->Size = System::Drawing::Size(279, 99);
         this->groupBox4->TabIndex = 11;
         this->groupBox4->TabStop = false;
         this->groupBox4->Text = L"RF Frequency";
         // 
         // label14
         // 
         this->label14->AutoSize = true;
         this->label14->Location = System::Drawing::Point(17, 76);
         this->label14->Name = L"label14";
         this->label14->Size = System::Drawing::Size(143, 13);
         this->label14->TabIndex = 15;
         this->label14->Text = L"Random Connect Frequency";
         // 
         // checkRandom
         // 
         this->checkRandom->AutoSize = true;
         this->checkRandom->Location = System::Drawing::Point(198, 76);
         this->checkRandom->Name = L"checkRandom";
         this->checkRandom->Size = System::Drawing::Size(15, 14);
         this->checkRandom->TabIndex = 14;
         this->checkRandom->UseVisualStyleBackColor = true;
         this->checkRandom->CheckedChanged += gcnew System::EventHandler(this, &Auth::checkRandom_CheckedChanged);
         // 
         // groupBox2
         // 
         this->groupBox2->Controls->Add(this->label3);
         this->groupBox2->Controls->Add(this->textChannelDeviceID);
         this->groupBox2->Controls->Add(this->label2);
         this->groupBox2->Controls->Add(this->textChannelTransType);
         this->groupBox2->Controls->Add(this->label4);
         this->groupBox2->Controls->Add(this->textChannelDeviceType);
         this->groupBox2->Location = System::Drawing::Point(6, 6);
         this->groupBox2->Name = L"groupBox2";
         this->groupBox2->Size = System::Drawing::Size(280, 98);
         this->groupBox2->TabIndex = 10;
         this->groupBox2->TabStop = false;
         this->groupBox2->Text = L"Channel ID";
         // 
         // tabPage3
         // 
         this->tabPage3->Controls->Add(this->groupBox12);
         this->tabPage3->Controls->Add(this->groupBox10);
         this->tabPage3->Controls->Add(this->groupBox3);
         this->tabPage3->Location = System::Drawing::Point(4, 22);
         this->tabPage3->Name = L"tabPage3";
         this->tabPage3->Padding = System::Windows::Forms::Padding(3);
         this->tabPage3->Size = System::Drawing::Size(292, 378);
         this->tabPage3->TabIndex = 2;
         this->tabPage3->Text = L"Host Parameters";
         this->tabPage3->UseVisualStyleBackColor = true;
         // 
         // groupBox12
         // 
         this->groupBox12->Controls->Add(this->label21);
         this->groupBox12->Controls->Add(this->numBlockSize);
         this->groupBox12->Controls->Add(this->checkBlockSize);
         this->groupBox12->Controls->Add(this->labelAppSpecific);
         this->groupBox12->Controls->Add(this->numUndiscoverableAppSpecific);
         this->groupBox12->Controls->Add(this->label20);
         this->groupBox12->Controls->Add(this->numUndiscoverableTime);
         this->groupBox12->Controls->Add(this->label19);
         this->groupBox12->Controls->Add(this->checkBroadcastMode);
         this->groupBox12->Controls->Add(this->checkPing);
         this->groupBox12->Location = System::Drawing::Point(7, 109);
         this->groupBox12->Name = L"groupBox12";
         this->groupBox12->Size = System::Drawing::Size(279, 181);
         this->groupBox12->TabIndex = 23;
         this->groupBox12->TabStop = false;
         this->groupBox12->Text = L"Host Features";
         // 
         // label21
         // 
         this->label21->AutoSize = true;
         this->label21->Location = System::Drawing::Point(216, 139);
         this->label21->Name = L"label21";
         this->label21->Size = System::Drawing::Size(32, 13);
         this->label21->TabIndex = 71;
         this->label21->Text = L"bytes";
         // 
         // numBlockSize
         // 
         this->numBlockSize->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {8, 0, 0, 0});
         this->numBlockSize->Location = System::Drawing::Point(125, 137);
         this->numBlockSize->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {-1, 0, 0, 0});
         this->numBlockSize->Name = L"numBlockSize";
         this->numBlockSize->Size = System::Drawing::Size(85, 20);
         this->numBlockSize->TabIndex = 70;
         // 
         // checkBlockSize
         // 
         this->checkBlockSize->AutoSize = true;
         this->checkBlockSize->Location = System::Drawing::Point(19, 138);
         this->checkBlockSize->Name = L"checkBlockSize";
         this->checkBlockSize->Size = System::Drawing::Size(109, 17);
         this->checkBlockSize->TabIndex = 69;
         this->checkBlockSize->Text = L"Limit block size to";
         this->checkBlockSize->UseVisualStyleBackColor = true;
         this->checkBlockSize->CheckedChanged += gcnew System::EventHandler(this, &Auth::checkBlockSize_CheckedChanged);
         // 
         // labelAppSpecific
         // 
         this->labelAppSpecific->AutoSize = true;
         this->labelAppSpecific->Location = System::Drawing::Point(97, 114);
         this->labelAppSpecific->Name = L"labelAppSpecific";
         this->labelAppSpecific->Size = System::Drawing::Size(138, 13);
         this->labelAppSpecific->TabIndex = 68;
         this->labelAppSpecific->Text = L"application specific duration";
         // 
         // numUndiscoverableAppSpecific
         // 
         this->numUndiscoverableAppSpecific->Location = System::Drawing::Point(9, 112);
         this->numUndiscoverableAppSpecific->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
         this->numUndiscoverableAppSpecific->Name = L"numUndiscoverableAppSpecific";
         this->numUndiscoverableAppSpecific->Size = System::Drawing::Size(75, 20);
         this->numUndiscoverableAppSpecific->TabIndex = 67;
         // 
         // label20
         // 
         this->label20->AutoSize = true;
         this->label20->Location = System::Drawing::Point(97, 89);
         this->label20->Name = L"label20";
         this->label20->Size = System::Drawing::Size(82, 13);
         this->label20->TabIndex = 66;
         this->label20->Text = L"seconds and/or";
         // 
         // numUndiscoverableTime
         // 
         this->numUndiscoverableTime->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
         this->numUndiscoverableTime->Location = System::Drawing::Point(9, 87);
         this->numUndiscoverableTime->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {7650, 0, 0, 0});
         this->numUndiscoverableTime->Name = L"numUndiscoverableTime";
         this->numUndiscoverableTime->Size = System::Drawing::Size(75, 20);
         this->numUndiscoverableTime->TabIndex = 65;
         this->numUndiscoverableTime->ValueChanged += gcnew System::EventHandler(this, &Auth::numUndiscoverableTime_ValueChanged);
         // 
         // label19
         // 
         this->label19->AutoSize = true;
         this->label19->Location = System::Drawing::Point(6, 65);
         this->label19->Name = L"label19";
         this->label19->Size = System::Drawing::Size(270, 13);
         this->label19->TabIndex = 64;
         this->label19->Text = L"On disconnect, ask client to become undiscoverable for";
         // 
         // checkBroadcastMode
         // 
         this->checkBroadcastMode->AutoSize = true;
         this->checkBroadcastMode->Location = System::Drawing::Point(19, 19);
         this->checkBroadcastMode->Name = L"checkBroadcastMode";
         this->checkBroadcastMode->Size = System::Drawing::Size(151, 17);
         this->checkBroadcastMode->TabIndex = 20;
         this->checkBroadcastMode->Text = L"Enable Broadcast ANT-FS";
         this->checkBroadcastMode->UseVisualStyleBackColor = true;
         // 
         // checkPing
         // 
         this->checkPing->AutoSize = true;
         this->checkPing->Checked = true;
         this->checkPing->CheckState = System::Windows::Forms::CheckState::Checked;
         this->checkPing->Location = System::Drawing::Point(19, 42);
         this->checkPing->Name = L"checkPing";
         this->checkPing->Size = System::Drawing::Size(83, 17);
         this->checkPing->TabIndex = 63;
         this->checkPing->Text = L"Enable Ping";
         this->checkPing->UseVisualStyleBackColor = true;
         // 
         // groupBox10
         // 
         this->groupBox10->Controls->Add(this->textFriendlyName);
         this->groupBox10->Location = System::Drawing::Point(7, 296);
         this->groupBox10->Name = L"groupBox10";
         this->groupBox10->Size = System::Drawing::Size(279, 72);
         this->groupBox10->TabIndex = 22;
         this->groupBox10->TabStop = false;
         this->groupBox10->Text = L"Host Friendly Name";
         // 
         // textFriendlyName
         // 
         this->textFriendlyName->Location = System::Drawing::Point(19, 31);
         this->textFriendlyName->MaxLength = 16;
         this->textFriendlyName->Name = L"textFriendlyName";
         this->textFriendlyName->Size = System::Drawing::Size(246, 20);
         this->textFriendlyName->TabIndex = 0;
         // 
         // groupBox3
         // 
         this->groupBox3->Controls->Add(this->label8);
         this->groupBox3->Controls->Add(this->textClientDeviceID);
         this->groupBox3->Controls->Add(this->label7);
         this->groupBox3->Controls->Add(this->textClientDeviceType);
         this->groupBox3->Controls->Add(this->textClientManufID);
         this->groupBox3->Controls->Add(this->label9);
         this->groupBox3->Location = System::Drawing::Point(6, 5);
         this->groupBox3->Name = L"groupBox3";
         this->groupBox3->Size = System::Drawing::Size(280, 102);
         this->groupBox3->TabIndex = 20;
         this->groupBox3->TabStop = false;
         this->groupBox3->Text = L"Client Search";
         // 
         // tabPageSecurity
         // 
         this->tabPageSecurity->Controls->Add(this->groupBox11);
         this->tabPageSecurity->Controls->Add(this->groupBox9);
         this->tabPageSecurity->Location = System::Drawing::Point(4, 22);
         this->tabPageSecurity->Name = L"tabPageSecurity";
         this->tabPageSecurity->Padding = System::Windows::Forms::Padding(3);
         this->tabPageSecurity->Size = System::Drawing::Size(292, 378);
         this->tabPageSecurity->TabIndex = 3;
         this->tabPageSecurity->Text = L"Security";
         this->tabPageSecurity->UseVisualStyleBackColor = true;
         // 
         // groupBox11
         // 
         this->groupBox11->Controls->Add(this->label16);
         this->groupBox11->Controls->Add(this->label17);
         this->groupBox11->Controls->Add(this->textBox_EncryptionKey2);
         this->groupBox11->Controls->Add(this->textBox_EncryptionKey1);
         this->groupBox11->Location = System::Drawing::Point(6, 83);
         this->groupBox11->Name = L"groupBox11";
         this->groupBox11->Size = System::Drawing::Size(280, 102);
         this->groupBox11->TabIndex = 23;
         this->groupBox11->TabStop = false;
         this->groupBox11->Text = L"Encryption Key (32 bytes hex)";
         // 
         // label16
         // 
         this->label16->AutoSize = true;
         this->label16->Location = System::Drawing::Point(3, 50);
         this->label16->Name = L"label16";
         this->label16->Size = System::Drawing::Size(66, 13);
         this->label16->TabIndex = 4;
         this->label16->Text = L"Bytes 16-31:";
         // 
         // label17
         // 
         this->label17->Location = System::Drawing::Point(4, 14);
         this->label17->Name = L"label17";
         this->label17->Size = System::Drawing::Size(68, 11);
         this->label17->TabIndex = 0;
         this->label17->Text = L"Bytes 0-15:";
         // 
         // textBox_EncryptionKey2
         // 
         this->textBox_EncryptionKey2->Location = System::Drawing::Point(6, 63);
         this->textBox_EncryptionKey2->Name = L"textBox_EncryptionKey2";
         this->textBox_EncryptionKey2->Size = System::Drawing::Size(268, 20);
         this->textBox_EncryptionKey2->TabIndex = 2;
         this->textBox_EncryptionKey2->Text = L"10,11,12,13,14,15,16,17,18,19,1A,1B,1C,1D,1E,1F";
         // 
         // textBox_EncryptionKey1
         // 
         this->textBox_EncryptionKey1->Location = System::Drawing::Point(6, 28);
         this->textBox_EncryptionKey1->Name = L"textBox_EncryptionKey1";
         this->textBox_EncryptionKey1->Size = System::Drawing::Size(268, 20);
         this->textBox_EncryptionKey1->TabIndex = 1;
         this->textBox_EncryptionKey1->Text = L"00,01,02,03,04,05,06,07,08,09,0A,0B,0C,0D,0E,0F";
         // 
         // groupBox9
         // 
         this->groupBox9->Controls->Add(this->textPasskey);
         this->groupBox9->Location = System::Drawing::Point(6, 9);
         this->groupBox9->Name = L"groupBox9";
         this->groupBox9->Size = System::Drawing::Size(280, 67);
         this->groupBox9->TabIndex = 22;
         this->groupBox9->TabStop = false;
         this->groupBox9->Text = L"Enter Passkey in Hex Byte Pairs";
         // 
         // textPasskey
         // 
         this->textPasskey->Location = System::Drawing::Point(6, 30);
         this->textPasskey->Name = L"textPasskey";
         this->textPasskey->Size = System::Drawing::Size(268, 20);
         this->textPasskey->TabIndex = 1;
         this->textPasskey->Text = L"00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00";
         // 
         // saveFileDialog1
         // 
         this->saveFileDialog1->DefaultExt = L"txt";
         this->saveFileDialog1->Filter = L"Text File|*.txt";
         this->saveFileDialog1->RestoreDirectory = true;
         this->saveFileDialog1->Title = L"Save Configuration File";
         this->saveFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Auth::saveFileDialog1_FileOk);
         // 
         // openFileDialog1
         // 
         this->openFileDialog1->DefaultExt = L"txt";
         this->openFileDialog1->FileName = L"openFileDialog1";
         this->openFileDialog1->Filter = L"Text File|*.txt";
         this->openFileDialog1->RestoreDirectory = true;
         this->openFileDialog1->Title = L"Load Config File";
         this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Auth::openFileDialog1_FileOk);
         // 
         // label15
         // 
         this->label15->AutoSize = true;
         this->label15->Location = System::Drawing::Point(3, 50);
         this->label15->Name = L"label15";
         this->label15->Size = System::Drawing::Size(66, 13);
         this->label15->TabIndex = 4;
         this->label15->Text = L"Bytes 16-31:";
         // 
         // label11
         // 
         this->label11->Location = System::Drawing::Point(4, 14);
         this->label11->Name = L"label11";
         this->label11->Size = System::Drawing::Size(68, 11);
         this->label11->TabIndex = 0;
         this->label11->Text = L"Bytes 0-15:";
         // 
         // selectFolderDialog
         // 
         this->selectFolderDialog->RootFolder = System::Environment::SpecialFolder::ApplicationData;
         // 
         // btnApply
         // 
         this->btnApply->Location = System::Drawing::Point(35, 413);
         this->btnApply->Name = L"btnApply";
         this->btnApply->Size = System::Drawing::Size(75, 23);
         this->btnApply->TabIndex = 27;
         this->btnApply->Text = L"&Apply";
         this->btnApply->UseVisualStyleBackColor = true;
         this->btnApply->Click += gcnew System::EventHandler(this, &Auth::btnApply_Click);
         // 
         // Auth
         // 
         this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
         this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
         this->ClientSize = System::Drawing::Size(307, 447);
         this->Controls->Add(this->btnApply);
         this->Controls->Add(this->tabControl1);
         this->Controls->Add(this->btnCancel);
         this->Controls->Add(this->btnOK);
         this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
         this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
         this->MaximizeBox = false;
         this->Name = L"Auth";
         this->Text = L"Options";
         this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Auth::Auth_FormClosing);
         this->groupBox1->ResumeLayout(false);
         this->groupBox1->PerformLayout();
         this->tabControl1->ResumeLayout(false);
         this->tabPage1->ResumeLayout(false);
         this->groupBox7->ResumeLayout(false);
         this->groupBox7->PerformLayout();
         this->groupBox6->ResumeLayout(false);
         this->groupBox6->PerformLayout();
         this->tabPage2->ResumeLayout(false);
         this->groupBox13->ResumeLayout(false);
         this->groupBox13->PerformLayout();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numProximityBin))->EndInit();
         this->groupBox8->ResumeLayout(false);
         this->groupBox8->PerformLayout();
         this->groupBox5->ResumeLayout(false);
         this->groupBox5->PerformLayout();
         this->groupBox4->ResumeLayout(false);
         this->groupBox4->PerformLayout();
         this->groupBox2->ResumeLayout(false);
         this->groupBox2->PerformLayout();
         this->tabPage3->ResumeLayout(false);
         this->groupBox12->ResumeLayout(false);
         this->groupBox12->PerformLayout();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numBlockSize))->EndInit();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUndiscoverableAppSpecific))->EndInit();
         (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numUndiscoverableTime))->EndInit();
         this->groupBox10->ResumeLayout(false);
         this->groupBox10->PerformLayout();
         this->groupBox3->ResumeLayout(false);
         this->groupBox3->PerformLayout();
         this->tabPageSecurity->ResumeLayout(false);
         this->groupBox11->ResumeLayout(false);
         this->groupBox11->PerformLayout();
         this->groupBox9->ResumeLayout(false);
         this->groupBox9->PerformLayout();
         this->ResumeLayout(false);

      }
#pragma endregion
   private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) 
      {
         OKClick();
      }

private: System::Void btnCancel_Click(System::Object^  sender, System::EventArgs^  e) 
      {
         this->Close();
      }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
      {  
         if(checkAutoInit->Checked)
         {
            comboPort->Enabled = FALSE;
            comboBaud->Enabled = FALSE;
         }
         else
         {
            comboPort->Enabled = TRUE;
            comboBaud->Enabled = TRUE;
         }
      }
         

private: System::Void buttonSave_Click(System::Object^  sender, System::EventArgs^  e) 
      {
		   this->saveFileDialog1->ShowDialog();
      }
      
private: System::Void checkRandom_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
      {
         if(checkRandom->Checked)
         {
            textChannelLinkFreq->Text = "255";
            textChannelLinkFreq->Enabled = FALSE;
         }
         else
         {
            textChannelLinkFreq->Text = "50";
            textChannelLinkFreq->Enabled = TRUE;
         }
      }
private: System::Void comboOptions_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
      {
         ComboQuickOptionsChanged();
      }
      
private: System::Void saveFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
      {
         if(FromGui(pstMyOptionsStruct))
            pstMyOptionsStruct->SaveConfigFile(this->saveFileDialog1->FileName);
      }
      
private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
      {
         if(pstMyOptionsStruct->LoadConfigFile(this->openFileDialog1->FileName))
            ToGui(pstMyOptionsStruct);
      }
      
private: System::Void buttonLoad_Click(System::Object^  sender, System::EventArgs^  e) 
      {
         this->openFileDialog1->ShowDialog();
      }
private: System::Void Auth_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
      {
         //Prevent the form from closing, and just hide it, so we can open it again
         e->Cancel = true;
         this->Hide();
      }

private: System::Void textChannelPeriodByPeriod_TextChanged(System::Object^  sender, System::EventArgs^  e) 
         {
            if(!textChannelPeriodByPeriod->Focused && sender != this)
               return;

            UpdateHz();
         }

private: System::Void textChannelPeriodByHz_TextChanged(System::Object^  sender, System::EventArgs^  e) 
         {
            if(!textChannelPeriodByHz->Focused)
               return;

            UpdatePeriod();
         }
private: System::Void buttonLogs_Click(System::Object^  sender, System::EventArgs^  e) 
         {
            ShowLogsDirectory();
         }

private: System::Void comboPath_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
         {
            ComboDebugPathChanged();
         }

private: System::Void numUndiscoverableTime_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
         {
            NumBoxUndiscoverableTimeChanged();
         }
private: System::Void checkBlockSize_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
         {
            if(checkBlockSize->Checked)
               numBlockSize->Value = 512;
            else
               numBlockSize->Value = 0;
         }

private: System::Void btnApply_Click(System::Object^  sender, System::EventArgs^  e) 
         {
            ApplyClick();
         }
};




 
}
