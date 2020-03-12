#include <boost/asio.hpp>

#include "http_server.h"
#include "mqtt_client.h"

#include <unordered_set>

int main(int argc, char **argv) {
  boost::asio::io_context ioc{1};

  MqttClient mqtt_client("http2mqtt", "127.0.0.1", 1883, ioc);
  HttpServer http_server("127.0.0.1", 8081, ioc);

  http_server.SetRequestHandler([&](const Request& request) {
    Response response;
    response.request_id = request.id;
    response.result = 200;
    response.body = "Hello World";
    http_server.PostResponse(response);
  });

  // in case of timeout
  mqtt_client.Subscribe("http2mqtt/response", [](const Topic& topic, const Message& message) {

  });
  return ioc.run();
}
