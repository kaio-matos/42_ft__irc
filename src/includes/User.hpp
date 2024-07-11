#ifndef USER_H
#define USER_H
#include <ft_irc.hpp>

class User {
public:
  User(void);

  User(std::string username, std::string realname);
  User(std::string nickname);

  User(const User &value);

  User &operator=(const User &value);

  ~User(void);

  bool operator==(const User &value) const;

  std::string identity();
  bool isComplete();

  std::string username;
  std::string realname;
  std::string nickname;
  std::string host;
};

std::ostream &operator<<(std::ostream &os, const User &value);

#endif
