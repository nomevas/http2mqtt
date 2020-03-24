//
// Created by nomenas on 19-03-20.
//

#ifndef HTTP2MQTTBRIDGE_UTILITIES_H
#define HTTP2MQTTBRIDGE_UTILITIES_H

#include <tao/json.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "action_type.h"

template <typename T, ActionType>
static void ThrowIfNotValid(const tao::json::value& json) {}
template <typename T>
static T Parse(const tao::json::value& json);
template <typename T>
static tao::json::value Serialize(const T& object);

#endif // HTTP2MQTTBRIDGE_UTILITIES_H
