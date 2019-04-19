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


#if !defined(FIT_MONITORING_INFO_MESG_LISTENER_HPP)
#define FIT_MONITORING_INFO_MESG_LISTENER_HPP

#include "fit_monitoring_info_mesg.hpp"

namespace fit
{

class MonitoringInfoMesgListener
{
   public:
      virtual ~MonitoringInfoMesgListener() {}
      virtual void OnMesg(MonitoringInfoMesg& mesg) = 0;
};

} // namespace fit

#endif // !defined(FIT_MONITORING_INFO_MESG_LISTENER_HPP)
