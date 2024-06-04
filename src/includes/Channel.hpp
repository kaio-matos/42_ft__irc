#ifndef CHANNEL_H
#define CHANNEL_H
#include <ft_irc.hpp>

template <typename T> class Channel {

public:
  typedef std::map<int, Client<T> > map;

  Channel(std::string topic) : topic(topic){};
  Channel(const Channel &value)
      : topic(value.topic), _clients(value._clients){};
  Channel &operator=(const Channel &value) {
    if (this != &value) {
      _clients = value._clients;
      topic = value.topic;
    }
    return *this;
  };
  ~Channel(void){};

  void connectClient(Client<T> &client) {
    DebugLog << "Client fd: " << client.socket.getFd();
    _clients.insert(
        typename Channel::map::value_type(client.socket.getFd(), client));
  };

  void disconnectClient(Client<T> client) {
    _clients.erase(_clients.find(_clients.begin(), _clients.end(), client));
  };

  void broadcast(Client<T> from, std::string message) {
    typename map::iterator it = _clients.begin();
    DebugLog << "BEGIN";
    for (it; it != _clients.end(); it++) {
      if (from != it->second) {
        DebugLog << it->second;
        it->second.socket.write(SSTR(it->second.socket.getFd()) + " " +
                                message);
      }
    }
    DebugLog << "END";
  };

  map getClients(void) const { return _clients; };

  Client<T> getClient(Socket<T> socket) const {
    return _clients.at(socket.getFd());
  }

  std::string topic;

private:
  map _clients;
};

std::ostream &operator<<(std::ostream &os, const Channel<sockaddr_in> &value);

#endif
