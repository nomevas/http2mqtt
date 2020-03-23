//
// Created by nomenas on 16-03-20.
//

#ifndef HTTP2MQTTBRIDGE_CRUD_STATUS_CODES_H
#define HTTP2MQTTBRIDGE_CRUD_STATUS_CODES_H

enum class CreateItemStatus {
  Success,
  ItemAlreadyExist,
  OperationForbidden,
  OutOfMemory
};

enum class ReadItemStatus {
  Success,
  ItemDoesntExist,
  OperationForbidden
};

enum class UpdateItemStatus {
  Success,
  ItemDoesntExist,
  OperationForbidden,
  OutOfMemory,
  InvalidArguments
};

enum class DeleteItemStatus {
  Success,
  ItemDoesntExist,
  OperationForbidden
};

#endif // HTTP2MQTTBRIDGE_CRUD_STATUS_CODES_H
