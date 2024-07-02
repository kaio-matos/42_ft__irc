#include <ft_irc.hpp>

std::string PASS(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {

  std::string requiredPassword;

  if (from_socket._logged) {
    Client<sockaddr_in> *client = irc.getClient(from_socket.getFd());
    if (client != NULL)
      return (ERR_ALREADYREGISTERED(client->user.nickname));
    return (ERR_ALREADYREGISTERED("user"));
  }
  if (args.size() < 1)
    return ERR_NEEDMOREPARAMS("User", "PASS");

  requiredPassword = irc.clientArgs.password;

  if (requiredPassword == args[0]) {
    from_socket._logged = true;
    return "";
  } else
    return (ERR_PASSWSMISMATCH("user"));
}