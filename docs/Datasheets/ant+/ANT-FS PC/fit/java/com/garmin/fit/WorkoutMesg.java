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


public class WorkoutMesg extends Mesg {


   public WorkoutMesg() {
      super(Factory.createMesg(MesgNum.WORKOUT));
   }

   public WorkoutMesg(final Mesg mesg) {
      super(mesg);
   }


   /**
    * Get sport field
    *
    * @return sport
    */
   public Sport getSport() {
      Short value = getFieldShortValue(4, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return Sport.getByValue(value);
   }

   /**
    * Set sport field
    *
    * @param sport
    */
   public void setSport(Sport sport) {
      setFieldValue(4, 0, sport.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get capabilities field
    *
    * @return capabilities
    */
   public Long getCapabilities() {
      return getFieldLongValue(5, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set capabilities field
    *
    * @param capabilities
    */
   public void setCapabilities(Long capabilities) {
      setFieldValue(5, 0, capabilities, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get num_valid_steps field
    * Comment: number of valid steps
    *
    * @return num_valid_steps
    */
   public Integer getNumValidSteps() {
      return getFieldIntegerValue(6, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set num_valid_steps field
    * Comment: number of valid steps
    *
    * @param numValidSteps
    */
   public void setNumValidSteps(Integer numValidSteps) {
      setFieldValue(6, 0, numValidSteps, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get wkt_name field
    *
    * @return wkt_name
    */
   public String getWktName() {
      return getFieldStringValue(8, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set wkt_name field
    *
    * @param wktName
    */
   public void setWktName(String wktName) {
      setFieldValue(8, 0, wktName, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

}
