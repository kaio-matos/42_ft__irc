#include <ft_irc.hpp>

// https://modern.ircdocs.horse/#privmsg-message
std::string PRIVMSG(std::vector<std::string> args,
                    Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() < 2) {
    return ERR_NEEDMOREPARAMS(from->user.nickname, "PRIVMSG");
  }

  std::string target_nickname_or_channel = args[0];
  std::string message = args[1];

  for (int i = 2; i < args.size(); i++) {
    message.append(" ");
    message.append(args[i]);
  }

  Channel<sockaddr_in> *target_channel =
      irc.getChannel(target_nickname_or_channel);
  Client<sockaddr_in> *target_client =
      irc.getClient(target_nickname_or_channel);

  if (false) { // TODO:
    return ERR_NOSUCHSERVER(from->user.nickname, "servername");
  }

  if (false) { // TODO:
    return ERR_CANNOTSENDTOCHAN(from->user.nickname,
                                target_channel->getTopic());
  }

  if (false) { // TODO:
    return ERR_NORECIPIENT(from->user.nickname, "PRIVMSG");
  }

  if (false) { // TODO:
    return ERR_NOTEXTTOSEND(from->user.nickname);
  }

  if (target_channel) {
    target_channel->broadcast(*from, ":" + from->user.nickname + " PRIVMSG " +
                                         target_channel->getTopic() + " " +
                                         message);
    return "";
  }

  if (target_client) {
    if (false) { // TODO:
      return RPL_AWAY(from->user.nickname, target_client->user.nickname,
                      target_client->awayMessage);
    }

    target_client->socket.write(":" + from->user.nickname + " PRIVMSG " +
                                target_client->user.nickname + " " + message +
                                "\r\n");
    return "";
  }

  return ERR_NOSUCHNICK(from->user.nickname, target_nickname_or_channel);
}
