using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ANT_Managed_Library.ANTFS;

namespace ANTFSClientDemo
{
    public partial class DirectoryEditor : Form
    {
        public enum EditorActions
        {
            Add,
            Edit,
        }

        FileHandler myFiles;
        EditorActions currentMode;
        ushort currentIndex;
        ANTFS_Directory.Entry currentEntry;
        string currentPath = "";


        public DirectoryEditor(FileHandler appFiles, EditorActions mode, ushort index)
        {
            InitializeComponent();
            myFiles = appFiles;
            currentMode = mode;
            currentIndex = index;

            if (mode == EditorActions.Add)
            {
                InitDefaultEntry(index);
            }
            else
            {
                currentEntry = appFiles.GetEntry(index);
                numIndex.Enabled = false;   // Do not allow modifications to the index
            }

            ToGui();
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            try
            {
                FromGui();
                if (currentMode == EditorActions.Add)
                    myFiles.AddEntry(currentPath, currentEntry);
                else
                    myFiles.UpdateEntry(currentPath, currentEntry);

                Close();    // Exit
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            if (openFileDialogScreen.ShowDialog() != DialogResult.OK)
                MessageBox.Show("Unable to open file dialog!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
        }

        private void openFileDialogScreen_FileOk(object sender, CancelEventArgs e)
        {
            textBoxFileName.Text = openFileDialogScreen.FileName;
        }

        private void FromGui()
        {
            currentEntry.FileIndex = (ushort) numIndex.Value;
            currentEntry.FileDataType = (byte) numType.Value;
            currentEntry.FileSubType = (byte) numSubType.Value;
            currentEntry.FileNumber = (ushort) numNumber.Value;
            currentEntry.GeneralFlags = (byte) numGeneralFlags.Value;
            currentEntry.SpecificFlags = (byte) numSpecificFlags.Value;
            currentPath = textBoxFileName.Text;

            if (currentPath == null || currentPath.Length == 0)
            {
                throw new ArgumentException("No path specified");
            }
        }

        private void ToGui()
        {
            numIndex.Value = currentEntry.FileIndex;
            numSubType.Value = currentEntry.FileSubType;
            numNumber.Value = currentEntry.FileNumber;
            numSpecificFlags.Value = currentEntry.SpecificFlags;
            numType.Value = currentEntry.FileDataType;
            numGeneralFlags.Value = currentEntry.GeneralFlags;
            textBoxFileName.Text = myFiles.GetPath(currentEntry.FileIndex);
        }

        private void InitDefaultEntry(ushort index)
        {
            currentEntry.FileIndex = index;
            currentEntry.FileDataType = ANTFS_Directory.FitDataType;
            currentEntry.FileNumber = 0;
            currentEntry.FileSubType = 0;
            currentEntry.GeneralFlags = (byte)(ANTFS_Directory.GeneralFlags.Read | ANTFS_Directory.GeneralFlags.Write | ANTFS_Directory.GeneralFlags.Erase);
            currentEntry.SpecificFlags = 0;
            currentEntry.TimeStamp = 0;
            currentEntry.FileSize = 0;
        }

        private void numType_ValueChanged(object sender, EventArgs e)
        {
            if (numType.Value == ANTFS_Directory.FitDataType)
            {
                ckSelected.Enabled = true;
            }
            else
            {
                ckSelected.Checked = false;
                ckSelected.Enabled = false;
            }

        }

        private void numGeneralFlags_ValueChanged(object sender, EventArgs e)
        {
            byte temp = (byte)numGeneralFlags.Value;
            ckRead.Checked = ((temp & (byte)ANTFS_Directory.GeneralFlags.Read) != 0);
            ckWrite.Checked = ((temp & (byte)ANTFS_Directory.GeneralFlags.Write) != 0);
            ckErase.Checked = ((temp & (byte)ANTFS_Directory.GeneralFlags.Erase) != 0);
            ckAppend.Checked = ((temp & (byte)ANTFS_Directory.GeneralFlags.Append) != 0);
            ckArchive.Checked = ((temp & (byte)ANTFS_Directory.GeneralFlags.Archive) != 0);
            ckCrypto.Checked = ((temp & (byte)ANTFS_Directory.GeneralFlags.Crypto) != 0);
        }

        private void ckRead_CheckedChanged(object sender, EventArgs e)
        {
            if (ckRead.Checked)
                numGeneralFlags.Value = (byte)numGeneralFlags.Value | (byte)ANTFS_Directory.GeneralFlags.Read;
            else
                numGeneralFlags.Value = (byte)numGeneralFlags.Value & ~(byte)ANTFS_Directory.GeneralFlags.Read;
        }

        private void ckWrite_CheckedChanged(object sender, EventArgs e)
        {
            if (ckWrite.Checked)
                numGeneralFlags.Value = (byte)numGeneralFlags.Value | (byte)ANTFS_Directory.GeneralFlags.Write;
            else
                numGeneralFlags.Value = (byte)numGeneralFlags.Value & ~(byte)ANTFS_Directory.GeneralFlags.Write;
        }

        private void ckErase_CheckedChanged(object sender, EventArgs e)
        {
            if (ckErase.Checked)
                numGeneralFlags.Value = (byte)numGeneralFlags.Value | (byte)ANTFS_Directory.GeneralFlags.Erase;
            else
                numGeneralFlags.Value = (byte)numGeneralFlags.Value & ~(byte)ANTFS_Directory.GeneralFlags.Erase;
        }

        private void ckArchive_CheckedChanged(object sender, EventArgs e)
        {
            if (ckArchive.Checked)
                numGeneralFlags.Value = (byte)numGeneralFlags.Value | (byte)ANTFS_Directory.GeneralFlags.Archive;
            else
                numGeneralFlags.Value = (byte)numGeneralFlags.Value & ~(byte)ANTFS_Directory.GeneralFlags.Archive;
        }

        private void ckAppend_CheckedChanged(object sender, EventArgs e)
        {
            if (ckAppend.Checked)
                numGeneralFlags.Value = (byte)numGeneralFlags.Value | (byte)ANTFS_Directory.GeneralFlags.Append;
            else
                numGeneralFlags.Value = (byte)numGeneralFlags.Value & ~(byte)ANTFS_Directory.GeneralFlags.Append;
        }

        private void ckCrypto_CheckedChanged(object sender, EventArgs e)
        {
            if (ckCrypto.Checked)
                numGeneralFlags.Value = (byte)numGeneralFlags.Value | (byte)ANTFS_Directory.GeneralFlags.Crypto;
            else
                numGeneralFlags.Value = (byte)numGeneralFlags.Value & ~(byte)ANTFS_Directory.GeneralFlags.Crypto;
        }

        private void numSpecificFlags_ValueChanged(object sender, EventArgs e)
        {
            byte temp = (byte)numSpecificFlags.Value;
            if (!(numType.Value == ANTFS_Directory.FitDataType))
            {
                ckSelected.Enabled = true;
            }
            ckSelected.Checked = ((temp & (byte)ANTFS_Directory.FitSpecificFlags.Selected) != 0);
            if (!(numType.Value == ANTFS_Directory.FitDataType))
            {
                ckSelected.Enabled = false;
            }
        }

        private void ckSelected_CheckedChanged(object sender, EventArgs e)
        {
            if (numType.Value != ANTFS_Directory.FitDataType)  // only use with FIT files
               return;

            if (ckSelected.Checked)
                numSpecificFlags.Value = (byte)numSpecificFlags.Value | (byte)ANTFS_Directory.FitSpecificFlags.Selected;
            else
                numSpecificFlags.Value = (byte)numSpecificFlags.Value & ~(byte)ANTFS_Directory.FitSpecificFlags.Selected;
        }        
    }
}
