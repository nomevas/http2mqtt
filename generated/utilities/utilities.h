//
// Created by nomenas on 19-03-20.
//

#ifndef HTTP2MQTTBRIDGE_UTILITIES_H
#define HTTP2MQTTBRIDGE_UTILITIES_H

#include <tao/json.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <crud_status_codes.h>

enum class HttpMethod {
  POST,
  PUT,
  GET,
  DELETE
};

template <typename T, HttpMethod Method>
static void ThrowIfNotValid(const tao::json::value& json) {}
template <typename T>
static T Parse(const tao::json::value& json);
template <typename T>
static tao::json::value ToJson(const T& object);

static void SetCreateItemResponse(tao::json::value* response, CreateItemStatus status_code, const boost::uuids::uuid& uuid) {
  switch (status_code) {
  case CreateItemStatus::Success:
    (*response)["status"] = 200;
    (*response)["body"] = "{uuid: " + boost::lexical_cast<std::string>(uuid) + "}";
    break;
  case CreateItemStatus::ItemAlreadyExist:
    (*response)["status"] = 400;
    (*response)["error"] = "Item already exist!";
  case CreateItemStatus::OperationForbidden:
    (*response)["status"] = 404;
    break;
  case CreateItemStatus::OutOfMemory:
    (*response)["status"] = 507;
    break;
  }
}

#endif // HTTP2MQTTBRIDGE_UTILITIES_H
