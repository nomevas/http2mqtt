#include <boost/asio.hpp>

#include "http_server.h"
#include "mqtt_client.h"

#include <tao/json.hpp>

int main(int argc, char **argv) {
  boost::asio::io_context ioc{1};

  MqttClient mqtt_client("http2mqtt", "127.0.0.1", 1883, ioc);
  HttpServer http_server("127.0.0.1", 8081, ioc);

  http_server.SetRequestHandler([&](const Request& request) {
    const tao::json::value request_json({
                                      {"request", request.id},
                                      {"body", request.body}});
    mqtt_client.Publish(
        "http2mqtt/" + request.target + "/" + request.method,
        tao::json::to_string(request_json),
        MQTT_NS::qos::at_most_once);
  });

  mqtt_client.Subscribe("http2mqtt/response", [&](const Topic& topic, const Message& message) {
    const tao::json::value request_json(message);
    Response response;
    response.request_id = request_json.as<unsigned long>("request_id");
    response.status = request_json.as<unsigned short>("status");
    response.body = request_json.as<unsigned short>("body");
    http_server.PostResponse(response);
  });

  return ioc.run();
}
