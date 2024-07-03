#include "ft_irc.hpp"

std::string PART(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from || !from_socket._logged) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() < 1) {
    return ERR_NEEDMOREPARAMS(from->user.nickname, "PART");
  }

  std::vector<std::string> channels = splitByComma(args[0]);
  std::string reason = args[1];

  for (std::vector<std::string>::iterator it = channels.begin();
       it != channels.end(); it++) {
    std::string channel_name = *it;

    Channel<sockaddr_in> *channel = irc.getChannel(channel_name);

    if (!channel) {
      DebugLog << ERR_NOSUCHCHANNEL(from->user.nickname, channel_name);
      from->socket.write(ERR_NOSUCHCHANNEL(from->user.nickname, channel_name));
      continue;
    }

    if (!channel->getClient(from->socket)) {
      DebugLog << ERR_NOTONCHANNEL(from->user.nickname,
                                   channel->getChannelName());
      from->socket.write(
          ERR_NOTONCHANNEL(from->user.nickname, channel->getChannelName()));
      continue;
    }

    channel->disconnectClient(*from);

    from->socket.write(
        MSG_PART(from->user.identity(), channel->getChannelName()));
  }

  return "";
}
