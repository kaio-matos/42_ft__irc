#ifndef CLIENT_H
#define CLIENT_H
#include <ft_irc.hpp>

template <typename T> class Client {
public:
  Client(User user, Socket<T> &socket) : user(user), socket(socket){};

  Client(const Client &value)
      : user(value.user), socket(value.socket),
        _pendingInvites(value._pendingInvites),
        awayMessage(value.awayMessage){};

  Client &operator=(const Client &value) {
    if (this != &value) {
      user = value.user;
      socket = value.socket;
      _pendingInvites = value._pendingInvites;
      awayMessage = value.awayMessage;
    }
    return *this;
  };

  ~Client(void){};

  bool operator==(const Client &value) const {
    return user == value.user && socket == value.socket;
  };
  bool operator!=(const Client &value) const { return !(*this == value); };

  void sendInvite(std::string channelname) {
    _pendingInvites.insert(channelname);
  }

  void acceptInviteFrom(std::string channelname) {
    _pendingInvites.erase(_pendingInvites.find(channelname));
  }

  bool hasPendingInviteFrom(std::string channelname) {
    if (_pendingInvites.find(channelname) != _pendingInvites.end())
      return true;
    return false;
  }

  User user;
  Socket<T> &socket;
  std::string awayMessage; // TODO:

private:
  std::set<std::string> _pendingInvites;
};

std::ostream &operator<<(std::ostream &os, const Client<sockaddr_in> &value);

#endif
