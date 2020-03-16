//
// Created by nomenas on 13-03-20.
//

#ifndef HTTP2MQTTBRIDGE_ROUTE_HANDLER_H
#define HTTP2MQTTBRIDGE_ROUTE_HANDLER_H

#include <tao/json.hpp>

using CompletionCallback = std::function<void(int status_code, const tao::json::value&)>;
using RouteHandler = std::function<void(const tao::json::value&, CompletionCallback)>;

#endif // HTTP2MQTTBRIDGE_ROUTE_HANDLER_H
