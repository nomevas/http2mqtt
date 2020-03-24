//
// Created by nomenas on 24-03-20.
//

#ifndef HTTP2MQTTBRIDGE_EVENT_H
#define HTTP2MQTTBRIDGE_EVENT_H

#include <boost/optional.hpp>

#include "action_type.h"

template <typename T>
struct Event {
  ActionType action_type;
  boost::uuids::uuid id;
  const T& object;
};

#endif // HTTP2MQTTBRIDGE_EVENT_H
