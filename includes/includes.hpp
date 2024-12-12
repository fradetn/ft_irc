#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <map>
#include <vector>
#include <poll.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Parser;

// Erreurs liées à l'authentification
# define ERR_ALREADYREGISTRED(nickname)				"462 " + nickname + " :You may not reregister"
# define ERR_PASSWDMISMATCH(nickname)				"464 " + nickname + " :Incorrect password (disconecting)"
# define ERR_NOTREGISTERED(command)					"451 " + command + " :You have not registered"
# define ERR_NONICKNAMEGIVEN(nickname)				"431 " + nickname + " :No nickname given"
# define ERR_NICKNAMEINUSE(nickname, newNickname)	"433 " + nickname + " " + newNickname + " :Nickname is already in use"

// Erreurs liées aux commandes
# define ERR_UNKNOWNCOMMAND(nickname, command)		"421 " + nickname + " " + command + " :Unknown command"
# define ERR_NEEDMOREPARAMS(nickname, command)		"461 " + nickname + " " + command + " :Not enough parameters"

// Erreurs liées aux channels
# define ERR_BADCHANNELKEY(channel)					"475 " + channel + " :Cannot join channel (+k)"
# define ERR_CHANNELISFULL(channel)					"471 " + channel + " :Cannot join channel (+l)"
# define ERR_BANNEDFROMCHAN(channel)				"474 " + channel + " :Cannot join channel (+b)"

#ifndef BUFFUR_SIZE
# define BUFFUR_SIZE 512
#endif

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 999
#endif

enum e_cmdType {
	CMD_NICK,
	CMD_USER,
	CMD_QUIT,
	CMD_JOIN,
	CMD_UNKNOWN
};

typedef std::vector<Parser>::iterator parserIt;
typedef std::vector<pollfd>::iterator pollFdIt;

void 						makeSocketNonBlock(int fd);
e_cmdType 					getCmdType(const std::string& command);
std::vector<std::string>	split(std::string toSplit, char delim);
bool getStringUntil(const std::string& input, std::string& result, char delimiter, size_t& startPos);

// template <typename T>
// bool searchInVector(std::vector<T> vector, T param) {
// 	std::vector<T>::iterator it;
// 	for (it = vector.begin(); it != vector.end(); ++it) {
// 		if (it->fd == fd)
// 			return (it);
// 	}
// 	return (it);
// };

#endif