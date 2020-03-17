//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USERSERVER_H
#define HTTP2MQTTBRIDGE_USERSERVER_H

#include <mqtt_client.h>
#include <boost/uuid/uuid.hpp>

template <typename Handler>
class UserRequestHandler {
public:
  UserRequestHandler(const std::string& root_topic, MqttClient& mqtt_client, Handler& handler)
  : mqtt_client_{mqtt_client}
  , handler_{handler} {
    mqtt_client.Subscribe(root_topic + "/api/user/POST",
        std::bind(&UserRequestHandler<Handler>::OnPostUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/api/user/GET",
        std::bind(&UserRequestHandler<Handler>::OnGetUsers, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/api/user/#/PUT",
        std::bind(&UserRequestHandler<Handler>::OnPutUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/api/user/#/GET",
        std::bind(&UserRequestHandler<Handler>::OnGetUser, this, std::placeholders::_1, std::placeholders::_2));
    mqtt_client.Subscribe(root_topic + "/api/user/#/DELETE",
        std::bind(&UserRequestHandler<Handler>::OnRemoveUser, this, std::placeholders::_1, std::placeholders::_2));}

protected:
  void OnPostUser(const std::string& topic, const std::string& message) {
    int stop = 0;
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
