using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace ANTFSClientDemo
{
    public partial class Options : Form
    {
        internal Settings currentConfig;

        enum Presets
        {
            Default,
            FR60,
            FR310XT,
            FitnessWatch,
            WeightScale,
            BloodPressureMonitor,
            None = -1
        }

        public Options(Settings config)
        {
            InitializeComponent();
            currentConfig = config;

            // If config exists, fill in the GUI
            if (currentConfig != null)
                ToGui(currentConfig);
        }

        /// <summary>
        /// Save config and exit
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonOk_Click(object sender, EventArgs e)
        {
            try
            {
                // Get configuration from UI
                FromGui(ref currentConfig);
                // Exit the form
                Close();
            }
            catch (Exception)
            {
                // There was a problem with an input, so we do not exit
                // so user can correct the problem
            }
        }

        /// <summary>
        /// Exit without saving the config changes
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// Save configuration to file
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonSave_Click(object sender, EventArgs e)
        {
            FromGui(ref currentConfig);
            if (saveConfigDialog.ShowDialog() != DialogResult.OK)
                MessageBox.Show("Unable to open file dialog!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
        }

        private void saveConfigDialog_FileOk(object sender, CancelEventArgs e)
        {
            try
            {
                currentConfig.saveToFile(saveConfigDialog.FileName);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unable to save configuration: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        /// <summary>
        /// Load configuration from file
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonLoad_Click(object sender, EventArgs e)
        {
            if (loadConfigDialog.ShowDialog() != DialogResult.OK)
                MessageBox.Show("Unable to open file dialog!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
        }

        private void loadConfigDialog_FileOk(object sender, CancelEventArgs e)
        {
            try
            {
                currentConfig = Settings.loadFromFile(loadConfigDialog.FileName);
                comboPresets.SelectedIndex = (int)Presets.None;
                ToGui(currentConfig);
            }
            catch (FileNotFoundException)
            {
                MessageBox.Show("Unable to open configuration file", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            catch (InvalidDataException)
            {
                MessageBox.Show("Config file does not have correct format", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error loading config: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void comboPresets_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings oldConfig = currentConfig;
            Presets presetConfig = (Presets)comboPresets.SelectedIndex;

            // Apply preset configuration
            switch (presetConfig)
            {
                case Presets.None:  // Do nothing
                    return;
                default:
                case Presets.Default:
                    currentConfig = Settings.DefaultDemo;
                    break;
                case Presets.FR60:
                    currentConfig = Settings.FR60;
                    break;
                case Presets.FR310XT:
                    currentConfig = Settings.FR310XT;
                    break;
                case Presets.FitnessWatch:
                    currentConfig = Settings.FitnessWatch;
                    break;
                case Presets.WeightScale:
                    currentConfig = Settings.WeightScale;
                    break;
                case Presets.BloodPressureMonitor:
                    currentConfig = Settings.BloodPressureMonitor;
                    break;
            }
            
            // Keep settings that do not depend on the device type
            currentConfig.baudRate = oldConfig.baudRate;
            currentConfig.friendlyName = oldConfig.friendlyName;
            currentConfig.passKey = oldConfig.passKey;
            currentConfig.usbDevice = oldConfig.usbDevice;
            currentConfig.isDebugEnabled = oldConfig.isDebugEnabled;
            currentConfig.isAutoInitEnabled = oldConfig.isAutoInitEnabled;

            // Update UI
            ToGui(currentConfig);
        }

        // Handle the channel period display and conversion betwen message rate and period
        private void textBoxPeriod32768_TextChanged(object sender, EventArgs e)
        {
            if (!textBoxPeriod32768.Focused && sender != this)
                return;

            try
            {
                ushort period32768ths = ushort.Parse(textBoxPeriod32768.Text);
                double periodHz = Math.Round((double) 32768 / period32768ths, 3);
                String rounded = periodHz.ToString();

                // Handle rounding to make sure it fits in display
                if (rounded.Length > 5 && !rounded.Equals("Infinity"))
                {
                    if (rounded[4] == '.')
                        rounded = rounded.Remove(4);
                    else
                        rounded = rounded.Remove(5);                    
                }
                textBoxPeriodHz.Text = rounded;
            }
            catch(Exception)
            {
                textBoxPeriodHz.Text = "error";
            }
            
        }

        // Handle the channel period display and conversion between message rate and period
        private void textBoxPeriodHz_TextChanged(object sender, EventArgs e)
        {
            if (!textBoxPeriodHz.Focused)
                return;

            try
            {
                double periodHz = double.Parse(textBoxPeriodHz.Text);
                if (periodHz < 0.5 || periodHz > 32768)
                    throw new Exception("Period out of range");
                ushort period32768ths = (ushort)Math.Round(32768 / periodHz);
                if (period32768ths == 0)    // we only get a zero if it is on a rollover past 65535
                    period32768ths = 65535;
                textBoxPeriod32768.Text = period32768ths.ToString();
            }
            catch (Exception)
            {
                textBoxPeriod32768.Text = "error";
            }

        }   

        void ToGui(Settings config)
        {
            // General Tab
            ckDebug.Checked = config.isDebugEnabled;
            ckAutoInit.Checked = config.isAutoInitEnabled;
            numUSB.Value = config.usbDevice;
            comboBaudRate.SelectedIndex = (int) config.baudRate;

            // ANT Channel Tab
            numDeviceType.Value = config.deviceType;
            numTxType.Value = config.transmissionType;
            numRadioFreq.Value = config.clientParams.BeaconRadioFrequency;

            // Handle update of link period
            switch(config.clientParams.LinkPeriod)
            {
                case ANT_Managed_Library.ANTFS.BeaconPeriod.HalfHz:
                    textBoxPeriod32768.Text = "65535";
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.OneHz:
                    textBoxPeriod32768.Text = "32768";
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.TwoHz:
                    textBoxPeriod32768.Text = "16384";
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.FourHz:
                    textBoxPeriod32768.Text = "8192";
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.EightHz:
                    textBoxPeriod32768.Text = "4096";
                    break;
                default:
                case ANT_Managed_Library.ANTFS.BeaconPeriod.Keep:
                    textBoxPeriod32768.Text = config.broadcastChannelPeriod.ToString();
                    break;
            }
            textBoxPeriod32768_TextChanged(this, null);

            radioAntFs.Checked = (config.network == Settings.Network.AntFs);
            radioAntPlus.Checked = (config.network == Settings.Network.AntPlus);
            radioPublic.Checked = (config.network == Settings.Network.Public);

            // Client Tab
            numBeaconMfgId.Value = config.clientParams.BeaconManufacturerID;
            numBeaconDevType.Value = config.clientParams.BeaconDeviceType;
            numDeviceId.Value = config.clientParams.SerialNumber;
            ckSerialNum.Checked = (config.clientParams.SerialNumber == 0);
            numDeviceId.Enabled = !ckSerialNum.Checked;
            ckPairing.Checked = config.clientParams.IsPairingEnabled;
            ckUploads.Checked = config.clientParams.IsUploadEnabled;
            ckDataAvailable.Checked = config.clientParams.IsDataAvailable;
            numTimeout.Value = config.clientParams.BeaconTimeout;
            numPairingTimeout.Value = config.clientParams.PairingTimeout;
            numMaxBlockSize.Value = config.maxBlockSize;
            ckLimitBlockSize.Checked = (config.maxBlockSize != 0);
            numMaxBlockSize.Enabled = ckLimitBlockSize.Checked;            
            textBoxName.Text = config.friendlyName;

            // Security Tab
            radioPassthru.Checked = (config.clientParams.AuthenticationType == ANT_Managed_Library.ANTFS.AuthenticationType.None);
            radioPairing.Checked = (config.clientParams.AuthenticationType == ANT_Managed_Library.ANTFS.AuthenticationType.Pairing);
            radioPasskey.Checked = (config.clientParams.AuthenticationType == ANT_Managed_Library.ANTFS.AuthenticationType.PassKey);            

            if (config.passKey != null && config.passKey.Length > 0)
                textBoxPassKey.Text = Settings.formatAsHexString(config.passKey);
            else
                 textBoxPassKey.Text = "";   // Clear

            if (config.cryptoKey != null && config.cryptoKey.Length == 32)
            {
                textBoxCrypto1.Text = Settings.formatAsHexString(config.cryptoKey, 0, 15);
                textBoxCrypto2.Text = Settings.formatAsHexString(config.cryptoKey, 16, 31);
            }
            else
            {
                textBoxCrypto1.Text = ""; // Clear
                textBoxCrypto2.Text = "";
            }
        }

        void FromGui(ref Settings config)
        {
            // General Tab
            config.isDebugEnabled = ckDebug.Checked;
            config.isAutoInitEnabled = ckAutoInit.Checked;
            config.usbDevice = (byte) numUSB.Value;
            config.baudRate = (Settings.BaudRate) comboBaudRate.SelectedIndex;

            // ANT Channel Tab
            config.deviceType = (byte) numDeviceType.Value;
            config.transmissionType = (byte) numTxType.Value;
            config.clientParams.BeaconRadioFrequency = (byte)numRadioFreq.Value;

            // Handle update of link period
            ushort previousConfig = config.broadcastChannelPeriod;
            try
            {
                double periodHz = double.Parse(textBoxPeriodHz.Text);   // validate it is not an error
                config.broadcastChannelPeriod = ushort.Parse(textBoxPeriod32768.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Invalid period: " + ex.Message, "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                config.broadcastChannelPeriod = previousConfig;
                throw ex;   // Rethrow to let caller know there was an error
            }

            switch (config.broadcastChannelPeriod)
            {
                case 65535:
                    config.clientParams.LinkPeriod = ANT_Managed_Library.ANTFS.BeaconPeriod.HalfHz;
                    break;
                case 32768:
                    config.clientParams.LinkPeriod = ANT_Managed_Library.ANTFS.BeaconPeriod.OneHz;
                    break;
                case 16384:
                    config.clientParams.LinkPeriod = ANT_Managed_Library.ANTFS.BeaconPeriod.TwoHz;
                    break;
                case 8192:
                    config.clientParams.LinkPeriod = ANT_Managed_Library.ANTFS.BeaconPeriod.FourHz;
                    break;
                case 4096:
                    config.clientParams.LinkPeriod = ANT_Managed_Library.ANTFS.BeaconPeriod.EightHz;
                    break;
                default:
                    config.clientParams.LinkPeriod = ANT_Managed_Library.ANTFS.BeaconPeriod.Keep;
                    break;
            }

            if (radioAntFs.Checked)
                config.network = Settings.Network.AntFs;
            else if (radioAntPlus.Checked)
                config.network = Settings.Network.AntPlus;
            else if (radioPublic.Checked)
                config.network = Settings.Network.Public;

            // Client Tab
            config.clientParams.BeaconManufacturerID = (ushort)numBeaconMfgId.Value;
            config.clientParams.BeaconDeviceType = (ushort)numBeaconDevType.Value;
            if (ckSerialNum.Checked)
                config.clientParams.SerialNumber = 0;
            else
                config.clientParams.SerialNumber = (uint)numDeviceId.Value;
            config.clientParams.IsPairingEnabled = ckPairing.Checked;
            config.clientParams.IsUploadEnabled = ckUploads.Checked;
            config.clientParams.IsDataAvailable = ckDataAvailable.Checked;
            config.clientParams.BeaconTimeout = (byte)numTimeout.Value;
            config.clientParams.PairingTimeout = (byte)numPairingTimeout.Value;
            if (!ckLimitBlockSize.Checked)
               config.maxBlockSize = 0;
            else
               config.maxBlockSize = (uint)numMaxBlockSize.Value;

            try
            {
               config.SetFriendlyName(textBoxName.Text);
            }
            catch (FormatException)
            {
               MessageBox.Show("The friendly name is not a valid ASCII string", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            // Security Tab
            if (radioPassthru.Checked)
                config.clientParams.AuthenticationType = ANT_Managed_Library.ANTFS.AuthenticationType.None;
            else if (radioPairing.Checked)
                config.clientParams.AuthenticationType = ANT_Managed_Library.ANTFS.AuthenticationType.Pairing;
            else if (radioPasskey.Checked)
                config.clientParams.AuthenticationType = ANT_Managed_Library.ANTFS.AuthenticationType.PassKey;

            
            // Parse the key
            try
            {
                config.passKey = Settings.parseHex(textBoxPassKey.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show("The passkey string has an invalid format", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                config.passKey = null;
                throw ex;   // Rethrow to let caller know there was an error
            }

            // Parse the crypto key
            try
            {
                byte[] tempKey = null;
                tempKey = Settings.parseHex(textBoxCrypto1.Text);
                if (tempKey.Length != 16)
                    throw new FormatException("Key is not the right length");
                Array.Copy(tempKey, config.cryptoKey, 16);

                tempKey = Settings.parseHex(textBoxCrypto2.Text);
                if (tempKey.Length != 16)
                    throw new FormatException("Key is not the right length");
                Array.Copy(tempKey, 0, config.cryptoKey, 16, 16);
            }
            catch (Exception ex)
            {
                MessageBox.Show("The crypto key has an invalid format" + ex.Message, "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                config.cryptoKey = null;
                throw ex;   // Rethrow to let caller know there was an error
            }
        }

        private void buttonLogs_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("explorer.exe", currentConfig.logPath);
        }

        private void comboPath_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboPath.SelectedIndex == (int)Settings.LogDirectory.AppData)
                currentConfig.SetLogPath(Settings.LogDirectory.AppData, null);
            else
            {
                if (selectFolderDialog.ShowDialog() == DialogResult.OK)
                {
                    currentConfig.SetLogPath(Settings.LogDirectory.Custom, selectFolderDialog.SelectedPath);
                }
            }
        }

        private void ckAutoInit_CheckedChanged(object sender, EventArgs e)
        {
           if (ckAutoInit.Checked)
           {
              numUSB.Enabled = false;
              comboBaudRate.Enabled = false;
           }
           else
           {
              numUSB.Enabled = true;
              comboBaudRate.Enabled = true;
           }
        }

        private void ckSerialNum_CheckedChanged(object sender, EventArgs e)
        {
           if (ckSerialNum.Checked)
           {
              numDeviceId.Value = 0;
              numDeviceId.Enabled = false;
           }
           else
           {
              // Make sure device ID is not zero
              // Zero means - auto generate from serial number
              numDeviceId.Value = 100;
              numDeviceId.Enabled = true;
           }
        }

        private void ckLimitBlockSize_CheckedChanged(object sender, EventArgs e)
        {
           if (ckLimitBlockSize.Checked)
           {
              numMaxBlockSize.Enabled = true;
              numMaxBlockSize.Value = 512;
           }
           else
           {
              // Zero means do not limit block size (attempt to send everything in a single block)
              numMaxBlockSize.Value = 0;
              numMaxBlockSize.Enabled = false;
           }
        }     
        
    }
}
