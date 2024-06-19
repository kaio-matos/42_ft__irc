#include <ft_irc.hpp>

std::string KICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  if (args.size() != 2) {
    return "usage: channel_name user_name";
  }
  std::string name = args[0];
  std::string channel_name = args[1];

  Channel<sockaddr_in> channel = GamesChannel<sockaddr_in>();

  try {
    Client<sockaddr_in> client = channel.getClient(name);
    channel.disconnectClient(client);
    client.socket.write("You got kicked from " + channel.getTopic() + "\n");
    return "Kicked " + name + " from " + channel.getTopic() + "\n";
  } catch (const std::exception e) {
    return "Client with name " + name + " not found " + "\n";
  }
}
