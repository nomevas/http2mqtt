//
// Created by nomenas on 13-03-20.
//

#include <mqtt_client.h>

#include "user_manager.h"
#include "user_request_handler.h"

#include <tao/json.hpp>

int main(int argc, char** argv) {
  boost::asio::io_context ioc{1};
  boost::asio::io_context::work work{ioc};

  MqttClient mqtt_client{"user_manager_daemon", "127.0.0.1", 1883, ioc};
  UserManager user_manager(ioc);
  UserRequestHandler<UserManager> user_handler("http2mqtt", mqtt_client, user_manager);

  return ioc.run();
}