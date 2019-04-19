using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ANT_Managed_Library
{
    /// <summary>
    /// Structure containing the data composing a channel ID
    /// </summary>
    public struct ANT_ChannelID
    {
        /// <summary>
        /// Device Number
        /// </summary>
        public UInt16 deviceNumber;
        /// <summary>
        /// Pairing Bit
        /// </summary>
        public bool pairingBit;
        /// <summary>
        /// Device Type ID
        /// </summary>
        public byte deviceTypeID;
        /// <summary>
        /// Transmission Type ID
        /// </summary>
        public byte transmissionTypeID;

        /// <summary>
        /// Initializes a new Channel ID Object
        /// </summary>
        /// <param name="deviceNumber">Device Number</param>
        /// <param name="deviceType">Device Type ID</param>
        /// <param name="transmissionType">Transmission Type ID</param>
        public ANT_ChannelID(ushort deviceNumber, byte deviceType, byte transmissionType)
        {
            this.deviceNumber = deviceNumber;
            this.deviceTypeID = deviceType;
            this.transmissionTypeID = transmissionType;
            this.pairingBit = false;
        }
    }
}
