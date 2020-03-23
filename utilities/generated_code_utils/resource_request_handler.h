//
// Created by nomenas on 23-03-20.
//

#ifndef HTTP2MQTTBRIDGE_RESOURCE_REQUEST_HANDLER_H
#define HTTP2MQTTBRIDGE_RESOURCE_REQUEST_HANDLER_H

#include <mqtt_client.h>
#include <iostream>

class ResourceRequestHandler {
public:
  using PostItemCallback = std::function<void(CreateItemStatus status_code, const boost::uuids::uuid&)>;
  using PostItemHandler = std::function<void(const tao::json::value& body, PostItemCallback callback)>;
  using GetItemCallback = std::function<void(ReadItemStatus status_code, const tao::json::value& object)>;
  using GetItemHandler = std::function<void(const boost::uuids::uuid&, GetItemCallback)>;

  ResourceRequestHandler(std::string resource_name, std::string root_topic, MqttClient& mqtt_client)
      : mqtt_client_{mqtt_client}
      , root_topic_{std::move(root_topic)}
      , resource_name_(std::move(resource_name)) {}

  void RegisterPostItemHandler(PostItemHandler handler) {
    mqtt_client_.Subscribe(root_topic_ + "/api/" + resource_name_ + "/POST",
        std::bind(&ResourceRequestHandler::OnPostMessage, this, std::placeholders::_2, std::move(handler)));
  }

  void RegisterGetItemHandler(GetItemHandler handler) {
    mqtt_client_.Subscribe(root_topic_ + "/api/" + resource_name_ + "/+/GET",
        std::bind(&ResourceRequestHandler::OnGetItemMessage, this, std::placeholders::_1, std::placeholders::_2, std::move(handler)));
  }

//  void RegisterPostItemHandler(std::function<void(const tao::json::value& body, CreateItemCallback callback)> handler) {
//    mqtt_client_.Subscribe(root_topic_ + "/api/" + resource_name_ + "/POST",
//                           std::bind(&ResourceRequestHandler::OnPostMessage, this, std::placeholders::_2, std::move(handler)));
//        mqtt_client.Subscribe(GetTopic("/api/user/GET"),
//                              std::bind(&ResourceRequestHandler::OnGetItemMessage, this, std::placeholders::_2));
//        mqtt_client.Subscribe(GetTopic("/api/user/+/PUT"),
//                              std::bind(&ResourceRequestHandler::OnPutMessage, this, std::placeholders::_1, std::placeholders::_2));
//        mqtt_client.Subscribe(GetTopic("/api/user/+/GET"),
//                              std::bind(&ResourceRequestHandler::OnGetItemsMessage, this, std::placeholders::_1, std::placeholders::_2));
//        mqtt_client.Subscribe(GetTopic("/api/user/+/DELETE"),
//                              std::bind(&ResourceRequestHandler::OnRemoveMessage, this, std::placeholders::_1, std::placeholders::_2));}
//  }
protected:
  void OnPostMessage(const std::string& message, PostItemHandler handler) {
    size_t session_id = {};
    try {
      const tao::json::value request = tao::json::from_string(message);
      const auto body = request.at("body");
      session_id = request.as<size_t>("session_id");
      tao::json::value response = {{"session_id", session_id}};
      handler(body, GetPostItemCallback(std::move(response)));
    } catch (const std::exception& ex) {
      tao::json::value response = {{"session_id", session_id}};
      response["status"] = 400;
      response["body"] = std::string("{\"error\": \"") + ex.what() + "\"}";
      mqtt_client_.Publish(root_topic_ + "/response", tao::json::to_string(response));
    }
  }

  void OnGetItemMessage(const WildcardValue& wildcard_value, const std::string& message, GetItemHandler handler) {
    size_t session_id = {};
    try {
      const tao::json::value request = tao::json::from_string(message);
      session_id = request.as<size_t>("session_id");
      tao::json::value response = {{"session_id", session_id}};
      handler(boost::lexical_cast<boost::uuids::uuid>(wildcard_value), GetGetItemCallback(std::move(response)));
    } catch (const std::exception& ex) {
      tao::json::value response = {{"session_id", session_id}};
      response["status"] = 404;
      response["body"] = "{\"error\": \"Resource doesn't exist\"}";
      mqtt_client_.Publish(root_topic_ + "/response", tao::json::to_string(response));
    }
  }

  void OnGetItemsMessage(const std::string& message) {

  }

  void OnPutMessage(const WildcardValue& wildcard_value, const std::string& message) {

  }

  void OnRemoveMessage(const WildcardValue& wildcard_value, const std::string& message) {

  }

  PostItemCallback GetPostItemCallback(tao::json::value response) {
    return [mqtt_client = mqtt_client_, root_topic = root_topic_, response = std::move(response)](
               CreateItemStatus status_code, const boost::uuids::uuid &uuid) mutable {
      switch (status_code) {
      case CreateItemStatus::Success:
        response["status"] = 200;
        response["body"] =
            "{\"uuid\": \"" + boost::lexical_cast<std::string>(uuid) + "\"}";
        break;
      case CreateItemStatus::ItemAlreadyExist:
        response["status"] = 400;
        response["body"] = "{\"error\": \"Item already exist\"}";
      case CreateItemStatus::OperationForbidden:
        response["status"] = 403;
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

  GetItemCallback GetGetItemCallback(tao::json::value response) {
    return [mqtt_client = mqtt_client_, root_topic = root_topic_, response = std::move(response)](
        ReadItemStatus status_code, const tao::json::value& object) mutable {
      switch (status_code) {
      case ReadItemStatus::Success:
        response["status"] = 200;
        response["body"] = object;
        break;
      case ReadItemStatus::ItemDoesntExist:
        response["status"] = 404;
        response["body"] = "{\"error\": \"Item already exist\"}";
      case ReadItemStatus::OperationForbidden:
        response["status"] = 403;
        response["body"] = "{\"error\": \"Operation forbidden\"}";
        break;
      }
      std::cout << "response: " << response << std::endl;
      mqtt_client.Publish(root_topic + "/response", tao::json::to_string(response));
    };
  }

private:
  MqttClient& mqtt_client_;
  std::string root_topic_;
  std::string resource_name_;
};

#endif // HTTP2MQTTBRIDGE_RESOURCE_REQUEST_HANDLER_H
