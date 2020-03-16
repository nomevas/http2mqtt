//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USER_H
#define HTTP2MQTTBRIDGE_USER_H

struct User {
  boost::uuids::uuid id;
  boost::optional<std::string> name;
  boost::optional<std::string> full_name;
  boost::optional<unsigned short> age;
};

#endif // HTTP2MQTTBRIDGE_USER_H
