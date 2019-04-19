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


#include <iostream>
#include <sstream>
#include "fit_decode.hpp"
#include "fit_crc.hpp"
#include "fit_factory.hpp"
#include "fit_mesg_listener.hpp"

namespace fit
{

Decode::Decode()
: mesgListener(NULL),
  mesgDefinitionListener(NULL)
{
   for (int i=0; i<FIT_MAX_LOCAL_MESGS; i++)
   {
      localMesgDefs[i] = MesgDefinition();
      localMesgDefs[i].SetLocalNum((FIT_UINT8) i);
   }
}

FIT_BOOL Decode::IsFIT(std::istream &file)
{
   FIT_UINT8 data;

   InitRead(file);

   try
   {
      while (!file.eof() && (state == STATE_FILE_HDR))
      {
         data = file.get();

         if (ReadByte(data) != RETURN_CONTINUE)
            return FIT_FALSE; // Error processing file header (not FIT).

         if (state != STATE_FILE_HDR)
            return FIT_TRUE; // File header processed successfully.
      }
   }
   catch (RuntimeException e)
   {
   }

   return FIT_FALSE; // Error processing file header (not FIT).
}

FIT_BOOL Decode::CheckIntegrity(std::istream &file)
{
   FIT_UINT8 data;

   InitRead(file);

   try
   {
      while (!file.eof())
      {
         data = file.get();

         switch (ReadByte(data)) {
            case RETURN_CONTINUE:
            case RETURN_MESG:
            case RETURN_MESG_DEF:
               break;

            case RETURN_END_OF_FILE:
               return FIT_TRUE;

            default:
               return FIT_FALSE;
         }
      }
   }
   catch (RuntimeException e)
   {
      // Fall through and return failure.
   }

   return FIT_FALSE;
}

FIT_BOOL Decode::Read(std::istream &file, MesgListener& mesgListener)
{
   this->file = &file;
   this->mesgListener = &mesgListener;
   this->mesgDefinitionListener = NULL;
   InitRead(file);
   return Resume();
}

FIT_BOOL Decode::Read(std::istream &file, MesgListener& mesgListener, MesgDefinitionListener& mesgDefinitionListener)
{
   this->file = &file;
   this->mesgListener = &mesgListener;
   this->mesgDefinitionListener = &mesgDefinitionListener;
   InitRead(file);
   return Resume();
}

void Decode::Pause(void)
{
   pause = FIT_TRUE;
}

FIT_BOOL Decode::Resume(void)
{
   FIT_UINT8 data;

   pause = FIT_FALSE;

   while (!file->eof())
   {
      if (pause)
         return FIT_FALSE;

      data = file->get();

      switch (ReadByte(data)) {
         case RETURN_CONTINUE:
            break;

         case RETURN_MESG:
            if (mesgListener)
               mesgListener->OnMesg(mesg);
            break;

         case RETURN_MESG_DEF:
            if (mesgDefinitionListener)
               mesgDefinitionListener->OnMesgDefinition(localMesgDefs[localMesgIndex]);
            break;

         case RETURN_END_OF_FILE:
            return FIT_TRUE;

         default:
            return FIT_TRUE;
      }
   }

   throw RuntimeException("FIT decode error: Unexpected end of input stream.");
   return FIT_TRUE;
}

void Decode::InitRead(std::istream &file)
{
   fileBytesLeft = 3; // Header byte + CRC.
   fileHdrOffset = 0;
   crc = 0;
   state = STATE_FILE_HDR;
   lastTimeOffset = 0;

   file.seekg(0, std::ios::beg);
}

Decode::RETURN Decode::ReadByte(FIT_UINT8 data)
{
   if (fileBytesLeft > 0)
   {
      crc = CRC::Get16(crc, data);

      fileBytesLeft--;

      if (fileBytesLeft == 1) // CRC low byte.
      {
         if (state != STATE_RECORD)
         {
            throw(RuntimeException("FIT decode error: Decoder not in correct state after last data byte in file. Check message definitions."));
            return RETURN_ERROR;
         }
               
         return RETURN_CONTINUE; // Next byte.
      }
      else if (fileBytesLeft == 0) // CRC high byte.
      {
         if (crc != 0)
         {
            throw(RuntimeException("FIT decode error: File CRC failed."));
            return RETURN_ERROR;
         }

         return RETURN_END_OF_FILE;
      }
   }

   switch (state) {
      case STATE_FILE_HDR:
         switch (fileHdrOffset++)
         {
            case 0:
               fileHdrSize = data;
               fileBytesLeft = fileHdrSize + 2;
               break;
            case 1:
               if ((data & FIT_PROTOCOL_VERSION_MAJOR_MASK) > (FIT_PROTOCOL_VERSION_MAJOR << FIT_PROTOCOL_VERSION_MAJOR_SHIFT))
               {
                  std::ostringstream message;
                  message << "FIT decode error: Protocol version " << (data & FIT_PROTOCOL_VERSION_MAJOR_MASK) << FIT_PROTOCOL_VERSION_MAJOR_SHIFT << "." << (data & FIT_PROTOCOL_VERSION_MINOR_MASK) << " not supported.  Must be " << FIT_PROTOCOL_VERSION_MAJOR << ".15 or earlier.";
                  throw RuntimeException(message.str());
                  return RETURN_ERROR;
               }
               break;
            case 4:
               fileDataSize = data & 0xFF;
               break;
            case 5:
               fileDataSize |= (FIT_UINT32) (data & 0xFF) << 8;
               break;
            case 6:
               fileDataSize |= (FIT_UINT32) (data & 0xFF) << 16;
               break;
            case 7:
               fileDataSize |= (FIT_UINT32) (data & 0xFF) << 24;
               break;
            case 8:
               if (data != '.')
               {
                  throw(RuntimeException("FIT decode error: File header signature mismatch.  File is not FIT."));
                  return RETURN_ERROR;
               }
               break;
            case 9:
               if (data != 'F')
               {
                  throw(RuntimeException("FIT decode error: File header signature mismatch.  File is not FIT."));
                  return RETURN_ERROR;
               }
               break;
            case 10:
               if (data != 'I')
               {
                  throw(RuntimeException("FIT decode error: File header signature mismatch.  File is not FIT."));
                  return RETURN_ERROR;
               }
               break;
            case 11:
               if (data != 'T')
               {
                  throw(RuntimeException("FIT decode error: File header signature mismatch.  File is not FIT."));
                  return RETURN_ERROR;
               }
               break;
            default:
               break;
         }

         if (fileHdrOffset == fileHdrSize)
         {
            fileBytesLeft = fileDataSize + 2; // include crc
            state = STATE_RECORD;
         }
         break;

      case STATE_RECORD:
         fieldIndex = 0;
         fieldBytesLeft = 0;

         if (fileBytesLeft > 1) {
            if ((data & FIT_HDR_TIME_REC_BIT) != 0) {
               Field timestampField = Field(Profile::MESG_RECORD, Profile::RECORD_MESG_TIMESTAMP);
               FIT_UINT8 timeOffset = data & FIT_HDR_TIME_OFFSET_MASK;

               timestamp += (timeOffset - lastTimeOffset) & FIT_HDR_TIME_OFFSET_MASK;
               lastTimeOffset = timeOffset;
               timestampField.SetUINT32Value(timestamp);

               localMesgIndex = (data & FIT_HDR_TIME_TYPE_MASK) >> FIT_HDR_TIME_TYPE_SHIFT;

               if (localMesgDefs[localMesgIndex].GetNum() == FIT_MESG_NUM_INVALID)
               {
                  std::ostringstream message;
                  message << "FIT decode error: Missing FIT message definition for local message number " << ((int)localMesgIndex) << ".";
                  throw(RuntimeException(message.str()));
                  return RETURN_ERROR;
               }

               mesg = Mesg(localMesgDefs[localMesgIndex].GetNum());
               mesg.SetLocalNum(localMesgIndex);
               mesg.AddField(timestampField);

               if (localMesgDefs[localMesgIndex].GetFields().size() == 0)
                  return RETURN_MESG;

               state = STATE_FIELD_DATA;
            }
            else
            {
               localMesgIndex = data & FIT_HDR_TYPE_MASK;

               if ((data & FIT_HDR_TYPE_DEF_BIT) != 0)
               {
                  state = STATE_RESERVED1;
               }
               else
               {
                  if (localMesgDefs[localMesgIndex].GetNum() == FIT_MESG_NUM_INVALID) 
                  {
                     std::ostringstream message;
                     message << "FIT decode error: Missing FIT message definition for local message number " << ((int)localMesgIndex) << ".";
                     throw(RuntimeException(message.str()));
                     return RETURN_ERROR;
                  }

                  mesg = Mesg(localMesgDefs[localMesgIndex].GetNum());
                  mesg.SetLocalNum(localMesgIndex);

                  if (localMesgDefs[localMesgIndex].GetFields().size() == 0)
                     return RETURN_MESG;

                  state = STATE_FIELD_DATA;
               }
            }
         }
         else
         {
            // We just got the low byte of the crc.
            state = STATE_FILE_CRC_HIGH;
         }
         break;

      case STATE_RESERVED1:
         localMesgDefs[localMesgIndex].ClearFields();
         state = STATE_ARCH;
         break;

      case STATE_ARCH:
         archs[localMesgIndex] = data;
         state = STATE_MESG_NUM_0;
         break;

      case STATE_MESG_NUM_0:
         // Read the global message number bytes in as if they are in little endian format.
         localMesgDefs[localMesgIndex].SetNum((FIT_UINT16)data);
         state = STATE_MESG_NUM_1;
         break;

      case STATE_MESG_NUM_1:
         localMesgDefs[localMesgIndex].SetNum(localMesgDefs[localMesgIndex].GetNum() | ((FIT_UINT16)data << 8));

         // We have to check for endianness.
         if (archs[localMesgIndex] == FIT_ARCH_ENDIAN_BIG) {
            localMesgDefs[localMesgIndex].SetNum((localMesgDefs[localMesgIndex].GetNum() >> 8) | ((localMesgDefs[localMesgIndex].GetNum() & 0xFF) << 8));
         }
         else if (archs[localMesgIndex] != FIT_ARCH_ENDIAN_LITTLE)
         {
            std::ostringstream message;
            message << "FIT decode error: Architecture " << archs[localMesgIndex] << " not supported."; 	
            throw(RuntimeException(message.str()));
            return RETURN_ERROR;
         }

         state = STATE_NUM_FIELDS;
         break;

      case STATE_NUM_FIELDS:
         numFields = data;

         if (numFields == 0)
         {
            state = STATE_RECORD;
            return RETURN_MESG_DEF;
         }

         state = STATE_FIELD_NUM;
         break;

      case STATE_FIELD_NUM:
         localMesgDefs[localMesgIndex].AddField(FieldDefinition());
         localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->SetNum(data);
         state = STATE_FIELD_SIZE;
         break;

      case STATE_FIELD_SIZE:
         localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->SetSize(data);
         state = STATE_FIELD_TYPE;
         break;

      case STATE_FIELD_TYPE:
         localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->SetType(data);

         if (++fieldIndex >= numFields)
         {
            state = STATE_RECORD;
            return RETURN_MESG_DEF;
         }
         else
         {
            state = STATE_FIELD_NUM;
         }
         break;

      case STATE_FIELD_DATA:
         if (fieldBytesLeft == 0)
         {
            fieldDataIndex = 0;
            fieldBytesLeft = localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetSize();
         }

         fieldData[fieldDataIndex++] = data;
         fieldBytesLeft--;

         if (fieldBytesLeft == 0)
         {
            int typeSize;
            int elements;

            if ((localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetType() & FIT_BASE_TYPE_NUM_MASK) < FIT_BASE_TYPES) // Ignore field if base type not supported.
            {
               typeSize = baseTypeSizes[(localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetType() & FIT_BASE_TYPE_NUM_MASK)];
               elements = localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetSize() / typeSize;

               if (((localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetType() & FIT_BASE_TYPE_ENDIAN_FLAG) != 0) && ((archs[localMesgIndex] & FIT_ARCH_ENDIAN_MASK) != FIT_ARCH_ENDIAN_LITTLE))
               {
                  // Swap the bytes for each element.
                  for (int element = 0; element < elements; element++)
                  {
                    for (int i = 0; i < (typeSize / 2); i++)
                    {
                      FIT_UINT8 tmp = fieldData[element * typeSize + i];
                      fieldData[element * typeSize + i] = fieldData[element * typeSize + typeSize - i - 1];
                      fieldData[element * typeSize + typeSize - i - 1] = tmp;
                    }
                  }
               }

               Field field(mesg.GetNum(), localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetNum());
               if (field.IsValid()) // If known field type.
               {
                  field.Read(&fieldData, localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetSize());

                  if (field.GetNumValues() > 0)
                  {
                     if (field.GetNumComponents() > 0)
                     {
                        int offset = 0;
                        int i;
                        for (i = 0; i < field.GetNumComponents(); i++)
                        {
                           const Profile::FIELD_COMPONENT* component = field.GetComponent(i);

                           if (component->num != FIT_FIELD_NUM_INVALID)
                           {
                              Field componentField(mesg.GetNum(), component->num);
                              FIT_FLOAT64 value;

                              if (componentField.IsSignedInteger())
                              {
                                 FIT_SINT32 bitsValue;

                                 bitsValue = field.GetBitsSignedValue(offset, component->bits);

                                 if (bitsValue == FIT_SINT32_INVALID)
                                    break; // No more data for components.

                                 value = (bitsValue / component->scale) - component->offset;
                              }
                              else
                              {
                                 FIT_UINT32 bitsValue;

                                 bitsValue = field.GetBitsValue(offset, component->bits);

                                 if (bitsValue == FIT_UINT32_INVALID)
                                    break; // No more data for components.

                                 if (component->accumulate)
                                    bitsValue = accumulator.Accumulate(mesg.GetNum(), field.GetNum(), i, bitsValue, component->bits);

                                 value = (bitsValue / component->scale) - component->offset;
                              }

                              if (mesg.HasField(component->num))
                              {
                                 mesg.SetFieldFLOAT64Value(component->num, value, mesg.GetFieldNumValues(component->num));
                              }
                              else
                              {
                                 componentField.SetFLOAT64Value(value);
                                 mesg.AddField(componentField);
                              }
                           }

                           offset += component->bits;
                        }
                     }
                    
                     mesg.AddField(field);
                    
                     // The special case time record.
                     if (localMesgDefs[localMesgIndex].GetFieldByIndex(fieldIndex)->GetNum() == FIT_FIELD_NUM_TIMESTAMP)
                     {
                        timestamp = field.GetUINT32Value();
                        lastTimeOffset = (FIT_UINT8) (timestamp & FIT_HDR_TIME_OFFSET_MASK);
                     }
                  }
               }
            }

            fieldIndex++;
         }

         if (fieldIndex >= localMesgDefs[localMesgIndex].GetFields().size())
         {
            state = STATE_RECORD;
            return RETURN_MESG;
         }
         break;

      default:
         break;
   }

   return RETURN_CONTINUE;
}

} // namespace fit