#ifndef IRC_H
#define IRC_H
#include <ft_irc.hpp>

template <typename T> class IRC {
public:
  IRC(void){};
  // IRC(std::map<int, Client<T> > clients,
  //     std::map<std::string, Channel<T> > channels)
  //     : clients(clients), channels(channels){};
  IRC(const IRC &value) : clients(value.clients), channels(value.channels){};
  IRC &operator=(const IRC &value) {
    if (this != &value) {
      clients = value.clients;
      channels = value.channels;
    }
    return *this;
  };
  ~IRC(void){};

  void addClient(Client<T> client) {
    clients.insert(typename std::map<int, Client<T> >::value_type(
        client.socket.getFd(), client));
  };

  void addChannel(Channel<T> channel) {
    channels.insert(typename std::map<std::string, Channel<T> >::value_type(
        channel.topic, channel));
  };

  Client<T> &getClient(std::string nickname) {
    typename std::map<int, Client<T> >::iterator it = clients.begin();

    for (; it != clients.end(); it++) {
      if (it->second.user.nickname == nickname) {
        return it->second;
      }
    }
    throw std::runtime_error("Client not found");
  }

  std::map<int, Client<T> > clients;
  std::map<std::string, Channel<T> > channels;
};
#endif
