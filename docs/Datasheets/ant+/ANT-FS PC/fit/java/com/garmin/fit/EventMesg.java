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


public class EventMesg extends Mesg implements MesgWithEvent {


   public EventMesg() {
      super(Factory.createMesg(MesgNum.EVENT));
   }

   public EventMesg(final Mesg mesg) {
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
    * Get event field
    *
    * @return event
    */
   public Event getEvent() {
      Short value = getFieldShortValue(0, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return Event.getByValue(value);
   }

   /**
    * Set event field
    *
    * @param event
    */
   public void setEvent(Event event) {
      setFieldValue(0, 0, event.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get event_type field
    *
    * @return event_type
    */
   public EventType getEventType() {
      Short value = getFieldShortValue(1, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return EventType.getByValue(value);
   }

   /**
    * Set event_type field
    *
    * @param eventType
    */
   public void setEventType(EventType eventType) {
      setFieldValue(1, 0, eventType.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get data16 field
    *
    * @return data16
    */
   public Integer getData16() {
      return getFieldIntegerValue(2, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set data16 field
    *
    * @param data16
    */
   public void setData16(Integer data16) {
      setFieldValue(2, 0, data16, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get data field
    *
    * @return data
    */
   public Long getData() {
      return getFieldLongValue(3, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set data field
    *
    * @param data
    */
   public void setData(Long data) {
      setFieldValue(3, 0, data, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get timer_trigger field
    *
    * @return timer_trigger
    */
   public TimerTrigger getTimerTrigger() {
      Short value = getFieldShortValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_TIMER_TRIGGER);
      if (value == null)
         return null;
      return TimerTrigger.getByValue(value);
   }

   /**
    * Set timer_trigger field
    *
    * @param timerTrigger
    */
   public void setTimerTrigger(TimerTrigger timerTrigger) {
      setFieldValue(3, 0, timerTrigger.value, Profile.SubFields.EVENT_MESG_DATA_FIELD_TIMER_TRIGGER);
   }

   /**
    * Get course_point_index field
    *
    * @return course_point_index
    */
   public Integer getCoursePointIndex() {
      return getFieldIntegerValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_COURSE_POINT_INDEX);
   }

   /**
    * Set course_point_index field
    *
    * @param coursePointIndex
    */
   public void setCoursePointIndex(Integer coursePointIndex) {
      setFieldValue(3, 0, coursePointIndex, Profile.SubFields.EVENT_MESG_DATA_FIELD_COURSE_POINT_INDEX);
   }

   /**
    * Get battery_level field
    * Units: V
    *
    * @return battery_level
    */
   public Float getBatteryLevel() {
      return getFieldFloatValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_BATTERY_LEVEL);
   }

   /**
    * Set battery_level field
    * Units: V
    *
    * @param batteryLevel
    */
   public void setBatteryLevel(Float batteryLevel) {
      setFieldValue(3, 0, batteryLevel, Profile.SubFields.EVENT_MESG_DATA_FIELD_BATTERY_LEVEL);
   }

   /**
    * Get virtual_partner_speed field
    * Units: m/s
    *
    * @return virtual_partner_speed
    */
   public Float getVirtualPartnerSpeed() {
      return getFieldFloatValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_VIRTUAL_PARTNER_SPEED);
   }

   /**
    * Set virtual_partner_speed field
    * Units: m/s
    *
    * @param virtualPartnerSpeed
    */
   public void setVirtualPartnerSpeed(Float virtualPartnerSpeed) {
      setFieldValue(3, 0, virtualPartnerSpeed, Profile.SubFields.EVENT_MESG_DATA_FIELD_VIRTUAL_PARTNER_SPEED);
   }

   /**
    * Get hr_high_alert field
    * Units: bpm
    *
    * @return hr_high_alert
    */
   public Short getHrHighAlert() {
      return getFieldShortValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_HR_HIGH_ALERT);
   }

   /**
    * Set hr_high_alert field
    * Units: bpm
    *
    * @param hrHighAlert
    */
   public void setHrHighAlert(Short hrHighAlert) {
      setFieldValue(3, 0, hrHighAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_HR_HIGH_ALERT);
   }

   /**
    * Get hr_low_alert field
    * Units: bpm
    *
    * @return hr_low_alert
    */
   public Short getHrLowAlert() {
      return getFieldShortValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_HR_LOW_ALERT);
   }

   /**
    * Set hr_low_alert field
    * Units: bpm
    *
    * @param hrLowAlert
    */
   public void setHrLowAlert(Short hrLowAlert) {
      setFieldValue(3, 0, hrLowAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_HR_LOW_ALERT);
   }

   /**
    * Get speed_high_alert field
    * Units: m/s
    *
    * @return speed_high_alert
    */
   public Float getSpeedHighAlert() {
      return getFieldFloatValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_SPEED_HIGH_ALERT);
   }

   /**
    * Set speed_high_alert field
    * Units: m/s
    *
    * @param speedHighAlert
    */
   public void setSpeedHighAlert(Float speedHighAlert) {
      setFieldValue(3, 0, speedHighAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_SPEED_HIGH_ALERT);
   }

   /**
    * Get speed_low_alert field
    * Units: m/s
    *
    * @return speed_low_alert
    */
   public Float getSpeedLowAlert() {
      return getFieldFloatValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_SPEED_LOW_ALERT);
   }

   /**
    * Set speed_low_alert field
    * Units: m/s
    *
    * @param speedLowAlert
    */
   public void setSpeedLowAlert(Float speedLowAlert) {
      setFieldValue(3, 0, speedLowAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_SPEED_LOW_ALERT);
   }

   /**
    * Get cad_high_alert field
    * Units: rpm
    *
    * @return cad_high_alert
    */
   public Integer getCadHighAlert() {
      return getFieldIntegerValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_CAD_HIGH_ALERT);
   }

   /**
    * Set cad_high_alert field
    * Units: rpm
    *
    * @param cadHighAlert
    */
   public void setCadHighAlert(Integer cadHighAlert) {
      setFieldValue(3, 0, cadHighAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_CAD_HIGH_ALERT);
   }

   /**
    * Get cad_low_alert field
    * Units: rpm
    *
    * @return cad_low_alert
    */
   public Integer getCadLowAlert() {
      return getFieldIntegerValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_CAD_LOW_ALERT);
   }

   /**
    * Set cad_low_alert field
    * Units: rpm
    *
    * @param cadLowAlert
    */
   public void setCadLowAlert(Integer cadLowAlert) {
      setFieldValue(3, 0, cadLowAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_CAD_LOW_ALERT);
   }

   /**
    * Get power_high_alert field
    * Units: watts
    *
    * @return power_high_alert
    */
   public Integer getPowerHighAlert() {
      return getFieldIntegerValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_POWER_HIGH_ALERT);
   }

   /**
    * Set power_high_alert field
    * Units: watts
    *
    * @param powerHighAlert
    */
   public void setPowerHighAlert(Integer powerHighAlert) {
      setFieldValue(3, 0, powerHighAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_POWER_HIGH_ALERT);
   }

   /**
    * Get power_low_alert field
    * Units: watts
    *
    * @return power_low_alert
    */
   public Integer getPowerLowAlert() {
      return getFieldIntegerValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_POWER_LOW_ALERT);
   }

   /**
    * Set power_low_alert field
    * Units: watts
    *
    * @param powerLowAlert
    */
   public void setPowerLowAlert(Integer powerLowAlert) {
      setFieldValue(3, 0, powerLowAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_POWER_LOW_ALERT);
   }

   /**
    * Get time_duration_alert field
    * Units: s
    *
    * @return time_duration_alert
    */
   public Float getTimeDurationAlert() {
      return getFieldFloatValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_TIME_DURATION_ALERT);
   }

   /**
    * Set time_duration_alert field
    * Units: s
    *
    * @param timeDurationAlert
    */
   public void setTimeDurationAlert(Float timeDurationAlert) {
      setFieldValue(3, 0, timeDurationAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_TIME_DURATION_ALERT);
   }

   /**
    * Get distance_duration_alert field
    * Units: m
    *
    * @return distance_duration_alert
    */
   public Float getDistanceDurationAlert() {
      return getFieldFloatValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_DISTANCE_DURATION_ALERT);
   }

   /**
    * Set distance_duration_alert field
    * Units: m
    *
    * @param distanceDurationAlert
    */
   public void setDistanceDurationAlert(Float distanceDurationAlert) {
      setFieldValue(3, 0, distanceDurationAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_DISTANCE_DURATION_ALERT);
   }

   /**
    * Get calorie_duration_alert field
    * Units: calories
    *
    * @return calorie_duration_alert
    */
   public Long getCalorieDurationAlert() {
      return getFieldLongValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_CALORIE_DURATION_ALERT);
   }

   /**
    * Set calorie_duration_alert field
    * Units: calories
    *
    * @param calorieDurationAlert
    */
   public void setCalorieDurationAlert(Long calorieDurationAlert) {
      setFieldValue(3, 0, calorieDurationAlert, Profile.SubFields.EVENT_MESG_DATA_FIELD_CALORIE_DURATION_ALERT);
   }

   /**
    * Get fitness_equipment_state field
    *
    * @return fitness_equipment_state
    */
   public FitnessEquipmentState getFitnessEquipmentState() {
      Short value = getFieldShortValue(3, 0, Profile.SubFields.EVENT_MESG_DATA_FIELD_FITNESS_EQUIPMENT_STATE);
      if (value == null)
         return null;
      return FitnessEquipmentState.getByValue(value);
   }

   /**
    * Set fitness_equipment_state field
    *
    * @param fitnessEquipmentState
    */
   public void setFitnessEquipmentState(FitnessEquipmentState fitnessEquipmentState) {
      setFieldValue(3, 0, fitnessEquipmentState.value, Profile.SubFields.EVENT_MESG_DATA_FIELD_FITNESS_EQUIPMENT_STATE);
   }

   /**
    * Get event_group field
    *
    * @return event_group
    */
   public Short getEventGroup() {
      return getFieldShortValue(4, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set event_group field
    *
    * @param eventGroup
    */
   public void setEventGroup(Short eventGroup) {
      setFieldValue(4, 0, eventGroup, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

}