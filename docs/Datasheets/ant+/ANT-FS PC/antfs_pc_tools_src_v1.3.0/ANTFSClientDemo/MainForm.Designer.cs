namespace ANTFSClientDemo
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.buttonOptions = new System.Windows.Forms.Button();
            this.buttonOpen = new System.Windows.Forms.Button();
            this.buttonClose = new System.Windows.Forms.Button();
            this.buttonOpenBeacon = new System.Windows.Forms.Button();
            this.buttonCloseBeacon = new System.Windows.Forms.Button();
            this.textLog = new System.Windows.Forms.RichTextBox();
            this.labelDir = new System.Windows.Forms.Label();
            this.listDirectory = new System.Windows.Forms.ListBox();
            this.labelStatus = new System.Windows.Forms.Label();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.buttonEdit = new System.Windows.Forms.Button();
            this.buttonDelete = new System.Windows.Forms.Button();
            this.groupBoxDirFunctions = new System.Windows.Forms.GroupBox();
            this.buttonLoadSample = new System.Windows.Forms.Button();
            this.buttonClear = new System.Windows.Forms.Button();
            this.buttonOpenBroadcast = new System.Windows.Forms.Button();
            this.buttonCloseBroadcast = new System.Windows.Forms.Button();
            this.buttonReturnBroadcast = new System.Windows.Forms.Button();
            this.groupBoxDirFunctions.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonOptions
            // 
            this.buttonOptions.Location = new System.Drawing.Point(12, 55);
            this.buttonOptions.Name = "buttonOptions";
            this.buttonOptions.Size = new System.Drawing.Size(75, 23);
            this.buttonOptions.TabIndex = 0;
            this.buttonOptions.Text = "Options";
            this.buttonOptions.UseVisualStyleBackColor = true;
            this.buttonOptions.Click += new System.EventHandler(this.buttonOptions_Click);
            // 
            // buttonOpen
            // 
            this.buttonOpen.Location = new System.Drawing.Point(13, 85);
            this.buttonOpen.Name = "buttonOpen";
            this.buttonOpen.Size = new System.Drawing.Size(75, 23);
            this.buttonOpen.TabIndex = 1;
            this.buttonOpen.Text = "Open";
            this.buttonOpen.UseVisualStyleBackColor = true;
            this.buttonOpen.Click += new System.EventHandler(this.buttonOpen_Click);
            // 
            // buttonClose
            // 
            this.buttonClose.Enabled = false;
            this.buttonClose.Location = new System.Drawing.Point(13, 115);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 23);
            this.buttonClose.TabIndex = 2;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // buttonOpenBeacon
            // 
            this.buttonOpenBeacon.Enabled = false;
            this.buttonOpenBeacon.Location = new System.Drawing.Point(402, 55);
            this.buttonOpenBeacon.Name = "buttonOpenBeacon";
            this.buttonOpenBeacon.Size = new System.Drawing.Size(76, 41);
            this.buttonOpenBeacon.TabIndex = 3;
            this.buttonOpenBeacon.Text = "Open Beacon";
            this.buttonOpenBeacon.UseVisualStyleBackColor = true;
            this.buttonOpenBeacon.Click += new System.EventHandler(this.buttonOpenBeacon_Click);
            // 
            // buttonCloseBeacon
            // 
            this.buttonCloseBeacon.Enabled = false;
            this.buttonCloseBeacon.Location = new System.Drawing.Point(403, 102);
            this.buttonCloseBeacon.Name = "buttonCloseBeacon";
            this.buttonCloseBeacon.Size = new System.Drawing.Size(75, 41);
            this.buttonCloseBeacon.TabIndex = 4;
            this.buttonCloseBeacon.Text = "Close Beacon";
            this.buttonCloseBeacon.UseVisualStyleBackColor = true;
            this.buttonCloseBeacon.Click += new System.EventHandler(this.buttonCloseBeacon_Click);
            // 
            // textLog
            // 
            this.textLog.HideSelection = false;
            this.textLog.Location = new System.Drawing.Point(94, 55);
            this.textLog.Name = "textLog";
            this.textLog.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.textLog.Size = new System.Drawing.Size(302, 206);
            this.textLog.TabIndex = 5;
            this.textLog.Text = "";
            // 
            // labelDir
            // 
            this.labelDir.AutoSize = true;
            this.labelDir.Location = new System.Drawing.Point(14, 278);
            this.labelDir.Name = "labelDir";
            this.labelDir.Size = new System.Drawing.Size(49, 13);
            this.labelDir.TabIndex = 6;
            this.labelDir.Text = "Directory";
            // 
            // listDirectory
            // 
            this.listDirectory.FormattingEnabled = true;
            this.listDirectory.Location = new System.Drawing.Point(15, 336);
            this.listDirectory.Name = "listDirectory";
            this.listDirectory.Size = new System.Drawing.Size(460, 147);
            this.listDirectory.TabIndex = 7;
            this.listDirectory.SelectedIndexChanged += new System.EventHandler(this.listDirectory_SelectedIndexChanged);
            // 
            // labelStatus
            // 
            this.labelStatus.AutoSize = true;
            this.labelStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStatus.Location = new System.Drawing.Point(87, 23);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(122, 25);
            this.labelStatus.TabIndex = 8;
            this.labelStatus.Text = "Status: Idle";
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(6, 19);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(75, 23);
            this.buttonAdd.TabIndex = 10;
            this.buttonAdd.Text = "Add Entry";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // buttonEdit
            // 
            this.buttonEdit.Enabled = false;
            this.buttonEdit.Location = new System.Drawing.Point(87, 19);
            this.buttonEdit.Name = "buttonEdit";
            this.buttonEdit.Size = new System.Drawing.Size(75, 23);
            this.buttonEdit.TabIndex = 11;
            this.buttonEdit.Text = "Edit Entry";
            this.buttonEdit.UseVisualStyleBackColor = true;
            this.buttonEdit.Click += new System.EventHandler(this.buttonEdit_Click);
            // 
            // buttonDelete
            // 
            this.buttonDelete.Enabled = false;
            this.buttonDelete.Location = new System.Drawing.Point(168, 19);
            this.buttonDelete.Name = "buttonDelete";
            this.buttonDelete.Size = new System.Drawing.Size(75, 23);
            this.buttonDelete.TabIndex = 12;
            this.buttonDelete.Text = "Delete Entry";
            this.buttonDelete.UseVisualStyleBackColor = true;
            this.buttonDelete.Click += new System.EventHandler(this.buttonDelete_Click);
            // 
            // groupBoxDirFunctions
            // 
            this.groupBoxDirFunctions.Controls.Add(this.buttonLoadSample);
            this.groupBoxDirFunctions.Controls.Add(this.buttonClear);
            this.groupBoxDirFunctions.Controls.Add(this.buttonAdd);
            this.groupBoxDirFunctions.Controls.Add(this.buttonDelete);
            this.groupBoxDirFunctions.Controls.Add(this.buttonEdit);
            this.groupBoxDirFunctions.Location = new System.Drawing.Point(15, 270);
            this.groupBoxDirFunctions.Name = "groupBoxDirFunctions";
            this.groupBoxDirFunctions.Size = new System.Drawing.Size(460, 55);
            this.groupBoxDirFunctions.TabIndex = 13;
            this.groupBoxDirFunctions.TabStop = false;
            this.groupBoxDirFunctions.Text = "Manage Directory";
            // 
            // buttonLoadSample
            // 
            this.buttonLoadSample.Location = new System.Drawing.Point(311, 19);
            this.buttonLoadSample.Name = "buttonLoadSample";
            this.buttonLoadSample.Size = new System.Drawing.Size(70, 23);
            this.buttonLoadSample.TabIndex = 14;
            this.buttonLoadSample.Text = "Sample Dir";
            this.buttonLoadSample.UseVisualStyleBackColor = true;
            this.buttonLoadSample.Click += new System.EventHandler(this.buttonLoadSample_Click);
            // 
            // buttonClear
            // 
            this.buttonClear.Location = new System.Drawing.Point(387, 19);
            this.buttonClear.Name = "buttonClear";
            this.buttonClear.Size = new System.Drawing.Size(67, 23);
            this.buttonClear.TabIndex = 13;
            this.buttonClear.Text = "Clear Dir";
            this.buttonClear.UseVisualStyleBackColor = true;
            this.buttonClear.Click += new System.EventHandler(this.buttonClear_Click);
            // 
            // buttonOpenBroadcast
            // 
            this.buttonOpenBroadcast.Enabled = false;
            this.buttonOpenBroadcast.Location = new System.Drawing.Point(12, 173);
            this.buttonOpenBroadcast.Name = "buttonOpenBroadcast";
            this.buttonOpenBroadcast.Size = new System.Drawing.Size(75, 41);
            this.buttonOpenBroadcast.TabIndex = 14;
            this.buttonOpenBroadcast.Text = "Open Broadcast";
            this.buttonOpenBroadcast.UseVisualStyleBackColor = true;
            this.buttonOpenBroadcast.Click += new System.EventHandler(this.buttonOpenBroadcast_Click);
            // 
            // buttonCloseBroadcast
            // 
            this.buttonCloseBroadcast.Enabled = false;
            this.buttonCloseBroadcast.Location = new System.Drawing.Point(12, 220);
            this.buttonCloseBroadcast.Name = "buttonCloseBroadcast";
            this.buttonCloseBroadcast.Size = new System.Drawing.Size(75, 41);
            this.buttonCloseBroadcast.TabIndex = 15;
            this.buttonCloseBroadcast.Text = "Close Broadcast";
            this.buttonCloseBroadcast.UseVisualStyleBackColor = true;
            this.buttonCloseBroadcast.Click += new System.EventHandler(this.buttonCloseBroadcast_Click);
            // 
            // buttonReturnBroadcast
            // 
            this.buttonReturnBroadcast.Enabled = false;
            this.buttonReturnBroadcast.Location = new System.Drawing.Point(403, 220);
            this.buttonReturnBroadcast.Name = "buttonReturnBroadcast";
            this.buttonReturnBroadcast.Size = new System.Drawing.Size(75, 41);
            this.buttonReturnBroadcast.TabIndex = 16;
            this.buttonReturnBroadcast.Text = "Return to Broadcast";
            this.buttonReturnBroadcast.UseVisualStyleBackColor = true;
            this.buttonReturnBroadcast.Click += new System.EventHandler(this.buttonReturnBroadcast_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(487, 495);
            this.Controls.Add(this.buttonReturnBroadcast);
            this.Controls.Add(this.buttonCloseBroadcast);
            this.Controls.Add(this.buttonOpenBroadcast);
            this.Controls.Add(this.groupBoxDirFunctions);
            this.Controls.Add(this.labelStatus);
            this.Controls.Add(this.listDirectory);
            this.Controls.Add(this.labelDir);
            this.Controls.Add(this.textLog);
            this.Controls.Add(this.buttonCloseBeacon);
            this.Controls.Add(this.buttonOpenBeacon);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.buttonOpen);
            this.Controls.Add(this.buttonOptions);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.Text = "ANT-FS PC Client";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Exit);
            this.groupBoxDirFunctions.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonOptions;
        private System.Windows.Forms.Button buttonOpen;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.Button buttonOpenBeacon;
        private System.Windows.Forms.Button buttonCloseBeacon;
        private System.Windows.Forms.RichTextBox textLog;
        private System.Windows.Forms.Label labelDir;
        private System.Windows.Forms.ListBox listDirectory;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.Button buttonEdit;
        private System.Windows.Forms.Button buttonDelete;
        private System.Windows.Forms.GroupBox groupBoxDirFunctions;
        private System.Windows.Forms.Button buttonClear;
        private System.Windows.Forms.Button buttonLoadSample;
        private System.Windows.Forms.Button buttonOpenBroadcast;
        private System.Windows.Forms.Button buttonCloseBroadcast;
        private System.Windows.Forms.Button buttonReturnBroadcast;
    }
}

