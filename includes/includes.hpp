#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <map>
#include <vector>
#include <poll.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Parser;
class Server;

/* -------------------------------------------------------------------------- */
/*                             Messages d'erreurs                             */
/* -------------------------------------------------------------------------- */

// Erreurs liées à l'authentification
# define ERR_NONICKNAMEGIVEN(nickname)				"431 " + nickname + " :No nickname given"
# define ERR_NICKNAMEINUSE(nickname, newNickname)	"433 " + nickname + " " + newNickname + " :Nickname is already in use"
# define ERR_NOTREGISTERED(command)					"451 " + command + " :You have not registered"
# define ERR_ALREADYREGISTRED(nickname)				"462 " + nickname + " :You may not reregister"
# define ERR_PASSWDMISMATCH(nickname)				"464 " + nickname + " :Incorrect password (disconecting)"
// Erreurs liées aux commandes
# define ERR_UNKNOWNCOMMAND(nickname, command)		"421 " + nickname + " " + command + " :Unknown command"
# define ERR_NEEDMOREPARAMS(nickname, command)		"461 " + nickname + " " + command + " :Not enough parameters"
// Erreurs liées aux channels
# define ERR_NOSUCHCHANNEL(channel)					"403 " + channel + " :No such channel"
# define ERR_CHANNELISFULL(channel)					"471 " + channel + " :Cannot join channel (+l)"
# define ERR_BANNEDFROMCHAN(channel)				"474 " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(channel)					"475 " + channel + " :Cannot join channel (+k)"

# define ERR_SHUTDOWN								"ERROR :Server shutting down"



/* -------------------------------------------------------------------------- */
/*                             Messages de reponse                            */
/* -------------------------------------------------------------------------- */

# define RPL_WELCOME(nickname, prefix)				"001 " + nickname + " :Welcome to the Internet Relay Network" + prefix 
# define RPL_YOURHOST(nickname, servName, version)	"002 " + nickname + " :Your host is " + servername + ", running version " + version
# define RPL_CREATED(nickname, date)				"003 " + nickname + " :This server was created " + date
# define RPL_MYINFO(nickname, servName, version)	"004 " + nickname + " " + servername + " " + version + " <available user modes> <available channel modes>"

# define RPL_TOPIC(nickname, channel, topic)		"332 " + nickname + " " + channel + " :" + topic
# define RPL_NAMREPLY(nickname, channel)			"353 " + nickname + " " + channel + " :"
# define RPL_ENDOFNAMES(nickname, channel)			"366 " + nickname + " " + channel + " :End of NAMES list"

# define RPL_QUIT(reason)							"Quit :" + reason
# define RPL_JOIN(channel)							"JOIN :" + channel


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

void 						handle_shutdown(int sig);
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