using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using ANT_Managed_Library.ANTFS;

namespace ANTFSClientDemo
{
   class FileType
   {
      internal byte fileType;
      internal byte defaultFlags;
      internal string matchName;

      // Read/write, single file
      internal static FileType Settings = new FileType(2, (byte)(ANTFS_Directory.GeneralFlags.Read | ANTFS_Directory.GeneralFlags.Write), "Settings*.fit");
      // Read/erase, multiple files
      internal static FileType Activity = new FileType(4, (byte)(ANTFS_Directory.GeneralFlags.Read | ANTFS_Directory.GeneralFlags.Erase), "Activity*.fit");
      // Read/write/erase, multiple files
      internal static FileType Workout = new FileType(5, (byte)(ANTFS_Directory.GeneralFlags.Read | ANTFS_Directory.GeneralFlags.Write | ANTFS_Directory.GeneralFlags.Erase), "Workout*.fit");
      // Read only, single file (circular buffer)
      internal static FileType WeightScale = new FileType(9, (byte)(ANTFS_Directory.GeneralFlags.Read), "WeightScale*.fit");
      // Read only, single file (circular buffer)
      internal static FileType BloodPressure = new FileType(14, (byte)(ANTFS_Directory.GeneralFlags.Read), "BloodPressure*.fit");

      private FileType(byte type, byte flags, string filter)
      {
         fileType = type;
         defaultFlags = flags;
         matchName = filter;
      }
   }
}
