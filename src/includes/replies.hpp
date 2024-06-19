#ifndef REPLIES_HPP
#define REPLIES_HPP

#define numericReply(numeric, target, message)      std::string(":ft-irc ") + numeric + " " + target + " " + message + "\r\n"

//comand replies
#define MSG_JOIN(user, channel)                     (":" + user + " JOIN " + channel + "\r\n")

//numeric replies  
#define RPL_TOPIC(target, channel, topic)           numericReply("332", target, channel + " :" + topic)
#define RPL_NAMREPLY(target, channel, nicknames)    numericReply("353", target, "= " + channel + " :" + nicknames)
#define RPL_ENDOFNAMES(target, channel)             numericReply("366", target, channel + " :End of NAMES list")
#define ERR_NOSUCHCHANNEL(target, channel)          numericReply("403", target, channel + " :No such channel")
#define ERR_NOTREGISTERED                           numericReply("451", "User", ":You have not registered")
#define ERR_NEEDMOREPARAMS(target, command)         numericReply("461", target, command + " :Not enough parameters")
#define ERR_CHANNELISFULL(target, channel)          numericReply("471", target, channel + " :Cannot join channel, already at max capacity")
#define ERR_INVITEONLYCHAN(target, channel)         numericReply("473", target, channel + " :Cannot join channel, invite needed")
#define ERR_BADCHANNELKEY(target, channel)          numericReply("475", target, channel + " :Cannot join channel, valid password needed")


#endif