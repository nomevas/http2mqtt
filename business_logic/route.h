//
// Created by nomenas on 13-03-20.
//

#ifndef HTTP2MQTTBRIDGE_ROUTE_H
#define HTTP2MQTTBRIDGE_ROUTE_H

#include "mqtt_client.h"
#include "route_handler.h"

class Route {
public:
  Route(MqttClient& mqtt_clinet, const std::string& path, std::string method, RouteHandler handler);

private:
  void ExecuteHandler(const std::string& topic, const std::string& message);

  MqttClient& mqtt_client_;
  std::string method_;
  RouteHandler handler_;
};

#endif // HTTP2MQTTBRIDGE_ROUTE_H
