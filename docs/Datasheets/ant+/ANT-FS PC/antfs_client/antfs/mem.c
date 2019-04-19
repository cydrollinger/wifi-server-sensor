/////////////////////////////////////////////////////////////////////////////////////////
// THE FOLLOWING EXAMPLE CODE IS INTENDED FOR LIMITED CIRCULATION ONLY.
// 
// Please forward all questions regarding this code to ANT Technical Support.
// 
// Dynastream Innovations Inc.
// 228 River Avenue
// Cochrane, Alberta, Canada
// T4C 2C1
// 
// (P) (403) 932-9292
// (F) (403) 932-6521
// (TF) 1-866-932-9292
// (E) support@thisisant.com
// 
// www.thisisant.com
//
// Reference Design Disclaimer
//
// The references designs and codes provided may be used with ANT devices only and remain the copyrighted property of 
// Dynastream Innovations Inc. The reference designs and codes are being provided on an "as-is" basis and as an accommodation, 
// and therefore all warranties, representations, or guarantees of any kind (whether express, implied or statutory) including, 
// without limitation, warranties of merchantability, non-infringement,
// or fitness for a particular purpose, are specifically disclaimed.
//
// ©2008 Dynastream Innovations Inc. All Rights Reserved
// This software may not be reproduced by
// any means without express written approval of Dynastream
// Innovations Inc.
//
/////////////////////////////////////////////////////////////////////////////////////////
#include "antfs.h"
#include "string.h"

#define MEM_DIR_SIZE            ((UCHAR) 9)
#define MEM_DIR_INVALID_INDEX   ((USHORT) 0xFFFF)

static USHORT LookupIndex(USHORT ucIndex_);

typedef struct 
{
   ANTFS_DIR_HEADER stHeader;
   ANTFS_DIR_STRUCT stDirFile[MEM_DIR_SIZE];       // 9 files in this mem simulator
} DirectoryFile;

// Sample directory
static const DirectoryFile stDirectory = 
{
   { ANTFS_DIR_STRUCT_VERSION, sizeof(ANTFS_DIR_STRUCT),  0, 0, 0, 0, 0, 0, 0, 0},  // Version 1, length of each subsequent entry = 16 bytes, system time not used
   {
      { 9, 0,        4, 0, 0, 0xA0, 0x00000000, 0x000296BC},    // Index 9, data type 0, identifier 4, read and erase, 0 bytes in length
      { 7, 1,        0, 0, 0, 0x60, 0x00000000, 0x00007E71},    // Index 7, data type 1, identifier 0, write and erase, 0 bytes in length
      { 8, 1,        1, 0, 0, 0x60, 0x00000000, 0x00007E73},    // Index 8, data type 1, identifier 1, write and erase, 0 bytes in length
      { 1, 0x80,     1, 0, 0, 0x80, 0x000000DA, 0x00007E63},    // Index 1, data type 128, identifier 1, read and write, 218 bytes in length
      { 2, 0x80,     2, 0, 0, 0xC0, 0x0000014E, 0x00007E65},    // Index 2, data type 128, identifier 2, read and write, 334 bytes in length
      { 3, 0x80,     3, 0, 0, 0xC0, 0x0000037B, 0x00007E67},    // Index 3, data type 128, identifier 3, read and write, 891 bytes in length
      { 4, 0x80,     3, 0, 0, 0xC0, 0x0000037B, 0x00007E6A},    // Index 4, data type 128, identifier 3, read and write, 891 bytes in length
      { 5, 0x80,     3, 0, 0, 0xF0, 0x0001D4C0, 0x00007E6C},    // Index 5, data type 128, identifier 3, read, write, erase, archive, 120000 bytes in length
      { 6, 0x80,  0x0A, 0, 0, 0x80, 0x00000000, 0x00007E6F}     // Index 6, data type 128, identifier 10, read-only, 0 bytes in length
   }
};

////////////////////////////////////////////////////////////////////////////////
// Mem_WriteFile
//
// Write files to system
// 
// Returns: TRUE if write operation was successful, FALSE otherwise
//
////////////////////////////////////////////////////////////////////////////////
BOOL Mem_WriteFile(USHORT usIndex_, ULONG ulOffset_, const void *pvData_, ULONG ulSize)
{
   if(usIndex_ < MEM_DIR_SIZE+1 && usIndex_ > 0)
   {
      USHORT usArrayIndex = LookupIndex(usIndex_);
      if(usArrayIndex != MEM_DIR_INVALID_INDEX)
      { 
         ULONG i;
         for (i = 0; i < ulSize; i++)
         {
            // Write data to file     
            // This function is implementation specific
            // There are no actual files in memory for this reference design,
            // so there is nowhere to write the data to
         }
         return TRUE;
      }
      else
      {
         return FALSE;   // Write failed
      }
   }
   return FALSE; // Write failed
}

////////////////////////////////////////////////////////////////////////////////
// Mem_ReadFile
//
// Get directory, or data to send, if a file download was requested
//
////////////////////////////////////////////////////////////////////////////////
BOOL Mem_ReadFile(USHORT usIndex_, ULONG ulOffset_, void *pvData_, ULONG ulSize_)
{
   // This function is implementation specific, depending on how the files are stored
   // in memory.
   if(usIndex_ == 0)    // If directory
   {
      UCHAR* pc2 = (UCHAR*) &stDirectory;
      memcpy((UCHAR*)pvData_, pc2 + ulOffset_, ulSize_);
   }
   else     // Fake data (no actual files stored in memory for this reference design)
   {
      ULONG i;
      UCHAR* pc = (UCHAR*)pvData_;
      for (i = 0; i < ulSize_; i++)
         pc[i] = ((UCHAR)(ulOffset_ + i));
   }
   return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Mem_EraseFile
//
// Erases file from system
//
// Returns: TRUE if the erase operation was successful, FALSE otherwise
//
////////////////////////////////////////////////////////////////////////////////
BOOL Mem_EraseFile(USHORT usIndex_)
{
   if(usIndex_ < MEM_DIR_SIZE+1 && usIndex_ > 0)
   {
      USHORT usArrayIndex = LookupIndex(usIndex_);
      if(usArrayIndex != MEM_DIR_INVALID_INDEX)
      { 
         // Erase file      
         // This function is implementation specific
         // There are no actual files in memory for this reference design,
         // so there is nothing to erase
         return TRUE;
      }
      else
      {
         return FALSE;   // Erase failed
      }
   }
   return FALSE; // Erase failed
}

////////////////////////////////////////////////////////////////////////////////
// Mem_GetMaxFileSize
//
// Retrieves file size information from directory, matching requested index
//
// Returns: file size (bytes) or 0 if the file does not exist
//
////////////////////////////////////////////////////////////////////////////////
ULONG Mem_GetMaxFileSize(USHORT usIndex_)
{
   if(usIndex_ < MEM_DIR_SIZE+1 && usIndex_ > 0)
   {
      USHORT usArrayIndex = LookupIndex(usIndex_);
      if(usArrayIndex != MEM_DIR_INVALID_INDEX)
         return(stDirectory.stDirFile[usArrayIndex].ulSize);
   }
   else if(usIndex_ == 0)  // Directory
   {
      return (ULONG) (MEM_DIR_SIZE+1) * sizeof(ANTFS_DIR_STRUCT); // Header + directory structures
   }
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Mem_GetDir
//
// Retrieves file information from directory, matching the requested index
//
// Returns:  
//
////////////////////////////////////////////////////////////////////////////////
BOOL Mem_GetDir(USHORT usIndex_, ANTFS_DIR_STRUCT* pstReturnedDir_)
{
   if (usIndex_ < MEM_DIR_SIZE + 1)
   {
      if (!usIndex_)    // Requested directory
      {
         pstReturnedDir_->ucGeneralFlags = 0x80;   // Can download
         pstReturnedDir_->ulSize = (ULONG) (MEM_DIR_SIZE+1) * sizeof(ANTFS_DIR_STRUCT);    // Header + directory structures
         pstReturnedDir_->usIndex = 0;    // Directory is index 0
      }
      else  // Requested a file
      {
         USHORT usArrayIndex = LookupIndex(usIndex_);   // Find requested index
         memcpy(pstReturnedDir_, &stDirectory.stDirFile[usArrayIndex], sizeof(ANTFS_DIR_STRUCT));
      }
      return TRUE;
   }
   return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// LookupIndex
//
// Get array index of a particular directory structure matching the requested
// file index
//
////////////////////////////////////////////////////////////////////////////////
USHORT LookupIndex(USHORT usIndex_)
{
   if(usIndex_)
   {
      USHORT i;
      for(i=0; i<MEM_DIR_SIZE; i++)
      {
         if(stDirectory.stDirFile[i].usIndex == usIndex_)
            return i;
      }
   }
   return MEM_DIR_INVALID_INDEX;
}
