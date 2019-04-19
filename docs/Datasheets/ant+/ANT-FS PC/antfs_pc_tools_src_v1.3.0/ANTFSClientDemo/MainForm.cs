using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using ANT_Managed_Library.ANTFS;
using ANT_Managed_Library;

namespace ANTFSClientDemo
{
    public partial class MainForm : Form
    {
        readonly string ConfigFileName = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "ANTFS_PC_Client\\ClientConfig.txt");
        readonly string StatusString = "Status: ";
        readonly byte ChannelNumber = 0;
        readonly byte NetworkNumber = 0;

        ANT_Device antDevice;
        ANTFS_ClientChannel antfsClient;
        ANT_Channel antfsBroadcastChannel;
        FileHandler appFiles;
        Settings appConfig;
        BackgroundWorker downloadProgress = new BackgroundWorker();
        AppState appState = AppState.Closed;

        ushort requestedIndex = 0xFFFF;
        byte[] BroadcastBuffer = new byte[8];

        enum AppState
        {
            Closed,
            Ready,
            Broadcast,
            AntFS,
            ReturnToBroadcast
        };

        public MainForm()
        {
            InitializeComponent();

            Version version = Assembly.GetExecutingAssembly().GetName().Version;
            printLine("SW Version: ARX" + version.Major + "." + version.Minor + version.Build);
            printLine("Library Version: " + ANTFS_ClientChannel.GetLibraryVersion());

            try
            { 
                // Load config file
                appConfig = Settings.loadFromFile(ConfigFileName);

                // Only load custom path if it already exists (e.g. config file comes from another computer)
                if (!System.IO.Directory.Exists(appConfig.logPath))
                    appConfig.SetLogPath(Settings.LogDirectory.AppData, null);
            }
            catch (Exception)
            {
                // Use the default
                appConfig = Settings.DefaultDemo;
            }

            appFiles = new FileHandler(appConfig);

            downloadProgress.WorkerReportsProgress = true;
            downloadProgress.WorkerSupportsCancellation = true;
            downloadProgress.DoWork += new DoWorkEventHandler(checkDownloadStatus);
            downloadProgress.ProgressChanged += new ProgressChangedEventHandler(updateDownloadStatus);

            setInterfaceState(ANTFS_ClientChannel.State.Off);
            refreshDirectoryView();
        }

        private void Exit(object sender, FormClosingEventArgs e)
        {
            lock (this)
            {
                if (antfsClient != null)
                {
                    antfsClient.Dispose();
                    antfsClient = null;
                }

                if (antDevice != null)
                {                    
                    antDevice.Dispose();
                    antDevice = null;
                    antfsBroadcastChannel = null;                    
                }
            }

            try
            {
                // Create app directory if it does not exist
                if (!System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(ConfigFileName)))
                    System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(ConfigFileName));
                
                // Save config
                appConfig.saveToFile(ConfigFileName);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unable to save configuration: " + ex.Message, "Error");
            }
        }
        

        /// <summary>
        /// Open connection to the USB stick
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonOpen_Click(object sender, EventArgs e)
        {
            if (antDevice == null)
            {
                try
                {
                    appState = AppState.Closed;
                    setInterfaceState(ANTFS_ClientChannel.State.Off);

                    // Setup debugging
                    if (appConfig.isDebugEnabled)
                    {
                        string debugPath = null;

                        try
                        {
                            // Create log directory if it does not exist
                            if (!System.IO.Directory.Exists(appConfig.logPath))
                                System.IO.Directory.CreateDirectory(appConfig.logPath);
                            debugPath = appConfig.logPath + "\\";
                            ANT_Common.enableDebugLogs(debugPath);
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("Unable to access specified log directory, check Options::" + ex.Message, "Error");
                        }
                    }
                    else
                    {
                        ANT_Common.disableDebugLogs();
                    }

                    // Attempt to connect to the device
                    if (appConfig.isAutoInitEnabled)
                    {
                        antDevice = new ANT_Device();
                    }
                    else
                    {
                        ushort baud = 50000;
                        if (appConfig.baudRate == Settings.BaudRate.Baud57600)
                            baud = 57600;
                        antDevice = new ANT_Device(appConfig.usbDevice, baud);
                    }

                    
                    
                    // Setup callback for ANT-FS events                    
                    antfsClient = new ANTFS_ClientChannel(antDevice.getChannel(ChannelNumber));
                    antfsClient.OnResponse += new Action<ANTFS_ClientChannel.Response>(handleAntfsResponses);

                    // Setup callbacks for ANT-FS broadcast mode
                    antDevice.deviceResponse += handleANTResponses;
                    antfsBroadcastChannel = antDevice.getChannel(ChannelNumber);
                    antfsBroadcastChannel.channelResponse += handleBroadcastChannelResponses;

                    // Setup callback for serial errors
                    antDevice.serialError += new ANT_Device.dSerialErrorHandler(handleSerialError);

                    // Update UI
                    appState = AppState.Ready;
                    setInterfaceState(ANTFS_ClientChannel.State.Idle);
                    printLine("Successful connection to USB stick");                                                                  
                } 
                catch (Exception)
                {
                    // There was a problem connecting to ANT or initializing the client
                    printLine("Failed to connect to USB stick");
                    appState = AppState.Closed;
                    setInterfaceState(ANTFS_ClientChannel.State.Off);                   

                    if (antDevice != null)
                    {
                        antDevice.Dispose();
                        antDevice = null;
                        antfsBroadcastChannel = null;
                    }

                    if (antfsClient != null)
                    {
                        antfsClient.Dispose();
                        antfsClient = null;
                    }
                }
            }
        }

        /// <summary>
        /// Close connection to the USB stick
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonClose_Click(object sender, EventArgs e)
        {
            // If we were tracking a download, cancel the task
            if (downloadProgress.WorkerSupportsCancellation)
                downloadProgress.CancelAsync();

            lock(this)
            {
                if (antDevice != null)
                {
                    antDevice.Dispose();
                    antDevice = null;
                    antfsBroadcastChannel = null;
                }

                if (antfsClient != null)
                {
                    antfsClient.Dispose();
                    antfsClient = null;
                }
            }

            appState = AppState.Closed;
            setInterfaceState(ANTFS_ClientChannel.State.Off);
            printLine("USB connection closed");
        }

        /// <summary>
        /// Initialize the client and start beaconing
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonOpenBeacon_Click(object sender, EventArgs e)
        {
            // Open beacon
            if (antfsClient != null)
            { 
                configureClient();      
                antfsClient.OpenBeacon();
            }
        }

        /// <summary>
        /// Close the beacon
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonCloseBeacon_Click(object sender, EventArgs e)
        {
            // If we were tracking a download, cancel the task
            if (downloadProgress.WorkerSupportsCancellation)
                downloadProgress.CancelAsync();

            // Close beacon
            if (antfsClient != null)
            {
                antfsClient.CloseBeacon(false);  // close beacon and channel
            }
        }

        /// <summary>
        /// Configure ANT-FS client parameters
        /// </summary>
        private void configureClient()
        {
            if (appConfig.clientParams.SerialNumber == 0)
                appConfig.clientParams.SerialNumber = antDevice.getSerialNumber();

            antfsClient.Configure(appConfig.clientParams);
            antfsClient.SetChannelID(appConfig.deviceType, appConfig.transmissionType);

            if (appConfig.network == Settings.Network.AntFs)
                antfsClient.SetClientNetworkKey(NetworkNumber, Settings.AntFsKey);
            else if (appConfig.network == Settings.Network.AntPlus)
                antfsClient.SetClientNetworkKey(NetworkNumber, Settings.AntPlusKey);
            else
                antfsClient.SetClientNetworkKey(NetworkNumber, Settings.PublicKey); // Note: This will only work if this is done after a reset

            antfsClient.SetFriendlyName(appConfig.friendlyName);
            antfsClient.SetPassKey(appConfig.passKey);

            if (appConfig.clientParams.LinkPeriod.Equals(BeaconPeriod.Keep))
                antfsClient.SetChannelPeriod(appConfig.broadcastChannelPeriod);
        }

        /// <summary>
        /// Serial error event handler
        /// </summary>
        /// <param name="sender">Reference to ANT_Device where the error happened</param>
        /// <param name="error">Serial error code</param>
        /// <param name="isCritical">If the error is critical, serial communication is no longer functional</param>
        private void handleSerialError(ANT_Device sender, ANT_Device.serialErrorCode error, bool isCritical)
        {
            printLine("Error: Serial Failure");
            BeginInvoke(new EventHandler(buttonClose_Click), this, null); // Close the device
        }

        /// <summary>
        /// ANT-FS event handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void handleAntfsResponses(ANTFS_ClientChannel.Response response)
        {
            switch (response)
            {
                case ANTFS_ClientChannel.Response.InitPass:
                    break;
                case ANTFS_ClientChannel.Response.SerialFail:
                    appState = AppState.Closed;
                    printLine("Error: Serial Failure");
                    BeginInvoke(new EventHandler(buttonClose_Click), this, null);                 
                    break;
                case ANTFS_ClientChannel.Response.BeaconOpen:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Beaconing);
                    printLine("Beacon open, Device ID: " + appConfig.clientParams.SerialNumber);
                    printLine("Waiting for a host to connect...");
                    break;
                case ANTFS_ClientChannel.Response.BeaconClosed:
                    if (appState == AppState.ReturnToBroadcast)
                    {
                        appState = AppState.Broadcast;
                        setInterfaceState(ANTFS_ClientChannel.State.Idle);
                        printLine("Return to broadcast");
                        printLine("Waiting for a host to request an ANT-FS session...");
                    }
                    else
                    {
                        appState = AppState.Ready;
                        setInterfaceState(ANTFS_ClientChannel.State.Idle);
                        printLine("Beacon closed");
                    }
                    break;
                case ANTFS_ClientChannel.Response.ConnectPass:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Connected);
                    printLine("Found by host device");
                    printLine("Waiting for host requests...");
                    break;
                case ANTFS_ClientChannel.Response.DisconnectPass:
                    ANTFS_DisconnectParameters disconnectParams = antfsClient.GetDisconnectParameters();
                    // Handle disconnect command type
                    if (disconnectParams.CommandType == (byte)DisconnectType.Broadcast)
                    {
                        appState = AppState.Broadcast;                        
                        printLine("Disconnected successfully: Return to Broadcast");                        
                    }
                    else
                    { 
                        appState = AppState.AntFS;                        
                        printLine("Disconnected successfully: Return to Link");
                        if (disconnectParams.CommandType != (byte)DisconnectType.Link)
                            printLine(String.Format("Custom disconnect command: {0}", disconnectParams.CommandType));  
                    }
                    // Handle undiscoverable mode
                    // Note: at this time, we will just become undiscoverable indefinitely, by closing the channel until it
                    // is manually opened again.  We may also want to add features like using the pairing bit to become undiscoverable
                    // to a wildcard search while staying connected to a paired device
                    if ((disconnectParams.TimeDuration != 0) || (disconnectParams.ApplicationSpecificDuration != 0))
                    {
                        if (disconnectParams.TimeDuration != 0)
                            printLine(String.Format("Undiscoverable mode requested for {0} seconds", disconnectParams.TimeDuration * 30));
                        if (disconnectParams.ApplicationSpecificDuration != 0)
                            printLine(String.Format("Undiscoverable application specific duration requested: {0}", disconnectParams.ApplicationSpecificDuration));
                        if (appState == AppState.AntFS)
                        {
                            BeginInvoke(new EventHandler(buttonCloseBeacon_Click), this, null);
                        }
                        else
                        {
                            BeginInvoke(new EventHandler(buttonCloseBroadcast_Click), this, null);
                        }
                        printLine("Please manually open the channel again as needed.");
                    }
                    else // Update the UI
                    {
                        if (appState == AppState.Broadcast)
                        {
                            setInterfaceState(ANTFS_ClientChannel.State.Idle);
                            printLine("Waiting for a host to request an ANT-FS session...");
                        }
                        else
                        {
                            setInterfaceState(ANTFS_ClientChannel.State.Beaconing);
                            printLine("Waiting for a host to connect...");
                        }
                    }
                    break;
                case ANTFS_ClientChannel.Response.ConnectionLost:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Beaconing);
                    printLine("Lost Connection");
                    printLine("Waiting for a host to begin session...");
                    break;
                case ANTFS_ClientChannel.Response.AuthenticatePass:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Transport);
                    printLine("Authenticate Passed!");
                    printLine("Waiting for host requests...");
                    break;
                case ANTFS_ClientChannel.Response.AuthenticateReject:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Beaconing);
                    printLine("Authenticate Rejected.");
                    printLine("Waiting for a host to begin session...");
                    break;
                case ANTFS_ClientChannel.Response.PairingRequest:
                    appState = AppState.AntFS;
                    printLine("Received pairing request");
                    setInterfaceState(ANTFS_ClientChannel.State.WaitingForPairingResponse);

                    // Prompt the user for a response
                    DialogResult result = handlePairing(antfsClient.GetHostName());
                    if (result == DialogResult.Yes)
                    {
                        antfsClient.SendPairingResponse(true);
                    }
                    else
                    {
                        antfsClient.SendPairingResponse(false);
                    }
                    break;
                case ANTFS_ClientChannel.Response.PairingTimeout:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Connected);
                    printLine("Pairing request timed out");
                    printLine("Waiting for host requests...");
                    break;
                case ANTFS_ClientChannel.Response.DownloadRequest:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.WaitingForDownloadData);
                    requestedIndex = 0;
                    try
                    {
                        requestedIndex = antfsClient.GetRequestedFileIndex();
                        if (requestedIndex == 0)
                            printLine("Download Request: Directory");
                        else
                            printLine("Download Request: File Index " + requestedIndex);
                        if (appFiles.IsFileEncrypted(requestedIndex))
                            printLine("Encrypting data for download...");
                        byte[] data = appFiles.ReadFile(requestedIndex);
                        antfsClient.SendDownloadResponse(DownloadResponse.Ok, requestedIndex, appConfig.maxBlockSize, data);
                        setInterfaceState(ANTFS_ClientChannel.State.Downloading);
                        printLine("Downloading...");                        
                    }
                    catch (ArgumentException)
                    {
                        printLine("Index does not exist");
                        antfsClient.SendDownloadResponse(DownloadResponse.InvalidIndex, requestedIndex, null);
                    }
                    catch (UnauthorizedAccessException)
                    {
                        printLine("Not enough permissions to download the file");
                        antfsClient.SendDownloadResponse(DownloadResponse.FileNotReadable, requestedIndex, null);
                    }
                    catch (Exception otherEx)
                    {
                        printLine(otherEx.Message);
                        antfsClient.SendDownloadResponse(DownloadResponse.NotReady, requestedIndex, null);
                    }
                    break;
                case ANTFS_ClientChannel.Response.DownloadFail:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Transport);
                    downloadProgress.CancelAsync();
                    requestedIndex = 0xFFFF;
                    printLine("Download Failed");
                    break;
                case ANTFS_ClientChannel.Response.DownloadFileNotReadable:
                case ANTFS_ClientChannel.Response.DownloadInvalidIndex:
                case ANTFS_ClientChannel.Response.DownloadNotReady:
                case ANTFS_ClientChannel.Response.DownloadReject:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Transport);
                    downloadProgress.CancelAsync();
                    requestedIndex = 0xFFFF;
                    printLine("Download Rejected");
                    break;
                case ANTFS_ClientChannel.Response.DownloadPass:
                    appState = AppState.AntFS;
                    appFiles.ArchiveEntry(requestedIndex);
                    setInterfaceState(ANTFS_ClientChannel.State.Transport);
                    downloadProgress.CancelAsync();
                    requestedIndex = 0xFFFF;
                    printLine("Download Passed");
                    refreshDirectoryView();
                    break;
                case ANTFS_ClientChannel.Response.EraseRequest:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Erasing);
                    ushort erasedIndex;
                    try
                    {
                        erasedIndex = antfsClient.GetRequestParameters().FileIndex;
                        printLine("Erase Request: File Index " + erasedIndex);
                        appFiles.DeleteFile(erasedIndex);
                        antfsClient.SendEraseResponse(EraseResponse.Ok);
                        printLine("Erasing...");
                    }
                    catch (ArgumentException)
                    {
                        printLine("Index does not exist");
                        antfsClient.SendEraseResponse(EraseResponse.Reject);
                    }
                    catch (UnauthorizedAccessException)
                    {
                        printLine("Not enough permissions to erase file");
                        antfsClient.SendEraseResponse(EraseResponse.Reject);
                    }
                    catch (Exception ex)
                    {
                        printLine(ex.Message);
                        antfsClient.SendEraseResponse(EraseResponse.Reject);
                    }
                    break;
                case ANTFS_ClientChannel.Response.EraseFail:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Transport);
                    printLine("Erase Response Failed");
                    break;
                case ANTFS_ClientChannel.Response.ErasePass:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Transport);
                    printLine("Erase Complete!");
                    refreshDirectoryView();
                    break;
                case ANTFS_ClientChannel.Response.EraseReject:
                    appState = AppState.AntFS;
                    setInterfaceState(ANTFS_ClientChannel.State.Transport);
                    printLine("Erase rejected");
                    break;
                default:
                    ANTFS_ClientChannel.State temp = antfsClient.GetStatus();
                    if (temp == ANTFS_ClientChannel.State.Off)
                        appState = AppState.Closed;
                    else if (temp != ANTFS_ClientChannel.State.Idle)
                        appState = AppState.AntFS;
                    setInterfaceState(antfsClient.GetStatus());                                         
                    break;
            }
        }

        #region ANT-FS broadcast

        private void buttonOpenBroadcast_Click(object sender, EventArgs e)
        {
            // Begin configuration of the client in broadcast mode
            if (antfsBroadcastChannel != null)
            {
                configureBroadcastChannel();
            }
        }

        private void buttonCloseBroadcast_Click(object sender, EventArgs e)
        {
            // Close the channel
            if (antfsBroadcastChannel != null)
            {
                closeBroadcastChannel();
            }
        }

        private void buttonReturnBroadcast_Click(object sender, EventArgs e)
        {
            if (antfsClient != null)
            {
                antfsClient.CloseBeacon(true);
                appState = AppState.ReturnToBroadcast;
            }
        }        

        private void handleANTResponses(ANT_Response response)
        {
#if DEBUG
            String stringToPrint;
            
            if (response.responseID == (byte)ANT_ReferenceLibrary.ANTMessageID.RESPONSE_EVENT_0x40)
            {
                if ((response.getChannelEventCode() == ANT_ReferenceLibrary.ANTEventID.RESPONSE_NO_ERROR_0x00))
                {
                    stringToPrint = "SET " + ((ANT_ReferenceLibrary.ANTMessageID)response.getMessageID()).ToString() + "... OK";
                    printDebugLine(stringToPrint);                    
                }
                else
                {
                    stringToPrint = "Response: " + response.getChannelEventCode().ToString() + " to " + ((ANT_ReferenceLibrary.ANTMessageID)response.getMessageID()).ToString();
                    printDebugLine(stringToPrint);
                }
            }
            else
            {
                stringToPrint = ((ANT_ReferenceLibrary.ANTMessageID)response.responseID).ToString();
                printDebugLine(stringToPrint);
            }
#endif
        }

        private void handleBroadcastChannelResponses(ANT_Response response)
        {
            if (appState != AppState.Broadcast)
                return;
                            
            if (response.responseID == (byte)ANT_ReferenceLibrary.ANTMessageID.RESPONSE_EVENT_0x40)
            {
                switch (response.getChannelEventCode())
                {
                    case ANT_ReferenceLibrary.ANTEventID.EVENT_TX_0x03:
                        antfsBroadcastChannel.sendBroadcastData(BroadcastBuffer);
                        BroadcastBuffer[7] = (byte)(BroadcastBuffer[7] + 1);
                        break;
                    default:                        
                        break;
                }
            }
            else
            {                
                if (response.responseID == (byte)ANT_ReferenceLibrary.ANTMessageID.ACKNOWLEDGED_DATA_0x4F)
                {
                    // Look for request page
                    if (response.getDataPayload()[0] == 0x46)
                    {
                        if (response.getDataPayload()[5] == 0x00 &&
                            response.getDataPayload()[6] == 0x43 &&  // Beacon
                            response.getDataPayload()[7] == 0x02)    // ANT-FS request
                        {
                            printLine("ANT-FS Session requested");
                            appState = AppState.AntFS;
                            configureClient();
                            if (appConfig.clientParams.LinkPeriod == BeaconPeriod.Keep)
                                antfsClient.SetChannelPeriod(appConfig.broadcastChannelPeriod);

                            antfsClient.OpenBeacon();
                        }
                    }
                }
            }               
        }

        private void configureBroadcastChannel()
        {
            byte[] theKey = Settings.PublicKey; // Note: there are some limitations on using the public key           
                      

            // Check channel status
            ANT_ChannelStatus theStatus = antfsBroadcastChannel.requestStatus(500);
            if (theStatus.BasicStatus == ANT_ReferenceLibrary.BasicChannelStatusCode.TRACKING_0x3)
            {
                return;
            }

            if (appConfig.network == Settings.Network.AntFs)
                theKey = Settings.AntFsKey;
            else if (appConfig.network == Settings.Network.AntPlus)
                theKey = Settings.AntPlusKey;

            
            if (theStatus.BasicStatus == ANT_ReferenceLibrary.BasicChannelStatusCode.UNASSIGNED_0x0)
            {        
                if (!antfsBroadcastChannel.assignChannel(ANT_ReferenceLibrary.ChannelType.BASE_Master_Transmit_0x10, NetworkNumber, 500))
                {
                    printLine("Error assigning channel");
                    return;
                }
            }

            if (!antDevice.setNetworkKey(NetworkNumber, theKey, 500))
            {
                printLine("Error assigning network key");
                return;
            }

            ushort deviceNumber= (ushort) antDevice.getSerialNumber();
            if (deviceNumber == 0)
                deviceNumber = 0x0f0f;

            if (!antfsBroadcastChannel.setChannelID(deviceNumber, false, appConfig.deviceType, appConfig.transmissionType, 500))
            {
                printLine("Error configuring channel ID");
                return;
            }

            if(!antfsBroadcastChannel.setChannelFreq(appConfig.clientParams.BeaconRadioFrequency, 500))
            {
                printLine("Error configuring radio frequency");
                return;
            }

            if(!antfsBroadcastChannel.setChannelPeriod(appConfig.broadcastChannelPeriod, 500))
            {
                printLine("Error configuring channel period");
                return;
            }

            if (!antfsBroadcastChannel.openChannel(500))
            {
                printLine("Failed to open the channel");
                return;
            }

            printLine("ANT-FS Broadcast channel open");
            appState = AppState.Broadcast;
            setInterfaceState(ANTFS_ClientChannel.State.Idle);
        }

        private void closeBroadcastChannel()
        {
            if (!antfsBroadcastChannel.closeChannel(2000))
            {
                printLine("Failed to close channel");
                return;
            }

            printLine("ANT-FS Broadcast channel closed");
            appState = AppState.Ready;
            setInterfaceState(ANTFS_ClientChannel.State.Idle);
        }

        #endregion

        #region Demo Configuration

        // Open configuration screen
        private void buttonOptions_Click(object sender, EventArgs e)
        {
            Options configDialog = new Options(appConfig);
            configDialog.FormClosed += new FormClosedEventHandler(options_FormClosed);
            configDialog.Show();
        }

        void options_FormClosed(object sender, FormClosedEventArgs e)
        {
            // Update the configuration when the Options form is closed
            appConfig = ((Options)sender).currentConfig;
        }    

        #endregion

        #region User Interface Updates

        /// <summary>
        /// Updates the status of the user interface per the status of the ANT-FS client
        /// This method updates the Status label at the top, as well as the state of the buttons
        /// </summary>
        private void setInterfaceState(ANTFS_ClientChannel.State clientState)
        {
            // InvokeRequired checks if the calling thread is the UI thread or another thread
            if (InvokeRequired)
            {
                // We are not on UI thread, we need to call BeginInvoke
                // The Action<T> delegate can be used to pass a method without explicitly declaring a custom delegate
                BeginInvoke(new Action<ANTFS_ClientChannel.State>(setInterfaceState), clientState);
                return;
            }

            // We are in the UI thread, so we can directly update the UI
            switch (clientState)
            {
                case ANTFS_ClientChannel.State.Off:
                    labelStatus.Text = StatusString + "Off";
                    buttonOpen.Enabled = true;
                    buttonClose.Enabled = false;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = false;
                    break;
                case ANTFS_ClientChannel.State.Idle:
                    labelStatus.Text = StatusString + "Ready";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = true;
                    buttonCloseBeacon.Enabled = false;                    
                    break;
                case ANTFS_ClientChannel.State.Opening:
                    labelStatus.Text = StatusString + "Starting ANT-FS...";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.Beaconing:
                    labelStatus.Text = StatusString + "Beaconing...";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.Connected:
                    labelStatus.Text = StatusString + "Connected";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.Disconnecting:
                    labelStatus.Text = StatusString + "Disconnecting...";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.Authenticating:
                    labelStatus.Text = StatusString + "Authenticating...";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.WaitingForPairingResponse:
                    labelStatus.Text = StatusString + "Waiting for pairing...";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.Transport:
                    labelStatus.Text = StatusString + "Transport";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.Downloading:
                    labelStatus.Text = StatusString + "Downloading...";
                    if (!downloadProgress.IsBusy)
                        downloadProgress.RunWorkerAsync();
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.WaitingForDownloadData:
                    labelStatus.Text = StatusString + "Processing download request...";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                case ANTFS_ClientChannel.State.Erasing:
                    labelStatus.Text = StatusString + "Erasing...";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
                default:
                    labelStatus.Text = StatusString + "Unknown";
                    buttonOpen.Enabled = false;
                    buttonClose.Enabled = true;
                    buttonOpenBeacon.Enabled = false;
                    buttonCloseBeacon.Enabled = true;
                    break;
            }

            // Update ANT-FS broadcast buttons
            switch (appState)
            {
                case AppState.AntFS:
                    buttonOpenBroadcast.Enabled = false;
                    buttonCloseBroadcast.Enabled = false;
                    if (clientState == ANTFS_ClientChannel.State.Beaconing)
                        buttonReturnBroadcast.Enabled = true;
                    else
                        buttonReturnBroadcast.Enabled = false;
                    break;
                case AppState.Closed:
                    buttonOpenBroadcast.Enabled = false;
                    buttonCloseBroadcast.Enabled = false;
                    buttonReturnBroadcast.Enabled = false;
                    break;
                case AppState.Ready:
                    buttonOpenBroadcast.Enabled = true;
                    buttonCloseBroadcast.Enabled = false;
                    buttonReturnBroadcast.Enabled = false;
                    break;
                case AppState.Broadcast:
                    labelStatus.Text = StatusString + "Broadcasting...";
                    buttonOpenBroadcast.Enabled = false;
                    buttonCloseBroadcast.Enabled = true;
                    buttonReturnBroadcast.Enabled = false;
                    break;
            }
        }

        /// <summary>
        /// Adds a new line with the provided text to the log in the main text box
        /// </summary>
        /// <param name="text">Text to append to the main text box</param>
        private void printLine(string text)
        {
            // InvokeRequired checks if the calling thread is the UI thread or another thread
            if (InvokeRequired)
            {
                // We are not on UI thread, we need to call BeginInvoke
                // The Action<T> delegate can be used to pass a method without explicitly declaring a custom delegate
                BeginInvoke(new Action<string>(printLine), text);   
            }
            else
            {
                // We are in the UI thread, so we can directly update the UI
                textLog.Focus();    // Focus on the text box so that it can auto scroll
                textLog.AppendText(text);
                textLog.AppendText(Environment.NewLine);
            }
        }

        /// <summary>
        /// Adds a new line with the provided text to the debug console
        /// </summary>
        /// <param name="text">Text to append to debug console</param>
        private void printDebugLine(string text)
        {
            System.Diagnostics.Debug.WriteLine(text);
        }

        /// <summary>
        /// Displays dialog to accept or reject a pairing request
        /// </summary>
        /// <param name="hostName">Friendly name of the host.</param>
        /// <returns>Yes (Accept) or No (Reject)</returns>
        private DialogResult handlePairing(string hostName)
        {
            // InvokeRequired checks if the calling thread is the UI thread or another thread
            if(InvokeRequired)
            {
                // We are not on UI thread, we need to call Invoke
                // We use Invoke to call this synchronously, blocking the UI thread
                // The Func<T, TR> delegate can be used to pass a method without explicitly declaring a custom delegate
                return (DialogResult) Invoke(new Func<string, DialogResult>(handlePairing), hostName);
            }

            if (hostName == null)
                hostName = "";

            // Make sure this form owns the message box, so that it appears in front of it
            return MessageBox.Show(this, "Pair with host " + hostName + "?",
                                   "Pair?",
                                   MessageBoxButtons.YesNo,
                                   MessageBoxIcon.Question);
        }

        private void refreshDirectoryView()
        {
            // InvokeRequired checks if the calling thread is the UI thread or another thread
            if (InvokeRequired)
            {
                // We are not on the UI thread, we need to call BeginInvoke
                // The MethodInvoker delegate can be used to invoke a method with a void parameter 
                // list without defining a custom delegate
                BeginInvoke(new MethodInvoker(refreshDirectoryView));
                return;
            }

            // Update beacon parameters if directory contents have changed
            bool previousDataAvailable = appConfig.clientParams.IsDataAvailable;

            if (appFiles.NumberOfEntries == 0)
                appConfig.clientParams.IsDataAvailable = false;
            else
                appConfig.clientParams.IsDataAvailable = true;

            if(antfsClient != null && previousDataAvailable != appConfig.clientParams.IsDataAvailable)
                antfsClient.SetDataAvailable(appConfig.clientParams.IsDataAvailable);

            // Update directory listing
            listDirectory.Items.Clear();
            for (int i = 0; i < appFiles.NumberOfEntries; i++)
            {
                listDirectory.Items.Add(appFiles.PrintEntry(i));
            }
        }

        #endregion

        #region Download Progress

        private void checkDownloadStatus(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            int progress = 0;

            do
            {
                if(worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }
                else
                {                    
                    progress = antfsClient.GetDownloadStatus();
                    worker.ReportProgress(progress);
                    if(progress <100)
                        System.Threading.Thread.Sleep(500);
                }
            } while(progress < 100);
        }

        private void updateDownloadStatus(object sender, ProgressChangedEventArgs e)
        {
            labelStatus.Text = StatusString + "Downloading..." + e.ProgressPercentage.ToString() + "%";
        }


        #endregion

        #region Directory Functions
        private void buttonAdd_Click(object sender, EventArgs e)
        {
            ushort nextIndex = (ushort) (appFiles.GetMaxDirIndex() + 1);
            if (nextIndex == 0)
                nextIndex = 1;
            DirectoryEditor editor = new DirectoryEditor(appFiles, DirectoryEditor.EditorActions.Add, nextIndex);
            editor.FormClosed += new FormClosedEventHandler(editor_FormClosed);
            editor.Show();            
        }              

        private void buttonEdit_Click(object sender, EventArgs e)
        {
            try
            {
                if (listDirectory.SelectedIndex == -1)
                    throw new ArgumentException("No index selected for editing");
                ushort editIndex = appFiles.GetDirIndex(listDirectory.SelectedIndex);
                DirectoryEditor editor = new DirectoryEditor(appFiles, DirectoryEditor.EditorActions.Edit, editIndex);
                editor.FormClosed += new FormClosedEventHandler(editor_FormClosed);
                editor.Show();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            try
            {
                if (listDirectory.SelectedIndex == -1)
                    throw new ArgumentException("No index selected for deleting");
                appFiles.DeleteEntry(listDirectory.SelectedIndex);
                refreshDirectoryView();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void buttonLoadSample_Click(object sender, EventArgs e)
        {
            try
            {
                appFiles.LoadSampleDirectory();
                refreshDirectoryView();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unable to load sample directory: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            appFiles.ClearDirectory();
            refreshDirectoryView();
        }

        private void listDirectory_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listDirectory.SelectedIndex == -1)
            {
                buttonEdit.Enabled = false;
                buttonDelete.Enabled = false;
            }
            else
            {
                buttonEdit.Enabled = true;
                buttonDelete.Enabled = true;
            }
        }

        void editor_FormClosed(object sender, FormClosedEventArgs e)
        {
            refreshDirectoryView();
        }        

        #endregion             

        

    }
}
