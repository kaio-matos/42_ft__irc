#include <ft_irc.hpp>

std::string INVITE(std::vector<std::string> args,
                   Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  return "response from invite\b";
}
