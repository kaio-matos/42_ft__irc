#include <ft_irc.hpp>

std::string NICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (!(args.size() == 1 || args.size() == 2)) {
    return "usage: NICK <nickname>\n";
  }
  std::string name = args[0];

  Client<sockaddr_in> *client = irc.getClient(from_socket.getFd());
  client->user.nickname = name;

  return "Nickname updated successfully to " + name + "\n";
}
