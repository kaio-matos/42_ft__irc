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
    _pendingMessageToRead = std::string();

    _logged = false;
    _isOpen = false;
    _isListener = false;
    _isClosed = false;
    isWritable = false;
    _isClient = true;
    _isServer = false;
    _pendingRemoval = false;
  }

  Socket(int domain, int type, int protocol) {
    _domain = domain;
    _type = type;
    _protocol = protocol;
    _fd = socket(domain, type, protocol);
    _addr = NULL;
    _pendingMessagesToWrite = std::queue<std::string>();
    _pendingMessageToRead = std::string();

    _isOpen = false;
    _isListener = false;
    _isClosed = false;
    isWritable = false;
    _isClient = false;
    _isServer = true;
    _pendingRemoval = false;

    _setSocketAsReusable(_fd);
    _setSocketAsNonBlocking(_fd);
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
    _pendingMessageToRead = value._pendingMessageToRead;
    if (value._addr != NULL) {
      _allocateAddr();
      memcpy(_addr, &value._addr, sizeof(value._addr));
    }

    _isOpen = value._isOpen;
    _isListener = value._isListener;
    _isClosed = value._isClosed;
    isWritable = value.isWritable;
    _isClient = value._isClient;
    _isServer = value._isServer;

    _pendingRemoval = value._pendingRemoval;

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
           _isListener == value._isListener && isWritable == value.isWritable &&
           _isClosed == value._isClosed && _isClient == value._isClient &&
           _isServer == value._isServer;
  }

  void bind(const T addr) {
    _allocateAddr();
    memcpy(_addr, &addr, sizeof(addr));
    if (::bind(_fd, reinterpret_cast<struct sockaddr *>(_addr),
               sizeof(*_addr)) == -1) {
      delete _addr;
      _addr = NULL;
      throw std::runtime_error("Error while binding the socket");
    }
  }

  std::string read(std::string eof) {
    std::string result = _pendingMessageToRead;
    int MAX_READ_BYTES = 100;
    int MAX_TRIES = 1;
    int tries = 0;
    char buff[MAX_READ_BYTES];
    bool hasReachedEOF = false;

    std::memset(buff, 0, MAX_READ_BYTES);
    int bytes = ::read(_fd, buff, MAX_READ_BYTES);

    if (bytes == -1) {
      throw std::runtime_error("Error while trying to read socket");
    }
    while (!hasReachedEOF && tries <= MAX_TRIES) {
      if (bytes == 0)
        return "";
      if (bytes != -1) {
        std::string buffer(buff, bytes);
        result.append(buffer);

        hasReachedEOF =
            buffer.substr(bytes - eof.length(), eof.length()) == eof;
      }

      if (hasReachedEOF) {
        break;
      }

      bytes = ::read(_fd, buff, MAX_READ_BYTES);
      if (bytes == -1) {
        tries++;
      }
    }

    if (hasReachedEOF) {
      _pendingMessageToRead = "";
      return result;
    }
    _pendingMessageToRead = result;
    return "\r\n";
  }

  void write(std::string str) {
    if (!isWritable) {
      _pendingMessagesToWrite.push(str);
      return;
    }
    ::write(_fd, str.c_str(), str.size());
  }

  void close(void) {
    if (_fd != -1) {
      if (::close(_fd) == -1) {
        // Socket::_isServerRunning = false;
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

  Socket<T> *accept(void) {
    Socket<T> *peer_socket = new Socket<struct sockaddr_in>();

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

    peer_socket->_fd = ::accept(
        _fd, reinterpret_cast<sockaddr *>(peer_socket->_addr), &peer_addr_size);
    if (peer_socket->_fd == -1) {
      delete peer_socket;
      return NULL;
    }

    _setSocketAsNonBlocking(peer_socket->_fd);
    return peer_socket;
  }

  void flushPendingMessages() {
    while (!_pendingMessagesToWrite.empty()) {
      write(_pendingMessagesToWrite.front());
      _pendingMessagesToWrite.pop();
    }
  }

  void erase() { _pendingRemoval = true; };

  bool isEmpty() const { return (_fd == -1 && _addr == NULL); }
  bool isOpen() const { return _isOpen; }
  bool isListener() const { return _isListener; }
  bool isClosed() const { return _isClosed; }
  bool isPendingRemoval() const { return _pendingRemoval; }
  bool isWritable;

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
  std::queue<std::string> _pendingMessagesToWrite;
  std::string _pendingMessageToRead;

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
  bool _pendingRemoval;

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
};

#endif
