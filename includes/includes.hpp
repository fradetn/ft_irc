#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <map>
#include <set>
#include <vector>
#include <deque>
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
class Client;
class Channel;

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
# define ERR_NORECIPIENT(nickname, command)			"411 " + nickname + " " + command + " :No recipient given"
# define ERR_NOTEXTTOSEND(nickname, command)		"412 " + nickname + " " + command + " :No text to send"
# define ERR_TOOMANYTARGETS(nickname, command)		"407 " + nickname + " " + command + " :Duplicate recipients. No message delivered"
# define ERR_NOSUCHNICK(nickname, command)			"401 " + nickname + " " + command + " :No such nick"
# define ERR_UNKNOWNMODE(nickname, charac)			"472 " + nickname + " " + charac  + " :is unknown mode char to me"
# define ERR_KEYSET(nickname, channel)				"467 " + nickname + " " + channel + " :Channel key already set"
// Erreurs liées aux channels
# define ERR_NOSUCHCHANNEL(channel)					"403 " + channel + " :No such channel"
# define ERR_NOTONCHANNEL(nickname, channel)		"442 " + nickname + " " + channel + " :You're not on that channel"
# define ERR_USERONCHANNEL(nickname, channel)		"443 " + nickname + " " + channel + " :is already on channel"
# define ERR_CHANNELISFULL(nick, channel)			"471 " + nick + " " + channel + " :Cannot join channel (+l)"
# define ERR_INVITEONLYCHAN(channel)				"473 " + channel + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(nick, channel)			"474 " + nick + " " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(nick, channel)			"475 " + nick + " " + channel + " :Cannot join channel (+k)"
# define ERR_CHANOPRIVSNEEDED(nickname, channel)	"482 " + nickname  + " " + channel + " :You're not channel operator"
# define ERR_USERNOTINCHANNEL(nick, target, chan)	"441 " + nick + " " + target + " " + chan + " :They aren't on that channel"

# define ERR_SHUTDOWN								"ERROR :Server shutting down"

/* -------------------------------------------------------------------------- */
/*                             Messages de reponse                            */
/* -------------------------------------------------------------------------- */

# define RPL_WELCOME(nickname, prefix)				"001 " + nickname + " :Welcome to the Internet Relay Network " + prefix 
# define RPL_YOURHOST(nickname, servName, version)	"002 " + nickname + " :Your host is " + servername + ", running version " + version
# define RPL_CREATED(nickname, date)				"003 " + nickname + " :This server was created " + date
# define RPL_MYINFO(nickname, servName, version)	"004 " + nickname + " " + servername + " " + version + " <available user modes> <available channel modes>"

# define RPL_NOTOPIC(channel)						"331 " + channel + " :No topic is set"
# define RPL_TOPIC(nickname, channel, topic)		"332 " + nickname + " " + channel + " :" + topic
# define RPL_NAMREPLY(nickname, channel)			"353 " + nickname + " " + channel + " :"
# define RPL_ENDOFNAMES(nickname, channel)			"366 " + nickname + " " + channel + " :End of /NAMES list"

# define RPL_QUIT(nickname, reason)					":" + nickname + " QUIT :" + reason
# define RPL_CAP()									"CAP * LS :"
# define RPL_PART(channel, reason)					"PART " + channel + " " + reason
# define RPL_KICK(channel, nickname, reason)		"KICK " + channel + " " + nickname +  " :" + reason
# define RPL_JOIN(channel)							"JOIN :" + channel
# define RPL_NICK(oldNick, newNick)					":" + oldNick + " NICK " + newNick
# define RPL_PRIVMSG(target, message)				"PRIVMSG " + target + " :" + message
# define RPL_NOTICE(target, message)				"NOTICE " + target + " :" + message
# define RPL_MODE(target, message)					"MODE " + target + " " + message
# define RPL_CHANNELMODEIS(nick, chan, mods)		"324 " + nick + " " + chan + " " + mods
# define RPL_INVITING(nick, invited, chan)			"341 " + nick + " " + invited + " " + chan

# define DEFAULT	"\033[0m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define WHITE		"\033[37m"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 512
#endif

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 999
#endif

#ifndef NB_CMD
# define NB_CMD 13
#endif

typedef void (Server::*cmdFunc_t)(Client *, Parser);

typedef std::vector<Parser>::iterator		parserIt;
typedef std::deque<pollfd>::iterator		pollFdIt;
typedef std::vector<Channel *>::iterator	channelIt;
typedef std::map<int, Client *>::iterator	clientsIt;

void 						handle_shutdown(int sig);
void 						makeSocketNonBlock(int fd);
bool 						isDigits(const std::string& str);
std::vector<std::string>	split(std::string toSplit, char delim);
bool getStringUntil(const std::string& input, std::string& result, char delimiter, size_t& startPos);

#endif