#ifndef CHANNEL_H
#define CHANNEL_H
#include <ft_irc.hpp>

template <typename T> class Channel {
public:
  Channel(std::string topic) : topic(topic) {
    clients = std::vector<Client<T> >();
  };
  Channel(const Channel &value)
      : topic(value.topic), clients(value.clients){

                            };
  Channel &operator=(const Channel &value) {
    if (this != &value) {
      clients = value.clients;
      topic = value.topic;
    }
    return *this;
  };
  ~Channel(void);

  std::vector<Client<T> > clients;
  std::string topic;
};

std::ostream &operator<<(std::ostream &os, const Channel<sockaddr_in> &value);

#endif
