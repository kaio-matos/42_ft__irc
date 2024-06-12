#include <ft_irc.hpp>

std::string TOPIC(std::vector<std::string> args,
                  Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
	
	//verificar a quantidade de argumentos passados
	if(args.size() < 1 || args.size() > 3)
		return 1;//retorna erro
	
	//caso der certo obter dados do cliente que enviou comando
	std::string clientuser = irc.clients.user.username;
	std::string clientuser = irc.clients.user.nickname;

	//obter dados do canal e verificar se existe
	std::string channelname = args[0];
	std::vector<Channel>::iterator it = std::find(irc.channels.begin(),irc.channels.end(), channelname);
	if( it == irc.channels.end())
		return 1;//nao achou o canal com o nome passado
	
	//caso achar usaremos a referencia do canal
	Channel &channel = (*it);
	
	//prosseguindo, verificar se o comando foi chamado para mostrar o topico
	if(args.size() == 1) {
		std::string channeltopic = irc.channels.topic;
		if(channeltopic.empty())
			return 1;//erro ou algo vazio mesmo
		return (channeltopic);//retornar a string do topico do canal, talvez seja melhor uma frase para depois
	}

	//verificar se eh operador e se o canal requer permissao
	/*
	 * if(irc.channel.isOperator() && !irc.channel.isTopicOpRequired())
	 * 	return 1;
	 */

	//Definir o novo topico, caso virar private lembrar de trocar por getters
	std::string newtopic = args[1];
	irc.channels.topic = newtopic;

	//notificar os clients do canal sobre a mudanca


}
