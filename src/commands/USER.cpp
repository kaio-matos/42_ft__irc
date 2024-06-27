#include "replies.hpp"
#include <ft_irc.hpp>

std::string USER(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() != 4) {
    return ERR_NEEDMOREPARAMS("User", "USER");
  }
  User createdUser(args[0], args[3]);
  Client<sockaddr_in> *found = irc.getClient(createdUser.nickname);

  if (found != NULL) {
    return ERR_ALREADYREGISTERED(createdUser.nickname);
  }

  Client<sockaddr_in> *createdClient =
      new Client<sockaddr_in>(createdUser, from_socket);

  irc.addClient(createdClient);

  std::string reply;
  reply
      .append(RPL_WELCOME(createdClient->user.nickname,
                          createdClient->user.nickname,
                          createdClient->user.username))
      .append(RPL_YOURHOST(
          createdClient->user.nickname, "weechat",
          "1")) // TODO: check how to get the client name and version
      .append(RPL_CREATED(createdClient->user.nickname))
      .append(RPL_MYINFO(createdClient->user.nickname));

  return reply;
}
