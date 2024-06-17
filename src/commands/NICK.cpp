#include <ft_irc.hpp>

std::string NICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (!(args.size() == 1 || args.size() == 2)) {
    return "usage: NICK <nickname>\n";
  }
  std::string name = args[0];

  std::map<int, Client<sockaddr_in> >::iterator clientIt =
      irc.clients.find(from_socket.getFd());
  clientIt->second.user.nickname = name;

  return "Nickname updated successfully to " + name + "\n";
}
