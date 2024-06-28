#ifndef REPLIES_HPP
#define REPLIES_HPP

#define SERVER_NAME "ft_irc"
#define SERVER_VERSION "1"
#define SERVER_USER_MODES "" // TODO: list of user modes
#define SERVER_CHANNEL_MODES "" // TODO: list of channel modes
#define SERVER_CREATED_AT "06/01/2024"

#define numericReply(numeric, target, message)          std::string(":ft-irc ") + numeric + " " + target + " " + message + "\r\n"

//comand replies
#define MSG(str)                                        std::string(":" + str + "\r\n")
#define MSG_JOIN(user, channel)                         std::string(":" + user + " JOIN " + channel + "\r\n")
#define MSG_TOPIC(nick, user, channel, topic)           std::string(":" + nick + "!~" + user + " TOPIC " + channel + " :" + topic + "\r\n")

//numeric replies  
#define RPL_WELCOME(target, nickname, username)         numericReply("001", target, ":Welcome to the " + SERVER_NAME + " Network, " + nickname + "[!" + username + "@*]")
#define RPL_YOURHOST(target, servername, version)       numericReply("002", target, ":Your host is " + servername + ", running version " + version)
#define RPL_CREATED(target)                             numericReply("003", target, ":This server was created " + SERVER_CREATED_AT)
#define RPL_MYINFO(target)                              numericReply("004", target, SERVER_NAME + " " + SERVER_VERSION +  " " + SERVER_USER_MODES + " " + SERVER_CHANNEL_MODES)
#define RPL_AWAY(target, nickname, message)             numericReply("301", target, nickname + " :" + message)
#define RPL_NOTOPIC(target, channel)                    numericReply("331", target, channel + " :No topic is set")
#define RPL_TOPIC(target, channel, topic)               numericReply("332", target, channel + " :" + topic)
#define RPL_INVITING(target, nickname, channel)         numericReply("341", target, nickname + " " + channel)
#define RPL_NAMREPLY(target, channel, nicknames)        numericReply("353", target, "= " + channel + " :" + nicknames)
#define RPL_ENDOFNAMES(target, channel)                 numericReply("366", target, channel + " :End of NAMES list")
#define ERR_NOSUCHNICK(target, nickname)                numericReply("401", target, nickname + " :No such nick/channel")
#define ERR_NOSUCHSERVER(target, servername)            numericReply("402", target, servername + " :No such server")
#define ERR_NOSUCHCHANNEL(target, channel)              numericReply("403", target, channel + " :No such channel")
#define ERR_CANNOTSENDTOCHAN(target, channel)           numericReply("404", target, channel + " :Cannot send to channel")
#define ERR_NORECIPIENT(target, command)                numericReply("411", target, ":No recipient given (" + command +  ")")
#define ERR_NOTEXTTOSEND(target)                        numericReply("412", target, ":No text to send")
#define ERR_NONICKNAMEGIVEN(target)                     numericReply("431", target, ":No nickname given")
#define ERR_ERRONEUSNICKNAME(target, nickname)          numericReply("432", target, nickname + " :Erroneus nickname")
#define ERR_NICKNAMEINUSE(target, nickname)             numericReply("433", target, nickname + " :Nickname is already in use")
#define ERR_USERNOTINCHANNEL(target, nickname, channel) numericReply("441", target, nickname + " " + channel + " :You're not channel operator")
#define ERR_NOTONCHANNEL(target, channel)               numericReply("442", target, channel + " :You're not on that channel")
#define ERR_USERONCHANNEL(target, nickname, channel)    numericReply("443", target, nickname + " " + channel + " :is already on channel")
#define ERR_NOTREGISTERED                               numericReply("451", "User", ":You have not registered")
#define ERR_NEEDMOREPARAMS(target, command)             numericReply("461", target, command + " :Not enough parameters")
#define ERR_ALREADYREGISTERED(target)                   numericReply("462", target, ":You may not reregister")
#define ERR_CHANNELISFULL(target, channel)              numericReply("471", target, channel + " :Cannot join channel, already at max capacity")
#define ERR_INVITEONLYCHAN(target, channel)             numericReply("473", target, channel + " :Cannot join channel, invite needed")
#define ERR_BADCHANNELKEY(target, channel)              numericReply("475", target, channel + " :Cannot join channel, valid password needed")
#define ERR_CHANOPRIVSNEEDED(target, channel)           numericReply("482", target, channel + " :You're not channel operator")

#endif
