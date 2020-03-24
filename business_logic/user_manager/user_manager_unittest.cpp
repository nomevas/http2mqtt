//
// Created by nomenas on 16-03-20.
//

#include "user.h"
#include "user_manager.h"

#include <boost/asio.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <gtest/gtest.h>

TEST(UserManagerTests, AddUser__Nominal__CallbackWithValidIDCalled) {
  boost::asio::io_context ioc{1};
  UserManager user_manager{ioc};
  User user;
  user.name="Full Name";
  user.age = 18;
  boost::optional<CreateItemStatus> status_code;
  boost::optional<boost::uuids::uuid> uuid;
  user_manager.AddUser(user, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    status_code = code;
    uuid = id;
  });

  ioc.run();

  EXPECT_EQ(CreateItemStatus::Success, status_code);
  EXPECT_LT(0, uuid->size());
}

TEST(UserManagerTests, GetUser__UserExist__CallbackWithUserObjectCalled) {
  boost::asio::io_context ioc{1};
  UserManager user_manager{ioc};
  User user1;
  user1.name="Full Name 1";
  user1.age = 18;
  boost::optional<boost::uuids::uuid> user_uuid1;
  user_manager.AddUser(user1, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    user_uuid1 = id;
  });
  User user2;
  user2.name="Full Name 2";
  user2.age = 13;
  boost::optional<boost::uuids::uuid> user_uuid2;
  user_manager.AddUser(user2, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    user_uuid2 = id;
  });
  ioc.run();

  ASSERT_TRUE(user_uuid2);
  User result;
  user_manager.GetUser(*user_uuid2, [&](GetItemStatus status_code, boost::optional<const User&> user){
    result = *user;
  });

  ioc.restart();
  ioc.run();

  EXPECT_EQ(*user_uuid2, *result.id);
  EXPECT_EQ(user2.age, result.age);
  EXPECT_EQ(user2.name, result.name);
}

TEST(UserManagerTests, GetUser__UserDoesntExist__CallbackWithNoneCalled) {
  boost::asio::io_context ioc{1};
  UserManager user_manager{ioc};
  GetItemStatus status_code;
  boost::optional<User> result;
  user_manager.GetUser(boost::uuids::random_generator()(), [&](GetItemStatus code, boost::optional<const User&> user){
    status_code = code;
    result = user;
  });

  ioc.run_one();

  EXPECT_EQ(GetItemStatus::ItemDoesntExist, status_code);
  EXPECT_FALSE(result);
}

TEST(UserManagerTests, GetUsers__Nominal__CallbackWithAllUsersCalled) {
  boost::asio::io_context ioc{1};

  UserManager user_manager{ioc};

  User user1;
  user1.name="Full Name 1";
  user1.age = 18;
  boost::optional<boost::uuids::uuid> user_uuid1;
  user_manager.AddUser(user1, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    user_uuid1 = id;
  });
  User user2;
  user2.name="Full Name 2";
  user2.age = 13;
  boost::optional<boost::uuids::uuid> user_uuid2;
  user_manager.AddUser(user2, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    user_uuid2 = id;
  });
  ioc.run();

  ASSERT_TRUE(user_uuid2);
  std::vector<std::reference_wrapper<const User>> users;
  user_manager.GetUsers({}, {}, [&](GetItemStatus, const std::vector<std::reference_wrapper<const User>>& local_users){
    users = local_users;
  });

  ioc.restart();
  ioc.run();

  ASSERT_EQ(2, users.size());
  size_t first_element_index = 0;
  size_t second_element_index = 1;
  if (*user_uuid1 != *users[0].get().id) {
    first_element_index = 1;
    second_element_index = 0;
  }

  EXPECT_EQ(user_uuid1, users[first_element_index].get().id);
  EXPECT_EQ(user1.age, users[first_element_index].get().age);
  EXPECT_EQ(user1.name, users[first_element_index].get().name);
  EXPECT_EQ(user_uuid2, users[second_element_index].get().id);
  EXPECT_EQ(user2.age, users[second_element_index].get().age);
  EXPECT_EQ(user2.name, users[second_element_index].get().name);
}

TEST(UserManagerTests, UpdateUser__UserExist__CallbackCalled) {
  boost::asio::io_context ioc{1};
  UserManager user_manager{ioc};
  User user;
  user.name="Full Name 1";
  user.age = 18;
  boost::optional<boost::uuids::uuid> user_uuid;
  user_manager.AddUser(user, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    user_uuid = id;
  });

  ioc.run();

  User user_update;
  user_update.name="Full Name 2";
  boost::optional<UpdateItemStatus> update_code;
  user_manager.UpdateUser(*user_uuid, user_update, [&](UpdateItemStatus code){
    update_code = code;
  });
  ioc.restart();
  ioc.run();

  ASSERT_EQ(UpdateItemStatus::Success, *update_code);

  boost::optional<User> updated_user;
  user_manager.GetUser(*user_uuid, [&](GetItemStatus code, boost::optional<const User&> user){
    updated_user = user;
  });

  ioc.restart();
  ioc.run();

  EXPECT_EQ(user_uuid, (*updated_user).id);
  EXPECT_EQ(user_update.name, (*updated_user).name);
  EXPECT_EQ(user.age, (*updated_user).age);
}

TEST(UserManagerTests, RemoveUser__UserExist__UserRemoved_CallbackCalled) {
  boost::asio::io_context ioc{1};

  UserManager user_manager{ioc};

  User user1;
  user1.name="Full Name 1";
  user1.age = 18;
  boost::optional<boost::uuids::uuid> user_uuid1;
  user_manager.AddUser(user1, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    user_uuid1 = id;
  });
  User user2;
  user2.name="Full Name 2";
  user2.age = 13;
  boost::optional<boost::uuids::uuid> user_uuid2;
  user_manager.AddUser(user2, [&](CreateItemStatus code, const boost::uuids::uuid& id){
    user_uuid2 = id;
  });

  ioc.run();
  ASSERT_TRUE(user_uuid2);
  boost::optional<DeleteItemStatus> delete_status;
  user_manager.RemoveUser(*user_uuid2, [&](DeleteItemStatus status){
    delete_status = status;
  });

  ioc.restart();
  ioc.run();

  boost::optional<User> updated_user;
  std::vector<std::reference_wrapper<const User>> users;
  user_manager.GetUsers({}, {}, [&](GetItemStatus, const std::vector<std::reference_wrapper<const User>>& local_users){
    users = local_users;
  });

  ioc.restart();
  ioc.run();

  EXPECT_EQ(1, users.size());
}