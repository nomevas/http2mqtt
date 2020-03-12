//
// Created by nomenas on 12-03-20.
//

#include "http2mqtt_bridge.h"

#include <tao/json.hpp>

Http2MqttBridge::Http2MqttBridge(
    std::shared_ptr<HttpServer> http_server, std::shared_ptr<MqttClient> mqtt_client)
    : mqtt_client_{mqtt_client}
    , http_server_{http_server} {
  http_server_->SetRequestHandler([mqtt_client = mqtt_client_](const Request& request) {
    const tao::json::value request_json(
        {
          {"request", request.id},
          {"body", request.body}
        });

    mqtt_client->Publish(
        "http2mqtt/" + request.target + "/" + request.method,
        tao::json::to_string(request_json),
        MQTT_NS::qos::at_most_once);
  });

  mqtt_client_->Subscribe("http2mqtt/response", [http_server = http_server_](const Topic& topic, const Message& message) {
    const tao::json::value request_json(message);
    Response response;
    response.request_id = request_json.as<unsigned long>("request_id");
    response.status = request_json.as<unsigned short>("status");
    response.body = request_json.as<unsigned short>("body");
    http_server->PostResponse(response);
  });
}