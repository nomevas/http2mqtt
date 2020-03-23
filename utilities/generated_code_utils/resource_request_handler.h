//
// Created by nomenas on 23-03-20.
//

#ifndef HTTP2MQTTBRIDGE_RESOURCE_REQUEST_HANDLER_H
#define HTTP2MQTTBRIDGE_RESOURCE_REQUEST_HANDLER_H

#include <mqtt_client.h>
#include "crud_callbacks_impl.h"

class ResourceRequestHandler {
public:
  ResourceRequestHandler(std::string resource_name, std::string root_topic, MqttClient& mqtt_client)
      : mqtt_client_{mqtt_client}
      , root_topic_{std::move(root_topic)}
      , resource_name_(std::move(resource_name)) {}

  void RegisterPostItemHandler(std::function<void(const tao::json::value& body, CreateItemCallback callback)> handler) {
    mqtt_client_.Subscribe(root_topic_ + "/api/" + resource_name_ + "/POST",
        std::bind(&ResourceRequestHandler::OnPostMessage, this, std::placeholders::_2, std::move(handler)));
//        mqtt_client.Subscribe(GetTopic("/api/user/GET"),
//                              std::bind(&ResourceRequestHandler::OnGetItemMessage, this, std::placeholders::_2));
//        mqtt_client.Subscribe(GetTopic("/api/user/+/PUT"),
//                              std::bind(&ResourceRequestHandler::OnPutMessage, this, std::placeholders::_1, std::placeholders::_2));
//        mqtt_client.Subscribe(GetTopic("/api/user/+/GET"),
//                              std::bind(&ResourceRequestHandler::OnGetItemsMessage, this, std::placeholders::_1, std::placeholders::_2));
//        mqtt_client.Subscribe(GetTopic("/api/user/+/DELETE"),
//                              std::bind(&ResourceRequestHandler::OnRemoveMessage, this, std::placeholders::_1, std::placeholders::_2));}
  }

protected:
  void OnPostMessage(const std::string& message, std::function<void(const tao::json::value& body, CreateItemCallback callback)> handler) {
    size_t session_id = {};
    try {
      const tao::json::value request = tao::json::from_string(message);
      const auto body = request.at("body");
      session_id = request.as<size_t>("session_id");
      tao::json::value response = {{"session_id", session_id}};
      handler(body, GetCreateItemCallback(mqtt_client_, root_topic_, std::move(response)));
    } catch (const std::exception& ex) {
      tao::json::value response = {{"session_id", session_id}};
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

private:
  MqttClient& mqtt_client_;
  std::string root_topic_;
  std::string resource_name_;
};

#endif // HTTP2MQTTBRIDGE_RESOURCE_REQUEST_HANDLER_H
