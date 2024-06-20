#ifndef USER_H
#define USER_H
#include <ft_irc.hpp>

class User {
public:

  User(std::string nickname, std::string username);

  User(const User &value);

  User &operator=(const User &value);
  
  ~User(void);

  bool operator==(const User &value) const;

  std::string nickname;
  std::string username;
};

std::ostream &operator<<(std::ostream &os, const User &value);

#endif
