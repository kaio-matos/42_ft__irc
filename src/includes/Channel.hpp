#ifndef CHANNEL_H
#define CHANNEL_H
#include <ft_irc.hpp>

// todo por nome e ajeitar o topic
template <typename T> class Channel {

public:
  typedef std::map<int, Client<T> *> map;

  Channel(std::string channelName)
      : _channelName(channelName), _operatorTopicOnly(false), _hasPasswd(false),
        _userLimit(-1), _hasUserlimit(false), _isInviteOnly(false),
        _clients(Channel::map()), _operators(Channel::map()) {}

  Channel(const Channel &value)
      : _channelName(value._channelName), _topic(value._topic),
        _clients(value._clients), _operators(value._operators),
        _operatorTopicOnly(value._operatorTopicOnly), _passwd(value._passwd),
        _hasPasswd(value._hasPasswd), _userLimit(value._userLimit),
        _hasUserlimit(value._hasUserlimit), _isInviteOnly(value._isInviteOnly) {
  }

  Channel &operator=(const Channel &value) {
    if (this != &value) {
      _channelName = value._channelName;
      _clients = value._clients;
      _operators = value._operators;
      _topic = value.getTopic();
      _operatorTopicOnly = value._operatorTopicOnly;
      _passwd = value._passwd;
      _hasPasswd = value._hasPasswd;
      _userLimit = value._userLimit;
      _hasUserlimit = value._hasUserlimit;
      _isInviteOnly = value._isInviteOnly;
    }
    return *this;
  }

  ~Channel(void){};

  void connectClient(Client<T> &client) {
    DebugLog << "Client fd: " << client.socket.getFd();
    _clients.insert(std::make_pair(client.socket.getFd(), &client));
    broadcast(client, "New user " + client.user.nickname +
                          " has entered the channel\n");
  };

  bool disconnectClient(Client<T> &client) {
    typename map::iterator it = _clients.begin();

    for (it = _clients.begin(); it != _clients.end(); ++it) {
      if (*it->second == client) {
        _clients.erase(it);
        return true;
      }
    }
    return false;
  };

  void broadcast(Client<T> exclude, std::string message) {
    typename map::iterator it = _clients.begin();
    for (it; it != _clients.end(); it++) {
      if (exclude != *it->second) {
        it->second->socket.write(SSTR(it->second->socket.getFd()) + " " +
                                 message);
      }
    }
  };

  void broadcast(std::string message) {
    typename map::iterator it = _clients.begin();
    for (it; it != _clients.end(); it++) {
      it->second->socket.write(SSTR(it->second->socket.getFd()) + " " +
                               message);
    }
  };

  map getClients(void) const { return _clients; };

  Client<T> *getClient(Socket<T> socket) {
    try {
      return _clients.at(socket.getFd());
    } catch (const std::exception &error) {
      return NULL;
    }
  }

  Client<T> *getClient(std::string nickname) {
    typename map::iterator it = _clients.begin();

    for (it; it != _clients.end(); it++) {
      if (nickname == it->second->user.nickname) {
        return it->second;
      }
    }
    return NULL;
  }

  std::string getChannelUsers() const {
    std::string userList;
    for (typename map::const_iterator it = _clients.begin();
         it != _clients.end(); ++it) {
      userList += it->second->user.nickname + " ";
    }
    if (!userList.empty()) {
      userList.resize(userList.size() -
                      1); // Remove o último caractere (espaço em branco)
    }
    return userList;
  }

  bool isClientInChannel(const Client<T> *client) const {
    typename std::map<int, Client<T> *>::const_iterator it = _clients.begin();

    for (; it != _clients.end(); ++it) {
      if (client->user.username == it->second->user.username) {
        return true;
      }
    }
    return false;
  }

  void addOperator(Client<T> *client) {
    _operators.insert(std::make_pair(client->socket.getFd(), client));
  };

  void removeOperator(const Client<T> *client) {
    _operators.erase(client->socket.getFd());
  }

  bool isOperator(const Client<T> *client) const {
    return _operators.find(client->socket.getFd()) != _operators.end();
  }

  // Password methods and getters
  std::string getPasswd() const { return (this->_passwd); }

  bool hasPassword() const { return (this->_hasPasswd); }

  void setPasswd(bool action, const std::string &passwd) {
    this->_hasPasswd = action;
    if (action == false)
      this->_passwd = "";
    else
      this->_passwd = passwd;
  }

  std::string getChannelName() const { return (this->_channelName); }

  void setChannelName(const std::string &channelName) {
    this->_channelName = channelName;
  }

  // Topic methods and getters
  std::string getTopic() const { return (this->_topic); }

  void setTopic(const std::string &topic) { this->_topic = topic; }

  void setTopicRestricted(bool action) { this->_operatorTopicOnly = action; }

  bool isTopicOPOnly() const { return (this->_operatorTopicOnly); }

  // User limit methods and getters
  size_t getUserLimit() const { return (this->_userLimit); }

  bool hasUserLimit() const { return (this->_hasUserlimit); }

  void setUserLimit(bool action, const size_t &userLimit) {
    this->_hasUserlimit = action;
    if (action == false)
      this->_userLimit = -1;
    else
      this->_userLimit = userLimit;
  }

  // Invite only methods and getters
  bool isInviteOnly() const { return (this->_isInviteOnly); }
  void setIsInviteOnly(bool action) { this->_isInviteOnly = action; }

  std::string getModes() const {

    std::string modes = "+";
    std::string modeParams = "";

    if (this->isInviteOnly())
      modes += 'i';

    if (this->isTopicOPOnly())
      modes += 't';

    if (this->hasPassword()) {
      modes += 'k';
      modeParams += this->getPasswd() + " ";
    }

    if (this->hasUserLimit()) {
      modes += 'l';
      std::ostringstream oss;
      oss << this->_userLimit;
      modeParams += oss.str() + " ";
    }

    return (std::string(modes + " " + modeParams));
  }

private:
  map _clients;
  map _operators;

  std::string _channelName;
  std::string _topic;
  bool _operatorTopicOnly;

  std::string _passwd;
  bool _hasPasswd;

  size_t _userLimit;
  bool _hasUserlimit;

  bool _isInviteOnly;
};

std::ostream &operator<<(std::ostream &os, const Channel<sockaddr_in> &value);

#endif
