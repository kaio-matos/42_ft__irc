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
  ~IRC(void) {
    typename std::map<int, Client<T> *>::iterator it = clients.begin();

    for (; it != clients.end(); it++) {
      delete it->second;
    }

    clients.clear();

    typename std::map<std::string, Channel<T> *>::iterator channelIt =
        channels.begin();

    for (; channelIt != channels.end(); channelIt++) {
      delete channelIt->second;
    }

    channels.clear();
  };

  void addClient(Client<T> *client) {
    clients.insert(std::make_pair(client->socket.getFd(), client));
  };

  void addChannel(Channel<T> *channel) {
    channels.insert(std::make_pair(channel->getChannelName(), channel));
  };

  Client<T> *getClient(std::string nickname) {
    typename std::map<int, Client<T> *>::iterator it = clients.begin();

    for (; it != clients.end(); it++) {
      if (it->second->user.nickname == nickname) {
        return it->second;
      }
    }
    return NULL;
  }

  Client<T> *getClient(int fd) {
    typename std::map<int, Client<T> *>::iterator it = clients.find(fd);
    if (it != clients.end())
      return (it->second);
    return (NULL);
  }

  Channel<T> *getChannel(std::string name) {
    typename std::map<std::string, Channel<T> *>::iterator it =
        channels.find(name);

    if (it != channels.end()) {
      return it->second;
    }
    return NULL;
  }

  void disconnectClient(Client<T> &client) {
    for (typename std::map<std::string, Channel<T> *>::iterator it =
             channels.begin();
         it != channels.end(); it++) {
      it->second->disconnectClient(client);
    }
    typename std::map<int, Client<T> *>::iterator clientIt =
        clients.find(client.socket.getFd());
    if (clientIt == clients.end())
      return;
    clients.erase(clientIt);
  }

  void broadcast(Client<T> &from, std::string message) {
    for (typename std::map<std::string, Channel<T> *>::iterator it =
             channels.begin();
         it != channels.end(); it++) {
      it->second->broadcast(from, message);
    }
  }

  std::map<int, Client<T> *> clients;
  std::map<std::string, Channel<T> *> channels;
};
#endif
