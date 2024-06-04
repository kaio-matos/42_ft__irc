#include <ft_irc.hpp>

std::string KICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  std::string name = args[0];

  std::string response = std::string("Kicking: ");

  return response + name + "\n";
}
