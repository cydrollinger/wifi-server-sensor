/*******************************************************************************
 * THE FOLLOWING EXAMPLE CODE IS INTENDED FOR LIMITED CIRCULATION ONLY.
 * 
 * Please forward all questions regarding this code to ANT Technical Support.
 * 
 * Dynastream Innovations Inc.
 * 228 River Avenue
 * Cochrane, Alberta, Canada
 * T4C 2C1
 * 
 * (P) (403) 932-9292
 * (F) (403) 932-6521
 * (TF) 1-866-932-9292
 * (E) support@thisisant.com
 * 
 * www.thisisant.com
 *
 * Reference Design Disclaimer
 *
 * The references designs and codes provided may be used with ANT devices only and remain the copyrighted property of 
 * Dynastream Innovations Inc. The reference designs and codes are being provided on an "as-is" basis and as an accommodation, 
 * and therefore all warranties, representations, or guarantees of any kind (whether express, implied or statutory) including, 
 * without limitation, warranties of merchantability, non-infringement,
 * or fitness for a particular purpose, are specifically disclaimed.
 *
 * ©2009 Dynastream Innovations Inc. All Rights Reserved
 * This software may not be reproduced by
 * any means without express written approval of Dynastream
 * Innovations Inc.
 *
 *******************************************************************************/

#pragma once

#include <stdio.h>
#include "config.h"
#include "types.h"
#include "ant_control.h"
#include "ant_dll.h"
#include "antmessage.h"

#define TX_BUFFER_SIZE 64

static char aucTxBuf[TX_BUFFER_SIZE];
static char aucTxExtBuf[TX_BUFFER_SIZE];
static BOOL bConnected = FALSE;
static BOOL bOpen = FALSE;

static void UnmanagedSender(UCHAR ucEvent_, char* pcBuffer_);

//////////////////////////////////////////////////////////////////////////////////////
/// Class Name Space
//////////////////////////////////////////////////////////////////////////////////////

namespace Scanning_Mode_Demo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
		Form1(void)
		{
			System::Windows::Forms::Control::CheckForIllegalCrossThreadCalls = FALSE;
			InitializeComponent();
		}

	   void ANTCallback(UCHAR ucEvent_, char* pcBuffer_)
		{  
			switch(ucEvent_)
			{
            // Handle events generated at ant_control
			   case ANT_EVENT_DEBUG:
			   {
					if(pcBuffer_)
						tbDisplay->AppendText(gcnew String(pcBuffer_));
					break;
				}
			   case ANT_EVENT_CONNECT:
				{
					button_Connect->Text = "Disconnect";
					button_Open->Enabled = FALSE;
					button_Config->Enabled = TRUE;
					bConnected = TRUE;
					tbDisplay->AppendText("Connected\r\n"); 
					break;
				}
			   case ANT_EVENT_DISCONNECT:
				{
					button_Connect->Text = "Connect";
					button_Open->Text = "Open";
					button_Open->Enabled = FALSE;
					button_Config->Enabled = FALSE;
					bConnected = FALSE;
					tbDisplay->AppendText("Disconnected\r\n");
					break;
				}
			   case ANT_EVENT_OPEN:
				{
					button_Open->Text = "Close";
					bOpen = TRUE;
					tbDisplay->AppendText("Started scanning mode...\r\n");  
					button_Send->Enabled = TRUE;
               button_SendAck->Enabled = TRUE;
               button_SendBurst->Enabled = TRUE;
               button_Config->Enabled = FALSE;
					break;
				}
			   case ANT_EVENT_CLOSE:
				{
					button_Open->Text = "Open Scan Mode";
					bOpen= FALSE;
					tbDisplay->AppendText("Closed...\r\n"); 
					button_Send->Enabled = FALSE;
               button_SendAck->Enabled = FALSE;
               button_SendBurst->Enabled = FALSE;
               button_Config->Enabled = TRUE;
               button_Open->Enabled = FALSE;
					break;
				}
            // Handle ANT messages (Rx data)
            case EVENT_RX_FLAG_ACKNOWLEDGED: // Handle flagged extended messages
            case EVENT_RX_FLAG_BURST_PACKET:
			   case EVENT_RX_FLAG_BROADCAST:  
				{	
					if(pcBuffer_)
					{
                   UCHAR ucFlag = (UCHAR) pcBuffer_[8]; // Flag byte

                   if(ucFlag & 0x80)    // extended message including channel ID
                   {
                        // Get Channel ID
                        USHORT usDeviceNo = ((UCHAR)pcBuffer_[10]) << 8 | (UCHAR)pcBuffer_[9];						
						      UCHAR ucDeviceType = (UCHAR) pcBuffer_[11];
						      UCHAR ucTransType = (UCHAR) pcBuffer_[12];

                        sprintf_s((char*)aucTxExtBuf, TX_BUFFER_SIZE, "<ChID:%d,%d,%d> ", usDeviceNo, ucDeviceType, ucTransType);
						      tbDisplay->AppendText( gcnew String(aucTxExtBuf) );
                   }
               }
               // INTENTIONAL FALL THRU
               // The rest of the messages follows the standard format
				}
            case EVENT_RX_ACKNOWLEDGED:
            case EVENT_RX_BURST_PACKET:  
            case EVENT_RX_BROADCAST:
            {
                if(ucEvent_ == EVENT_RX_ACKNOWLEDGED || ucEvent_ == EVENT_RX_FLAG_ACKNOWLEDGED)
                    tbDisplay->AppendText("Acked Rx: ");           
                else if(ucEvent_ == EVENT_RX_BURST_PACKET || ucEvent_ == EVENT_RX_FLAG_BURST_PACKET)
                    tbDisplay->AppendText("Burst Rx: ");             
                else      
                    tbDisplay->AppendText("Rx: "); 

                // Display received data
                sprintf_s((char*)aucTxExtBuf, TX_BUFFER_SIZE, "[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]\r\n", (UCHAR)pcBuffer_[0],(UCHAR)pcBuffer_[1],(UCHAR)pcBuffer_[2],(UCHAR)pcBuffer_[3], (UCHAR)pcBuffer_[4], (UCHAR)pcBuffer_[5], (UCHAR)pcBuffer_[6], (UCHAR)pcBuffer_[7]);
					 tbDisplay->AppendText( gcnew String(aucTxExtBuf) );
                
                break;
            }
            case EVENT_RX_EXT_ACKNOWLEDGED:  // Legacy extended messages
            case EVENT_RX_EXT_BURST_PACKET:
			   case EVENT_RX_EXT_BROADCAST:
				{	
					if(pcBuffer_)
					{
                  // Get Channel ID
                  USHORT usDeviceNo = ((UCHAR)pcBuffer_[1]) << 8 | (UCHAR)pcBuffer_[0];						
						UCHAR ucDeviceType = (UCHAR) pcBuffer_[2];
						UCHAR ucTransType = (UCHAR) pcBuffer_[3];

                  // Display received data
                  sprintf_s((char*)aucTxExtBuf, TX_BUFFER_SIZE, "<ChID:%d,%d,%d> ", usDeviceNo, ucDeviceType, ucTransType);
						tbDisplay->AppendText( gcnew String(aucTxExtBuf) );
						
                  if(ucEvent_ == EVENT_RX_EXT_ACKNOWLEDGED)
                    tbDisplay->AppendText("Acked Rx: ");           
                  else if(ucEvent_ == EVENT_RX_EXT_BURST_PACKET)
                    tbDisplay->AppendText("Burst Rx: ");             
                  else      
                    tbDisplay->AppendText("Rx: ");

                  sprintf_s((char*)aucTxExtBuf, TX_BUFFER_SIZE, "[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]\r\n", (UCHAR)pcBuffer_[4],(UCHAR)pcBuffer_[5],(UCHAR)pcBuffer_[6],(UCHAR)pcBuffer_[7], (UCHAR)pcBuffer_[8], (UCHAR)pcBuffer_[9], (UCHAR)pcBuffer_[10], (UCHAR)pcBuffer_[11]);
						tbDisplay->AppendText( gcnew String(aucTxExtBuf) );
               }
               break;
				}
			   case EVENT_TX:
				{  
                tbDisplay->AppendText("Transmitted data\r\n"); // Pending broadcast was transmitted
					 break;
				}
            case EVENT_TRANSFER_TX_COMPLETED:
            {   
                tbDisplay->AppendText("Tx success!\r\n");   // Pending burst/ack was transmitted successfully
                break;
            }
				default:
				    break;
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

   private: System::Windows::Forms::Button^  button_Config;
   private: System::Windows::Forms::Button^  button_Connect;
   private: System::Windows::Forms::Button^  button_Open;
   private: System::Windows::Forms::Button^  button_Send;
   private: System::Windows::Forms::Button^  button_SendAck;
   private: System::Windows::Forms::Button^  button_SendBurst;
   private: System::Windows::Forms::CheckBox^  checkBox_RxOnly;
   private: System::Windows::Forms::GroupBox^  groupBox2; 
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_BurstSize;
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_DeviceNumExt;
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_DeviceType;
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_DeviceTypeExt;
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_DevNumScan;
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_RF;
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_TxType;
   private: System::Windows::Forms::NumericUpDown^  numericUpDown_TxTypeExt;
   private: System::Windows::Forms::TextBox^	tbDisplay;
   private: System::Windows::Forms::TextBox^	tbSendExt;
	private: System::Windows::Forms::ComboBox^  cmbUSB;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;   
	private: System::Windows::Forms::Label^  label3; 
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
   private: System::Windows::Forms::Label^  label10;
   private: System::Windows::Forms::Label^  label11;
   private: System::Windows::Forms::Label^  label7;
   private: System::Windows::Forms::Label^  label8;
   private: System::Windows::Forms::Label^  label9;


	private:
		/// <summary>
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
          System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
          this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
          this->numericUpDown_RF = (gcnew System::Windows::Forms::NumericUpDown());
          this->checkBox_RxOnly = (gcnew System::Windows::Forms::CheckBox());
          this->label9 = (gcnew System::Windows::Forms::Label());
          this->numericUpDown_TxType = (gcnew System::Windows::Forms::NumericUpDown());
          this->label8 = (gcnew System::Windows::Forms::Label());
          this->numericUpDown_DeviceType = (gcnew System::Windows::Forms::NumericUpDown());
          this->label7 = (gcnew System::Windows::Forms::Label());
          this->numericUpDown_DevNumScan = (gcnew System::Windows::Forms::NumericUpDown());
          this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
          this->label11 = (gcnew System::Windows::Forms::Label());
          this->label10 = (gcnew System::Windows::Forms::Label());
          this->numericUpDown_BurstSize = (gcnew System::Windows::Forms::NumericUpDown());
          this->button_SendBurst = (gcnew System::Windows::Forms::Button());
          this->button_SendAck = (gcnew System::Windows::Forms::Button());
          this->numericUpDown_TxTypeExt = (gcnew System::Windows::Forms::NumericUpDown());
          this->numericUpDown_DeviceTypeExt = (gcnew System::Windows::Forms::NumericUpDown());
          this->numericUpDown_DeviceNumExt = (gcnew System::Windows::Forms::NumericUpDown());
          this->button_Send = (gcnew System::Windows::Forms::Button());
          this->label5 = (gcnew System::Windows::Forms::Label());
          this->label6 = (gcnew System::Windows::Forms::Label());
          this->label4 = (gcnew System::Windows::Forms::Label());
          this->tbSendExt = (gcnew System::Windows::Forms::TextBox());
          this->label1 = (gcnew System::Windows::Forms::Label());
          this->button_Connect = (gcnew System::Windows::Forms::Button());
          this->label3 = (gcnew System::Windows::Forms::Label());
          this->button_Config = (gcnew System::Windows::Forms::Button());
          this->cmbUSB = (gcnew System::Windows::Forms::ComboBox());
          this->tbDisplay = (gcnew System::Windows::Forms::TextBox());
          this->button_Open = (gcnew System::Windows::Forms::Button());
          this->label2 = (gcnew System::Windows::Forms::Label());
          this->groupBox1->SuspendLayout();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_RF))->BeginInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_TxType))->BeginInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DeviceType))->BeginInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DevNumScan))->BeginInit();
          this->groupBox2->SuspendLayout();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_BurstSize))->BeginInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_TxTypeExt))->BeginInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DeviceTypeExt))->BeginInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DeviceNumExt))->BeginInit();
          this->SuspendLayout();
          // 
          // groupBox1
          // 
          this->groupBox1->Controls->Add(this->numericUpDown_RF);
          this->groupBox1->Controls->Add(this->checkBox_RxOnly);
          this->groupBox1->Controls->Add(this->label9);
          this->groupBox1->Controls->Add(this->numericUpDown_TxType);
          this->groupBox1->Controls->Add(this->label8);
          this->groupBox1->Controls->Add(this->numericUpDown_DeviceType);
          this->groupBox1->Controls->Add(this->label7);
          this->groupBox1->Controls->Add(this->numericUpDown_DevNumScan);
          this->groupBox1->Controls->Add(this->groupBox2);
          this->groupBox1->Controls->Add(this->button_Connect);
          this->groupBox1->Controls->Add(this->label3);
          this->groupBox1->Controls->Add(this->button_Config);
          this->groupBox1->Controls->Add(this->cmbUSB);
          this->groupBox1->Controls->Add(this->tbDisplay);
          this->groupBox1->Controls->Add(this->button_Open);
          this->groupBox1->Controls->Add(this->label2);
          this->groupBox1->Location = System::Drawing::Point(8, 9);
          this->groupBox1->Name = L"groupBox1";
          this->groupBox1->Size = System::Drawing::Size(396, 526);
          this->groupBox1->TabIndex = 2;
          this->groupBox1->TabStop = false;
          this->groupBox1->Text = L"Receiver Control";
          // 
          // numericUpDown_RF
          // 
          this->numericUpDown_RF->Location = System::Drawing::Point(32, 62);
          this->numericUpDown_RF->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {124, 0, 0, 0});
          this->numericUpDown_RF->Name = L"numericUpDown_RF";
          this->numericUpDown_RF->Size = System::Drawing::Size(42, 20);
          this->numericUpDown_RF->TabIndex = 30;
          this->numericUpDown_RF->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {66, 0, 0, 0});
          // 
          // checkBox_RxOnly
          // 
          this->checkBox_RxOnly->AutoSize = true;
          this->checkBox_RxOnly->Location = System::Drawing::Point(287, 63);
          this->checkBox_RxOnly->Name = L"checkBox_RxOnly";
          this->checkBox_RxOnly->Size = System::Drawing::Size(61, 17);
          this->checkBox_RxOnly->TabIndex = 29;
          this->checkBox_RxOnly->Text = L"Rx-only";
          this->checkBox_RxOnly->UseVisualStyleBackColor = true;
          // 
          // label9
          // 
          this->label9->AutoSize = true;
          this->label9->Location = System::Drawing::Point(222, 46);
          this->label9->Name = L"label9";
          this->label9->Size = System::Drawing::Size(64, 13);
          this->label9->TabIndex = 25;
          this->label9->Text = L"Trans. Type";
          // 
          // numericUpDown_TxType
          // 
          this->numericUpDown_TxType->Location = System::Drawing::Point(231, 62);
          this->numericUpDown_TxType->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
          this->numericUpDown_TxType->Name = L"numericUpDown_TxType";
          this->numericUpDown_TxType->Size = System::Drawing::Size(42, 20);
          this->numericUpDown_TxType->TabIndex = 28;
          // 
          // label8
          // 
          this->label8->AutoSize = true;
          this->label8->Location = System::Drawing::Point(153, 46);
          this->label8->Name = L"label8";
          this->label8->Size = System::Drawing::Size(68, 13);
          this->label8->TabIndex = 25;
          this->label8->Text = L"Device Type";
          // 
          // numericUpDown_DeviceType
          // 
          this->numericUpDown_DeviceType->Location = System::Drawing::Point(167, 62);
          this->numericUpDown_DeviceType->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
          this->numericUpDown_DeviceType->Name = L"numericUpDown_DeviceType";
          this->numericUpDown_DeviceType->Size = System::Drawing::Size(43, 20);
          this->numericUpDown_DeviceType->TabIndex = 27;
          // 
          // label7
          // 
          this->label7->AutoSize = true;
          this->label7->Location = System::Drawing::Point(85, 46);
          this->label7->Name = L"label7";
          this->label7->Size = System::Drawing::Size(61, 13);
          this->label7->TabIndex = 25;
          this->label7->Text = L"Device No.";
          // 
          // numericUpDown_DevNumScan
          // 
          this->numericUpDown_DevNumScan->Location = System::Drawing::Point(89, 62);
          this->numericUpDown_DevNumScan->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65535, 0, 0, 0});
          this->numericUpDown_DevNumScan->Name = L"numericUpDown_DevNumScan";
          this->numericUpDown_DevNumScan->Size = System::Drawing::Size(57, 20);
          this->numericUpDown_DevNumScan->TabIndex = 26;
          // 
          // groupBox2
          // 
          this->groupBox2->Controls->Add(this->label11);
          this->groupBox2->Controls->Add(this->label10);
          this->groupBox2->Controls->Add(this->numericUpDown_BurstSize);
          this->groupBox2->Controls->Add(this->button_SendBurst);
          this->groupBox2->Controls->Add(this->button_SendAck);
          this->groupBox2->Controls->Add(this->numericUpDown_TxTypeExt);
          this->groupBox2->Controls->Add(this->numericUpDown_DeviceTypeExt);
          this->groupBox2->Controls->Add(this->numericUpDown_DeviceNumExt);
          this->groupBox2->Controls->Add(this->button_Send);
          this->groupBox2->Controls->Add(this->label5);
          this->groupBox2->Controls->Add(this->label6);
          this->groupBox2->Controls->Add(this->label4);
          this->groupBox2->Controls->Add(this->tbSendExt);
          this->groupBox2->Controls->Add(this->label1);
          this->groupBox2->Location = System::Drawing::Point(23, 385);
          this->groupBox2->Name = L"groupBox2";
          this->groupBox2->Size = System::Drawing::Size(350, 133);
          this->groupBox2->TabIndex = 25;
          this->groupBox2->TabStop = false;
          this->groupBox2->Text = L"Outgoing Extended Messages";
          // 
          // label11
          // 
          this->label11->AutoSize = true;
          this->label11->Location = System::Drawing::Point(146, 99);
          this->label11->Name = L"label11";
          this->label11->Size = System::Drawing::Size(45, 13);
          this->label11->TabIndex = 34;
          this->label11->Text = L"packets";
          // 
          // label10
          // 
          this->label10->AutoSize = true;
          this->label10->Location = System::Drawing::Point(6, 99);
          this->label10->Name = L"label10";
          this->label10->Size = System::Drawing::Size(54, 13);
          this->label10->TabIndex = 33;
          this->label10->Text = L"Burst Size";
          // 
          // numericUpDown_BurstSize
          // 
          this->numericUpDown_BurstSize->Location = System::Drawing::Point(83, 97);
          this->numericUpDown_BurstSize->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
          this->numericUpDown_BurstSize->Name = L"numericUpDown_BurstSize";
          this->numericUpDown_BurstSize->Size = System::Drawing::Size(57, 20);
          this->numericUpDown_BurstSize->TabIndex = 32;
          this->numericUpDown_BurstSize->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
          // 
          // button_SendBurst
          // 
          this->button_SendBurst->Enabled = false;
          this->button_SendBurst->Location = System::Drawing::Point(222, 103);
          this->button_SendBurst->Name = L"button_SendBurst";
          this->button_SendBurst->Size = System::Drawing::Size(107, 24);
          this->button_SendBurst->TabIndex = 31;
          this->button_SendBurst->Text = L"Send Burst";
          this->button_SendBurst->UseVisualStyleBackColor = true;
          this->button_SendBurst->Click += gcnew System::EventHandler(this, &Form1::button_SendBurst_Click);
          // 
          // button_SendAck
          // 
          this->button_SendAck->Enabled = false;
          this->button_SendAck->Location = System::Drawing::Point(222, 76);
          this->button_SendAck->Name = L"button_SendAck";
          this->button_SendAck->Size = System::Drawing::Size(107, 24);
          this->button_SendAck->TabIndex = 30;
          this->button_SendAck->Text = L"Send Ack";
          this->button_SendAck->UseVisualStyleBackColor = true;
          this->button_SendAck->Click += gcnew System::EventHandler(this, &Form1::button_SendAck_Click);
          // 
          // numericUpDown_TxTypeExt
          // 
          this->numericUpDown_TxTypeExt->Location = System::Drawing::Point(83, 71);
          this->numericUpDown_TxTypeExt->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
          this->numericUpDown_TxTypeExt->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
          this->numericUpDown_TxTypeExt->Name = L"numericUpDown_TxTypeExt";
          this->numericUpDown_TxTypeExt->Size = System::Drawing::Size(57, 20);
          this->numericUpDown_TxTypeExt->TabIndex = 29;
          this->numericUpDown_TxTypeExt->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
          // 
          // numericUpDown_DeviceTypeExt
          // 
          this->numericUpDown_DeviceTypeExt->Location = System::Drawing::Point(83, 46);
          this->numericUpDown_DeviceTypeExt->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
          this->numericUpDown_DeviceTypeExt->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
          this->numericUpDown_DeviceTypeExt->Name = L"numericUpDown_DeviceTypeExt";
          this->numericUpDown_DeviceTypeExt->Size = System::Drawing::Size(58, 20);
          this->numericUpDown_DeviceTypeExt->TabIndex = 28;
          this->numericUpDown_DeviceTypeExt->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
          // 
          // numericUpDown_DeviceNumExt
          // 
          this->numericUpDown_DeviceNumExt->Location = System::Drawing::Point(83, 18);
          this->numericUpDown_DeviceNumExt->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65535, 0, 0, 0});
          this->numericUpDown_DeviceNumExt->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
          this->numericUpDown_DeviceNumExt->Name = L"numericUpDown_DeviceNumExt";
          this->numericUpDown_DeviceNumExt->Size = System::Drawing::Size(57, 20);
          this->numericUpDown_DeviceNumExt->TabIndex = 27;
          this->numericUpDown_DeviceNumExt->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {49, 0, 0, 0});
          // 
          // button_Send
          // 
          this->button_Send->Enabled = false;
          this->button_Send->Location = System::Drawing::Point(222, 46);
          this->button_Send->Name = L"button_Send";
          this->button_Send->Size = System::Drawing::Size(107, 24);
          this->button_Send->TabIndex = 15;
          this->button_Send->Text = L"Send Broadcast";
          this->button_Send->UseVisualStyleBackColor = true;
          this->button_Send->Click += gcnew System::EventHandler(this, &Form1::button_Send_Click);
          // 
          // label5
          // 
          this->label5->AutoSize = true;
          this->label5->Location = System::Drawing::Point(6, 73);
          this->label5->Name = L"label5";
          this->label5->Size = System::Drawing::Size(64, 13);
          this->label5->TabIndex = 23;
          this->label5->Text = L"Trans. Type";
          // 
          // label6
          // 
          this->label6->AutoSize = true;
          this->label6->Location = System::Drawing::Point(161, 23);
          this->label6->Name = L"label6";
          this->label6->Size = System::Drawing::Size(30, 13);
          this->label6->TabIndex = 24;
          this->label6->Text = L"Data";
          // 
          // label4
          // 
          this->label4->AutoSize = true;
          this->label4->Location = System::Drawing::Point(6, 48);
          this->label4->Name = L"label4";
          this->label4->Size = System::Drawing::Size(68, 13);
          this->label4->TabIndex = 22;
          this->label4->Text = L"Device Type";
          // 
          // tbSendExt
          // 
          this->tbSendExt->Location = System::Drawing::Point(199, 20);
          this->tbSendExt->Name = L"tbSendExt";
          this->tbSendExt->Size = System::Drawing::Size(130, 20);
          this->tbSendExt->TabIndex = 16;
          this->tbSendExt->Text = L"00,00,00,00,00,00,00,00";
          this->tbSendExt->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
          // 
          // label1
          // 
          this->label1->AutoSize = true;
          this->label1->Location = System::Drawing::Point(6, 20);
          this->label1->Name = L"label1";
          this->label1->Size = System::Drawing::Size(61, 13);
          this->label1->TabIndex = 21;
          this->label1->Text = L"Device No.";
          // 
          // button_Connect
          // 
          this->button_Connect->Location = System::Drawing::Point(187, 12);
          this->button_Connect->Name = L"button_Connect";
          this->button_Connect->Size = System::Drawing::Size(86, 22);
          this->button_Connect->TabIndex = 6;
          this->button_Connect->Text = L"Connect";
          this->button_Connect->UseVisualStyleBackColor = true;
          this->button_Connect->Click += gcnew System::EventHandler(this, &Form1::button_Connect_Click);
          // 
          // label3
          // 
          this->label3->AutoSize = true;
          this->label3->Location = System::Drawing::Point(29, 46);
          this->label3->Name = L"label3";
          this->label3->Size = System::Drawing::Size(45, 13);
          this->label3->TabIndex = 14;
          this->label3->Text = L"RF Freq";
          // 
          // button_Config
          // 
          this->button_Config->Enabled = false;
          this->button_Config->Location = System::Drawing::Point(23, 97);
          this->button_Config->Name = L"button_Config";
          this->button_Config->Size = System::Drawing::Size(151, 25);
          this->button_Config->TabIndex = 13;
          this->button_Config->Text = L"Config Scan Mode";
          this->button_Config->UseVisualStyleBackColor = true;
          this->button_Config->Click += gcnew System::EventHandler(this, &Form1::button_Config_Click);
          // 
          // cmbUSB
          // 
          this->cmbUSB->FormattingEnabled = true;
          this->cmbUSB->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"0", L"1", L"2", L"3"});
          this->cmbUSB->Location = System::Drawing::Point(97, 13);
          this->cmbUSB->Name = L"cmbUSB";
          this->cmbUSB->Size = System::Drawing::Size(67, 21);
          this->cmbUSB->TabIndex = 9;
          this->cmbUSB->Text = L"0";
          // 
          // tbDisplay
          // 
          this->tbDisplay->Location = System::Drawing::Point(23, 128);
          this->tbDisplay->Multiline = true;
          this->tbDisplay->Name = L"tbDisplay";
          this->tbDisplay->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
          this->tbDisplay->Size = System::Drawing::Size(350, 251);
          this->tbDisplay->TabIndex = 8;
          this->tbDisplay->Text = L"The scanning mode is not supported in AP1 chipsets/modules\r\n";
          // 
          // button_Open
          // 
          this->button_Open->Enabled = false;
          this->button_Open->Location = System::Drawing::Point(224, 97);
          this->button_Open->Name = L"button_Open";
          this->button_Open->Size = System::Drawing::Size(149, 25);
          this->button_Open->TabIndex = 7;
          this->button_Open->Text = L"Open Scan Mode";
          this->button_Open->UseVisualStyleBackColor = true;
          this->button_Open->Click += gcnew System::EventHandler(this, &Form1::button_Open_Click);
          // 
          // label2
          // 
          this->label2->AutoSize = true;
          this->label2->Location = System::Drawing::Point(20, 16);
          this->label2->Name = L"label2";
          this->label2->Size = System::Drawing::Size(71, 13);
          this->label2->TabIndex = 3;
          this->label2->Text = L"USB Port No.";
          // 
          // Form1
          // 
          this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
          this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
          this->ClientSize = System::Drawing::Size(412, 539);
          this->Controls->Add(this->groupBox1);
          this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
          this->Name = L"Form1";
          this->Text = L"ANT Scanning Mode Demo";
          this->groupBox1->ResumeLayout(false);
          this->groupBox1->PerformLayout();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_RF))->EndInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_TxType))->EndInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DeviceType))->EndInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DevNumScan))->EndInit();
          this->groupBox2->ResumeLayout(false);
          this->groupBox2->PerformLayout();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_BurstSize))->EndInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_TxTypeExt))->EndInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DeviceTypeExt))->EndInit();
          (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown_DeviceNumExt))->EndInit();
          this->ResumeLayout(false);

      }
#pragma endregion

// Connect to ANT
private: System::Void button_Connect_Click(System::Object^  sender, System::EventArgs^  e) {
			if(!bConnected)
			{
				UCHAR ucUSB;
				USHORT usBaud = 57600;  // Scanning mode is not supported on AP1 

				ucUSB = Convert::ToByte(cmbUSB->Text); 
				if(ucUSB > 4)
					ucUSB = 0;

				if(ANTControl_Connect((ANT_RESPONSE_FUNC) UnmanagedSender, ucUSB, usBaud))
				{  
					bConnected = TRUE;
				}
			}
			else
			{
				ANTControl_Disconnect();
				bConnected = FALSE;
			}

		 }

// Disconnect from ANT if closing application
private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
		if(bConnected)
			ANTControl_Disconnect(); 
		}

// Open/close scanning mode
private: System::Void button_Open_Click(System::Object^  sender, System::EventArgs^  e) {
		 if(!bOpen)
			 ANTControl_OpenScanningMode();
		 else
			 ANTControl_CloseScanningMode();
		 }

// Config scanning mode
private: System::Void button_Config_Click(System::Object^  sender, System::EventArgs^  e) {
          // Read config
          USHORT usDeviceNo = Convert::ToUInt16(numericUpDown_DevNumScan->Value);
          UCHAR ucDeviceType = Convert::ToByte(numericUpDown_DeviceType->Value);
          UCHAR ucTransType = Convert::ToByte(numericUpDown_TxType->Value);
          UCHAR ucRFFreq = Convert::ToByte(numericUpDown_RF->Value);
          BOOL bRxOnly = (checkBox_RxOnly->Checked == TRUE);

          // Config scanning mode
          ANTControl_ConfigScanningMode(usDeviceNo, ucDeviceType, ucTransType, ucRFFreq, bRxOnly);
          tbDisplay->AppendText("Set scanning mode configuration\r\n");

          button_Open->Enabled = TRUE;
		 }

private: System::Void button_Send_Click(System::Object^  sender, System::EventArgs^  e) {
             SendExt(MESG_EXT_BROADCAST_DATA_ID);
			 
		 }
private: System::Void button_SendAck_Click(System::Object^  sender, System::EventArgs^  e) {
             SendExt(MESG_EXT_ACKNOWLEDGED_DATA_ID);
         }

private: System::Void button_SendBurst_Click(System::Object^  sender, System::EventArgs^  e) {
             SendExt(MESG_EXT_BURST_DATA_ID);
         }

private: void SendExt(UCHAR ucMessage_){
          // Send broadcast/ack/burst depending on button pressed
          USHORT usExtDeviceNo;
			 UCHAR ucExtDeviceType;
			 UCHAR ucExtTransType;
			 UCHAR aucMessage[8]= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
          UCHAR ucNumBurstPackets;
			 BOOL bStatus = TRUE;

			 // Obtain channel ID of destination
          usExtDeviceNo = Convert::ToUInt16(numericUpDown_DeviceNumExt->Value);
          ucExtDeviceType = Convert::ToByte(numericUpDown_DeviceTypeExt->Value);
			 ucExtTransType = Convert::ToByte(numericUpDown_TxTypeExt->Value);
          ucNumBurstPackets = Convert::ToByte(numericUpDown_BurstSize->Value);

			// Obtain payload
			 if(tbSendExt->Text->Length != 23)
			 {
				 MessageBox::Show("Invalid Hex Format", "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
				 bStatus = FALSE;
			 }
			 else
			 {
				try
				{	
					aucMessage[0] = Convert::ToByte(tbSendExt->Text->Substring(0,2), 16);
					aucMessage[1] = Convert::ToByte(tbSendExt->Text->Substring(3,2), 16);
					aucMessage[2] = Convert::ToByte(tbSendExt->Text->Substring(6,2), 16);
					aucMessage[3] = Convert::ToByte(tbSendExt->Text->Substring(9,2), 16);
					aucMessage[4] = Convert::ToByte(tbSendExt->Text->Substring(12,2), 16);
					aucMessage[5] = Convert::ToByte(tbSendExt->Text->Substring(15,2), 16);
					aucMessage[6] = Convert::ToByte(tbSendExt->Text->Substring(18,2), 16);
					aucMessage[7] = Convert::ToByte(tbSendExt->Text->Substring(21,2), 16);
				}
				catch(...)
				{
					MessageBox::Show("Invalid Hex Format", "Input Error", MessageBoxButtons::OK,MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
					return;
				}
			 }
			
          // If input is valid, attempt to send the message
			 if(bStatus)
          {
              if(ucMessage_ == MESG_EXT_BROADCAST_DATA_ID)
              {
                  ANTControl_SendExtBroadcast(usExtDeviceNo, ucExtDeviceType, ucExtTransType, aucMessage);
              }
              else if(ucMessage_ == MESG_EXT_ACKNOWLEDGED_DATA_ID)
              {
                  ANTControl_SendExtAcknowledged(usExtDeviceNo, ucExtDeviceType, ucExtTransType, aucMessage);
              }
              else if(ucMessage_ == MESG_EXT_BURST_DATA_ID)  
              {
                  ANTControl_SendExtBurst(usExtDeviceNo, ucExtDeviceType, ucExtTransType, aucMessage, ucNumBurstPackets);
              }
          }
     }

};


}