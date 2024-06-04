#include <ft_irc.hpp>

std::string BROADCAST(std::vector<std::string> args,
                      Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() != 2) {
    return "usage: BROADCAST channel message\n";
  }

  std::string channel = args[0];
  std::string message = args[1];

  if (channel == "games") {
    Channel<sockaddr_in> c = GamesChannel<sockaddr_in>();
    c.broadcast(c.getClient(from_socket), message + "\n");
    return "Message sent successfully";
  }

  return "Channel not found";
}
