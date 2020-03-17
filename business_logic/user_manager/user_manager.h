//
// Created by nomenas on 13-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USER_MANAGER_H
#define HTTP2MQTTBRIDGE_USER_MANAGER_H

#include <boost/asio.hpp>

#include <boost/uuid/uuid.hpp>

#include <map>

#include <tao/json.hpp>

#include <user.h>

#include "crud_status_codes.h"

class UserManager {
public:
  UserManager(boost::asio::io_context& ioc);

  void AddUser(User user, std::function<void(CreateItemStatus status_code, const boost::uuids::uuid&)> callback);
  void UpdateUser(boost::uuids::uuid id, User user, std::function<void(UpdateItemStatus status_code)> callback);
  void GetUser(boost::uuids::uuid id, std::function<void(ReadItemStatus status_code, boost::optional<const User&>)> callback);
  void GetUsers(boost::uuids::uuid cursor, std::vector<boost::uuids::uuid> ids,
      std::function<void(ReadItemStatus, const std::vector<std::reference_wrapper<const User>>&)>);
  void RemoveUser(boost::uuids::uuid id, std::function<void(DeleteItemStatus status_code)> callback);

private:
  boost::asio::io_context& ioc_;
  std::map<boost::uuids::uuid, User> users_;
};

#endif // HTTP2MQTTBRIDGE_USER_MANAGER_H
