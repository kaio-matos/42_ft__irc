#include <ft_irc.hpp>

User::User(std::string username, std::string realname)
    : username(username), realname(realname), nickname(""), host("ft.irc") {}

User::User(std::string nickname)
    : username(""), realname(""), nickname(nickname), host("ft.irc") {}

User::User(const User &value)
    : username(value.username), realname(value.realname),
      nickname(value.nickname), host(value.host) {}

User &User::operator=(const User &value) {
  if (this != &value) {
    username = value.username;
    realname = value.realname;
    nickname = value.nickname;
    host = value.host;
  }
  return *this;
}

bool User::operator==(const User &value) const {
  return username == value.username && realname == value.realname &&
         nickname == value.nickname;
};

User::~User(void) {}

std::string User::identity() { return nickname + "!" + username + "@" + host; }

bool User::isComplete() {
  return !nickname.empty() && !username.empty() && !realname.empty();
}
