#include <ft_irc.hpp>

std::string INVITE(std::vector<std::string> args,
                   Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() < 2)
    return ERR_NEEDMOREPARAMS(from->user.nickname, "INVITE");
  std::string nickname = args[0];
  std::string channel_name = args[1];

  Channel<sockaddr_in> *to_channel = irc.getChannel(channel_name);

  if (!to_channel) {
    return ERR_NOSUCHCHANNEL(from->user.nickname, channel_name);
  }

  from = to_channel->getClient(from_socket);

  if (!from) {
    return ERR_NOTONCHANNEL(from->user.nickname, to_channel->getChannelName());
  }

  Client<sockaddr_in> *invited = irc.getClient(nickname);

  if (!invited) {
    return ERR_NOSUCHNICK(
        nickname,
        nickname); // TODO: Check if we can use this error for this command
  }

  if (to_channel->isOperator(*from) && to_channel->isInviteOnly()) {
    return ERR_CHANOPRIVSNEEDED(from->user.nickname,
                                to_channel->getChannelName());
  }

  if (to_channel->getClient(nickname)) {
    return ERR_USERONCHANNEL(from->user.nickname, invited->user.nickname,
                             to_channel->getChannelName());
  }

  invited->socket.write(":" + from->user.getIdentity() + " INVITE " +
                        invited->user.nickname + " " +
                        to_channel->getChannelName() + "\r\n");

  invited->sendInvite(to_channel->getChannelName());

  return RPL_INVITING(from->user.nickname, invited->user.nickname,
                      to_channel->getChannelName());
}
