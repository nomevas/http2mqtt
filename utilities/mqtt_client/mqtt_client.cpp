//
// Created by nomenas on 12-03-20.
//

#include "mqtt_client.h"

#include <iostream>

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
           MQTT_NS::buffer contents){
        std::cout << "publish received."
                  << " dup: "    << pubopts.get_dup()
                  << " qos: "    << pubopts.get_qos()
                  << " retain: " << pubopts.get_retain() << std::endl;
        if (packet_id)
          std::cout << "packet_id: " << *packet_id << std::endl;
        std::cout << "topic_name: " << topic_name << std::endl;
        std::cout << "contents: " << contents << std::endl;

        return true;
      });

  mqtt_client_->connect();
}

void MqttClient::Subscribe(const Topic& topic, MessageHandler handler) {

}
void MqttClient::Publish(const Topic& topic, const Message& message, MQTT_NS::qos qos) {
  mqtt_client_->publish(topic, message, qos);
}