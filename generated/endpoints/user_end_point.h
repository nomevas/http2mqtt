//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USERSERVER_H
#define HTTP2MQTTBRIDGE_USERSERVER_H

#include <mqtt_client.h>

#include "user.h"
#include <end_point.h>

template <typename Handler>
class UserEndPoint : public EndPoint {
public:
  UserEndPoint(std::string root_topic, MqttClient& mqtt_client, Handler& handler)
  : EndPoint{"user", std::move(root_topic), mqtt_client}
  , handler_{handler} {
    RegisterPostItemHandler(std::bind(&UserEndPoint::CreateUser, this,
        std::placeholders::_1, std::placeholders::_2));
    RegisterGetItemHandler(std::bind(&UserEndPoint::GetUser, this,
        std::placeholders::_1, std::placeholders::_2));
    RegisterGetItemsHandler(std::bind(&UserEndPoint::GetUsers, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    RegisterPutItemHandler(std::bind(&UserEndPoint::UpdateUser, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    RegisterDeleteItemHandler(std::bind(&UserEndPoint::DeleteUser, this,
        std::placeholders::_1, std::placeholders::_2));
    handler_.ConnectToUserEventStream(std::bind(&EndPoint::PublishEvent<User>,
        this, std::placeholders::_1));
  }

protected:
  void CreateUser(const tao::json::value& body, PostItemCallback callback) {
    ThrowIfNotValid<User, ActionType::Create>(body);
    handler_.AddUser(Parse<User>(body), std::move(callback));
  }

  void GetUser(const boost::uuids::uuid& uuid, GetItemCallback callback) {
    handler_.GetUser(uuid, CreateGetItemWrapperCallback<User>(std::move(callback)));
  }

  void GetUsers(const boost::uuids::uuid& cursor, const std::vector<boost::uuids::uuid>& ids, GetItemsCallback callback) {
    handler_.GetUsers(cursor, ids, CreateGetItemsWrapperCallback<User>(std::move(callback)));
  }

  void UpdateUser(const boost::uuids::uuid& uuid, const tao::json::value& body, PutItemCallback callback) {
    ThrowIfNotValid<User, ActionType::Update>(body);
    handler_.UpdateUser(uuid, Parse<User>(body), std::move(callback));
  }

  void DeleteUser(const boost::uuids::uuid& uuid, DeleteItemCallback callback) {
    handler_.RemoveUser(uuid, std::move(callback));
  }

private:
  Handler& handler_;
};

#endif // HTTP2MQTTBRIDGE_USERSERVER_H
