//
// Created by nomenas on 13-03-20.
//

#include "route.h"

Route::Route(MqttClient& mqtt_client, const std::string& path, std::string method, RouteHandler handler)
: mqtt_client_{mqtt_client}
, method_{std::move(method)}
, handler_{handler}{
  mqtt_client_.Subscribe(path, std::bind(&Route::ExecuteHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void Route::ExecuteHandler(const std::string& topic, const std::string& message) {

}