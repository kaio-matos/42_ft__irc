#include <ft_irc.hpp>

void ctrl_c_handler(int s) { Socket<sockaddr_in>::cleanup(); }

void sigpipe_handler(int s) {
  // TODO: handle with broken pipes (if the client connection is closed
  // unexpectedly)
  DebugLog << "Broken pipe";
  exit(1);
}

static bool parseArguments(int argc, char **argv, ClientArgs &args) {
  int port;
  std::stringstream ss;

  if (argc != 3) {
    std::cerr << "Error: missing arguments." << std::endl;
    return (false);
  }
  ss.str(argv[1]);
  if (!(ss >> port) || ss.fail() || !ss.eof()) {
    std::cerr << "Error: invalid argument: " << argv[1] << std::endl;
    return (false);
  }
  args.port = port;
  if (std::strlen(argv[2]) < 1) {
    std::cerr << "Error: invalid argument: " << argv[2] << std::endl;
    return (false);
  }
  args.password = std::string(argv[2]);
  return (true);
}

template <typename T>
std::string onRequest(std::string request, Socket<T> &from_socket,
                      IRC<T> &irc) {
  if (request.empty())
    return "";
  DebugLog << "---------------------------------------------";
  DebugLog << request;

  request.erase(std::remove(request.begin(), request.end(), '\n'),
                request.end());
  std::vector<std::string> args = split(trim(request));
  std::string command = args[0];
  args.erase(args.begin());
  if (command == "USER")
    return USER(args, from_socket, irc);

  if (command == "KICK")
    return KICK(args, from_socket, irc);

  if (command == "NICK")
    return NICK(args, from_socket, irc);

  if (command == "INVITE")
    return INVITE(args, from_socket, irc);

  if (command == "TOPIC")
    return TOPIC(args, from_socket, irc);

  if (command == "JOIN")
    return JOIN(args, from_socket, irc);

  if (command == "MODE")
    return MODE(args, from_socket, irc);

  if (command == "PRIVMSG")
    return PRIVMSG(args, from_socket, irc);

  if (command == "QUIT")
    return QUIT(args, from_socket, irc);

  if (command == "PING")
    return PING(args, from_socket, irc);

  if (command == "PART")
    return PART(args, from_socket, irc);

  if (command == "PASS")
    return PASS(args, from_socket, irc);

  DebugLog << "---------------------------------------------";
  return "";
}

template <typename T> void sendResponse(Socket<T> &to_socket, IRC<T> &irc) {}

template <typename T> void onDisconnect(Socket<T> &from_socket, IRC<T> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());
  std::vector<std::string> args;

  if (from) {
    args.push_back(":User " + from->user.nickname + " was disconnected");
    QUIT(args, from_socket, irc);
    return;
  }
}

int main(int argc, char **argv) {

  ClientArgs clientArgs;

  if (!parseArguments(argc, argv, clientArgs)) {
    std::cerr << "Usage: " << argv[0] << " PORT PASSWORD" << std::endl;
    return (1);
  }

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
  addr.sin_port = htons(clientArgs.port);

  tcp_socket.bind(addr);
  tcp_socket.listen(5);

  DebugLog << "Listening on:\n" << tcp_socket;
  IRC<sockaddr_in> irc(clientArgs);

  tcp_socket.poll(onRequest, sendResponse, onDisconnect, irc, "\n");

  tcp_socket.close();

  return 0;
}
