#include <ft_irc.hpp>

bool PROGRAM_EXECUTED_SIGINT = false;
bool PROGRAM_EXECUTED_SIGPIPE = false;

void sigint_handler(int s) {
  (void)s;
  PROGRAM_EXECUTED_SIGINT = true;
}
void sigpipe_handler(int s) {
  (void)s;
  PROGRAM_EXECUTED_SIGPIPE = true;
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
  signal(SIGINT, sigint_handler);
  signal(SIGPIPE, sigpipe_handler);

  try {
    IRC<sockaddr_in> irc(clientArgs);

    Server<sockaddr_in, IRC<sockaddr_in> > app(
        "0.0.0.0", clientArgs.port, onRequest, onDisconnect, irc, "\n");

    app.start();

  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << std::endl;
  }

  return 0;
}
