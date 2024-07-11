#include <ft_irc.hpp>

std::string QUIT(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!irc.isRegistered(from)) {
    return ERR_NOTREGISTERED;
  }

  std::string message;

  for (size_t i = 0; i < args.size(); i++) {
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
