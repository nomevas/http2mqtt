//
// Created by nomenas on 24-03-20.
//

#ifndef HTTP2MQTTBRIDGE_EVENT_H
#define HTTP2MQTTBRIDGE_EVENT_H

#include <boost/optional.hpp>

enum class EventType {
  Created,
  Updated,
  Deleted
};

template <typename T>
struct Event {
  EventType type;
  boost::uuids::uuid id;
  const T& object;
};

#endif // HTTP2MQTTBRIDGE_EVENT_H
