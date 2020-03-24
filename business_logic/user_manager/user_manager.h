//
// Created by nomenas on 13-03-20.
//

#ifndef HTTP2MQTTBRIDGE_USER_MANAGER_H
#define HTTP2MQTTBRIDGE_USER_MANAGER_H

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include <map>

#include <tao/json.hpp>

#include <user.h>

#include <crud_callbacks.h>
#include <event.h>

class UserManager {
public:
  UserManager(boost::asio::io_context& ioc);

  void AddUser(User user, CreateItemCallback callback);
  void UpdateUser(boost::uuids::uuid id, User user, UpdateItemCallback callback);
  void GetUser(boost::uuids::uuid id, GetItemCallback<User> callback);
  void GetUsers(boost::uuids::uuid cursor, std::vector<boost::uuids::uuid> ids, GetItemsCallback<User> callback);
  void RemoveUser(boost::uuids::uuid id, DeleteItemCallback callback);

  boost::signals2::connection ConnectToUserEventStream(std::function<void(Event<User>)> sink);

private:
  boost:: asio::io_context& ioc_;
  std::map<boost::uuids::uuid, User> users_;
  boost::signals2::signal<void(Event<User>)> user_event_stream_;
};

#endif // HTTP2MQTTBRIDGE_USER_MANAGER_H
