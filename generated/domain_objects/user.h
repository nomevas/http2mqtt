//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USER_H
#define HTTP2MQTTBRIDGE_USER_H

#include <boost/uuid/uuid.hpp>
#include <boost/optional.hpp>

#include <domain_object_apis.h>

struct User {
  boost::optional<boost::uuids::uuid> id;
  boost::optional<std::string> name;
  boost::optional<unsigned short> age;
};

template <>
void ThrowIfNotValid<User, ActionType::Create>(const tao::json::value& json) {
  if (!json.find("name")) {
    throw std::runtime_error("User.name is mandatory field!");
  }
}

template <>
User Parse<User>(const tao::json::value& json) {
  User user;
  if (json.find("id")) {
    user.id = boost::lexical_cast<boost::uuids::uuid>(json.as<std::string>("id"));
  }
  if (json.find("name")) {
    user.name = json.as<std::string>("name");
  }
  if (json.find("age")) {
    user.age = json.as<unsigned short>("age");
  }
  return user;
}

template <>
tao::json::value Serialize<User>(const User& object) {
  tao::json::value return_value;

  if (object.id) {
    return_value["id"] = boost::lexical_cast<std::string>(object.id.value());
  }
  if (object.name) {
    return_value["name"] = object.name.value();
  }
  if (object.age) {
    return_value["age"] = object.age.value();
  }

  return return_value;
}

#endif // HTTP2MQTTBRIDGE_USER_H
