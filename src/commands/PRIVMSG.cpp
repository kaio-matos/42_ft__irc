#include <ft_irc.hpp>

// https://modern.ircdocs.horse/#privmsg-message
std::string PRIVMSG(std::vector<std::string> args,
                    Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() < 2) {
    return "usage: PRIVMSG <target> <text to be sent>";
  }
  std::string target_nickname = args[0];
  std::string message = std::string(args[1]);

  message.erase(
      message.begin()); // delete ':' from the beggining of the message
  for (int i = 2; i < args.size(); i++) {
    message.append(" ");
    message.append(args[i]);
  }

  try {
    Client<sockaddr_in> &client = irc.getClient(target_nickname);
    client.socket.write(message);
    return "\n";
  } catch (const std::exception e) {
    return "No such nick/channel\n";
  }
}
