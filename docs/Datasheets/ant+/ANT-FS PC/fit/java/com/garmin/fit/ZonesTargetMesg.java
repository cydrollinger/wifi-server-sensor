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


public class ZonesTargetMesg extends Mesg {


   public ZonesTargetMesg() {
      super(Factory.createMesg(MesgNum.ZONES_TARGET));
   }

   public ZonesTargetMesg(final Mesg mesg) {
      super(mesg);
   }


   /**
    * Get max_heart_rate field
    *
    * @return max_heart_rate
    */
   public Short getMaxHeartRate() {
      return getFieldShortValue(1, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set max_heart_rate field
    *
    * @param maxHeartRate
    */
   public void setMaxHeartRate(Short maxHeartRate) {
      setFieldValue(1, 0, maxHeartRate, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get threshold_heart_rate field
    *
    * @return threshold_heart_rate
    */
   public Short getThresholdHeartRate() {
      return getFieldShortValue(2, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set threshold_heart_rate field
    *
    * @param thresholdHeartRate
    */
   public void setThresholdHeartRate(Short thresholdHeartRate) {
      setFieldValue(2, 0, thresholdHeartRate, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get functional_threshold_power field
    *
    * @return functional_threshold_power
    */
   public Integer getFunctionalThresholdPower() {
      return getFieldIntegerValue(3, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set functional_threshold_power field
    *
    * @param functionalThresholdPower
    */
   public void setFunctionalThresholdPower(Integer functionalThresholdPower) {
      setFieldValue(3, 0, functionalThresholdPower, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get hr_calc_type field
    *
    * @return hr_calc_type
    */
   public HrZoneCalc getHrCalcType() {
      Short value = getFieldShortValue(5, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return HrZoneCalc.getByValue(value);
   }

   /**
    * Set hr_calc_type field
    *
    * @param hrCalcType
    */
   public void setHrCalcType(HrZoneCalc hrCalcType) {
      setFieldValue(5, 0, hrCalcType.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get pwr_calc_type field
    *
    * @return pwr_calc_type
    */
   public PwrZoneCalc getPwrCalcType() {
      Short value = getFieldShortValue(7, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return PwrZoneCalc.getByValue(value);
   }

   /**
    * Set pwr_calc_type field
    *
    * @param pwrCalcType
    */
   public void setPwrCalcType(PwrZoneCalc pwrCalcType) {
      setFieldValue(7, 0, pwrCalcType.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

}
