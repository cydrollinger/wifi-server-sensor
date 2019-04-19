namespace ANTFSClientDemo
{
    partial class DirectoryEditor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
           this.groupDir = new System.Windows.Forms.GroupBox();
           this.numType = new System.Windows.Forms.NumericUpDown();
           this.labelType = new System.Windows.Forms.Label();
           this.numIndex = new System.Windows.Forms.NumericUpDown();
           this.labelIndex = new System.Windows.Forms.Label();
           this.groupBoxSpecific = new System.Windows.Forms.GroupBox();
           this.ckSelected = new System.Windows.Forms.CheckBox();
           this.labelFlags2 = new System.Windows.Forms.Label();
           this.numSpecificFlags = new System.Windows.Forms.NumericUpDown();
           this.groupBoxGeneral = new System.Windows.Forms.GroupBox();
           this.ckCrypto = new System.Windows.Forms.CheckBox();
           this.ckAppend = new System.Windows.Forms.CheckBox();
           this.ckArchive = new System.Windows.Forms.CheckBox();
           this.ckErase = new System.Windows.Forms.CheckBox();
           this.ckWrite = new System.Windows.Forms.CheckBox();
           this.ckRead = new System.Windows.Forms.CheckBox();
           this.labelFlags = new System.Windows.Forms.Label();
           this.numGeneralFlags = new System.Windows.Forms.NumericUpDown();
           this.buttonBrowse = new System.Windows.Forms.Button();
           this.textBoxFileName = new System.Windows.Forms.TextBox();
           this.groupIdentifier = new System.Windows.Forms.GroupBox();
           this.numNumber = new System.Windows.Forms.NumericUpDown();
           this.labelNumber = new System.Windows.Forms.Label();
           this.numSubType = new System.Windows.Forms.NumericUpDown();
           this.labelSubType = new System.Windows.Forms.Label();
           this.groupBoxFile = new System.Windows.Forms.GroupBox();
           this.buttonOk = new System.Windows.Forms.Button();
           this.buttonCancel = new System.Windows.Forms.Button();
           this.openFileDialogScreen = new System.Windows.Forms.OpenFileDialog();
           this.groupDir.SuspendLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numType)).BeginInit();
           ((System.ComponentModel.ISupportInitialize)(this.numIndex)).BeginInit();
           this.groupBoxSpecific.SuspendLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numSpecificFlags)).BeginInit();
           this.groupBoxGeneral.SuspendLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numGeneralFlags)).BeginInit();
           this.groupIdentifier.SuspendLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numNumber)).BeginInit();
           ((System.ComponentModel.ISupportInitialize)(this.numSubType)).BeginInit();
           this.groupBoxFile.SuspendLayout();
           this.SuspendLayout();
           // 
           // groupDir
           // 
           this.groupDir.Controls.Add(this.numType);
           this.groupDir.Controls.Add(this.labelType);
           this.groupDir.Controls.Add(this.numIndex);
           this.groupDir.Controls.Add(this.labelIndex);
           this.groupDir.Location = new System.Drawing.Point(12, 12);
           this.groupDir.Name = "groupDir";
           this.groupDir.Size = new System.Drawing.Size(168, 82);
           this.groupDir.TabIndex = 10;
           this.groupDir.TabStop = false;
           this.groupDir.Text = "Directory Entry";
           // 
           // numType
           // 
           this.numType.Location = new System.Drawing.Point(85, 43);
           this.numType.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
           this.numType.Name = "numType";
           this.numType.Size = new System.Drawing.Size(57, 20);
           this.numType.TabIndex = 3;
           this.numType.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
           this.numType.Value = new decimal(new int[] {
            128,
            0,
            0,
            0});
           this.numType.ValueChanged += new System.EventHandler(this.numType_ValueChanged);
           // 
           // labelType
           // 
           this.labelType.AutoSize = true;
           this.labelType.Location = new System.Drawing.Point(6, 45);
           this.labelType.Name = "labelType";
           this.labelType.Size = new System.Drawing.Size(76, 13);
           this.labelType.TabIndex = 2;
           this.labelType.Text = "File Data Type";
           // 
           // numIndex
           // 
           this.numIndex.Location = new System.Drawing.Point(85, 17);
           this.numIndex.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
           this.numIndex.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
           this.numIndex.Name = "numIndex";
           this.numIndex.Size = new System.Drawing.Size(57, 20);
           this.numIndex.TabIndex = 1;
           this.numIndex.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
           this.numIndex.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
           // 
           // labelIndex
           // 
           this.labelIndex.AutoSize = true;
           this.labelIndex.Location = new System.Drawing.Point(6, 19);
           this.labelIndex.Name = "labelIndex";
           this.labelIndex.Size = new System.Drawing.Size(33, 13);
           this.labelIndex.TabIndex = 0;
           this.labelIndex.Text = "Index";
           // 
           // groupBoxSpecific
           // 
           this.groupBoxSpecific.Controls.Add(this.ckSelected);
           this.groupBoxSpecific.Controls.Add(this.labelFlags2);
           this.groupBoxSpecific.Controls.Add(this.numSpecificFlags);
           this.groupBoxSpecific.Location = new System.Drawing.Point(315, 12);
           this.groupBoxSpecific.Name = "groupBoxSpecific";
           this.groupBoxSpecific.Size = new System.Drawing.Size(84, 96);
           this.groupBoxSpecific.TabIndex = 15;
           this.groupBoxSpecific.TabStop = false;
           this.groupBoxSpecific.Text = "Specific";
           // 
           // ckSelected
           // 
           this.ckSelected.AutoSize = true;
           this.ckSelected.Location = new System.Drawing.Point(6, 61);
           this.ckSelected.Name = "ckSelected";
           this.ckSelected.Size = new System.Drawing.Size(68, 17);
           this.ckSelected.TabIndex = 9;
           this.ckSelected.Text = "Selected";
           this.ckSelected.UseVisualStyleBackColor = true;
           this.ckSelected.CheckedChanged += new System.EventHandler(this.ckSelected_CheckedChanged);
           // 
           // labelFlags2
           // 
           this.labelFlags2.AutoSize = true;
           this.labelFlags2.Location = new System.Drawing.Point(6, 14);
           this.labelFlags2.Name = "labelFlags2";
           this.labelFlags2.Size = new System.Drawing.Size(58, 13);
           this.labelFlags2.TabIndex = 8;
           this.labelFlags2.Text = "Flags (hex)";
           // 
           // numSpecificFlags
           // 
           this.numSpecificFlags.Hexadecimal = true;
           this.numSpecificFlags.Location = new System.Drawing.Point(11, 31);
           this.numSpecificFlags.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
           this.numSpecificFlags.Name = "numSpecificFlags";
           this.numSpecificFlags.Size = new System.Drawing.Size(45, 20);
           this.numSpecificFlags.TabIndex = 7;
           this.numSpecificFlags.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
           this.numSpecificFlags.ValueChanged += new System.EventHandler(this.numSpecificFlags_ValueChanged);
           // 
           // groupBoxGeneral
           // 
           this.groupBoxGeneral.Controls.Add(this.ckCrypto);
           this.groupBoxGeneral.Controls.Add(this.ckAppend);
           this.groupBoxGeneral.Controls.Add(this.ckArchive);
           this.groupBoxGeneral.Controls.Add(this.ckErase);
           this.groupBoxGeneral.Controls.Add(this.ckWrite);
           this.groupBoxGeneral.Controls.Add(this.ckRead);
           this.groupBoxGeneral.Controls.Add(this.labelFlags);
           this.groupBoxGeneral.Controls.Add(this.numGeneralFlags);
           this.groupBoxGeneral.Location = new System.Drawing.Point(204, 12);
           this.groupBoxGeneral.Name = "groupBoxGeneral";
           this.groupBoxGeneral.Size = new System.Drawing.Size(100, 169);
           this.groupBoxGeneral.TabIndex = 14;
           this.groupBoxGeneral.TabStop = false;
           this.groupBoxGeneral.Text = "General";
           // 
           // ckCrypto
           // 
           this.ckCrypto.AutoSize = true;
           this.ckCrypto.Location = new System.Drawing.Point(15, 149);
           this.ckCrypto.Name = "ckCrypto";
           this.ckCrypto.Size = new System.Drawing.Size(56, 17);
           this.ckCrypto.TabIndex = 7;
           this.ckCrypto.Text = "Crypto";
           this.ckCrypto.UseVisualStyleBackColor = true;
           this.ckCrypto.CheckedChanged += new System.EventHandler(this.ckCrypto_CheckedChanged);
           // 
           // ckAppend
           // 
           this.ckAppend.AutoSize = true;
           this.ckAppend.Location = new System.Drawing.Point(15, 130);
           this.ckAppend.Name = "ckAppend";
           this.ckAppend.Size = new System.Drawing.Size(63, 17);
           this.ckAppend.TabIndex = 6;
           this.ckAppend.Text = "Append";
           this.ckAppend.UseVisualStyleBackColor = true;
           this.ckAppend.CheckedChanged += new System.EventHandler(this.ckAppend_CheckedChanged);
           // 
           // ckArchive
           // 
           this.ckArchive.AutoSize = true;
           this.ckArchive.Location = new System.Drawing.Point(15, 111);
           this.ckArchive.Name = "ckArchive";
           this.ckArchive.Size = new System.Drawing.Size(62, 17);
           this.ckArchive.TabIndex = 5;
           this.ckArchive.Text = "Archive";
           this.ckArchive.UseVisualStyleBackColor = true;
           this.ckArchive.CheckedChanged += new System.EventHandler(this.ckArchive_CheckedChanged);
           // 
           // ckErase
           // 
           this.ckErase.AutoSize = true;
           this.ckErase.Checked = true;
           this.ckErase.CheckState = System.Windows.Forms.CheckState.Checked;
           this.ckErase.Location = new System.Drawing.Point(15, 92);
           this.ckErase.Name = "ckErase";
           this.ckErase.Size = new System.Drawing.Size(53, 17);
           this.ckErase.TabIndex = 4;
           this.ckErase.Text = "Erase";
           this.ckErase.UseVisualStyleBackColor = true;
           this.ckErase.CheckedChanged += new System.EventHandler(this.ckErase_CheckedChanged);
           // 
           // ckWrite
           // 
           this.ckWrite.AutoSize = true;
           this.ckWrite.Checked = true;
           this.ckWrite.CheckState = System.Windows.Forms.CheckState.Checked;
           this.ckWrite.Location = new System.Drawing.Point(15, 73);
           this.ckWrite.Name = "ckWrite";
           this.ckWrite.Size = new System.Drawing.Size(51, 17);
           this.ckWrite.TabIndex = 3;
           this.ckWrite.Text = "Write";
           this.ckWrite.UseVisualStyleBackColor = true;
           this.ckWrite.CheckedChanged += new System.EventHandler(this.ckWrite_CheckedChanged);
           // 
           // ckRead
           // 
           this.ckRead.AutoSize = true;
           this.ckRead.Checked = true;
           this.ckRead.CheckState = System.Windows.Forms.CheckState.Checked;
           this.ckRead.Location = new System.Drawing.Point(15, 54);
           this.ckRead.Name = "ckRead";
           this.ckRead.Size = new System.Drawing.Size(52, 17);
           this.ckRead.TabIndex = 2;
           this.ckRead.Text = "Read";
           this.ckRead.UseVisualStyleBackColor = true;
           this.ckRead.CheckedChanged += new System.EventHandler(this.ckRead_CheckedChanged);
           // 
           // labelFlags
           // 
           this.labelFlags.AutoSize = true;
           this.labelFlags.Location = new System.Drawing.Point(10, 15);
           this.labelFlags.Name = "labelFlags";
           this.labelFlags.Size = new System.Drawing.Size(58, 13);
           this.labelFlags.TabIndex = 1;
           this.labelFlags.Text = "Flags (hex)";
           // 
           // numGeneralFlags
           // 
           this.numGeneralFlags.Hexadecimal = true;
           this.numGeneralFlags.Location = new System.Drawing.Point(15, 32);
           this.numGeneralFlags.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
           this.numGeneralFlags.Name = "numGeneralFlags";
           this.numGeneralFlags.Size = new System.Drawing.Size(45, 20);
           this.numGeneralFlags.TabIndex = 0;
           this.numGeneralFlags.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
           this.numGeneralFlags.Value = new decimal(new int[] {
            224,
            0,
            0,
            0});
           this.numGeneralFlags.ValueChanged += new System.EventHandler(this.numGeneralFlags_ValueChanged);
           // 
           // buttonBrowse
           // 
           this.buttonBrowse.Location = new System.Drawing.Point(297, 26);
           this.buttonBrowse.Name = "buttonBrowse";
           this.buttonBrowse.Size = new System.Drawing.Size(75, 23);
           this.buttonBrowse.TabIndex = 13;
           this.buttonBrowse.Text = "Browse";
           this.buttonBrowse.UseVisualStyleBackColor = true;
           this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
           // 
           // textBoxFileName
           // 
           this.textBoxFileName.Location = new System.Drawing.Point(8, 28);
           this.textBoxFileName.Name = "textBoxFileName";
           this.textBoxFileName.Size = new System.Drawing.Size(283, 20);
           this.textBoxFileName.TabIndex = 11;
           this.textBoxFileName.Text = "settings.fit";
           // 
           // groupIdentifier
           // 
           this.groupIdentifier.Controls.Add(this.numNumber);
           this.groupIdentifier.Controls.Add(this.labelNumber);
           this.groupIdentifier.Controls.Add(this.numSubType);
           this.groupIdentifier.Controls.Add(this.labelSubType);
           this.groupIdentifier.Location = new System.Drawing.Point(12, 101);
           this.groupIdentifier.Name = "groupIdentifier";
           this.groupIdentifier.Size = new System.Drawing.Size(168, 80);
           this.groupIdentifier.TabIndex = 10;
           this.groupIdentifier.TabStop = false;
           this.groupIdentifier.Text = "Identifier";
           // 
           // numNumber
           // 
           this.numNumber.Location = new System.Drawing.Point(84, 47);
           this.numNumber.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
           this.numNumber.Name = "numNumber";
           this.numNumber.Size = new System.Drawing.Size(58, 20);
           this.numNumber.TabIndex = 9;
           this.numNumber.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
           // 
           // labelNumber
           // 
           this.labelNumber.AutoSize = true;
           this.labelNumber.Location = new System.Drawing.Point(6, 49);
           this.labelNumber.Name = "labelNumber";
           this.labelNumber.Size = new System.Drawing.Size(63, 13);
           this.labelNumber.TabIndex = 8;
           this.labelNumber.Text = "File Number";
           // 
           // numSubType
           // 
           this.numSubType.Location = new System.Drawing.Point(84, 21);
           this.numSubType.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
           this.numSubType.Name = "numSubType";
           this.numSubType.Size = new System.Drawing.Size(58, 20);
           this.numSubType.TabIndex = 7;
           this.numSubType.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
           // 
           // labelSubType
           // 
           this.labelSubType.AutoSize = true;
           this.labelSubType.Location = new System.Drawing.Point(6, 24);
           this.labelSubType.Name = "labelSubType";
           this.labelSubType.Size = new System.Drawing.Size(72, 13);
           this.labelSubType.TabIndex = 6;
           this.labelSubType.Text = "File Sub Type";
           // 
           // groupBoxFile
           // 
           this.groupBoxFile.Controls.Add(this.textBoxFileName);
           this.groupBoxFile.Controls.Add(this.buttonBrowse);
           this.groupBoxFile.Location = new System.Drawing.Point(13, 188);
           this.groupBoxFile.Name = "groupBoxFile";
           this.groupBoxFile.Size = new System.Drawing.Size(386, 67);
           this.groupBoxFile.TabIndex = 16;
           this.groupBoxFile.TabStop = false;
           this.groupBoxFile.Text = "Select File";
           // 
           // buttonOk
           // 
           this.buttonOk.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
           this.buttonOk.Location = new System.Drawing.Point(321, 117);
           this.buttonOk.Name = "buttonOk";
           this.buttonOk.Size = new System.Drawing.Size(75, 29);
           this.buttonOk.TabIndex = 17;
           this.buttonOk.Text = "OK";
           this.buttonOk.UseVisualStyleBackColor = true;
           this.buttonOk.Click += new System.EventHandler(this.buttonOk_Click);
           // 
           // buttonCancel
           // 
           this.buttonCancel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
           this.buttonCancel.Location = new System.Drawing.Point(321, 150);
           this.buttonCancel.Name = "buttonCancel";
           this.buttonCancel.Size = new System.Drawing.Size(75, 32);
           this.buttonCancel.TabIndex = 18;
           this.buttonCancel.Text = "Cancel";
           this.buttonCancel.UseVisualStyleBackColor = true;
           this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
           // 
           // openFileDialogScreen
           // 
           this.openFileDialogScreen.InitialDirectory = "\"./SampleDirectory\"";
           this.openFileDialogScreen.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialogScreen_FileOk);
           // 
           // DirectoryEditor
           // 
           this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
           this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
           this.ClientSize = new System.Drawing.Size(412, 262);
           this.Controls.Add(this.buttonCancel);
           this.Controls.Add(this.buttonOk);
           this.Controls.Add(this.groupBoxFile);
           this.Controls.Add(this.groupBoxGeneral);
           this.Controls.Add(this.groupBoxSpecific);
           this.Controls.Add(this.groupIdentifier);
           this.Controls.Add(this.groupDir);
           this.Name = "DirectoryEditor";
           this.Text = "DirectoryEntry";
           this.groupDir.ResumeLayout(false);
           this.groupDir.PerformLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numType)).EndInit();
           ((System.ComponentModel.ISupportInitialize)(this.numIndex)).EndInit();
           this.groupBoxSpecific.ResumeLayout(false);
           this.groupBoxSpecific.PerformLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numSpecificFlags)).EndInit();
           this.groupBoxGeneral.ResumeLayout(false);
           this.groupBoxGeneral.PerformLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numGeneralFlags)).EndInit();
           this.groupIdentifier.ResumeLayout(false);
           this.groupIdentifier.PerformLayout();
           ((System.ComponentModel.ISupportInitialize)(this.numNumber)).EndInit();
           ((System.ComponentModel.ISupportInitialize)(this.numSubType)).EndInit();
           this.groupBoxFile.ResumeLayout(false);
           this.groupBoxFile.PerformLayout();
           this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupDir;
        private System.Windows.Forms.GroupBox groupBoxGeneral;
        private System.Windows.Forms.CheckBox ckAppend;
        private System.Windows.Forms.CheckBox ckArchive;
        private System.Windows.Forms.CheckBox ckErase;
        private System.Windows.Forms.CheckBox ckWrite;
        private System.Windows.Forms.CheckBox ckRead;
        private System.Windows.Forms.Label labelFlags;
        private System.Windows.Forms.NumericUpDown numGeneralFlags;
        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.TextBox textBoxFileName;
        private System.Windows.Forms.GroupBox groupIdentifier;
        private System.Windows.Forms.NumericUpDown numNumber;
        private System.Windows.Forms.Label labelNumber;
        private System.Windows.Forms.NumericUpDown numSubType;
        private System.Windows.Forms.Label labelSubType;
        private System.Windows.Forms.NumericUpDown numType;
        private System.Windows.Forms.Label labelType;
        private System.Windows.Forms.NumericUpDown numIndex;
        private System.Windows.Forms.Label labelIndex;
        private System.Windows.Forms.GroupBox groupBoxSpecific;
        private System.Windows.Forms.CheckBox ckSelected;
        private System.Windows.Forms.Label labelFlags2;
        private System.Windows.Forms.NumericUpDown numSpecificFlags;
        private System.Windows.Forms.GroupBox groupBoxFile;
        private System.Windows.Forms.Button buttonOk;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.OpenFileDialog openFileDialogScreen;
        private System.Windows.Forms.CheckBox ckCrypto;
    }
}