#include <ft_irc.hpp>

std::string KICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from || !from_socket._logged) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() != 2 && args.size() != 3) {
    return ERR_NEEDMOREPARAMS(from->user.nickname, "KICK");
  }

  std::string nickname = args[0];
  std::string channel_name = args[1];
  std::string comment;
  if (args.size() == 3) {
    comment = args[2];
  }

  Channel<sockaddr_in> *channel = irc.getChannel(channel_name);

  if (!channel) {
    return (ERR_NOSUCHCHANNEL(from->user.nickname, channel_name));
  }

  if (!channel->getClient(from->socket)) {
    return ERR_NOTONCHANNEL(from->user.nickname, channel->getChannelName());
  }

  if (!channel->isOperator(from)) {
    return ERR_CHANOPRIVSNEEDED(from->user.nickname, channel->getChannelName());
  }

  Client<sockaddr_in> *client = channel->getClient(nickname);

  if (!client) {
    return ERR_USERNOTINCHANNEL(from->user.nickname, nickname,
                                channel->getChannelName());
  }

  if (channel->isOperator(client)) {
    return ERR_CHANOPRIVSNEEDED(from->user.nickname, channel->getChannelName());
  }

  if (from == client) {
    return ERR_USERNOTINCHANNEL(from->user.nickname, nickname,
                                channel->getChannelName());
  }

  if (channel->disconnectClient(*client)) {
    std::string message;
    if (!comment.empty()) {
      message = MSG_KICK(from->user.identity(), channel->getChannelName(),
                         client->user.nickname, comment);
    } else {
      message = MSG_KICK(from->user.identity(), channel->getChannelName(),
                         client->user.nickname, "");
    }
    channel->broadcast(message);
    client->socket.write(message);
  }

  return "";
}
