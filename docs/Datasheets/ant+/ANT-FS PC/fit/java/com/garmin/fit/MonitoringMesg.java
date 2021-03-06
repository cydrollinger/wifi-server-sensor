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


public class MonitoringMesg extends Mesg {


   public MonitoringMesg() {
      super(Factory.createMesg(MesgNum.MONITORING));
   }

   public MonitoringMesg(final Mesg mesg) {
      super(mesg);
   }


   /**
    * Get timestamp field
    * Units: s
    * Comment: Must align to logging interval, for example, time must be 00:00:00 for daily log.
    *
    * @return timestamp
    */
   public DateTime getTimestamp() {
      return timestampToDateTime(getFieldLongValue(253, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD));
   }

   /**
    * Set timestamp field
    * Units: s
    * Comment: Must align to logging interval, for example, time must be 00:00:00 for daily log.
    *
    * @param timestamp
    */
   public void setTimestamp(DateTime timestamp) {
      setFieldValue(253, 0, timestamp.getTimestamp(), Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get device_index field
    * Comment: Associates this data to device_info message.  Not required for file with single device (sensor).
    *
    * @return device_index
    */
   public Short getDeviceIndex() {
      return getFieldShortValue(0, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set device_index field
    * Comment: Associates this data to device_info message.  Not required for file with single device (sensor).
    *
    * @param deviceIndex
    */
   public void setDeviceIndex(Short deviceIndex) {
      setFieldValue(0, 0, deviceIndex, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get calories field
    * Units: kcal
    *
    * @return calories
    */
   public Integer getCalories() {
      return getFieldIntegerValue(1, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set calories field
    * Units: kcal
    *
    * @param calories
    */
   public void setCalories(Integer calories) {
      setFieldValue(1, 0, calories, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get distance field
    * Units: m
    *
    * @return distance
    */
   public Float getDistance() {
      return getFieldFloatValue(2, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set distance field
    * Units: m
    *
    * @param distance
    */
   public void setDistance(Float distance) {
      setFieldValue(2, 0, distance, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get cycles field
    * Units: cycles
    *
    * @return cycles
    */
   public Long getCycles() {
      return getFieldLongValue(3, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set cycles field
    * Units: cycles
    *
    * @param cycles
    */
   public void setCycles(Long cycles) {
      setFieldValue(3, 0, cycles, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get active_time field
    * Units: s
    *
    * @return active_time
    */
   public Float getActiveTime() {
      return getFieldFloatValue(4, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set active_time field
    * Units: s
    *
    * @param activeTime
    */
   public void setActiveTime(Float activeTime) {
      setFieldValue(4, 0, activeTime, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get activity_type field
    *
    * @return activity_type
    */
   public ActivityType getActivityType() {
      Short value = getFieldShortValue(5, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return ActivityType.getByValue(value);
   }

   /**
    * Set activity_type field
    *
    * @param activityType
    */
   public void setActivityType(ActivityType activityType) {
      setFieldValue(5, 0, activityType.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get activity_subtype field
    *
    * @return activity_subtype
    */
   public ActivitySubtype getActivitySubtype() {
      Short value = getFieldShortValue(6, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return ActivitySubtype.getByValue(value);
   }

   /**
    * Set activity_subtype field
    *
    * @param activitySubtype
    */
   public void setActivitySubtype(ActivitySubtype activitySubtype) {
      setFieldValue(6, 0, activitySubtype.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get compressed_distance field
    *
    * @return compressed_distance
    */
   public Integer getCompressedDistance() {
      return getFieldIntegerValue(8, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set compressed_distance field
    *
    * @param compressedDistance
    */
   public void setCompressedDistance(Integer compressedDistance) {
      setFieldValue(8, 0, compressedDistance, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get compressed_cycles field
    *
    * @return compressed_cycles
    */
   public Integer getCompressedCycles() {
      return getFieldIntegerValue(9, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set compressed_cycles field
    *
    * @param compressedCycles
    */
   public void setCompressedCycles(Integer compressedCycles) {
      setFieldValue(9, 0, compressedCycles, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get compressed_active_time field
    *
    * @return compressed_active_time
    */
   public Integer getCompressedActiveTime() {
      return getFieldIntegerValue(10, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set compressed_active_time field
    *
    * @param compressedActiveTime
    */
   public void setCompressedActiveTime(Integer compressedActiveTime) {
      setFieldValue(10, 0, compressedActiveTime, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get local_timestamp field
    * Comment: Must align to logging interval, for example, time must be 00:00:00 for daily log.
    *
    * @return local_timestamp
    */
   public Long getLocalTimestamp() {
      return getFieldLongValue(11, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set local_timestamp field
    * Comment: Must align to logging interval, for example, time must be 00:00:00 for daily log.
    *
    * @param localTimestamp
    */
   public void setLocalTimestamp(Long localTimestamp) {
      setFieldValue(11, 0, localTimestamp, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

}
