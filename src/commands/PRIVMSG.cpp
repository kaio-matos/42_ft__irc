#include <ft_irc.hpp>

// https://modern.ircdocs.horse/#privmsg-message
std::string PRIVMSG(std::vector<std::string> args,
                    Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from || !from_socket._logged) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() < 1) {
    return ERR_NORECIPIENT(from->user.nickname, "PRIVMSG");
  }

  if (args.size() < 2) {
    return ERR_NOTEXTTOSEND(from->user.nickname);
  }

  std::vector<std::string> targets = splitByComma(args[0]);
  std::string message;

  for (size_t i = 1; i < args.size(); i++) {
    if (i != 1)
      message.append(" ");
    message.append(args[i]);
  }

  for (std::vector<std::string>::iterator it = targets.begin();
       it != targets.end(); it++) {
    std::string target_nickname_or_channel = *it;

    Channel<sockaddr_in> *target_channel =
        irc.getChannel(target_nickname_or_channel);
    Client<sockaddr_in> *target_client =
        irc.getClient(target_nickname_or_channel);

    if (target_channel) {
      if (!target_channel->getClient(from->socket)) {
        from->socket.write(ERR_NOTONCHANNEL(from->user.nickname,
                                            target_channel->getChannelName()));
        continue;
      }

      target_channel->broadcast(
          *from, MSG_PRIVMSG(from->user.nickname,
                             target_channel->getChannelName(), message));
      continue;
    }

    if (target_client) {
      target_client->socket.write(MSG_PRIVMSG(
          from->user.nickname, target_client->user.nickname, message));
      continue;
    }
    from->socket.write(
        ERR_NOSUCHNICK(from->user.nickname, target_nickname_or_channel));
  }

  return "";
}
