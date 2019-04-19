using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using ANT_Managed_Library.ANTFS;
using AES_CTR;

namespace ANTFSClientDemo
{
    public class FileHandler
    {
        internal Settings currentConfig;      
        public static readonly string SampleDirectoryPath = AppDomain.CurrentDomain.BaseDirectory + "SampleDirectory";

        public static readonly DateTime ReferenceDate = new DateTime(1989, 12, 31, 0, 0, 0);   // December 31, 1989, 0:00 hrs
        public static readonly DateTime PowerUpTime = DateTime.UtcNow;    // Set the initial power up time to the time the application is launched

        private FileType[] supportedFiles = {FileType.Settings, FileType.Activity, FileType.Workout, FileType.WeightScale, FileType.BloodPressure};

        class FileEntry
        {
            internal ushort index;   // this index is the one that maps it to the ANT-FS directory structure
            internal string path;

            internal FileEntry(ushort fileIndex, string filePath)
            {
                index = fileIndex;
                path = filePath;
            }
        }
        
        ANTFS_Directory AntfsDir;   // ANT-FS directory structure
        List<FileEntry> filesDir;   // File system directory structure

        /// <summary>
        /// Retrieve the number of entries in the directory
        /// </summary>
        public int NumberOfEntries
        {
            get { return filesDir.Count; }
        }
        
        /// <summary>
        /// Create empty directory
        /// </summary>
        public FileHandler(Settings config)
        {
            AntfsDir = new ANTFS_Directory();
            filesDir = new List<FileEntry>();
            currentConfig = config;

            UpdateDate();
        }       
        
        /// <summary>
        /// Load sample directory included with application
        /// </summary>
        public void LoadSampleDirectory()
        {
            ClearDirectory();

            ANTFS_Directory.Entry newEntry;
            string[] sampleFiles = null;
            ushort currentIndex = 1;

            foreach(FileType type in supportedFiles)
            {
               try
               {
                  sampleFiles = Directory.GetFiles(SampleDirectoryPath, type.matchName);
                  for (ushort i = 0; i < sampleFiles.Length; i++)
                  {
                     newEntry = new ANTFS_Directory.Entry();
                     newEntry.FileDataType = ANTFS_Directory.FitDataType;
                     newEntry.FileSubType = type.fileType;
                     newEntry.FileIndex = currentIndex;
                     newEntry.FileNumber = i;
                     newEntry.GeneralFlags = type.defaultFlags;

                     AddEntry(sampleFiles[i], newEntry);
                     currentIndex++;
                  }                  
               }
               catch (Exception ex)
               {
                  throw new FileNotFoundException("Unable to access sample directory", ex);
               }
            }
        }

        /// <summary>
        /// Adds a file to the directory list.
        /// Throws an exception if the entry already exists
        /// Creates a new empty file if the file does not exist
        /// </summary>
        /// <param name="path">File path</param>
        /// <param name="entry">ANT-FS directory entry</param>
        public void AddEntry(string path, ANTFS_Directory.Entry entry)
        {
            
            FileInfo fi = new FileInfo(path);

            // Figure out if file exists
            if (!fi.Exists)
            {
                // Create new file
                File.Create(path).Dispose();
                entry.FileSize = 0;
                entry.TimeStamp = (UInt32)(DateTime.UtcNow - ReferenceDate).TotalSeconds;
            }
            else
            {
                // Figure out file size
                if (fi.Length > UInt32.MaxValue)
                    throw new FileLoadException("File is too big");

                entry.FileSize = (UInt32)fi.Length;
                entry.TimeStamp = (UInt32)(fi.LastWriteTimeUtc - ReferenceDate).TotalSeconds;

                if ((entry.GeneralFlags & (byte)ANTFS_Directory.GeneralFlags.Crypto) != 0)
                    entry.FileSize += 16; // add the crypto vector size
            }

            // Add to the directory
            AntfsDir.AddEntry(entry); // throws ArgumentException if it fails, let the exception bubble up

            // Add to our internal list
            filesDir.Add(new FileEntry(entry.FileIndex, path));

            // Update the date in the directory header
            UpdateDate();
        }

        /// <summary>
        /// Updates an existing directory entry
        /// Throws an exception if the entry does not exist
        /// </summary>
        /// <param name="path"></param>
        /// <param name="entry"></param>
        public void UpdateEntry(string path, ANTFS_Directory.Entry entry)
        {            
            FileInfo fi = new FileInfo(path);

            // Figure out if file exists
            if (!fi.Exists)
            {
                // Create new file
                File.Create(path).Dispose();
                entry.FileSize = 0;
                entry.TimeStamp = (UInt32)(DateTime.UtcNow - ReferenceDate).TotalSeconds;
            }
            else
            {
                // Figure out file size
                if (fi.Length > UInt32.MaxValue)
                    throw new FileLoadException("File is too big");

                entry.FileSize = (UInt32)fi.Length;
                entry.TimeStamp = (UInt32)(fi.LastWriteTimeUtc - ReferenceDate).TotalSeconds;

                if ((entry.GeneralFlags & (byte)ANTFS_Directory.GeneralFlags.Crypto) != 0)
                    entry.FileSize += 16; // add the crypto vector size
            }

            // Find the entry
            int index = filesDir.FindIndex(x => x.index == entry.FileIndex);

            if(index == -1) // File not on the list
                throw new ArgumentException("Entry does not exist");

            // Try to update the ANT-FS directory first
            AntfsDir.AddOrReplaceEntry(entry); // throws ArgumentException if it fails, let the exception bubble up
            // Update the entry
            filesDir[index].index = entry.FileIndex;
            filesDir[index].path = path;

            // Update the date in the directory header
            UpdateDate();
        }

        /// <summary>
        /// Deletes an entry from the directory
        /// </summary>
        /// <param name="listIndex">Index of the file in the file system list.  This index might not be the same as the one in the internal ANT-FS directory list.</param>
        public void DeleteEntry(int listIndex)
        {
            if (listIndex >= filesDir.Count)
                throw new ArgumentException("Invalid list index");

            // Try to update the ANT-FS directory first
            AntfsDir.ForceDeleteEntry(filesDir[listIndex].index); // throws ArgumentException if it fails, let the exception bubble up

            // Remove the entry from the list
            filesDir.RemoveAt(listIndex);

            // Update the date in the directory header
            UpdateDate();
        }

        /// <summary>
        /// Clears all directory entries
        /// </summary>
        public void ClearDirectory()
        {
            filesDir.Clear();
            AntfsDir.Clear();

            // Update the date in the directory header
            UpdateDate();
        }

        /// <summary>
        /// Gets the maximum index present in the directory.
        /// This can be used to figure out what index to use when creating a new entry.
        /// although it does not account for deleted entries
        /// </summary>
        /// <returns>Maximum ANT-FS directory entry</returns>
        public ushort GetMaxDirIndex()
        {
            if (filesDir.Count == 0)
                return 0;

            return filesDir.Max(x => x.index);
        }

        public ushort GetDirIndex(int listIndex)
        {
            if (listIndex >= filesDir.Count)
                throw new ArgumentException("Entry does not exist");

            return filesDir[listIndex].index;            
        }

        public int GetListIndex(ushort dirIndex)
        {
            // Find the entry
            int index = filesDir.FindIndex(x => x.index == dirIndex);

            if (index == -1) // File not on the list
                throw new ArgumentException("Entry does not exist");

            return index;
        }

        public string GetPath(ushort dirIndex)
        {
            // Find the entry
            int index = filesDir.FindIndex(x => x.index == dirIndex);

            if (index == -1)
                return "";

            return filesDir[index].path;

        }

        /// <summary>
        /// Retrieves an entry from the ANT-FS directory corresponding to an index in the file system
        /// </summary>
        /// <param name="listIndex">Index of the file in the file system list.  This index might not be the same as the one in the internal ANT-FS directory list.</param>
        /// <returns></returns>
        public ANTFS_Directory.Entry GetEntry(ushort index)
        {
            return AntfsDir.GetEntry(index); // throws ArgumentException if it fails, let the exception bubble up
        }

        /// <summary>
        /// Gets a formatted version of a directory entry for display purposes
        /// </summary>
        /// <param name="listIndex">Index of the file in the file system list.  This index might not be the same as the one in the internal ANT-FS directory list.</param>
        /// <returns></returns>
        public string PrintEntry(int listIndex)
        {
            if (listIndex >= filesDir.Count)
                throw new ArgumentException("Invalid list index");

            ANTFS_Directory.Entry entry = AntfsDir.GetEntry(filesDir[listIndex].index); // throws ArgumentException if it fails, let the exception bubble up

            StringBuilder text = new StringBuilder();
            text.AppendFormat("Index: {0}", entry.FileIndex);
            text.AppendFormat(" Data Type: {0}", entry.FileDataType);
            text.AppendFormat(" Identifier: {0} {1}", entry.FileSubType, entry.FileNumber);
            text.Append(" Flags: ");
            text.Append(ANTFS_Directory.ParseFlags(entry.GeneralFlags));
            if (entry.FileDataType == ANTFS_Directory.FitDataType && AntfsDir.IsFileSelected(entry.FileIndex))
                text.Append("|Sel");
            text.AppendFormat(" File Size: {0}", entry.FileSize);
            if (entry.TimeStamp < 0x0FFFFFFF)
                text.AppendFormat("  Date: {0}", entry.TimeStamp); // System time
            else
                text.AppendFormat("  Date: {0}", ReferenceDate.AddSeconds(entry.TimeStamp).ToLocalTime().ToString("yyyy-MM-dd HH:mm:ss"));

            return text.ToString(); 
        }

        /// <summary>
        /// Deletes a file from the file system.
        /// This implementation does not actually erase files, to protect the integrity of sample file sets while developing.
        /// </summary>
        /// <param name="index">ANT-FS directory index</param>
        public void DeleteFile(ushort index)
        {
            if(index == 0)
                throw new UnauthorizedAccessException("Directory cannot be erased");

            // Find the entry
            int i = filesDir.FindIndex(x => x.index == index);
            if(i == -1)
                throw new ArgumentException("Entry does not exist");

            // Try to update the ANT-FS directory first
            AntfsDir.DeleteEntry(index); // Let exceptions bubble up
            // Update the entry
            filesDir.RemoveAt(i);
        }

        /// <summary>
        /// Checks if a file can be deleted
        /// </summary>
        /// <param name="index">ANT-FS directory index</param>
        /// <returns>True if file can be erased, false otherwise</returns>
        public bool IsFileEraseable(ushort index)
        {
            if (index == 0)
                return false;
            
            return AntfsDir.IsFileEraseable(index);
        }

        /// <summary>
        /// Checks if a file is marked as encrypted
        /// </summary>
        /// <param name="index">ANT-FS directory index</param>
        /// <returns>True if file is encrypted, false otherwise</returns>
        public bool IsFileEncrypted(ushort index)
        {
            if (index == 0)
                return false;

            return AntfsDir.IsFileEncrypted(index);
        }

        /// <summary>
        /// Gets binary data corresponding to an entry in the directory
        /// </summary>
        /// <param name="index">ANT-FS directory index</param>
        /// <returns>Byte array containing the data</returns>
        public byte[] ReadFile(ushort index)
        {
            if (index == 0)
            {
                // Update system time
                TimeSpan systemTime = DateTime.UtcNow.Subtract(PowerUpTime);
                AntfsDir.dirHeader.SystemTime = Convert.ToUInt32(systemTime.TotalSeconds);
                return AntfsDir.ToByteArray();  // The directory
            }

            // Find the entry
            int i = filesDir.FindIndex(x => x.index == index);

            if (i == -1)
                throw new ArgumentException("File does not exist");

            if (!AntfsDir.IsFileReadable(index))
                throw new UnauthorizedAccessException("Not enough permissions to read the file");

            // Try to get the file from the system
            byte[] readData = File.ReadAllBytes(filesDir[i].path); // Let exceptions bubble up

            if (!AntfsDir.IsFileEncrypted(index) || readData.Length == 0)
                return readData;    // If data is not encrypted, return as is

            // Handle encryption: files are stored unecrypted, and encrypted on the
            // fly when a download is requested
            if (currentConfig.cryptoKey == null)
                throw new InvalidOperationException("Crypto selected, but no key configured");

            return AES_CTR.AES_CTR.FS_encrypt_bytes(readData, currentConfig.cryptoKey);
        }

        /// <summary>
        /// Marks a downloaded file as archived
        /// </summary>
        /// <param name="index">ANT-FS directory index</param>
        public void ArchiveEntry(ushort index)
        {            
            if (index == 0) // There is no entry for the directory, so no need to do anything...
                return;

            // Find the entry
            int i = filesDir.FindIndex(x => x.index == index);

            if (i == -1)
                throw new ArgumentException("File does not exist");

            ANTFS_Directory.Entry tempEntry = GetEntry(index);  // Let exceptions bubble up
            tempEntry.GeneralFlags |= (byte) ANTFS_Directory.GeneralFlags.Archive;   // mark as Archived
            // Try to update the ANT-FS directory first
            AntfsDir.AddOrReplaceEntry(tempEntry); // throws ArgumentException if it fails, let the exception bubble up
            // Update the entry
            filesDir[i].index = tempEntry.FileIndex;
            // Update the date in the directory header
            UpdateDate();
        }

        private void UpdateDate()
        {
            TimeSpan date = DateTime.UtcNow.Subtract(ReferenceDate);
            AntfsDir.dirHeader.Date = Convert.ToUInt32(date.TotalSeconds);
        }
    }
}
