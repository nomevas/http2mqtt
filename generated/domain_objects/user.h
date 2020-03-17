//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USER_H
#define HTTP2MQTTBRIDGE_USER_H

#include <boost/uuid/uuid.hpp>
#include <boost/optional.hpp>

struct User {
  boost::optional<boost::uuids::uuid> id;
  boost::optional<std::string> name;
  boost::optional<unsigned short> age;
};

#endif // HTTP2MQTTBRIDGE_USER_H
