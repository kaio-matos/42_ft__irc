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

static std::string getNicknameIfNotSet(const User &client) {
  std::string nickname = client.nickname;

  if (nickname.empty()) {
    return ("user");
  }
  return (nickname);
}

std::string NICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {
  Client<sockaddr_in> *from = irc.getClient(from_socket.getFd());

  if (!from || !from_socket._logged) {
    return ERR_NOTREGISTERED;
  }

  if (args.size() < 1)
    return ERR_NONICKNAMEGIVEN(getNicknameIfNotSet(from->user));

  std::string nickname = args[0];

  if (irc.getClient(nickname) != NULL) {
    return ERR_NICKNAMEINUSE(getNicknameIfNotSet(from->user), nickname);
  }

  if (!isNicknameValid(nickname)) {
    return ERR_ERRONEUSNICKNAME(getNicknameIfNotSet(from->user), nickname);
  }

  from->user.nickname = nickname;

  return "";
}
