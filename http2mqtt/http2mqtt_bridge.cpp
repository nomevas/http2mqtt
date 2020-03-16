//
// Created by nomenas on 12-03-20.
//

#include "http2mqtt_bridge.h"

#include <tao/json.hpp>
#include <boost/lexical_cast.hpp>

Http2MqttBridge::Http2MqttBridge(
    const Topic& request_root_topic, std::shared_ptr<HttpServer> http_server, std::shared_ptr<MqttClient> mqtt_client)
    : request_root_topic_{request_root_topic}
    , mqtt_client_{mqtt_client}
    , http_server_{http_server} {
  http_server_->SetRequestHandler([mqtt_client = mqtt_client_, root_topic = request_root_topic_](SessionID session_id, const Input & request) {
    tao::json::value request_json({
            {"session_id", session_id},
            {"body", request.body()}
        });

    std::string target = boost::lexical_cast<std::string>(request.target());

    auto query_position = target.find('?');
    if (query_position != std::string::npos) {
      request_json["query_arguments"] = target.substr(query_position + 1);
      target = target.substr(0, query_position);
    }

    const auto topic = root_topic + "/request/" + target + "/" + boost::lexical_cast<std::string>(request.method_string());
    mqtt_client->Publish(topic, tao::json::to_string(request_json), MQTT_NS::qos::at_most_once);
  });

  mqtt_client_->Subscribe(request_root_topic_ + "/response", [http_server = http_server_](const Topic& topic, const Message& message) {
    const tao::json::value request_json(message);

    Output response;
    response.keep_alive(false);
    response.result(request_json.as<unsigned short>("status"));

    response.set(boost::beast::http::field::content_type, "application/json");
    response.set(boost::beast::http::field::content_length, response.body().size());
    response.body() = request_json.as<std::string>("body");

    http_server->PostResponse(request_json.as<SessionID>("session_id"), response);
  });
}