#include "http2mqtt_bridge.h"
#include <boost/asio.hpp>

int main(int argc, char **argv) {
  boost::asio::io_context ioc{1};

  Http2MqttBridge bridge(
      "http2mqtt",
    std::make_shared<HttpServer>("127.0.0.1", 8081, ioc),
    std::make_shared<MqttClient>("http2mqtt", "127.0.0.1", 1883, ioc));

  return ioc.run();
}
