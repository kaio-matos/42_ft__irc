#include <ft_irc.hpp>

std::string JOIN(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  // typedef std::map<int, Client<sockaddr_in> >::iterator ClientIterator;
  // typedef std::map<std::string, Channel<sockaddr_in> >::iterator
  // ChannelIterator;

  // ClientIterator client_it = irc.clients.find(from_socket.getFd());

  Client<sockaddr_in> *client = irc.getClient(from_socket.getFd());
  std::string nick = client->user.nickname;
  std::string user = client->user.username;

  if (!client) {
    return ERR_NOTREGISTERED;
  }
  std::vector<std::string> channels = splitByComma(args[0]);
  std::vector<std::string> key;
  if (key.size() > 1)
    key = splitByComma(key[1]);

  std::string reply;
  if (key.size() != channels.size() && !key.empty())
    return ERR_NEEDMOREPARAMS(user, "JOIN");

  /*
  //no irc se der "join 0" ele desconecta de todos os canais
  if (channels[0] == "0") {
      for (ChannelIterator channel_it = irc.channels.begin(); channel_it !=
  irc.channels.end(); ++channel_it) {
          channel_it->second.disconnectClient(client);
      }
      return "";
  }*/

  for (size_t i = 0; i < channels.size(); i++) {
    std::string channelName = channels[i];
    std::string channelKey;
    if (!key.empty())
      channelKey = key[i];

    if (channelName[0] != '#' && channelName[0] != '&') {
      reply += ERR_NOSUCHCHANNEL(user, channelName);
      continue;
    }

    Channel<sockaddr_in> *channel = irc.getChannel(channelName);
    if (!channel) {
      std::cout << "Bp 1\n" << std::endl;
      irc.addChannel(Channel<sockaddr_in>(channelName));
      std::cout << "Bp 2\n" << std::endl;
      channel = irc.getChannel(channelName);
      std::cout << "Bp 3 " << channelName << "\n" << std::endl;
      // channel->addOperator(*client); // Adiciona o cliente como operador
      std::cout << "Bp 4\n" << std::endl;
    }
    std::cout << "Bp 5\n" << std::endl;

    /*
    if (channel->getIsInviteOnly() && !client.channelOnInviteList(channelName))
    { reply += ERR_INVITEONLYCHAN(user, channelName); continue;
    }
    */

    //if (channel->isClientInChannel(*client))
      //continue;
    std::cout << "Bp 6\n" << std::endl;
    if (!channelKey.empty() && channel->getPasswd() != channelKey) {
      reply += ERR_BADCHANNELKEY(user, channelName);
      continue;
    }
    std::cout << "Bp 7\n" << std::endl;
    if (channel->getClients().size() >= channel->getUserLimit()) {
      reply += ERR_CHANNELISFULL(user, channelName);
      continue;
    }

    std::cout << "Bp 8\n" << std::endl;
    channel->connectClient(*client);
    std::cout << "Bp 9\n" << std::endl;
    std::string channelUsers = channel->getChannelUsers();

    reply += MSG_JOIN(user, channelName) +
             (channel->getTopic().empty()
                  ? ""
                  : RPL_TOPIC(nick, channelName, channel->getTopic())) +
             RPL_NAMREPLY(nick, channelName, channelUsers) +
             RPL_ENDOFNAMES(nick, channelName);
  }

  return reply;
}
