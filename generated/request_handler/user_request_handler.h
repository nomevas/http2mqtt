//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USERSERVER_H
#define HTTP2MQTTBRIDGE_USERSERVER_H

#include <mqtt_client.h>

#include "user.h"
#include <request_handler.h>

template <typename Handler>
class UserRequestHandler : public RequestHandler {
public:
  UserRequestHandler(std::string root_topic, MqttClient& mqtt_client, Handler& handler)
  : RequestHandler{std::move(root_topic), mqtt_client}
  , handler_{handler} {
    mqtt_client.Subscribe(GetTopic("/api/user/POST"),
                          std::bind(&RequestHandler::OnPostMessage, this, std::placeholders::_2));
    mqtt_client.Subscribe(GetTopic("/api/user/GET"),
                          std::bind(&RequestHandler::OnGetItemMessage, this, std::placeholders::_2));
    mqtt_client.Subscribe(GetTopic("/api/user/+/PUT"),
                          std::bind(&RequestHandler::OnPutMessage, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(GetTopic("/api/user/+/GET"),
                          std::bind(&RequestHandler::OnGetItemsMessage, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(GetTopic("/api/user/+/DELETE"),
                          std::bind(&RequestHandler::OnRemoveMessage, this, std::placeholders::_1, std::placeholders::_2));}

protected:

  void PostItem(const tao::json::value& body, CreateItemCallback callback) override {
    ThrowIfNotValid<User, HttpMethod::POST>(body);
    handler_.AddUser(Parse<User>(body), std::move(callback));
  }

  void OnGetUsers(const std::string& message) {

  }

  void OnPutUser(const WildcardValue& wildcard_value, const std::string& message) {

  }

  void OnGetUser(const WildcardValue& wildcard_value, const std::string& message) {

  }

  void OnRemoveUser(const WildcardValue& wildcard_value, const std::string& message) {

  }

private:
  Handler& handler_;
};

#endif // HTTP2MQTTBRIDGE_USERSERVER_H
