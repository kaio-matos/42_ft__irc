#include <ft_irc.hpp>

std::string KICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (args.size() != 2 && args.size() != 3) {
    return ERR_NEEDMOREPARAMS(from->user.nickname, "KICK");
  }

  std::string nickname = args[0];
  std::string channel_name = args[1];
  std::string comment;
  if (args.size() == 3) {
    comment = args[2];
  }

  DebugLog << "hello";

  Channel<sockaddr_in> *channel = irc.getChannel(channel_name);

  if (!channel) {
    return (ERR_NOSUCHCHANNEL(from->user.nickname, channel_name));
  }

  if (!channel->getClient(from->socket)) {
    return ERR_NOTONCHANNEL(from->user.nickname, channel->getTopic());
  }

  if (false) { // TODO:
    return ERR_CHANOPRIVSNEEDED(from->user.nickname, channel->getTopic());
  }

  Client<sockaddr_in> *client = channel->getClient(nickname);

  if (!client) {
    return ERR_USERNOTINCHANNEL(client->user.nickname, nickname,
                                channel->getTopic());
  }

  if (channel->disconnectClient(*client)) {
    if (!comment.empty()) {
      channel->broadcast(client->user.nickname + " got kicked because " +
                         comment);
    } else {
      channel->broadcast(client->user.nickname + " got kicked\r\n");
    }
  }

  // TODO: check if we can send this
  client->socket.write("You got kicked from " + channel->getTopic() + "\n");
  return "";
}
