//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_ACTION_STATUS_CODES_H
#define HTTP2MQTTBRIDGE_ACTION_STATUS_CODES_H

enum class CreateItemStatus {
  Success,
  ItemAlreadyExist,
  OperationForbidden,
  OutOfMemory
};

enum class GetItemStatus {
  Success,
  ItemDoesntExist,
  OperationForbidden,
  InternalError
};

enum class UpdateItemStatus {
  Success,
  ItemDoesntExist,
  OperationForbidden,
  OutOfMemory
};

enum class DeleteItemStatus {
  Success,
  ItemDoesntExist,
  OperationForbidden
};

#endif // HTTP2MQTTBRIDGE_ACTION_STATUS_CODES_H
