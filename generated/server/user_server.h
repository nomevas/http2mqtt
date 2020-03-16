//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USERSERVER_H
#define HTTP2MQTTBRIDGE_USERSERVER_H

#include <mqtt_client.h>
#include <boost/uuid/uuid.hpp>

template <typename Handler>
class UserServer {
public:
  UserServer(const std::string& root_topic, MqttClient& mqtt_client, Handler& handler)
  : mqtt_client_{mqtt_client}
  , handler_{handler} {
    mqtt_client.Subscribe(root_topic + "/request/user/POST",
        std::bind(&UserServer<Handler>::OnPostUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/request/user/GET",
        std::bind(&UserServer<Handler>::OnGetUsers, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/request/user/#/PUT",
        std::bind(&UserServer<Handler>::OnPutUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/request/user/#/GET",
        std::bind(&UserServer<Handler>::OnGetUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/request/user/#/DELETE",
        std::bind(&UserServer<Handler>::OnRemoveUser, this, std::placeholders::_1, std::placeholders::_2));}

protected:
  void OnPostUser(const std::string& topic, const std::string& message) {

  }

  void OnGetUsers(const std::string& topic, const std::string& message) {

  }

  void OnPutUser(const std::string& topic, const std::string& message) {

  }

  void OnGetUser(const std::string& topic, const std::string& message) {

  }

  void OnRemoveUser(const std::string& topic, const std::string& message) {

  }

private:
  MqttClient& mqtt_client_;
  Handler& handler_;
};

#endif // HTTP2MQTTBRIDGE_USERSERVER_H
