#include <ft_irc.hpp>

User::User(std::string nickname, std::string username)
    : nickname(nickname), username(username), host("ft.irc") {}

User::User(const User &value)
    : nickname(value.nickname), username(value.username), host(value.host) {}

User &User::operator=(const User &value) {
  if (this != &value) {
    nickname = value.nickname;
    username = value.username;
    host = value.host;
  }
  return *this;
}

bool User::operator==(const User &value) const {
  return username == value.username && nickname == value.nickname;
};

User::~User(void) {}

std::string User::identity() { return nickname + "!" + username + "@" + host; }
