#ifndef SERVER_H
#define SERVER_H
#include <ft_irc.hpp>

template <typename T, typename Arg> class Server {

public:
  Server(std::string address, int port,
         std::string (*onRequest)(std::string, Socket<T> &, Arg &),
         void (*onDisconnect)(Socket<T> &, Arg &), Arg &argument,
         std::string eof)
      : _onRequest(onRequest), _onDisconnect(onDisconnect), _argument(argument),
        _eof(eof) {
    _server_socket = NULL;
    try {
      _server_socket = new Socket<struct sockaddr_in>(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = stringAddressToBytes(address);
      addr.sin_port = htons(port);
      _server_socket->bind(addr);
      _server_socket->listen(5);

      _sockets = std::map<int, Socket<sockaddr_in> *>();
      _sockets.insert(std::make_pair(_server_socket->getFd(), _server_socket));

      _isServerRunning = false;
    } catch (const std::exception &error) {
      delete _server_socket;
      throw std::runtime_error(error.what());
    }
  }
  ~Server() {
    typename std::map<int, Socket<T> *>::iterator it;

    for (it = _sockets.begin(); it != _sockets.end(); it++) {
      Socket<sockaddr_in> *socket = it->second;
      socket->close();
      delete socket;
    }

    _sockets.clear();
  }

  void start() {
    DebugLog << "Starting Server on socket: \n" << *_server_socket;
    _isServerRunning = true;

    while (_isServerRunning) {
      if (PROGRAM_EXECUTED_SIGINT) {
        stop();
        break;
      }

      std::vector<pollfd> fds = _getPollFds();

      int length = poll(fds.data(), fds.size(), 5 * 60 * 1000);
      if (length < 0) {
        _isServerRunning = false;
        continue;
      }
      if (length == 0) {
        DebugLog << BOLDBLUE << "Poll timed out";
        continue;
      }

      for (size_t i = 0; i < fds.size(); i++) {
        int fd = fds[i].fd;
        bool canWrite = fds[i].revents & POLLOUT;
        bool canRead = fds[i].revents & POLLIN;

        if (_sockets[fd]->isServer()) {
          if (canRead) {
            Socket<struct sockaddr_in> *new_socket = _server_socket->accept();
            _sockets.insert(std::make_pair(new_socket->getFd(), new_socket));
            DebugLog << BOLDGREEN << "New client accepted with fd ["
                     << new_socket->getFd() << "]";
          }
          continue;
        }

        Socket<struct sockaddr_in> *peer_socket = _sockets[fd];

        if (peer_socket->isPendingRemoval()) {
          deleteSocket(*peer_socket);
          continue;
        }

        if (canWrite) {
          peer_socket->isWritable = true;
          _handleWritableSocket(peer_socket);
        } else {
          peer_socket->isWritable = false;
        }

        if (canRead) {
          _handleReadableSocket(peer_socket);
        }
      }
    }
  };

  void stop() { _isServerRunning = false; }

  void deleteSocket(Socket<sockaddr_in> &socket) {
    try {
      int fd = socket.getFd();
      Socket<sockaddr_in> *found = _sockets.at(fd);
      found->close();
      delete found;
      _sockets.erase(fd);
    } catch (const std::exception &error) {
    }
  }

private:
  bool _isServerRunning;
  std::map<int, Socket<sockaddr_in> *> _sockets;
  Socket<sockaddr_in> *_server_socket;
  std::string (*_onRequest)(std::string, Socket<T> &, Arg &);
  void (*_onDisconnect)(Socket<T> &, Arg &);
  Arg &_argument;
  std::string _eof;

  void _handleWritableSocket(Socket<sockaddr_in> *peer_socket) {
    peer_socket->flushPendingMessages();
  }

  void _handleReadableSocket(Socket<sockaddr_in> *peer_socket) {
    std::string request = peer_socket->read(_eof);
    bool hasDisconnected = request.empty();

    if (hasDisconnected) {
      DebugLog << RED << "Disconnecting fd [" << peer_socket->getFd() << "]";
      _onDisconnect(*peer_socket, _argument);
      peer_socket->erase();
      return;
    }

    std::vector<std::string> commands = split(request, "\r\n");
    for (size_t i = 0; i < commands.size(); i++) {
      std::string response = _onRequest(commands[i], *peer_socket, _argument);
      peer_socket->write(response);
    }
  }

  std::vector<pollfd> _getPollFds() {
    typename std::map<int, Socket<T> *>::iterator it;
    std::vector<pollfd> fds;

    for (it = _sockets.begin(); it != _sockets.end(); it++) {
      fds.push_back((pollfd){
          .fd = it->second->getFd(), .events = POLLOUT | POLLIN, .revents = 0});
    }

    return fds;
  }
};

#endif
