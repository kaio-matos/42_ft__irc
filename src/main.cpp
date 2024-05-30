#include <ft_irc.hpp>

void ctrl_c_handler(int s) {
  DebugLog << "Closing sockets";
  exit(1);
}

std::string USER(std::vector<std::string> args) {
  User createdUser(args[0], args[1]);

  return "User identified successfully as " + createdUser.nickname + " (" +
         createdUser.username + ")\n";
}
std::string KICK(std::vector<std::string> args) {
  std::string name = args[0];

  std::string response = std::string("Kicking: ");

  return response + name + "\n";
}
std::string INVITE(std::vector<std::string> args) {
  return "response from invite\b";
}
std::string TOPIC(std::vector<std::string> args) {
  return "response from topic\n";
}
std::string MODE(std::vector<std::string> args) {
  return "response from mode\n";
}

template <typename T>
std::string onRequest(std::string request, Socket<T> &from_socket) {
  DebugLog << "---------------------------------------------";
  DebugLog << request;

  request.erase(std::remove(request.begin(), request.end(), '\n'),
                request.end());
  std::vector<std::string> args = split(request);
  std::string command = args[0];
  args.erase(args.begin());

  if (command == "USER")
    return USER(args);

  if (command == "KICK")
    return KICK(args);

  if (command == "INVITE")
    return INVITE(args);

  if (command == "TOPIC")
    return TOPIC(args);

  if (command == "MODE")
    return MODE(args);

  DebugLog << "---------------------------------------------";
  return "Error: command not found\n";
}

int main() {
  DebugLog << BOLDCYAN
           << "-------------------------- Starting ft_irc "
              "--------------------------";
  signal(SIGINT, ctrl_c_handler);
  // signal(SIGABRT, ctrl_c_handler);
  Socket<struct sockaddr_in> tcp_socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = stringAddressToBytes("0.0.0.0");
  addr.sin_port = htons(8080);

  tcp_socket.bind(addr);
  tcp_socket.listen(5);

  DebugLog << "Listening on:\n" << tcp_socket;

  tcp_socket.poll(onRequest, "\n");

  tcp_socket.close();

  return 0;
}
