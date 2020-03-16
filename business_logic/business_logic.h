//
// Created by nomenas on 13-03-20.
//

#ifndef HTTP2MQTTBRIDGE_BUSINESS_LOGIC_H
#define HTTP2MQTTBRIDGE_BUSINESS_LOGIC_H

#include <boost/asio.hpp>

#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>

#include <map>

#include <tao/json.hpp>

#include "route_handler.h"

struct MyObject {
  boost::uuids::uuid id;
  boost::optional<std::string> string_value;
  boost::optional<int> int_value;
};

class BusinessLogic {
public:
  BusinessLogic(boost::asio::io_context& ioc);

  // do we want to deal with json objects ?
  // how are we going to check if returned object confirm spec?
  void AddItem(const tao::json::value& object, CompletionCallback callback);
  void UpdateItem(const boost::uuids::uuid id, const tao::json::value& object, CompletionCallback callback);
  void GetItem(const boost::uuids::uuid id, CompletionCallback callback);
  void GetItemsPaginated(std::vector<std::pair<std::string, std::string>> headers, CompletionCallback callback);
  void RemoveItem(const boost::uuids::uuid id, CompletionCallback callback);

private:
  boost::asio::io_context& ioc_;
  std::map<boost::uuids::uuid, MyObject> container_;
};

#endif // HTTP2MQTTBRIDGE_BUSINESS_LOGIC_H
