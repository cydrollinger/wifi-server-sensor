//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009 Dynastream Innovations Inc.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// To use the ANT-FS functionality in the managed library, you must:
// 1. Import ANT_NET.dll as a reference
// 2. Reference the ANT_Managed_Library.ANTFS namespace
// 3. Include the following files in the working directory of your application:
//  - DSI_CP310xManufacturing_3_1.dll
//  - DSI_SiUSBXp_3_1.dll
//  - ANT_WrappedLib.dll
//  - ANT_NET.dll
//////////////////////////////////////////////////////////////////////////

using System;
using System.Text;
using System.IO;
using System.Collections.Generic;
using System.Threading;
using ANT_Managed_Library.ANTFS;

namespace ANTFS_Demo
{
    class HostDemo
    {
        ANTFS_Host host0;
        ANTFS_Directory dirFS;
        byte[] aucPassKey;
        bool bDone = false;
  
        // USB Device Parameters (if not using automatic initialization)
        //readonly byte ucUSBDeviceNum = 0;       // USB Device Number
        //readonly ushort ulBaudRate = 50000;     // Baud rate (50000 for AP1, and 57600 for AT2 and AP2)

        // ANT Channel Configuration Parameters
#error MUST TYPE ANT-FS NETWORK KEY HERE, OR SET TO ALL 0'S IF USING THE PUBLIC KEY
        readonly byte[] aucNetworkKey = {0x__, 0x__, 0x__, 0x__, 0x__, 0x__, 0x__, 0x__}   // INSERT ANT-FS NETWORK KEY HERE !!!!
        readonly byte ucSearchRF = (byte)Default.SearchRadioFrequency;      // Radio Frequency to use during search
        readonly byte ucConnectRF = (byte)Default.TransportRadioFrequency;  // Radio Frequency to use once a connection is established
        readonly ushort usDeviceNumber = 0;     // Device Number (ANT Channel ID)
        readonly byte ucDeviceType = 0;         // Device Type (ANT Channel ID)
        readonly byte ucTransmissionType = 0;   // Transmission Type (ANT Channel ID)

        // ANT-FS Configuration Parameters
        string strHostFriendlyName = "ManagedDemo"; // ANT-FS Host Friendly Name
        uint ulClientDeviceID = 0xABCDEF12;     // ANT-FS Client Device ID
        ushort usClientManufacturerID = 2;      // ANT-FS Client Manufacturer ID
        ushort usClientDeviceType = 416;        // ANT-FS Client Device Type

        /// <summary>
        /// Create and start ANT-FS host demo application
        /// </summary>
        static void Main()
        {
            HostDemo theDemo = new HostDemo();
            theDemo.Start();
        }

        /// <summary>
        /// Connect to an USB device and start ANT-FS host
        /// </summary>
        public void Start()
        {
            Console.WriteLine("ANT-FS HOST DEMO");
            Thread threadDisplayStatus = new Thread(new ThreadStart(UpdateTransferStatus));  // Handle display progress of download/upload
            threadDisplayStatus.IsBackground = true;
            PrintMenu();
            Console.WriteLine("Attempting to connect to an ANT USB device...");
            try
            {
                // Connect to USB device, automatically detecting USB parameters
                // Alternatively, we can specify the parameters, along with an optional timeout
                // If the library is unable to connect within the configured timeout, a timeout response will be generated
                // If the timeout is set to 0, the library will continue to try to connect to the USB device until it is successful
                // e.g. host0 = new ANTFS_Host(ucUSBDeviceNum, ulBaudRate, false, 60000);
                host0 = new ANTFS_Host();
                                
                // Setup callback to handle ANT-FS response events
                host0.OnResponse += new ANTFS_Host.ResponseHandler(HandleResponses);
                threadDisplayStatus.Start();
                while (!bDone)
                {
                    string command = Console.ReadLine();
                    HandleUserInput(command);
                    Thread.Sleep(0);
                }
            }
            catch(ANTFS_Exception antEx)  // Handle exceptions thrown by ANT Managed library
            {
                Console.WriteLine(antEx.Message);
            }
            catch (Exception ex)  // Handle other exceptions
            {
                Console.WriteLine("Demo failed: " + ex.Message);    
            }
            finally
            {
                if (!threadDisplayStatus.Join(3000))
                    threadDisplayStatus.Abort();
            }
        }

        /// <summary>
        /// Handle ANT-FS response events
        /// </summary>
        /// <param name="sender">Object raising the event</param>
        /// <param name="args">Event arguments</param>
        public void HandleResponses(object sender, ANTFS_EventArgs args)
        {
            Console.WriteLine(Print.AsString(args.responseCurrent));   // Display response
            switch (args.responseCurrent)
            {
                case Response.ConnectionLost:
                case Response.DisconnectPass:
                case Response.OpenPass:    
                    // Configure ANT channel parameters
                    host0.SetNetworkKey(aucNetworkKey);
                    host0.SetChannelID(ucDeviceType, ucTransmissionType);
                    // Clear any stored directory
                    dirFS = null;
                    // Clear any previously configured search list
                    host0.ClearSearchDeviceList();
                    // Configure search parameters
                    if(host0.AddSearchDevice(ulClientDeviceID, usClientManufacturerID, usClientDeviceType) == 0)
                        throw new Exception("Error adding search device: ");
                    Console.WriteLine("Searching for devices...");
                    // Start searching for an ANT-FS client matching the search criteria
                    // The library will search for the specified timeout, and will generate a timeout event if no device is found
                    // Set the timeout to 0 to disable it - the library will continue searching until a device is found or there is an error
                    host0.SearchForDevice(ucSearchRF, ucConnectRF, usDeviceNumber, 60000);  // Timeout = 60 seconds
                    break;
                case Response.ConnectPass:
                    // A device matching the search criteria was found
                    // Obtain and display the device parameters
                    ANTFS_SearchResults foundDevice = host0.GetFoundDeviceParameters();
                    if (foundDevice != null)
                        Console.WriteLine(foundDevice.ToString());
                    else
                        Console.WriteLine("Error obtaining device parameters");
                    break;
                case Response.AuthenticatePass:
                    // The authentication request was accepted
                    // Obtain additional parameters the client sent with the response, if any
                    // If using Pairing authentication mode, the client will send a Passkey after accepting the request
                    byte[] authString = host0.GetAuthResponse();
                    if (authString.Length > 0)
                    {
                        Console.WriteLine("Received Passkey: Stored for use in next session");
                        aucPassKey = authString;    // Store Passkey for future use
                    }           
                    break;
                case Response.DownloadPass:
                    Console.WriteLine(host0.GetDownloadStatus().ToString());
                    // Download completed successfully
                    // Retrieve downloaded data
                    byte[] dlData = host0.GetTransferData();
                    if ((dlData.Length > 0) && (args.usCurrentIndex != null))
                    {
                        if (args.usCurrentIndex == 0)   // Directory
                        {
                            // Parse downloaded directory
                            Console.WriteLine("Received Directory");
                            dirFS = new ANTFS_Directory(dlData);
                            Console.WriteLine(dirFS.ToString());
                        }
                        else
                        {
                            // Store downloaded file
                            Console.WriteLine("Downloaded file " + args.usCurrentIndex + ", Download size: " + host0.GetDownloadSize());
                            if (dlData != null)
                            {
                                File.WriteAllBytes("rawdataout.txt", dlData);
                                Console.WriteLine("Saved to: rawdataout.txt");
                            }
                            else
                            {
                                Console.WriteLine("No data available");
                            }
                        }
                    }
                    else
                    {
                        Console.WriteLine("No data available");
                    }
                    break;
                case Response.UploadPass:
                    // Upload completed successfully
                    Console.WriteLine(host0.GetUploadStatus().ToString());
                    break;
                case Response.Timeout:
                    // Timeout event
                    switch (args.responseTimeout)
                    {
                        case Response.OpenPass: // Failed to connect to an ANT USB device
                            Console.WriteLine("Error initializing");
                            Console.WriteLine("   Is USB stick plugged in?");
                            Console.WriteLine("   Are the baud rate and device number correct?");
                            Console.WriteLine("Press any key to exit");
                            bDone = true;
                            break;
                        case Response.ConnectPass:  // Search timeout
                            Console.WriteLine("Error searching for devices");
                            Console.WriteLine("Press any key to exit");
                            bDone = true;
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Display user menu
        /// </summary>
        public void PrintMenu()
        {
            Console.WriteLine(Environment.NewLine);
            Console.WriteLine("M - Print this menu");
            Console.WriteLine("A - Authenticate");
            Console.WriteLine("D - Download");
            Console.WriteLine("U - Upload");
            Console.WriteLine("E - Erase");
            Console.WriteLine("X - Disconnect");
            Console.WriteLine("C - Cancel");
            Console.WriteLine("P - Print directory");
            Console.WriteLine("V - Request version");
            Console.WriteLine("S - Request status");
            Console.WriteLine("Q - Quit");
            Console.WriteLine(Environment.NewLine);
        }

        /// <summary>
        /// Process user input and execute requested commands
        /// </summary>
        /// <param name="command">User command</param>
        public void HandleUserInput(string command)
        {
            try
            {
                switch (command)
                {
                    case "M":
                    case "m":
                        PrintMenu();
                        break;
                    case "A":
                    case "a":
                        // Send authentication request based on user selection
                        // The library will wait for the configured timeout to receive a response to the request
                        Console.WriteLine("Select authentication mode:");
                        Console.WriteLine("1 - Skip authentication");
                        Console.WriteLine("2 - Request pairing");
                        Console.WriteLine("3 - Send passkey");
                        string authChoice = Console.ReadLine();
                        switch (authChoice)
                        {
                            case "1":
                                host0.Authenticate(AuthenticationType.None, 60000); // Passthru authentication. Timeout = 60 sec
                                break;
                            case "2":
                                host0.Authenticate(AuthenticationType.Pairing, strHostFriendlyName, 60000); // Request pairing. Timeout = 60 sec
                                break;
                            case "3":
                                if (aucPassKey != null)
                                    host0.Authenticate(AuthenticationType.PassKey, aucPassKey, 60000);   // Use passkey, if we have one. Timeout = 60 sec
                                else
                                    Console.WriteLine("Try pairing first");
                                break;
                            default:
                                Console.WriteLine("Invalid authentication type.");
                                break;
                        }
                        break;
                    case "D":
                    case "d":
                        // Send a download request based on user selection
                        Console.WriteLine("Select the file index to download.");
                        Console.WriteLine("Choose 0 for the directory");
                        string downloadChoice = Console.ReadLine();
                        host0.Download(ParseInput(downloadChoice), 0, 0);   // Start download from the beginning, and do not limit size
                        break;
                    case "U":
                    case "u":
                        // Send an upload request based on user selection
                        Console.WriteLine("Select the file index to upload.");
                        string uploadChoice = Console.ReadLine();
                        byte[] ulData;
                        if (File.Exists("rawdataout.txt"))
                            ulData = File.ReadAllBytes("rawdataout.txt");
                        else
                            ulData = new byte[200];
                        host0.Upload(ParseInput(uploadChoice), ulData);
                        Console.WriteLine("Uploading: {0} bytes", ulData.Length);
                        break;
                    case "E":
                    case "e":
                        // Send an erase request depending on user selection
                        Console.WriteLine("Select the file index to erase");
                        string eraseChoice = Console.ReadLine();
                        host0.EraseData(ParseInput(eraseChoice));
                        break;
                    case "X":
                    case "x":
                        // Disconnect from the remote device
                        // Optionally, the remote device can be put in a blackout list
                        host0.Disconnect();
                        break;
                    case "C":
                    case "c":
                        // Cancel pending operations
                        host0.Cancel();
                        break;
                    case "P":
                    case "p":
                        if (dirFS != null)
                            Console.WriteLine(dirFS.ToString());
                        else
                            Console.WriteLine("Try requesting the directory first");
                        break;
                    case "V":
                    case "v":
                        Console.WriteLine("ANT-FS Library Version " + host0.GetLibraryVersion());
                        break;
                    case "S":
                    case "s":
                        Console.WriteLine("Status: " + Print.AsString(host0.GetStatus()));
                        break;
                    case "Q":
                    case "q":
                        bDone = true;
                        break;
                    default:
                        if(!bDone)
                            Console.WriteLine("Invalid option selected");
                        break;
                }
            }
            catch (ANTFS_RequestFailed_Exception antEx)
            {
                // Inform the user that the operation requested failed, and resume operation
                Console.WriteLine(antEx.Message);
            }
            catch (System.ArgumentException argEx)
            {
                // Inform the user about the invalid input, and resume operation
                Console.WriteLine(argEx.Message);
            }
        }

        /// <summary>
        /// Parse user input
        /// </summary>
        /// <param name="selection">String containing user input</param>
        /// <returns>Numeric input</returns>
        private ushort ParseInput(string selection)
        {
            try
            {
                return UInt16.Parse(selection);
            }
            catch(Exception ex)
            {
                throw new System.ArgumentException("Invalid input", ex);
            }
        }

 
        /// <summary>
        /// Display status of an ongoing transfer (upload or download)
        /// </summary>
        public void UpdateTransferStatus()
        {
            while (!bDone)
            {
                State hostState = host0.GetStatus();
                TransferStatus currentStatus = null;
                if (hostState == State.Uploading)
                    currentStatus = host0.GetUploadStatus();
                else if (hostState == State.Downloading)
                    currentStatus = host0.GetDownloadStatus();
                if (currentStatus != null)
                    Console.WriteLine(currentStatus.ToString());
                System.Threading.Thread.Sleep(300);
            }
        }
    
    }
}
