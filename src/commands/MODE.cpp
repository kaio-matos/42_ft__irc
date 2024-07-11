#include <ft_irc.hpp>

static bool hasOnlyDigits(const char *str) {
  while (*str) {
    if (!isdigit(*str)) {
      return (false);
    }
    str++;
  }
  return (true);
}

static bool requiresModeParam(char mode) {

  return (std::string("okl").find(mode) != std::string::npos);
}

static bool hasRequiredModeParams(const std::vector<std::string> args) {

  const std::string &modes = args.at(1);
  std::vector<std::string> modeParams(args.begin() + 2, args.end());
  std::size_t countRequiresModeParam =
      std::count_if(modes.begin(), modes.end(), requiresModeParam);

  if (countRequiresModeParam != modeParams.size()) {
    return (false);
  }

  return (true);
}

std::string MODE(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc) {

  Client<sockaddr_in> *client = irc.getClient(from_socket.getFd());
  if (!client || !from_socket._logged) {
    return ERR_NOTREGISTERED;
  }
  std::string nick = client->user.nickname;
  std::string user = client->user.username;

  if (args.size() < 1)
    return ERR_NEEDMOREPARAMS(nick, "MODE");

  std::string channelName = args[0];
  Channel<sockaddr_in> *channel = irc.getChannel(channelName);

  if (!channel)
    return (ERR_NOSUCHCHANNEL(nick, channelName));

  Channel<sockaddr_in> &channelRef = *channel;
  if (!channelRef.isClientInChannel(client))
    return (ERR_NOTONCHANNEL(nick, channelName));

  if (args.size() == 1 && (args[0][0] == '#' || args[0][0] == '&'))
    return (RPL_CHANNELMODEIS(nick, channelName, channel->getModes()));

  if (!channelRef.isOperator(client))
    return ERR_CHANOPRIVSNEEDED(nick, channelName);

  std::string::size_type pos = args[1].find_first_not_of("iolkt+-");
  if (pos != std::string::npos)
    return (ERR_UNKNOWNMODE(nick, args.at(pos)));

  if (!hasRequiredModeParams(args))
    return ERR_NEEDMOREPARAMS(nick, "MODE");

  std::string reply = MSG_MODE(nick, user, channelName);
  std::string modes = args[1];
  std::vector<std::string> modesParams(args.begin() + 2, args.end());
  std::string replyParams;
  size_t paramPosition = 0;
  bool action;

  for (size_t i = 0; i < modes.size(); i++) {
    char modeChar = modes[i];
    switch (modeChar) {

    case '+': {

      if (modes[i + 1] == '+' || modes[i + 1] == '-')
        continue;

      action = true;
      reply += '+';

      continue;
    }

    case '-': {

      if (modes[i + 1] == '+' || modes[i + 1] == '-')
        continue;

      action = false;
      reply += '-';

      continue;
    }

    case 'i': {

      channelRef.setIsInviteOnly(action);
      reply += 'i';

      continue;
    }

    case 't': {

      channelRef.setTopicRestricted(action);
      reply += 't';

      continue;
    }

    case 'k': {

      replyParams += modesParams[paramPosition] + " ";

      if (action) {
        channelRef.setPasswd(action, modesParams[paramPosition++]);
      } else {
        channelRef.setPasswd(action, modesParams[paramPosition++]);
      }

      reply += 'k';

      continue;
    }

    case 'l': {

      replyParams += modesParams[paramPosition] + " ";

      if (action) {
        if (!hasOnlyDigits(modesParams[paramPosition].c_str()) ||
            std::atoi(modesParams[paramPosition].c_str()) <= 0)
          return (ERR_UNKNOWNMODE(nick, 'l'));
        channelRef.setUserLimit(
            action, std::atoi(modesParams[paramPosition++].c_str()));
      } else
        channelRef.setUserLimit(action, 0);

      reply += 'l';

      continue;
    }

    case 'o': {

      replyParams += modesParams[paramPosition] + " ";
      if (channelRef.getClient(modesParams[paramPosition]) == NULL)
        return (ERR_USERNOTINCHANNEL(modesParams[paramPosition], nick,
                                     channelName));
      if (client == channelRef.getClient(modesParams[paramPosition]))
        return (ERR_UNKNOWNMODE(nick, 'o'));
      if (action) {
        channelRef.addOperator(
            channelRef.getClient(modesParams[paramPosition++]));
      } else if (channelRef.isOperator(
                     channelRef.getClient(modesParams[paramPosition++]))) {
        channelRef.removeOperator(
            channelRef.getClient(modesParams[--paramPosition]));
        modesParams[paramPosition++];
      }

      reply += 'o';
      continue;
    }

    default:
      continue;
    }
  }
  reply += " " + replyParams + "\r\n";
  channelRef.broadcast(*client, reply);

  return reply;
}
