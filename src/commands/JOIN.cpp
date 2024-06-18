#include <ft_irc.hpp>

std::string JOIN(std::vector<std::string> args, Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
    typedef std::map<int, Client<sockaddr_in> >::iterator ClientIterator;
    typedef std::map<std::string, Channel<sockaddr_in> >::iterator ChannelIterator;

    ClientIterator client_it = irc.clients.find(from_socket.getFd());
    if (client_it == irc.clients.end()) {
        return ERR_NOTREGISTERED;
    }

    Client<sockaddr_in> &client = client_it->second;
    std::string nick = client.user.nickname;
    std::string user = client.user.username;

    std::vector<std::string> channels = splitByComma(args[0]);
    std::vector<std::string> key;
    if (key.size() > 1)
        key = splitByComma(key[1]);

    std::string reply;
    if (key.size() != channels.size() && !key.empty())
        return ERR_NEEDMOREPARAMS(user, "JOIN");
    
    //no irc se der "join 0" ele desconecta de todos os canais
    if (channels[0] == "0") {
        for (ChannelIterator channel_it = irc.channels.begin(); channel_it != irc.channels.end(); ++channel_it) {
            channel_it->second.disconnectClient(client);
        }
        return "";
    }

    for (size_t i = 0; i < channels.size(); i++) {
        std::string channelName = channels[i];
        std::string channelKey;
        if (!key.empty())
            channelKey = key[i];

        if (channelName[0] != '#' && channelName[0] != '&') {
            reply += ERR_NOSUCHCHANNEL(user, channelName);
            continue;
        }

        ChannelIterator channel_it = irc.channels.find(channelName);
        Channel<sockaddr_in> *channel;
        if (channel_it != irc.channels.end()) {
            channel = &(channel_it->second);
        } else {
            irc.addChannel(Channel<sockaddr_in>(channelName));
            channel_it = irc.channels.find(channelName); // Re-busca para atualizar o iterator
            channel = &(channel_it->second);
            channel->addOperator(client); // Adiciona o cliente como operador
        }

        /*
        if (channel->getIsInviteOnly() && !client.channelOnInviteList(channelName)) {
            reply += ERR_INVITEONLYCHAN(user, channelName);
            continue;
        }
        */

        if (!channelKey.empty() && channel->getPasswd() != channelKey) {
            reply += ERR_BADCHANNELKEY(user, channelName);
            continue;
        }

        if (channel->getClients().size() >= channel->getUserLimit()) {
            reply += ERR_CHANNELISFULL(user ,channelName);
            continue;
        }

        channel->connectClient(client);
        std::string channelUsers = channel->getChannelUsers();

        reply += MSG_JOIN(user, channelName) +
                 (channel->getTopic().empty() ? "" : RPL_TOPIC(nick, channelName, channel->getTopic())) +
                 RPL_NAMREPLY(nick, channelName, channelUsers) +
                 RPL_ENDOFNAMES(nick, channelName);
    }

    return reply;
}