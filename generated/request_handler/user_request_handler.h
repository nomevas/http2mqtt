//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USERSERVER_H
#define HTTP2MQTTBRIDGE_USERSERVER_H

#include <mqtt_client.h>
#include <iostream>

#include "user.h"

template <typename Handler>
class UserRequestHandler {
public:
  UserRequestHandler(std::string root_topic, MqttClient& mqtt_client, Handler& handler)
  : mqtt_client_{mqtt_client}
  , handler_{handler}
  , root_topic_{std::move(root_topic)} {
    mqtt_client.Subscribe(root_topic_ + "/api/user/POST",
        std::bind(&UserRequestHandler<Handler>::OnPostUser, this, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic_ + "/api/user/GET",
        std::bind(&UserRequestHandler<Handler>::OnGetUsers, this, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic_ + "/api/user/+/PUT",
        std::bind(&UserRequestHandler<Handler>::OnPutUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic_ + "/api/user/+/GET",
        std::bind(&UserRequestHandler<Handler>::OnGetUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic_ + "/api/user/+/DELETE",
        std::bind(&UserRequestHandler<Handler>::OnRemoveUser, this, std::placeholders::_1, std::placeholders::_2));}

protected:
  void OnPostUser(const std::string& message) {
    tao::json::value response;
    try {
      const tao::json::value message_json(message);
      response["session_id"] = message_json.as<size_t>("session_id");
      const auto body = message_json.find("body");
      ThrowIfNotValid<User, HttpMethod::POST>(body);
      handler_.AddUser(Parse<User>(*body),
          [this, response = std::move(response)](CreateItemStatus status_code, const boost::uuids::uuid& uuid) mutable {
        SetCreateItemResponse(&response, status_code, uuid);
        mqtt_client_.Publish(root_topic_ + "/response", tao::json::to_string(response));
      });
    } catch (const std::exception& ex) {
      response["status"] = 400;
      response["error"] = ex.what();
      mqtt_client_.Publish(root_topic_ + "/response", tao::json::to_string(response));
    }
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
  MqttClient& mqtt_client_;
  Handler& handler_;
  std::string root_topic_;
};

#endif // HTTP2MQTTBRIDGE_USERSERVER_H
