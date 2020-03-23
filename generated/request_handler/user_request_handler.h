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
    RegisterPostItemHandler(std::bind(&UserRequestHandler::CreateUser, this,
        std::placeholders::_1, std::placeholders::_2));
    RegisterGetItemHandler(std::bind(&UserRequestHandler::GetUser, this,
        std::placeholders::_1, std::placeholders::_2));
    RegisterPutItemHandler(std::bind(&UserRequestHandler::UpdateUser, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  }

protected:
  void CreateUser(const tao::json::value& body, PostItemCallback callback) {
    ThrowIfNotValid<User, HttpMethod::POST>(body);
    handler_.AddUser(Parse<User>(body), std::move(callback));
  }

  void GetUser(const boost::uuids::uuid& uuid, GetItemCallback callback) {
    handler_.GetUser(uuid, [callback = std::move(callback)](ReadItemStatus status_code, boost::optional<const User&> user) {
      const auto object = ToJson<User>(*user);
      ThrowIfNotValid<User, HttpMethod::GET>(object);
      callback(status_code, object);
    });
  }

  void OnGetUsers(const std::string& message) {

  }

  void UpdateUser(const boost::uuids::uuid& uuid, const tao::json::value& body, PutItemCallback callback) {
    ThrowIfNotValid<User, HttpMethod::PUT>(body);
    handler_.UpdateUser(uuid, Parse<User>(body), std::move(callback));
  }

  void OnRemoveUser(const WildcardValue& wildcard_value, const std::string& message) {

  }

private:
  Handler& handler_;
};

#endif // HTTP2MQTTBRIDGE_USERSERVER_H
