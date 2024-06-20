#include <ft_irc.hpp>

std::string NICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (args.size() < 1)
    return ERR_NONICKNAMEGIVEN(from->user.nickname);

  std::string nickname = args[0];

  if (false) { // TODO: check nick format
    return ERR_ERRONEUSNICKNAME(from->user.nickname, nickname);
  }

  if (irc.getClient(nickname)) {
    return ERR_NICKNAMEINUSE(from->user.nickname, nickname);
  }

  from->user.nickname = nickname;

  return "";
}
