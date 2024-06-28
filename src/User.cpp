#include <ft_irc.hpp>

User::User(std::string nickname, std::string username)
    : username(username), nickname(nickname), host("ft.irc") {}

User::User(const User &value)
    : username(value.username), nickname(value.nickname) {}

User &User::operator=(const User &value) {
  if (this != &value) {
    nickname = value.nickname;
    username = value.username;
  }
  return *this;
}

bool User::operator==(const User &value) const {
  return username == value.username && nickname == value.nickname;
};

User::~User(void) {}

std::string User::getIdentity() {
  return nickname + "!~" + username + "@" + host;
}
