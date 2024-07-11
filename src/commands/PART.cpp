#include "ft_irc.hpp"

std::string PART(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!irc.isRegistered(from)) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() < 1) {
    return ERR_NEEDMOREPARAMS(from->user.nickname, "PART");
  }

  std::vector<std::string> channels = splitByComma(args[0]);
  std::string reason;

  for (size_t i = 1; i < args.size(); i++) {
    if (i != 1)
      reason.append(" ");
    reason.append(args[i]);
  }

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

    std::string message;
    if (reason.empty()) {
      message = MSG_PART(from->user.identity(), channel->getChannelName());
      channel->broadcast(message);
      from->socket.write(message);
    } else {
      message = MSG_PARTREASON(from->user.identity(), channel->getChannelName(),
                               reason);
      channel->broadcast(message);
      from->socket.write(message);
    }
  }

  return "";
}
