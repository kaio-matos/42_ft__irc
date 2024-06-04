#include "IRC.hpp"
#include <ft_irc.hpp>

template <typename T> Channel<T> &GamesChannel(void) {
  static Channel<T> *createdChannel = new Channel<T>("Games");
  return *createdChannel;
}

void ctrl_c_handler(int s) {
  DebugLog << "Closing sockets";
  exit(1);
}

void sigpipe_handler(int s) {
  // TODO: handle with broken pipes (if the client connection is closed
  // unexpectedly)
  DebugLog << "Broken pipe";
  exit(1);
}

template <typename T>
std::string USER(std::vector<std::string> args, Socket<T> &from_socket,
                 IRC<T> &irc) {
  if (args.size() != 2) {
    return "usage: USER username nickname\n";
  }

  User createdUser(args[0], args[1]);
  Client<T> createdClient(createdUser, from_socket);

  irc.addClient(createdClient);

  GamesChannel<T>().connectClient(createdClient);

  return "User identified successfully as " + createdUser.nickname + " (" +
         createdUser.username + ")\n";
}

template <typename T>
std::string KICK(std::vector<std::string> args, Socket<T> &from_socket,
                 IRC<T> &irc) {
  std::string name = args[0];

  std::string response = std::string("Kicking: ");

  return response + name + "\n";
}

template <typename T>
std::string INVITE(std::vector<std::string> args, Socket<T> &from_socket,
                   IRC<T> &irc) {
  return "response from invite\b";
}

template <typename T>
std::string TOPIC(std::vector<std::string> args, Socket<T> &from_socket,
                  IRC<T> &irc) {
  return "response from topic\n";
}

template <typename T>
std::string MODE(std::vector<std::string> args, Socket<T> &from_socket,
                 IRC<T> &irc) {
  return "response from mode\n";
}

template <typename T>
std::string BROADCAST(std::vector<std::string> args, Socket<T> &from_socket,
                      IRC<T> &irc) {
  if (args.size() != 2) {
    return "usage: BROADCAST channel message\n";
  }

  std::string channel = args[0];
  std::string message = args[1];

  if (channel == "games") {
    Channel<T> c = GamesChannel<T>();
    c.broadcast(c.getClient(from_socket), message + "\n");
    return "Message sent successfully";
  }

  return "Channel not found";
}

template <typename T>
std::string onRequest(std::string request, Socket<T> &from_socket,
                      IRC<T> &irc) {
  DebugLog << "---------------------------------------------";
  DebugLog << request;

  request.erase(std::remove(request.begin(), request.end(), '\n'),
                request.end());
  std::vector<std::string> args = split(request);
  std::string command = args[0];
  args.erase(args.begin());

  if (command == "USER")
    return USER(args, from_socket, irc);

  if (command == "KICK")
    return KICK(args, from_socket, irc);

  if (command == "INVITE")
    return INVITE(args, from_socket, irc);

  if (command == "TOPIC")
    return TOPIC(args, from_socket, irc);

  if (command == "MODE")
    return MODE(args, from_socket, irc);

  if (command == "BROADCAST")
    return BROADCAST(args, from_socket, irc);

  DebugLog << "---------------------------------------------";
  return "Error: command not found\n";
}

template <typename T> void sendResponse(Socket<T> &to_socket, IRC<T> &irc) {}

int main() {
  DebugLog << BOLDCYAN
           << "-------------------------- Starting ft_irc "
              "--------------------------";
  signal(SIGINT, ctrl_c_handler);
  signal(SIGPIPE, sigpipe_handler);
  // signal(SIGABRT, ctrl_c_handler);
  Socket<struct sockaddr_in> tcp_socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = stringAddressToBytes("0.0.0.0");
  addr.sin_port = htons(8080);

  tcp_socket.bind(addr);
  tcp_socket.listen(5);

  DebugLog << "Listening on:\n" << tcp_socket;
  IRC<sockaddr_in> irc;

  tcp_socket.poll(onRequest, sendResponse, irc, "\n");

  tcp_socket.close();

  return 0;
}
