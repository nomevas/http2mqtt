//
// Created by nomenas on 12-03-20.
//

#include "mqtt_client.h"

MqttClient::MqttClient(const std::string& client_id,
                       const std::string& server_address,
                       unsigned int server_port,
                       boost::asio::io_context &ioc)
: mqtt_client_{MQTT_NS::make_sync_client(ioc, server_address, server_port)} {
  mqtt_client_->set_client_id(client_id);
  mqtt_client_->set_clean_session(true);
  using packet_id_t = typename std::remove_reference_t<decltype(*mqtt_client_)>::packet_id_t;

  mqtt_client_->set_publish_handler(
      [&]
          (MQTT_NS::optional<packet_id_t> packet_id,
           MQTT_NS::publish_options pubopts,
           MQTT_NS::buffer topic_name,
           MQTT_NS::buffer contents) {
        if (packet_id) {
          const auto it = handlers_.find(*packet_id);
          if (it != handlers_.end()) {
            int ok = 0;
            //it->second(topic_name, contents);
          } else {
            int nok = 0;
          }
        }

        return true;
      });

  mqtt_client_->connect();
}

void MqttClient::Subscribe(const Topic& topic, MessageHandler handler, MQTT_NS::qos qos) {
  handlers_.emplace(mqtt_client_->subscribe(topic, qos), std::move(handler));
}

void MqttClient::Publish(const Topic& topic, const Message& message, MQTT_NS::qos qos) {
  mqtt_client_->publish(topic, message, qos);
}