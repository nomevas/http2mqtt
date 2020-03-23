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
    RegisterGetItemsHandler(std::bind(&UserRequestHandler::GetUsers, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    RegisterPutItemHandler(std::bind(&UserRequestHandler::UpdateUser, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    RegisterDeleteItemHandler(std::bind(&UserRequestHandler::DeleteUser, this,
        std::placeholders::_1, std::placeholders::_2));
  }

protected:
  void CreateUser(const tao::json::value& body, PostItemCallback callback) {
    ThrowIfNotValid<User, HttpMethod::POST>(body);
    handler_.AddUser(Parse<User>(body), std::move(callback));
  }

  void GetUser(const boost::uuids::uuid& uuid, GetItemCallback callback) {
    handler_.GetUser(uuid, [callback = std::move(callback)](ReadItemStatus status_code, boost::optional<const User&> user) {
      tao::json::value object = tao::json::null;

      try {
        object = ToJson<User>(user.value());
        ThrowIfNotValid<User, HttpMethod::GET>(object);
      } catch (const std::exception&) {
        status_code = ReadItemStatus::InternalError;
        object = tao::json::null;
      }

      callback(status_code, object);
    });
  }

  void GetUsers(const boost::uuids::uuid& cursor, const std::vector<boost::uuids::uuid>& ids, GetItemsCallback callback) {
    handler_.GetUsers(cursor, ids, [callback = std::move(callback)](ReadItemStatus status_code, const std::vector<std::reference_wrapper<const User>>& users) {
      tao::json::value object = tao::json::empty_array;

      try {
        std::vector< tao::json::value> array;
        for (auto&& user : users) {
          array.emplace_back(ToJson<User>(user));
          ThrowIfNotValid<User, HttpMethod::GET>(array.back());
        }
        object = array;
      } catch (const std::exception&) {
        status_code = ReadItemStatus::InternalError;
        object = tao::json::empty_array;
      }

      callback(status_code, object);
    });
  }

  void UpdateUser(const boost::uuids::uuid& uuid, const tao::json::value& body, PutItemCallback callback) {
    ThrowIfNotValid<User, HttpMethod::PUT>(body);
    handler_.UpdateUser(uuid, Parse<User>(body), std::move(callback));
  }

  void DeleteUser(const boost::uuids::uuid& uuid, DeleteItemCallback callback) {
    handler_.RemoveUser(uuid, std::move(callback));
  }

private:
  Handler& handler_;
};

#endif // HTTP2MQTTBRIDGE_USERSERVER_H
