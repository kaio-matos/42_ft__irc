#include <ft_irc.hpp>

std::string USER(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() != 2) {
    return "usage: USER username nickname\n";
  }

  User createdUser(args[0], args[1]);
  Client<sockaddr_in> createdClient(createdUser, from_socket);

  irc.addClient(createdClient);

  GamesChannel<sockaddr_in>().connectClient(createdClient);

  return "User identified successfully as " + createdUser.nickname + " (" +
         createdUser.username + ")\n";
}
