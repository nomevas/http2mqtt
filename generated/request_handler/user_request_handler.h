//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USERSERVER_H
#define HTTP2MQTTBRIDGE_USERSERVER_H

#include <mqtt_client.h>

#include "user.h"
#include <resource_request_handler.h>

template <typename Handler>
class UserRequestHandler : public ResourceRequestHandler {
public:
  UserRequestHandler(std::string root_topic, MqttClient& mqtt_client, Handler& handler)
  : ResourceRequestHandler{"user", std::move(root_topic), mqtt_client}
  , handler_{handler} {
    RegisterPostItemHandler(std::bind(&UserRequestHandler::CreateUser, this, std::placeholders::_1, std::placeholders::_2));
  }
protected:

  void CreateUser(const tao::json::value& body, CreateItemCallback callback) {
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
