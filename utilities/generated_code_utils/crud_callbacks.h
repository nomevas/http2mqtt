//
// Created by nomenas on 20-03-20.
//

#ifndef HTTP2MQTTBRIDGE_CRUD_CALLBACKS_H
#define HTTP2MQTTBRIDGE_CRUD_CALLBACKS_H

#include "crud_status_codes.h"

#include <boost/uuid/uuid.hpp>
#include <vector>

using CreateItemCallback = std::function<void(CreateItemStatus status_code, const boost::uuids::uuid&)>;
using UpdateItemCallback = std::function<void(UpdateItemStatus status_code)>;
template <typename T>
using GetItemCallback = std::function<void(ReadItemStatus status_code, boost::optional<const T&>)>;
template <typename T>
using GetItemsCallback = std::function<void(ReadItemStatus, const std::vector<std::reference_wrapper<const T>>&)>;
using DeleteItemCallback = std::function<void(DeleteItemStatus status_code)>;

#endif // HTTP2MQTTBRIDGE_CRUD_CALLBACKS_H
