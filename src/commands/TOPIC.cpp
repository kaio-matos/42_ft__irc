#include <ft_irc.hpp>

std::string TOPIC(std::vector<std::string> args,
                  Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
	

	std::string channelName = args[0];
 	Channel<sockaddr_in> *channel = irc.getChannel(channelName);

	typedef std::map<int, Client<sockaddr_in> >::iterator ClientIterator;
	ClientIterator client_it = irc.clients.find(from_socket.getFd());
    if (client_it == irc.clients.end()) {
        return ERR_NOTREGISTERED;
    }

	Client<sockaddr_in> *client = irc.getClient(from_socket.getFd());
    std::string nick = client->user.nickname;
    std::string user = client->user.username;

	//verificar a quantidade de argumentos passados
	if(args.size() < 1 || args.size() > 3)
		return ERR_NEEDMOREPARAMS(nick, "TOPIC");//retorna erro

  	if(!channel)
		return (ERR_NOSUCHCHANNEL(nick, channelName));

	//caso achar usaremos a referencia do canal
	Channel<sockaddr_in> &channelRef = *channel;
	if(!channelRef.isClientInChannel(*client))
		return (ERR_NOTONCHANNEL(nick, channelName));

	//prosseguindo, verificar se o comando foi chamado para mostrar o topico
	if(args.size() == 1) {
		std::string channeltopic = channelRef.getTopic();
		if(channeltopic.empty())
			return (RPL_NOTOPIC(nick, channelName));//erro ou algo vazio mesmo
		return (RPL_TOPIC(nick, channelName, channeltopic));//retornar a string do topico do canal, talvez seja melhor uma frase para depois
	}

	//verificar se eh operador e se o canal requer permissao
	if(channelRef.isOperator(*client) && !channelRef.isTopicOPOnly())
		return (ERR_CHANOPRIVSNEEDED(nick, channelName));

	//Definir o novo topico
	std::string newTopic = args[1];
	channel->setTopic(newTopic);

	//notificar os clients do canal sobre a mudanca
	channelRef.broadcast(*client ,MSG_TOPIC(nick, user , channelName, newTopic));
	
	return "";
}
