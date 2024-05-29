#include <ft_irc.hpp>

void ctrl_c_handler(int s) {
  DebugLog << "Closing sockets";
  exit(1);
}

std::string onRequest(std::string request) {
  DebugLog << "---------------------------------------------";
  DebugLog << "Reading peer socket";
  DebugLog << request;

  std::string response = "response";

  DebugLog << "---------------------------------------------";
  return response;
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

  tcp_socket.poll(onRequest);

  tcp_socket.close();

  return 0;
}
