//
// Created by nomenas on 19-03-20.
//

#include <gtest/gtest.h>

#include "mqtt_client.h"

class TestMqttClient : public MqttClient {
public:
  void DoSubscribe(Topic topic, MessageHandler handler) {
    MqttClient::DoSubscribe(std::move(topic), std::move(handler));
  }

  bool OnPublished(MQTT_NS::buffer topic_name,
                   MQTT_NS::buffer contents) {
    return MqttClient::OnPublished(std::move(topic_name), std::move(contents));
  }
};

class FilterTests : public testing::TestWithParam<std::tuple<Topic, Topic, boost::optional<WildcardValue>>> {};

TEST_P(FilterTests, Body) {
  const auto& params = GetParam();

  TestMqttClient mqtt_client;
  boost::optional<WildcardValue> wildcard_value;
  mqtt_client.DoSubscribe(std::get<0>(params), [&](const WildcardValue& id, const Message&){
    wildcard_value = id;
  });

  MQTT_NS::buffer topic(boost::string_view(std::get<1>(params)));
  MQTT_NS::buffer message(boost::string_view("message"));
  mqtt_client.OnPublished(topic, message);
  if (wildcard_value)
    std::cout << "wildcard_value: " << *wildcard_value << std::endl;
  EXPECT_EQ(std::get<2>(params), wildcard_value);
}

INSTANTIATE_TEST_SUITE_P(SingleLevelFilterTests, FilterTests, testing::Values(
    std::make_tuple("topic_root/+/path", "topic_root/id/path", "id"),
    std::make_tuple("+/path", "id/path", "id"),
    std::make_tuple("topic_root/+", "topic_root/id", "id"),
    std::make_tuple("topic_root/+/GET", "topic_root/GET", boost::optional<WildcardValue>{}),
    std::make_tuple("topic_root/+/path", "topic_root/id/and/something/else/path", boost::optional<WildcardValue>{}),
    std::make_tuple("topic_root/1/+/path", "topic_root/id/path", boost::optional<WildcardValue>{}),
    std::make_tuple("+/path/1", "id/path", boost::optional<WildcardValue>{})));

INSTANTIATE_TEST_SUITE_P(MultiLevelFilterTests, FilterTests, testing::Values(
    std::make_tuple("topic_root/#/path/with/more/items", "topic_root/path/with/more/items", "path/with/more/items"),
    std::make_tuple("topic_root/#", "topic_root/path", "path"),
    std::make_tuple("topic_root/id/#", "topic_root/something/else", boost::optional<WildcardValue>{})));

INSTANTIATE_TEST_SUITE_P(TopicFilterTests, FilterTests, testing::Values(
    std::make_tuple("topic_root/path/with/more/items", "topic_root/path/with/more/items", ""),
    std::make_tuple("topic_root/path/with/more/items/1", "topic_root/path/with/more/items", boost::optional<WildcardValue>{}),
    std::make_tuple("topic_root/path/with/more/items", "topic_root/path/with/more/items/1", boost::optional<WildcardValue>{})));