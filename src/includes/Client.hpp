#ifndef CLIENT_H
#define CLIENT_H
#include <ft_irc.hpp>

template <typename T> class Client {
public:
  Client(User &user, Socket<T> &socket) : user(user), socket(socket){};
  Client(const Client &value) : user(value.user), socket(value.socket){};
  Client &operator=(const Client &value) {
    if (this != &value) {
      user = value.user;
      socket = value.socket;
    }
    return *this;
  };
  ~Client(void){};

  User &user;
  Socket<T> &socket;
};

std::ostream &operator<<(std::ostream &os, const Client<sockaddr_in> &value);

#endif
