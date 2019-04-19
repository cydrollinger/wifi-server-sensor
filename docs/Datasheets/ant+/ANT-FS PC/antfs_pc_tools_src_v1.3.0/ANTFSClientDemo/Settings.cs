using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using ANT_Managed_Library.ANTFS;

namespace ANTFSClientDemo
{
    public class Settings
    {
        #region Definitions and Constants
        public enum Network : byte
        {
            AntFs,
            AntPlus,
            Public
        }

        public enum BaudRate : byte
        {
            Baud50000,
            Baud57600
        }

        public enum LogDirectory : byte
        {
            AppData,
            Custom
        }

#error MUST TYPE ANT-FS NETWORK KEY HERE, OR SET TO ALL 0'S IF USING THE PUBLIC KEY
        internal readonly static byte[] AntFsKey = {0x__, 0x__, 0x__, 0x__, 0x__, 0x__, 0x__, 0x__};   // INSERT ANT-FS NETWORK KEY HERE !!!!
        internal readonly static byte[] AntPlusKey = {0x__, 0x__, 0x__, 0x__, 0x__, 0x__, 0x__, 0x__};   // INSERT ANT+ NETWORK KEY HERE !!!!    
        internal readonly static byte[] PublicKey = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // More precisely, all 0's is an invalid key, which results in no change to the default key

        readonly static string ConfigName = "ClientSettings";
        readonly static string VersionName = "Version";
        readonly static string GeneralConfigName = "GeneralSettings";
        readonly static string AntfsConfigName = "AntfsClientParameters";

        // Presets
        internal static Settings DefaultDemo = new Settings();
        internal static Settings FR60 = new Settings(Network.AntFs, 1, 988, 1, BeaconPeriod.EightHz, AuthenticationType.PassKey);
        internal static Settings FR310XT = new Settings(Network.AntFs, 1, 1018, 1, BeaconPeriod.EightHz, AuthenticationType.PassKey);
        internal static Settings FitnessWatch = new Settings(Network.AntPlus, 1, 1, 15, BeaconPeriod.FourHz, AuthenticationType.None);
        internal static Settings WeightScale = new Settings(Network.AntPlus, 119, 1, 15, BeaconPeriod.FourHz, AuthenticationType.PassKey);
        internal static Settings BloodPressureMonitor = new Settings(Network.AntPlus, 18, 1, 15, BeaconPeriod.FourHz, AuthenticationType.PassKey);

        #endregion


        // Application settings
        public Boolean isDebugEnabled = false;
        public LogDirectory logPathSelection = LogDirectory.AppData;
        public string logPath = "";

        // USB settings
        public Boolean isAutoInitEnabled = false;
        public BaudRate baudRate = BaudRate.Baud50000; // USB1/AP1
        public byte usbDevice = 0;

        // ANT channel parameters
        public byte deviceType = 1;
        public byte transmissionType = 5;
        public Network network = Network.AntFs;
        public ushort broadcastChannelPeriod = 8192;

        public uint maxBlockSize = 0;
      
        public string friendlyName = "PCDemo";
        internal byte[] friendlyNameAsByteArray;    // Do not save in config file

        public byte[] passKey = new byte[16]; // by default, initialized to zero
        public byte[] cryptoKey = new byte[32]; // by default, initialized to zero

        internal ANTFS_ClientParameters clientParams;   // Not saved in config file as part of the Settings node, save in its own node     

        

        /// <summary>
        /// Default constructor
        /// </summary>
        Settings()
        {
            clientParams.BeaconDeviceType = 416;    // To match PC host config
            clientParams.BeaconManufacturerID = 2;  // To match PC host config
            clientParams.BeaconRadioFrequency = 50;
            clientParams.LinkPeriod = BeaconPeriod.FourHz;
            clientParams.AuthenticationType = AuthenticationType.None;
            clientParams.SerialNumber = 0;   // Use serial number from USB
            clientParams.IsPairingEnabled = true;
            clientParams.IsUploadEnabled = false;
            clientParams.IsDataAvailable = false;            
            clientParams.BeaconTimeout = 60;
            clientParams.PairingTimeout = 60;

            SetFriendlyName(friendlyName);
            SetLogPath(LogDirectory.AppData, null);
        }

        /// <summary>
        /// Device specific constructor
        /// </summary>
        /// <param name="theNetwork">Network key and frequency to use</param>
        /// <param name="theDeviceType">ANT channel device type</param>
        /// <param name="theBeaconDeviceType">ANT-FS beacon device type</param>
        /// <param name="theBeaconManufacturerID">Manufacturer ID</param>
        /// <param name="thePeriod">Channel period</param>
        Settings(Network theNetwork, byte theDeviceType, ushort theBeaconDeviceType, ushort theBeaconManufacturerID, BeaconPeriod thePeriod, AuthenticationType authType)
        {            
            network = theNetwork;
            if(network == Network.AntPlus)
            {
                clientParams.BeaconRadioFrequency = 57;
            }
            else
            {
                clientParams.BeaconRadioFrequency = 50;
            }
            deviceType = theDeviceType;
            clientParams.BeaconDeviceType = theBeaconDeviceType;
            clientParams.BeaconManufacturerID = theBeaconManufacturerID;
            clientParams.LinkPeriod = thePeriod;
            switch (clientParams.LinkPeriod)
            {
                case ANT_Managed_Library.ANTFS.BeaconPeriod.HalfHz:
                    broadcastChannelPeriod = 65535;
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.OneHz:
                    broadcastChannelPeriod = 32768;
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.TwoHz:
                    broadcastChannelPeriod = 16384;
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.FourHz:
                    broadcastChannelPeriod = 8192;
                    break;
                case ANT_Managed_Library.ANTFS.BeaconPeriod.EightHz:
                    broadcastChannelPeriod = 4096;
                    break;
            }
            clientParams.AuthenticationType = authType;
            clientParams.SerialNumber = 0;   // Use serial number from USB
            clientParams.IsPairingEnabled = true;
            clientParams.IsUploadEnabled = false;
            clientParams.IsDataAvailable = true;
            clientParams.BeaconTimeout = 60;
            clientParams.PairingTimeout = 60;

            SetFriendlyName(friendlyName);
            SetLogPath(LogDirectory.AppData, null);
        }

        /// <summary>
        ///  Xml based constructor
        /// </summary>
        /// <param name="generalConfig">Xml node corresponding to the general settings</param>
        /// <param name="antfsConfig">Xml node corresponding to the ANT-FS client struct settings</param>
        Settings(XmlNode generalConfig, XmlNode antfsConfig)
        {
            // Load the general config
            // First, get list of the nodes in the configuration file
            XmlNodeList xmlFields = generalConfig.ChildNodes;
            foreach (XmlNode node in xmlFields)
            {
                // Get the name of the field from the config
                System.Reflection.FieldInfo field = typeof(Settings).GetField(node.LocalName);
                
                if(field.FieldType.BaseType == typeof(Enum))    // Parse enums
                    field.SetValue(this, Enum.Parse(field.FieldType, node.FirstChild.Value));
                else if (field.FieldType == typeof(System.Byte[])) // Parse byte array
                {
                    if (node.LocalName == "passKey")
                        passKey = parseHex(node.InnerText);
                    else if (node.LocalName == "cryptoKey")
                        cryptoKey = parseHex(node.InnerText);
                }
                else // Parse other types
                    field.SetValue(this, Convert.ChangeType(node.FirstChild.Value, field.FieldType));
            }

            // Now fill in the ANTFS_ClientParameters
            // This is a struct (value type), so we use the RuntimeHelpers to box the value type
            // and create an object we could use with Reflection
            object clientParamsObject = System.Runtime.CompilerServices.RuntimeHelpers.GetObjectValue(clientParams);

            xmlFields = antfsConfig.ChildNodes;
            foreach (XmlNode node in xmlFields)
            {
                // Get the name of the field
                System.Reflection.FieldInfo field = typeof(ANTFS_ClientParameters).GetField(node.LocalName);

                if (field.FieldType.BaseType == typeof(Enum)) // Parse enums
                    field.SetValue(clientParamsObject, Enum.Parse(field.FieldType, node.FirstChild.Value)); // Get value
                else // Parse other types
                    field.SetValue(clientParamsObject, Convert.ChangeType(node.FirstChild.Value, field.FieldType)); // Get value
            }

            // Unbox back into value type
            clientParams = (ANTFS_ClientParameters) clientParamsObject;
            clientParams.PairingTimeout = 60;
            
            // Convert the friendly name to byte array
            SetFriendlyName(friendlyName);

            // Set the log path
            SetLogPath(logPathSelection, logPath);
        }

        internal void SetFriendlyName(String name)
        {
            friendlyName = name;

            if(name == null || name.Length == 0)
            {
                friendlyNameAsByteArray = null;
                return;
            }

            try
            {
                friendlyNameAsByteArray = System.Text.Encoding.ASCII.GetBytes(name);
            }
            catch (Exception ex)
            {
                friendlyName = null;
                friendlyNameAsByteArray = null;
                throw new FormatException("Friendly name is not a valid ASCII string", ex);
            }
        }

        internal void SetLogPath(LogDirectory selectedPath, string path)
        {
            if (selectedPath == LogDirectory.AppData)
            {
                logPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "ANTFS_PC_Client\\logs");
            }
            else if(selectedPath == LogDirectory.Custom)
            {
                logPath = path;
            }
        }


        #region Save/Load

        internal XElement convertVersionToXElement()
        {
            return new XElement(VersionName, new XAttribute("type", typeof(String)), ANTFS_ClientChannel.GetLibraryVersion());
        }

        internal XElement convertSettingsToXElement()
        {
            XElement settingsXml = new XElement(GeneralConfigName);

            // Get all the public fields of this class
            System.Reflection.FieldInfo[] fields = typeof(Settings).GetFields();
            foreach (System.Reflection.FieldInfo fi in fields)
            {
                // Add each element to the Xml tree
                if (fi.Name.Equals("passKey")) // store byte arrays as comma delimited hex pairs
                {
                    settingsXml.Add(new XElement(fi.Name, new XAttribute("type", fi.FieldType), formatAsHexString(passKey)));
                }
                else if (fi.Name.Equals("cryptoKey")) // byte array
                {
                    settingsXml.Add(new XElement(fi.Name, new XAttribute("type", fi.FieldType), formatAsHexString(cryptoKey)));
                }
                else
                {
                    settingsXml.Add(new XElement(fi.Name, new XAttribute("type", fi.FieldType), fi.GetValue(this)));
                }
            }   

            return settingsXml;
        }

        internal XElement convertAntfsStructToXElement()
        {
            XElement settingsXml = new XElement(AntfsConfigName);

            // Add the ANTFS_Client_Params struct to the config
            System.Reflection.FieldInfo[] fields =  typeof(ANTFS_ClientParameters).GetFields();
            foreach (System.Reflection.FieldInfo fi in fields)
            {
                // Add to xml tree
                settingsXml.Add(new XElement(fi.Name, new XAttribute("type", fi.FieldType), fi.GetValue(clientParams)));
            }

            return settingsXml;
        }

        internal void saveToFile(string filePath)
        {
            if(File.Exists(filePath))
                File.Delete(filePath);  // Delete old file

            // Create new XML document
            XElement configXml = new XElement(ConfigName, 
                new XComment("ANT-FS Client Settings - Created: " + DateTime.Now.ToString()),
                convertVersionToXElement(),
                convertSettingsToXElement(),
                convertAntfsStructToXElement());
            configXml.Save(filePath);
        }

        internal static Settings loadFromFile(string filePath)
        {
            XmlDocument configFile = new XmlDocument();
            configFile.Load(filePath);

            if (!configFile.DocumentElement.LocalName.Equals(ConfigName))
                throw new InvalidDataException("The configuration file does not have a valid format");

            // The ClientSettings should have three child nodes, plus a comment: (1) Version, (2) GeneralSettings, (3) AntfsClientParameters
            // Get the child node list
            XmlNodeList xmlNodes = configFile.DocumentElement.ChildNodes;

            if(xmlNodes.Count != 4)
                throw new InvalidDataException("The configuration file does not have a valid format");

            // Check the version
            if (xmlNodes[1].LocalName.Equals(VersionName))
            {
                if (! xmlNodes[1].InnerText.Equals(ANTFS_ClientChannel.GetLibraryVersion()))
                    throw new InvalidDataException("Format mismatch, file might not be parsed correctly");
            }
            else
                throw new InvalidDataException("The configuration file does not have a valid format");

            if (!xmlNodes[2].LocalName.Equals(GeneralConfigName) || !xmlNodes[3].LocalName.Equals(AntfsConfigName))
                throw new InvalidDataException("The configuration file does not have a valid format");

            return new Settings(xmlNodes[2], xmlNodes[3]);
        }

        internal static byte[] parseHex(string text)
        {
            //First we split the string by the delimiters
            string[] byteStrings = text.Split(new char[] { ',' });

            // No string
            if (byteStrings.Length == 0)
                return null;

            //Make sure it isn't too long
            if (byteStrings.Length > Byte.MaxValue)
                throw new FormatException("Can't parse string, exceeds maximum length (255 bytes)");

            //Parse the given values as hex
            byte[] toReturn = new byte[Math.Max(byteStrings.Length, 0)];

            int i;
            for (i = 0; i < byteStrings.Length; ++i)
            {
                toReturn[i] = Convert.ToByte(byteStrings[i], 16);
            }

            return toReturn;
        }

        internal static string formatAsHexString(byte[] byteArray)
        {
            //Make sure it isn't too long
            if (byteArray.Length > Byte.MaxValue)
                throw new FormatException("Can't format string, exceeds maximum length (255 bytes)");

            StringBuilder tempString = new StringBuilder();
            foreach (byte b in byteArray)
            {
                tempString.AppendFormat("{0:X2},", b); // Format as hex, comma separated
            }
            tempString.Remove(tempString.Length - 1, 1);    //Remove extra comma
            return tempString.ToString();
        }

        internal static string formatAsHexString(byte[] byteArray, byte beginIndex, byte endIndex)
        {
            if(endIndex < beginIndex)
                throw new ArgumentException("Invalid index boundaries specified");

            byte[] tempArray = new byte[endIndex - beginIndex + 1];
            Array.Copy(byteArray, beginIndex, tempArray, 0, endIndex - beginIndex + 1);

            return formatAsHexString(tempArray);
        }

        #endregion
    }
}
