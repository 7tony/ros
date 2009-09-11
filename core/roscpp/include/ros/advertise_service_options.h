/*
 * Copyright (C) 2009, Willow Garage, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the names of Stanford University or Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROSCPP_ADVERTISE_SERVICE_OPTIONS_H
#define ROSCPP_ADVERTISE_SERVICE_OPTIONS_H

#include "ros/forwards.h"

namespace ros
{

struct AdvertiseServiceOptions
{
  AdvertiseServiceOptions()
  : callback_queue(0)
  {
  }

  /**
   * \brief Constructor
   * \param _service Service name to advertise on
   * \param _helper Helper object used for creating messages and calling callbacks
   */
  AdvertiseServiceOptions(const std::string& _service, const ServiceMessageHelperPtr& _helper)
  : service(_service)
  , md5sum(_helper->getMD5Sum())
  , datatype(_helper->getDataType())
  , req_datatype(_helper->getRequestDataType())
  , res_datatype(_helper->getResponseDataType())
  , helper(_helper)
  , callback_queue(0)
  {}

  /**
   * \brief Templated convenience constructor
   * \param _service Service name to advertise on
   * \param _callback Callback to call when this service is called
   */
  template<class MReq, class MRes>
  void init(const std::string& _service, const boost::function<bool(MReq&, MRes&)>& _callback)
  {
    if (MReq::__s_getServerMD5Sum() != MRes::__s_getServerMD5Sum())
    {
      ROS_FATAL("woah! the request and response parameters to the server "
      "callback function must be autogenerated from the same "
      "server definition file (.srv). your advertise_servce "
      "call for %s appeared to use request/response types "
      "from different .srv files.", service.c_str());
      ROS_BREAK();
    }

    service = _service;
    md5sum = MReq::__s_getServerMD5Sum();
    datatype = MReq::__s_getServiceDataType();
    req_datatype = MReq::__s_getDataType();
    res_datatype = MRes::__s_getDataType();
    helper = ServiceMessageHelperPtr(new ServiceMessageHelperT<MReq, MRes>(_callback));
  }

  std::string service;                                                ///< Service name
  std::string md5sum;                                                 ///< MD5 of the service
  std::string datatype;                                               ///< Datatype of the service
  std::string req_datatype;                                           ///< Request message datatype
  std::string res_datatype;                                           ///< Response message datatype

  ServiceMessageHelperPtr helper;                                     ///< Helper object used for creating messages and calling callbacks

  CallbackQueueInterface* callback_queue;                             ///< Queue to add callbacks to.  If NULL, the global callback queue will be used

  /**
   * A shared pointer to an object to track for these callbacks.  If set, the a weak_ptr will be created to this object,
   * and if the reference count goes to 0 the subscriber callbacks will not get called.
   *
   * \note Note that setting this will cause a new reference to be added to the object before the
   * callback, and for it to go out of scope (and potentially be deleted) in the code path (and therefore
   * thread) that the callback is invoked from.
   */
  VoidPtr tracked_object;

  template<class Service>
  static AdvertiseServiceOptions create(const std::string& service,
                                 const boost::function<bool(typename Service::Request&, typename Service::Response&)>& callback,
                                 const VoidPtr& tracked_object,
                                 CallbackQueueInterface* queue)
  {
    AdvertiseServiceOptions ops;
    ops.init<typename Service::Request, typename Service::Response>(service, callback);
    ops.tracked_object = tracked_object;
    ops.callback_queue = queue;
    return ops;
  }
};



}

#endif

