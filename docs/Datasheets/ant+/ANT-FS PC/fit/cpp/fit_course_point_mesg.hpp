////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Dynastream Innovations Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2008-2012 Dynastream Innovations Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 4.10Release
// Tag = $Name: AKW4_100 $
////////////////////////////////////////////////////////////////////////////////


#if !defined(FIT_COURSE_POINT_MESG_HPP)
#define FIT_COURSE_POINT_MESG_HPP

#include "fit_mesg.hpp"

namespace fit
{

class CoursePointMesg : public Mesg
{
   public:
      CoursePointMesg(void) : Mesg(Profile::MESG_COURSE_POINT)
      {
      }

      CoursePointMesg(const Mesg &mesg) : Mesg(mesg)
      {
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns message_index field
      ///////////////////////////////////////////////////////////////////////
      FIT_MESSAGE_INDEX GetMessageIndex(void) const
      {
         return GetFieldUINT16Value(254, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set message_index field
      ///////////////////////////////////////////////////////////////////////
      void SetMessageIndex(FIT_MESSAGE_INDEX messageIndex)
      {
         SetFieldUINT16Value(254, messageIndex, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns timestamp field
      ///////////////////////////////////////////////////////////////////////
      FIT_DATE_TIME GetTimestamp(void) const
      {
         return GetFieldUINT32Value(1, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set timestamp field
      ///////////////////////////////////////////////////////////////////////
      void SetTimestamp(FIT_DATE_TIME timestamp)
      {
         SetFieldUINT32Value(1, timestamp, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns position_lat field
      // Units: semicircles
      ///////////////////////////////////////////////////////////////////////
      FIT_SINT32 GetPositionLat(void) const
      {
         return GetFieldSINT32Value(2, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set position_lat field
      // Units: semicircles
      ///////////////////////////////////////////////////////////////////////
      void SetPositionLat(FIT_SINT32 positionLat)
      {
         SetFieldSINT32Value(2, positionLat, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns position_long field
      // Units: semicircles
      ///////////////////////////////////////////////////////////////////////
      FIT_SINT32 GetPositionLong(void) const
      {
         return GetFieldSINT32Value(3, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set position_long field
      // Units: semicircles
      ///////////////////////////////////////////////////////////////////////
      void SetPositionLong(FIT_SINT32 positionLong)
      {
         SetFieldSINT32Value(3, positionLong, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns distance field
      // Units: m
      ///////////////////////////////////////////////////////////////////////
      FIT_FLOAT32 GetDistance(void) const
      {
         return GetFieldFLOAT32Value(4, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set distance field
      // Units: m
      ///////////////////////////////////////////////////////////////////////
      void SetDistance(FIT_FLOAT32 distance)
      {
         SetFieldFLOAT32Value(4, distance, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns type field
      ///////////////////////////////////////////////////////////////////////
      FIT_COURSE_POINT GetType(void) const
      {
         return GetFieldENUMValue(5, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set type field
      ///////////////////////////////////////////////////////////////////////
      void SetType(FIT_COURSE_POINT type)
      {
         SetFieldENUMValue(5, type, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns name field
      ///////////////////////////////////////////////////////////////////////
      FIT_WSTRING GetName(void) const
      {
         return GetFieldSTRINGValue(6, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set name field
      ///////////////////////////////////////////////////////////////////////
      void SetName(FIT_WSTRING name)
      {
         SetFieldSTRINGValue(6, name, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

};

} // namespace fit

#endif // !defined(FIT_COURSE_POINT_MESG_HPP)
