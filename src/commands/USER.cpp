#include "replies.hpp"
#include <ft_irc.hpp>

std::string USER(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() != 4) {
    return ERR_NEEDMOREPARAMS(args[3], "USER");
  }
  User createdUser(args[0], args[3]);
  Client<sockaddr_in> *found = irc.getClient(createdUser.nickname);

  if (found != NULL) {
    return ERR_ALREADYREGISTERED(createdUser.nickname);
  }

  Client<sockaddr_in> createdClient(createdUser, from_socket);

  irc.addClient(createdClient);

  return "";
}
