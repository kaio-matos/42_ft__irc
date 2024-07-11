#include <ft_irc.hpp>

static bool isNicknameValid(const std::string &nickname) {
  char _char;

  if (nickname.empty() || nickname.size() > 9) {
    return (false);
  }
  _char = nickname.at(0);
  if (!std::isalpha(_char) &&
      std::string("[]\\`_^{|}").find(_char) == std::string::npos) {
    return (false);
  }
  for (size_t i = 1; i < nickname.size(); i++) {
    _char = nickname.at(i);
    if (!std::isalnum(_char) && _char != '-' &&
        std::string("[]\\`_^{|}").find(_char) == std::string::npos) {
      return (false);
    }
  }
  return (true);
}

static std::string getNicknameIfNotSet(const Client<sockaddr_in> *client) {
  if (!client)
    return "user";
  std::string nickname = client->user.nickname;

  if (nickname.empty()) {
    return ("user");
  }
  return (nickname);
}

std::string NICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from_socket._logged) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() < 1)
    return ERR_NONICKNAMEGIVEN(getNicknameIfNotSet(from));

  std::string nickname = args[0];

  if (irc.getClient(nickname) != NULL) {
    return ERR_NICKNAMEINUSE(getNicknameIfNotSet(from), nickname);
  }

  if (!isNicknameValid(nickname)) {
    return ERR_ERRONEUSNICKNAME(getNicknameIfNotSet(from), nickname);
  }

  if (!from) {
    User createdUser(nickname);
    from = new Client<sockaddr_in>(createdUser, from_socket);
    irc.addClient(from);
  }

  std::string previousNickname = from->user.nickname;
  std::string previousIdentity = from->user.identity();
  from->user.nickname = nickname;

  if (from->user.username.empty()) {
    irc.broadcast(MSG_NICK(previousNickname, from->user.nickname));
  } else {
    irc.broadcast(MSG_NICK(previousIdentity, from->user.nickname));
  }

  return "";
}
