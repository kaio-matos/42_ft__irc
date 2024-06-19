#include <ft_irc.hpp>

// https://modern.ircdocs.horse/#privmsg-message
std::string PRIVMSG(std::vector<std::string> args,
                    Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() < 2) {
    return "usage: PRIVMSG <target> <text to be sent>";
  }
  Client<sockaddr_in> *client = irc.getClient(from_socket.getFd());

  std::string target_nickname_or_channel = args[0];
  std::string message = std::string(args[1]);

  message.erase(
      message.begin()); // delete ':' from the beggining of the message
  for (int i = 2; i < args.size(); i++) {
    message.append(" ");
    message.append(args[i]);
  }

  Channel<sockaddr_in> *target_channel =
      irc.getChannel(target_nickname_or_channel);
  Client<sockaddr_in> *target_client =
      irc.getClient(target_nickname_or_channel);

  if (target_channel) {
    target_channel->broadcast(*client,
                              ":" + client->user.nickname + " PRIVMSG " +
                                  target_channel->getTopic() + " :" + message);
    return "\n";
  }

  if (target_client) {
    target_client->socket.write(":" + client->user.nickname +
                                " PRIVMSG :" + message);
    return "\n";
  }

  return "No such nick/channel\n";
}
