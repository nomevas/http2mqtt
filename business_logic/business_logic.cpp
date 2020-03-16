//
// Created by nomenas on 13-03-20.
//

#include "business_logic.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

BusinessLogic::BusinessLogic(boost::asio::io_context& ioc)
: ioc_{ioc} {

}

void BusinessLogic::AddItem(const tao::json::value& object, CompletionCallback callback) {
  //    ioc.post([object = message["body"], session_id = message["session_id"]](){
//
//      MyObject my_object;
//      my_object.id = boost::uuids::random_generator()();
//      my_object.string_value = object["string_value"];
//      my_object.int_value = object["int_value"];
//
//      container.insert(uuid, my_object);
//    });

}