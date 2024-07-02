#include <ft_irc.hpp>

std::string PASS(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {

  std::string requiredPassword;

  DebugLog << "antes do PASS:\n";
  Client<sockaddr_in> *client = irc.getClient(from_socket.getFd());
  DebugLog << "adepois do PASS\n";
  if (client->user.logged)
    return (ERR_ALREADYREGISTERED(client->user.nickname));

  if (args.size() < 1)
    return ERR_NEEDMOREPARAMS("User", "PASS");

  requiredPassword = irc.clientArgs.password;

  if (requiredPassword == args[1]) {
    client->user.logged = true;
    return "";
  } else
    return (ERR_PASSWSMISMATCH("user"));
}