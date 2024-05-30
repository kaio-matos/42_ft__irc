#include "User.hpp"

User::User(std::string username, std::string nickname)
    : username(username), nickname(nickname) {}

User::User(const User &value)
    : username(value.username), nickname(value.nickname) {}

User &User::operator=(const User &value) {
  if (this != &value) {
    nickname = value.nickname;
    username = value.username;
  }
  return *this;
}

User::~User(void) {}
