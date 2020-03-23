//
// Created by nomenas on 23-03-20.
//

#ifndef HTTP2MQTTBRIDGE_REQUEST_HANDLER_H
#define HTTP2MQTTBRIDGE_REQUEST_HANDLER_H

#include <mqtt_client.h>
#include "crud_callbacks_impl.h"

class RequestHandler {
public:
  RequestHandler(std::string root_topic, MqttClient& mqtt_client)
      : mqtt_client_{mqtt_client}
      , root_topic_{std::move(root_topic)} {}

  virtual void PostItem(const tao::json::value& body, CreateItemCallback callback) {};

protected:
  void OnPostMessage(const std::string& message) {
    tao::json::value response;
    try {
      const tao::json::value request = tao::json::from_string(message);
      const auto body = request.at("body");
      response["session_id"] = request.as<size_t>("session_id");
      PostItem(body, GetCreateItemCallback(mqtt_client_, root_topic_, std::move(response)));
    } catch (const std::exception& ex) {
      response["status"] = 400;
      response["body"] = std::string("{\"error\": \"") + ex.what() + "\"}";
      mqtt_client_.Publish(root_topic_ + "/response", tao::json::to_string(response));
    }
  }

  void OnGetItemMessage(const std::string& message) {

  }

  void OnGetItemsMessage(const WildcardValue& wildcard_value, const std::string& message) {

  }

  void OnPutMessage(const WildcardValue& wildcard_value, const std::string& message) {

  }

  void OnRemoveMessage(const WildcardValue& wildcard_value, const std::string& message) {

  }

  std::string GetTopic(const std::string& topic) {
    return root_topic_ + topic;
  }

private:
  MqttClient& mqtt_client_;
  std::string root_topic_;
};

#endif // HTTP2MQTTBRIDGE_REQUEST_HANDLER_H
