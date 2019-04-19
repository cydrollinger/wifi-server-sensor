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


public class RecordMesg extends Mesg {


   public RecordMesg() {
      super(Factory.createMesg(MesgNum.RECORD));
   }

   public RecordMesg(final Mesg mesg) {
      super(mesg);
   }


   /**
    * Get timestamp field
    * Units: s
    *
    * @return timestamp
    */
   public DateTime getTimestamp() {
      return timestampToDateTime(getFieldLongValue(253, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD));
   }

   /**
    * Set timestamp field
    * Units: s
    *
    * @param timestamp
    */
   public void setTimestamp(DateTime timestamp) {
      setFieldValue(253, 0, timestamp.getTimestamp(), Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get position_lat field
    * Units: semicircles
    *
    * @return position_lat
    */
   public Integer getPositionLat() {
      return getFieldIntegerValue(0, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set position_lat field
    * Units: semicircles
    *
    * @param positionLat
    */
   public void setPositionLat(Integer positionLat) {
      setFieldValue(0, 0, positionLat, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get position_long field
    * Units: semicircles
    *
    * @return position_long
    */
   public Integer getPositionLong() {
      return getFieldIntegerValue(1, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set position_long field
    * Units: semicircles
    *
    * @param positionLong
    */
   public void setPositionLong(Integer positionLong) {
      setFieldValue(1, 0, positionLong, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get altitude field
    * Units: m
    *
    * @return altitude
    */
   public Float getAltitude() {
      return getFieldFloatValue(2, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set altitude field
    * Units: m
    *
    * @param altitude
    */
   public void setAltitude(Float altitude) {
      setFieldValue(2, 0, altitude, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get heart_rate field
    * Units: bpm
    *
    * @return heart_rate
    */
   public Short getHeartRate() {
      return getFieldShortValue(3, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set heart_rate field
    * Units: bpm
    *
    * @param heartRate
    */
   public void setHeartRate(Short heartRate) {
      setFieldValue(3, 0, heartRate, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get cadence field
    * Units: rpm
    *
    * @return cadence
    */
   public Short getCadence() {
      return getFieldShortValue(4, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set cadence field
    * Units: rpm
    *
    * @param cadence
    */
   public void setCadence(Short cadence) {
      setFieldValue(4, 0, cadence, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get distance field
    * Units: m
    *
    * @return distance
    */
   public Float getDistance() {
      return getFieldFloatValue(5, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set distance field
    * Units: m
    *
    * @param distance
    */
   public void setDistance(Float distance) {
      setFieldValue(5, 0, distance, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get speed field
    * Units: m/s
    *
    * @return speed
    */
   public Float getSpeed() {
      return getFieldFloatValue(6, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set speed field
    * Units: m/s
    *
    * @param speed
    */
   public void setSpeed(Float speed) {
      setFieldValue(6, 0, speed, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get power field
    * Units: watts
    *
    * @return power
    */
   public Integer getPower() {
      return getFieldIntegerValue(7, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set power field
    * Units: watts
    *
    * @param power
    */
   public void setPower(Integer power) {
      setFieldValue(7, 0, power, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * @return number of compressed_speed_distance
    */
   public int getNumCompressedSpeedDistance() {
      return getNumFieldValues(8, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get compressed_speed_distance field
    *
    * @param index of compressed_speed_distance
    * @return compressed_speed_distance
    */
   public Byte getCompressedSpeedDistance(int index) {
      return getFieldByteValue(8, index, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set compressed_speed_distance field
    *
    * @param index of compressed_speed_distance
    * @param compressedSpeedDistance
    */
   public void setCompressedSpeedDistance(int index, Byte compressedSpeedDistance) {
      setFieldValue(8, index, compressedSpeedDistance, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get grade field
    * Units: %
    *
    * @return grade
    */
   public Float getGrade() {
      return getFieldFloatValue(9, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set grade field
    * Units: %
    *
    * @param grade
    */
   public void setGrade(Float grade) {
      setFieldValue(9, 0, grade, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get resistance field
    * Comment: Relative. 0 is none  254 is Max.
    *
    * @return resistance
    */
   public Short getResistance() {
      return getFieldShortValue(10, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set resistance field
    * Comment: Relative. 0 is none  254 is Max.
    *
    * @param resistance
    */
   public void setResistance(Short resistance) {
      setFieldValue(10, 0, resistance, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get time_from_course field
    * Units: s
    *
    * @return time_from_course
    */
   public Float getTimeFromCourse() {
      return getFieldFloatValue(11, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set time_from_course field
    * Units: s
    *
    * @param timeFromCourse
    */
   public void setTimeFromCourse(Float timeFromCourse) {
      setFieldValue(11, 0, timeFromCourse, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get cycle_length field
    * Units: m
    *
    * @return cycle_length
    */
   public Float getCycleLength() {
      return getFieldFloatValue(12, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set cycle_length field
    * Units: m
    *
    * @param cycleLength
    */
   public void setCycleLength(Float cycleLength) {
      setFieldValue(12, 0, cycleLength, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get temperature field
    * Units: C
    *
    * @return temperature
    */
   public Byte getTemperature() {
      return getFieldByteValue(13, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set temperature field
    * Units: C
    *
    * @param temperature
    */
   public void setTemperature(Byte temperature) {
      setFieldValue(13, 0, temperature, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * @return number of speed_1s
    */
   public int getNumSpeed1s() {
      return getNumFieldValues(17, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get speed_1s field
    * Units: m/s
    * Comment: Speed at 1s intervals.  Timestamp field indicates time of last array element.
    *
    * @param index of speed_1s
    * @return speed_1s
    */
   public Float getSpeed1s(int index) {
      return getFieldFloatValue(17, index, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set speed_1s field
    * Units: m/s
    * Comment: Speed at 1s intervals.  Timestamp field indicates time of last array element.
    *
    * @param index of speed_1s
    * @param speed1s
    */
   public void setSpeed1s(int index, Float speed1s) {
      setFieldValue(17, index, speed1s, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get cycles field
    *
    * @return cycles
    */
   public Short getCycles() {
      return getFieldShortValue(18, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set cycles field
    *
    * @param cycles
    */
   public void setCycles(Short cycles) {
      setFieldValue(18, 0, cycles, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get total_cycles field
    * Units: cycles
    *
    * @return total_cycles
    */
   public Long getTotalCycles() {
      return getFieldLongValue(19, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set total_cycles field
    * Units: cycles
    *
    * @param totalCycles
    */
   public void setTotalCycles(Long totalCycles) {
      setFieldValue(19, 0, totalCycles, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get compressed_accumulated_power field
    *
    * @return compressed_accumulated_power
    */
   public Integer getCompressedAccumulatedPower() {
      return getFieldIntegerValue(28, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set compressed_accumulated_power field
    *
    * @param compressedAccumulatedPower
    */
   public void setCompressedAccumulatedPower(Integer compressedAccumulatedPower) {
      setFieldValue(28, 0, compressedAccumulatedPower, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get accumulated_power field
    * Units: watts
    *
    * @return accumulated_power
    */
   public Long getAccumulatedPower() {
      return getFieldLongValue(29, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set accumulated_power field
    * Units: watts
    *
    * @param accumulatedPower
    */
   public void setAccumulatedPower(Long accumulatedPower) {
      setFieldValue(29, 0, accumulatedPower, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get left_right_balance field
    *
    * @return left_right_balance
    */
   public Short getLeftRightBalance() {
      return getFieldShortValue(30, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set left_right_balance field
    *
    * @param leftRightBalance
    */
   public void setLeftRightBalance(Short leftRightBalance) {
      setFieldValue(30, 0, leftRightBalance, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get gps_accuracy field
    * Units: m
    *
    * @return gps_accuracy
    */
   public Short getGpsAccuracy() {
      return getFieldShortValue(31, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set gps_accuracy field
    * Units: m
    *
    * @param gpsAccuracy
    */
   public void setGpsAccuracy(Short gpsAccuracy) {
      setFieldValue(31, 0, gpsAccuracy, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get vertical_speed field
    * Units: m/s
    *
    * @return vertical_speed
    */
   public Float getVerticalSpeed() {
      return getFieldFloatValue(32, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set vertical_speed field
    * Units: m/s
    *
    * @param verticalSpeed
    */
   public void setVerticalSpeed(Float verticalSpeed) {
      setFieldValue(32, 0, verticalSpeed, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get calories field
    * Units: kcal
    *
    * @return calories
    */
   public Integer getCalories() {
      return getFieldIntegerValue(33, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set calories field
    * Units: kcal
    *
    * @param calories
    */
   public void setCalories(Integer calories) {
      setFieldValue(33, 0, calories, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

}