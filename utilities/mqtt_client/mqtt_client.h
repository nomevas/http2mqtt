//
// Created by nomenas on 12-03-20.
//

#ifndef HTTP2MQTTBRIDGE_MQTTCLIENT_H
#define HTTP2MQTTBRIDGE_MQTTCLIENT_H

#include <mqtt_client_cpp.hpp>
#include <unordered_map>

using Topic = std::string;
using Message = std::string;

class MqttClient {
public:
  using MessageHandler = std::function<void(const Topic&, const Message&)>;

  MqttClient(const std::string& client_id,
      const std::string& server_address,
      unsigned int server_port,
      boost::asio::io_context &ioc);

  void Subscribe(const Topic& topic, MessageHandler handler, MQTT_NS::qos qos = MQTT_NS::qos::at_most_once);
  void Publish(const Topic& topic, const Message& message, MQTT_NS::qos qos);

private:
  std::shared_ptr<MQTT_NS::callable_overlay<MQTT_NS::sync_client<MQTT_NS::tcp_endpoint<as::ip::tcp::socket, as::io_context::strand>>>> mqtt_client_;
  std::unordered_map<size_t, MessageHandler> handlers_;
};

#endif // HTTP2MQTTBRIDGE_MQTTCLIENT_H
