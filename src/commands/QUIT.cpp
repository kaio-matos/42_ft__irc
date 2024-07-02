#include <ft_irc.hpp>

std::string QUIT(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from || !from_socket._logged) {
    return ERR_NOTREGISTERED;
  }

  std::string message = args[0];

  if (message.empty()) {
    irc.broadcast(*from, from->user.nickname + " is exiting the network");
  }

  irc.broadcast(*from, from->user.nickname +
                           " is exiting the network the message: \"Quit: " +
                           message + "\"");
  irc.disconnectClient(*from);
  return "";
}
