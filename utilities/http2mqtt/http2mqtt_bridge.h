//
// Created by nomenas on 12-03-20.
//

#ifndef HTTP2MQTTBRIDGE_HTTP2MQTT_BRIDGE_H
#define HTTP2MQTTBRIDGE_HTTP2MQTT_BRIDGE_H

#include <string>
#include <memory>

#include "http_server.h"
#include "mqtt_client.h"

class Http2MqttBridge {
public:
  Http2MqttBridge(const Topic& root_topic,
      std::shared_ptr<HttpServer> http_server,
      std::shared_ptr<MqttClient> mqtt_client);

private:
  Topic root_topic_;
  std::shared_ptr<HttpServer> http_server_;
  std::shared_ptr<MqttClient> mqtt_client_;
};

#endif // HTTP2MQTTBRIDGE_HTTP2MQTT_BRIDGE_H
