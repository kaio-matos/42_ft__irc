#ifndef FT_IRC_H
#define FT_IRC_H

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <cerrno>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#include <map>
#include <queue>
#include <vector>

#define SSTR(x)                                                                \
  static_cast<std::ostringstream &>((std::ostringstream() << std::dec << x))   \
      .str()

std::string readfd(int fd);
unsigned int stringAddressToBytes(std::string str);
std::string getAddressFromSockAddrin(const struct sockaddr_in addr);
std::string readFile(std::string filename, char separator);
std::string replaceAll(std::string str, const std::string &from,
                       const std::string &to);

std::vector<std::string> split(std::string str);
std::vector<std::string> splitByComma(const std::string &data);

template <typename T> class Socket;
std::ostream &operator<<(std::ostream &os, const Socket<sockaddr_in> &value);

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#include <Log.hpp>

#include <User.hpp>

#include <Socket.hpp>

#include <Client.hpp>

#include <Channel.hpp>

#include <IRC.hpp>

#include <replies.hpp>

std::string BROADCAST(std::vector<std::string> args,
                      Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string INVITE(std::vector<std::string> args,
                   Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string KICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string MODE(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string TOPIC(std::vector<std::string> args,
                  Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string USER(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string NICK(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string PRIVMSG(std::vector<std::string> args,
                    Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string JOIN(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

std::string QUIT(std::vector<std::string> args,
                 Socket<sockaddr_in> &from_socket, IRC<sockaddr_in> &irc);

#endif
