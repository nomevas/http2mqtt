//
// Created by nomenas on 13-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USER_MANAGER_H
#define HTTP2MQTTBRIDGE_USER_MANAGER_H

#include <boost/asio.hpp>

#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>

#include <map>

#include <tao/json.hpp>

#include <user.h>

class UserManager {
public:
  UserManager(boost::asio::io_context& ioc);

  void AddUser(const User& user, std::function<void(int status_code, const boost::uuids::uuid&)>);
  void UpdateUser(const boost::uuids::uuid& id, const User& user, std::function<void(int status_code)>);
  void GetUser(const boost::uuids::uuid& id, std::function<void(boost::optional<User>)>);
  void GetUsers(const boost::uuids::uuid& cursor, std::function<void(const std::vector<User>&)>);
  void RemoveItem(const boost::uuids::uuid id, std::function<void(int status_code)>);

private:
  boost::asio::io_context& ioc_;
  std::map<boost::uuids::uuid, User> users_;
};

#endif // HTTP2MQTTBRIDGE_USER_MANAGER_H
