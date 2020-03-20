//
// Created by nomenas on 13-03-20.
//

#include "user_manager.h"

#include <boost/uuid/uuid_generators.hpp>

#include <iostream>

UserManager::UserManager(boost::asio::io_context& ioc)
: ioc_{ioc} {}


void UserManager::AddUser(User user, CreateItemCallback callback) {
    ioc_.post([this, user = std::move(user), callback = std::move(callback)]() mutable {
      user.id = boost::uuids::random_generator()();
      users_[user.id.get()] = user;
      callback(CreateItemStatus::Success, user.id.get());
    });
}

void UserManager::UpdateUser(boost::uuids::uuid id, User user, UpdateItemCallback callback) {
  ioc_.post([this, id = std::move(id), user = std::move(user), callback = std::move(callback)]() mutable {
    const auto it = users_.find(id);
    if (it != users_.end()) {
      if (user.name) {
        it->second.name = user.name;
      }
      if (user.age) {
        it->second.age = user.age;
      }
      callback(UpdateItemStatus ::Success);
    } else {
      callback(UpdateItemStatus ::ItemDoesntExist);
    }
  });
}

void UserManager::GetUser(boost::uuids::uuid id, GetItemCallback<User> callback) {
  ioc_.post([this, id = id, callback = std::move(callback)]() mutable {
    const auto it = users_.find(id);
    if (it != users_.end()) {
      callback(ReadItemStatus::Success, it->second);
    } else {
      callback(ReadItemStatus::ItemDoesntExist, {});
    }
  });
}

void UserManager::GetUsers(boost::uuids::uuid, std::vector<boost::uuids::uuid>, GetItemsCallback<User> callback) {
  ioc_.post([this, callback = std::move(callback)]() mutable {
    std::vector<std::reference_wrapper<const User>> users;
    for (auto item : users_) {
      users.emplace_back(users_[item.first]);
    }
    callback(ReadItemStatus::Success, users);
  });
}

void UserManager::RemoveUser(const boost::uuids::uuid id, DeleteItemCallback callback) {
  ioc_.post([this, id = std::move(id), callback = std::move(callback)]() mutable {
    if (users_.find(id) != users_.end()) {
      users_.erase(id);
      callback(DeleteItemStatus::Success);
    } else {
      callback(DeleteItemStatus::ItemDoesntExist);
    }
  });
}