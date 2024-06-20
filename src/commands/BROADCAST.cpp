#include <ft_irc.hpp>

std::string BROADCAST(std::vector<std::string> args,
                      Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() != 2) {
    return "usage: BROADCAST channel message\n";
  }

  std::string channelName = args[0];
  std::string message = args[1];

  Channel<sockaddr_in> *channel = irc.getChannel(channelName);
  if (!channel)
    return (ERR_NOSUCHCHANNEL(irc.getClient(from_socket.getFd())->user.nickname,
                              channelName));

  channel->broadcast(*channel->getClient(from_socket), message + "\n");
  return "Message sent successfully";
}
