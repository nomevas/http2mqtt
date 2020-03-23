//
// Created by nomenas on 12-03-20.
//

#include "http2mqtt_bridge.h"

#include <tao/json.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

Http2MqttBridge::Http2MqttBridge(
    const Topic& request_root_topic, std::shared_ptr<HttpServer> http_server, std::shared_ptr<MqttClient> mqtt_client)
    : request_root_topic_{request_root_topic}
    , mqtt_client_{mqtt_client}
    , http_server_{http_server} {
  http_server_->SetRequestHandler([mqtt_client = mqtt_client_, root_topic = request_root_topic_](SessionID session_id, const Request & request) {
    try {
      tao::json::value request_json({
          {"session_id", session_id},
          {"body", tao::json::from_string(request.body())}
      });

      std::string target = boost::lexical_cast<std::string>(request.target());
      if (target.rfind('/') == target.size() - 1) {
        target.resize(target.size() - 1);
      }

      auto query_position = target.find('?');
      if (query_position != std::string::npos) {
        request_json["query_arguments"] = target.substr(query_position + 1);
        target = target.substr(0, query_position);
      }

      const auto topic = root_topic + target + "/" + boost::lexical_cast<std::string>(request.method_string());
      mqtt_client->Publish(topic, tao::json::to_string(request_json));
    } catch (const std::exception& ex) {
      std::cout << "Publish request exception: " << ex.what() << std::endl;
    }
  });

  mqtt_client_->Subscribe(request_root_topic_ + "/response", [http_server = http_server_](const Topic& topic, const Message& message) {
    try {
      const tao::json::value response_json = tao::json::from_string(message);

      Response response;
      response.result(response_json.as<unsigned short>("status"));

      if (response_json.find("body")) {
        response.set(boost::beast::http::field::content_type, "application/json");
        response.set(boost::beast::http::field::content_length, response.body().size());
        response.body() = response_json.as<std::string>("body");
      }

      http_server->PostResponse(response_json.as<SessionID>("session_id"), std::move(response));
    } catch (const std::exception& ex) {
      std::cout << "Handle response exception: " << ex.what() << std::endl;
    }
  });
}