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


package com.garmin.fit;

public enum Language {
   ENGLISH((short)0),
   FRENCH((short)1),
   ITALIAN((short)2),
   GERMAN((short)3),
   SPANISH((short)4),
   CROATIAN((short)5),
   CZECH((short)6),
   DANISH((short)7),
   DUTCH((short)8),
   FINNISH((short)9),
   GREEK((short)10),
   HUNGARIAN((short)11),
   NORWEGIAN((short)12),
   POLISH((short)13),
   PORTUGUESE((short)14),
   SLOVAKIAN((short)15),
   SLOVENIAN((short)16),
   SWEDISH((short)17),
   RUSSIAN((short)18),
   TURKISH((short)19),
   LATVIAN((short)20),
   UKRAINIAN((short)21),
   ARABIC((short)22),
   FARSI((short)23),
   BULGARIAN((short)24),
   ROMANIAN((short)25),
   CUSTOM((short)254),
   INVALID((short)255);


   protected short value;




   private Language(short value) {
     this.value = value;
   }

   protected static Language getByValue(final Short value) {
      for (final Language type : Language.values()) {
         if (value == type.value)
            return type;
      }

      return Language.INVALID;
   }

   public short getValue() {
      return value;
   }


}
