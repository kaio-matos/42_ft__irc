#include <ft_irc.hpp>

std::string PING(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() != 1) {
    return ERR_NEEDMOREPARAMS("User", "PING");
  }

  std::string token = args[0];

  return MSG_PONG(SERVER_NAME, token);
}
