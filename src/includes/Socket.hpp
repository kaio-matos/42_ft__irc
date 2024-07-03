#ifndef SOCKET_H
#define SOCKET_H
#include <ft_irc.hpp>

template <typename T> class Socket {
public:
  Socket(void) {
    _domain = -1;
    _type = -1;
    _protocol = -1;
    _fd = -1;
    _addr = NULL;
    _pendingMessagesToWrite = std::queue<std::string>();

    _logged = false;
    _isOpen = false;
    _isListener = false;
    _isClosed = false;
    _isWritable = false;
    _isClient = true;
    _isServer = false;
  }

  Socket(int domain, int type, int protocol) {
    _domain = domain;
    _type = type;
    _protocol = protocol;
    _fd = socket(domain, type, protocol);
    _addr = NULL;
    _pendingMessagesToWrite = std::queue<std::string>();

    _isOpen = false;
    _isListener = false;
    _isClosed = false;
    _isWritable = false;
    _isClient = false;
    _isServer = true;

    _setSocketAsReusable(_fd);
    _setSocketAsNonBlocking(_fd);

    _sockets.push_back(this);
  }

  Socket(const Socket &value) { *this = value; }

  Socket &operator=(const Socket &value) {
    _domain = value._domain;
    _type = value._type;
    _protocol = value._protocol;
    _fd = value._fd;
    _addr = NULL;
    _pendingMessagesToWrite =
        std::queue<std::string>(value._pendingMessagesToWrite);
    if (value._addr != NULL) {
      _allocateAddr();
      memcpy(_addr, &value._addr, sizeof(value._addr));
    }

    _isOpen = value._isOpen;
    _isListener = value._isListener;
    _isClosed = value._isClosed;
    _isWritable = value._isWritable;
    _isClient = value._isClient;
    _isServer = value._isServer;
    return *this;
  }

  ~Socket(void) {
    // TODO: This was commented due to issues with std::vector, is there another
    // option?

    // close();
    delete _addr;
    _addr = NULL;
  }

  bool operator==(const Socket &value) const {
    return _domain == value._domain && _type == value._type &&
           _protocol == value._protocol && _fd == value._fd &&
           _addr == value._addr && _isOpen == value._isOpen &&
           _isListener == value._isListener &&
           _isWritable == value._isWritable && _isClosed == value._isClosed &&
           _isClient == value._isClient && _isServer == value._isServer;
  }

  // void bind(const struct sockaddr addr);
  void bind(const T addr) {
    _allocateAddr();
    // _addr->sin_family = addr.sin_family;
    // _addr->sin_addr.s_addr = addr.sin_addr.s_addr;
    // _addr->sin_port = addr.sin_port;
    memcpy(_addr, &addr, sizeof(addr));
    if (::bind(_fd, reinterpret_cast<struct sockaddr *>(_addr),
               sizeof(*_addr)) == -1) {
      throw std::runtime_error("Error while binding the socket");
    }
  }

  void connect(void) {}

  std::string read(std::string eof) {
    std::string result;
    int MAX_READ_BYTES = 100;
    char buff[MAX_READ_BYTES];

    int bytes = ::read(_fd, buff, MAX_READ_BYTES);
    if (bytes == -1) {
      throw std::runtime_error("Error while trying to read socket");
    }
    while (bytes) {
      std::string buffer(buff, bytes);
      result.append(buffer);

      if (buffer.substr(bytes - eof.length(), eof.length()) == eof) {
        break;
      }

      bytes = ::read(_fd, buff, MAX_READ_BYTES);
    }
    return result;
  }

  void write(std::string str) {
    if (!_isWritable) {
      _pendingMessagesToWrite.push(str);
      return;
    }
    ::write(_fd, str.c_str(), str.size());
  }

  void close(void) {
    if (_fd != -1) {
      if (::close(_fd) == -1) {
        Socket::_isServerRunning = false;
      }
    }
  }

  void listen(int n) {
    if (!_isServer) {
      throw std::runtime_error(
          "Error while trying to listen on a non server socket");
    }
    if (::listen(_fd, n) == -1) {
      throw std::runtime_error("Error while setting socket as listener");
    }
    _isListener = true;
  }

  void accept(Socket *peer_socket) {
    if (peer_socket->_addr != NULL) {
      return;
    }

    if (!_isListener) {
      throw std::runtime_error(
          "Socket is trying to accept messages without setting listener flag");
    }
    if (!_isServer) {
      throw std::runtime_error(
          "Error: trying to accept on a non server socket");
    }

    peer_socket->_allocateAddr();
    unsigned int peer_addr_size = sizeof(*peer_socket->_addr);

    int cfd = ::accept(_fd, reinterpret_cast<sockaddr *>(peer_socket->_addr),
                       &peer_addr_size);
    if (cfd == -1) {
      throw std::runtime_error("Error while trying to accept a request ");
    }

    _setSocketAsNonBlocking(cfd);
    DebugLog << BOLDGREEN << "New client accepted with fd [" << cfd << "]";
    peer_socket->_fd = cfd;
    _sockets.push_back(peer_socket);
  }

  template <typename Arg>
  void poll(std::string (*onRequest)(std::string, Socket<T> &, Arg &),
            void (*sendResponse)(Socket<T> &, Arg &), Arg &argument,
            std::string eof) {
    Socket::_isServerRunning = true;
    while (_isServerRunning) {
      std::vector<pollfd> fds;
      int timeout_ms = (5 * 60 * 1000);

      for (int i = 0; i < _sockets.size(); i++) {
        fds.push_back(
            (pollfd){.fd = _sockets[i]->getFd(), .events = POLLOUT | POLLIN});
      }

      int ret = ::poll(fds.data(), fds.size(), timeout_ms);
      if (ret < 0) {
        Socket::_isServerRunning = false;
        continue;
      } else if (ret == 0) {
        DebugLog << BOLDBLUE << "Poll timed out";
        continue;
      }

      for (int i = 0; i < fds.size(); i++) {
        Socket<struct sockaddr_in> *peer_socket;

        bool canWrite = fds[i].revents & POLLOUT;
        bool canRead = fds[i].revents & POLLIN;

        if (*this == *_sockets[i]) {
          if (canRead) {
            peer_socket = new Socket<struct sockaddr_in>();
            accept(peer_socket);
          }
          continue;
        } else {
          peer_socket = _sockets[i];
        }

        if (canWrite) {
          peer_socket->_isWritable = true;
          peer_socket->_flushPendingMessages();
          sendResponse(*peer_socket, argument);
        } else {
          peer_socket->_isWritable = false;
        }

        if (canRead) {
          std::string request = peer_socket->read(eof);
          std::string response = onRequest(request, *peer_socket, argument);
          peer_socket->write(response);
        }
      }
    }
    Socket::_cleanup();
  }

  static void cleanup() { Socket::_isServerRunning = false; }

  bool isEmpty() const { return (_fd == -1 && _addr == NULL); }
  bool isOpen() const { return _isOpen; }
  bool isListener() const { return _isListener; }
  bool isClosed() const { return _isClosed; }
  bool isWritable() const { return _isWritable; }

  bool isClient() const { return _isClient; }
  bool isServer() const { return _isServer; }

  int getFd() const { return _fd; }
  int getDomain() const { return _domain; }
  int getType() const { return _type; }
  int getProtocol() const { return _protocol; }
  std::queue<std::string> getPendingMessagesToWrite() const {
    return _pendingMessagesToWrite;
  }
  T getRawAddr() const {
    if (_addr == NULL)
      throw std::runtime_error("Addr not available");
    return *_addr;
  }
  bool _logged;

private:
  static std::vector<Socket<T> *> _sockets;
  static bool _isServerRunning;
  std::queue<std::string> _pendingMessagesToWrite;

  int _fd;
  int _domain;
  int _type;
  int _protocol;
  T *_addr;

  bool _isOpen;
  bool _isListener;
  bool _isClosed;
  bool _isClient;
  bool _isServer;
  bool _isWritable;

  void _allocateAddr(void) {
    if (_addr == NULL) {
      _addr = new T;
    }
  }

  void _setSocketAsReusable(int fd) {
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
      throw std::runtime_error("Error while setting socket as reusable");
    }
  }

  void _setSocketAsNonBlocking(int fd) {
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
      throw std::runtime_error("Error while setting socket as non-blocking");
    }
  }

  void _flushPendingMessages() {
    while (!_pendingMessagesToWrite.empty()) {
      write(_pendingMessagesToWrite.front());
      _pendingMessagesToWrite.pop();
    }
  }

  void _cleanup() {
    typename std::vector<Socket<T> *>::iterator it;

    for (it = Socket::_sockets.begin(); it != Socket::_sockets.end(); it++) {
      Socket<T> *socket = *it;
      socket->close();
      if (!socket->isServer()) { // work around because the server socket is
        // not allocated, so we cant delete it
        delete socket;
      }
    }

    Socket::_sockets.clear();
  }
};
template <typename T>
std::vector<Socket<T> *> Socket<T>::_sockets = std::vector<Socket<T> *>();
template <typename T> bool Socket<T>::_isServerRunning = false;

#endif
