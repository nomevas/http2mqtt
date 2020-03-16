//
// Created by nomenas on 13-03-20.
//

#include <mqtt_client.h>

#include "business_logic.h"
#include "route.h"

int main(int argc, char** argv) {
  boost::asio::io_context ioc{1};

  MqttClient mqtt_client{"business_logic", "127.0.0.1", 1883, ioc};
  // pass mqtt_client for sending events
  BusinessLogic business_logic(ioc);

  std::vector<Route> routes;
  routes.emplace_back(mqtt_client, "http2mqtt/request/business_logic", "POST",
      std::bind(&BusinessLogic::AddItem, &business_logic, std::placeholders::_1, std::placeholders::_2));
//  routes.emplace_back(mqtt_client, "http2mqtt/request/business_logic/GET",
//      std::bind(&BusinessLogic::GetItemPaginated, &business_logic, std::placeholders::_1, std::placeholders::_2));
//  routes.emplace_back(mqtt_client, "http2mqtt/request/business_logic/#/GET",
//      std::bind(&BusinessLogic::AddItem, &business_logic, std::placeholders::_1, std::placeholders::_2));
//  routes.emplace_back(mqtt_client, "http2mqtt/request/business_logic/#/PUT",
//      std::bind(&BusinessLogic::AddItem, &business_logic, std::placeholders::_1, std::placeholders::_2));
//  routes.emplace_back(mqtt_client, "http2mqtt/request/business_logic/#/DELETE",
//      std::bind(&BusinessLogic::AddItem, &business_logic, std::placeholders::_1, std::placeholders::_2));

  return ioc.run();
}