#include <ft_irc.hpp>

std::string QUIT(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from || !from_socket._logged) {
    return ERR_NOTREGISTERED;
  }

  std::string message;

  for (int i = 0; i < args.size(); i++) {
    if (i != 0)
      message.append(" ");
    message.append(args[i]);
  }

  if (message.empty()) {
    irc.broadcastToConnectedChannels(*from,
                                     MSG_QUIT(from->user.identity(), ""));
  } else {
    irc.broadcastToConnectedChannels(*from,
                                     MSG_QUIT(from->user.identity(), message));
  }
  irc.disconnectClient(*from);

  return "";
}
