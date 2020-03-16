//
// Created by nomenas on 13-03-20.
//

#include <mqtt_client.h>

#include "user_manager.h"
#include "user_server.h"

int main(int argc, char** argv) {
  boost::asio::io_context ioc{1};

  MqttClient mqtt_client{"user_manager_daemon", "127.0.0.1", 1883, ioc};
  UserManager user_manager(ioc);
  UserServer<UserManager> user_handler("http2mqtt", mqtt_client, user_manager);

  return ioc.run();
}