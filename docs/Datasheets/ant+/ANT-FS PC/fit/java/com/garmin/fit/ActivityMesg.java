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


public class ActivityMesg extends Mesg implements MesgWithEvent {


   public ActivityMesg() {
      super(Factory.createMesg(MesgNum.ACTIVITY));
   }

   public ActivityMesg(final Mesg mesg) {
      super(mesg);
   }


   /**
    * Get timestamp field
    *
    * @return timestamp
    */
   public DateTime getTimestamp() {
      return timestampToDateTime(getFieldLongValue(253, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD));
   }

   /**
    * Set timestamp field
    *
    * @param timestamp
    */
   public void setTimestamp(DateTime timestamp) {
      setFieldValue(253, 0, timestamp.getTimestamp(), Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get total_timer_time field
    * Units: s
    * Comment: Exclude pauses
    *
    * @return total_timer_time
    */
   public Float getTotalTimerTime() {
      return getFieldFloatValue(0, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set total_timer_time field
    * Units: s
    * Comment: Exclude pauses
    *
    * @param totalTimerTime
    */
   public void setTotalTimerTime(Float totalTimerTime) {
      setFieldValue(0, 0, totalTimerTime, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get num_sessions field
    *
    * @return num_sessions
    */
   public Integer getNumSessions() {
      return getFieldIntegerValue(1, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set num_sessions field
    *
    * @param numSessions
    */
   public void setNumSessions(Integer numSessions) {
      setFieldValue(1, 0, numSessions, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get type field
    *
    * @return type
    */
   public Activity getType() {
      Short value = getFieldShortValue(2, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
      if (value == null)
         return null;
      return Activity.getByValue(value);
   }

   /**
    * Set type field
    *
    * @param type
    */
   public void setType(Activity type) {
      setFieldValue(2, 0, type.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get event field
    *
    * @return event
    */
   public Event getEvent() {
      Short value = getFieldShortValue(3, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
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
      setFieldValue(3, 0, event.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get event_type field
    *
    * @return event_type
    */
   public EventType getEventType() {
      Short value = getFieldShortValue(4, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
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
      setFieldValue(4, 0, eventType.value, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get local_timestamp field
    * Comment: Use to convert activity timestamps to local time if device does not support time zone and daylight savings time correction.
    *
    * @return local_timestamp
    */
   public Long getLocalTimestamp() {
      return getFieldLongValue(5, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set local_timestamp field
    * Comment: Use to convert activity timestamps to local time if device does not support time zone and daylight savings time correction.
    *
    * @param localTimestamp
    */
   public void setLocalTimestamp(Long localTimestamp) {
      setFieldValue(5, 0, localTimestamp, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Get event_group field
    *
    * @return event_group
    */
   public Short getEventGroup() {
      return getFieldShortValue(6, 0, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

   /**
    * Set event_group field
    *
    * @param eventGroup
    */
   public void setEventGroup(Short eventGroup) {
      setFieldValue(6, 0, eventGroup, Fit.SUBFIELD_INDEX_MAIN_FIELD);
   }

}
