//
// Created by nomenas on 20-03-20.
//

#ifndef HTTP2MQTTBRIDGE_CRUD_CALLBACKS_IMPL_H
#define HTTP2MQTTBRIDGE_CRUD_CALLBACKS_IMPL_H

#include "crud_callbacks.h"

static CreateItemCallback GetCreateItemCallback(MqttClient& mqtt_client, const std::string& root_topic, tao::json::value response) {
  return [mqtt_client, root_topic, response = std::move(response)]
      (CreateItemStatus status_code, const boost::uuids::uuid& uuid) mutable {
    switch (status_code) {
    case CreateItemStatus::Success:
      response["status"] = 200;
      response["body"] = "{\"uuid\": \"" + boost::lexical_cast<std::string>(uuid) + "\"}";
      break;
    case CreateItemStatus::ItemAlreadyExist:
      response["status"] = 400;
      response["body"] = "{\"error\": \"Item already exist\"}";
    case CreateItemStatus::OperationForbidden:
      response["status"] = 404;
      response["body"] = "{\"error\": \"Operation forbidden\"}";
      break;
    case CreateItemStatus::OutOfMemory:
      response["status"] = 507;
      response["body"] = "{\"error\": \"Out of memory\"}";
      break;
    }

    mqtt_client.Publish(root_topic + "/response", tao::json::to_string(response));
  };
}

#endif // HTTP2MQTTBRIDGE_CRUD_CALLBACKS_IMPL_H
