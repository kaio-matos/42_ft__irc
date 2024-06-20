#ifndef CHANNEL_H
#define CHANNEL_H
#include <ft_irc.hpp>

template <typename T> class Channel {

public:
  typedef std::map<int, Client<T> > map;

  Channel(std::string topic)
      : _topic(topic), _opTopicOnly(false), _hasPasswd(false), _userLimit(-1),
        _hasUserlimit(false), _isInviteOnly(false) {}

  Channel(const Channel &value)
      : _topic(value._topic), _clients(value._clients),
        _operators(value._operators), _opTopicOnly(value._opTopicOnly),
        _passwd(value._passwd), _hasPasswd(value._hasPasswd),
        _userLimit(value._userLimit), _hasUserlimit(value._hasUserlimit),
        _isInviteOnly(value._isInviteOnly) {}

  Channel &operator=(const Channel &value) {
    if (this != &value) {
      _clients = value._clients;
      _operators = value._operators;
      _topic = value.getTopic();
      _opTopicOnly = value._opTopicOnly;
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
    _clients.insert(
        typename Channel::map::value_type(client.socket.getFd(), client));
    broadcast(client,
              "New user " + client.user.nickname + " has entered the channel");
  };

  bool disconnectClient(Client<T> client) {
    typename map::iterator it = _clients.begin();

    for (it = _clients.begin(); it != _clients.end(); ++it) {
      if (it->second == client) {
        _clients.erase(it);
        return true;
      }
    }
    return false;
  };

  void broadcast(Client<T> from, std::string message) {
    typename map::iterator it = _clients.begin();
    for (it; it != _clients.end(); it++) {
      if (from != it->second) {
        DebugLog << it->second;
        it->second.socket.write(SSTR(it->second.socket.getFd()) + " " +
                                message);
      }
    }
  };

  map getClients(void) const { return _clients; };

  Client<T> *getClient(Socket<T> socket) {
    return &_clients.at(socket.getFd());
  }

  Client<T> *getClient(std::string name) {
    typename map::iterator it = _clients.begin();

    for (it; it != _clients.end(); it++) {
      if (name == it->second.user.username) {
        return &it->second;
      }
    }
    return NULL;
  }

  std::string getChannelUsers() const {
    std::string userList;
    for (typename map::const_iterator it = _clients.begin();
         it != _clients.end(); ++it) {
      userList += it->second.user.nickname + " ";
    }
    if (!userList.empty()) {
      userList.resize(userList.size() -
                      1); // Remove o último caractere (espaço em branco)
    }
    return userList;
  }

  void addOperator(const Client<T> &client) {
    _operators.insert(
        typename Channel::map::value_type(client.socket.getFd(), client));
  }

  void removeOperator(const Client<T> &client) {
    _operators.erase(client.socket.getFd());
  }

  bool isOperator(const Client<T> &client) const {
    return _operators.find(client.socket.getFd()) != _operators.end();
  }

  // Password methods and getters
  std::string getPasswd() const { return (this->_passwd); }

  void setPasswd(bool action, const std::string &passwd) {
    this->_hasPasswd = action;
    if (action == false)
      this->_passwd = "";
    else
      this->_passwd = passwd;
  }

  // Topic methods and getters
  std::string getTopic() const { return (this->_topic); }

  void setTopic(const std::string &topic) { this->_topic = topic; }

  void setTopicRestricted(bool action) { this->_opTopicOnly = action; }

  bool isTopicOPOnly() {
    if (this->_opTopicOnly)
      return false;
    return true;
  }

  // User limit methods and getters
  size_t getUserLimit() const { return (this->_userLimit); }

  void setUserLimit(bool action, const size_t &userLimit) {
    this->_hasUserlimit = action;
    if (action == false)
      this->_userLimit = -1;
    else
      this->_userLimit = userLimit;
  }

  // Invite only methods and getters
  bool getIsInviteOnly() const { return (this->_isInviteOnly); }

  void setIsInviteOnly(bool action) { this->_isInviteOnly = action; }

private:
  map _clients;
  map _operators;

  std::string _topic;
  bool _opTopicOnly;

  std::string _passwd;
  bool _hasPasswd;

  size_t _userLimit;
  bool _hasUserlimit;

  bool _isInviteOnly;
};

std::ostream &operator<<(std::ostream &os, const Channel<sockaddr_in> &value);

#endif
