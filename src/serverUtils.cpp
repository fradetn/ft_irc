#include "includes.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Parser.hpp"

/* -------------------------------------------------------------------------- */
/*                               GetBy functions                              */
/* -------------------------------------------------------------------------- */

Client		*Server::getClientByNick(std::string const &nickname) {
	std::map<int, Client *>::iterator it;

	for (it = this->clients.begin(); it != this->clients.end(); ++it)
		if (nickname == (*it).second->getNickName())
			return ((*it).second);
	return (NULL);
}
Client		*Server::getClientByUser(std::string const &username) {
	std::map<int, Client *>::iterator it;

	for (it = this->clients.begin(); it != this->clients.end(); ++it)
		if (username == (*it).second->getUserName())
			return ((*it).second);
	return (NULL);
}
Channel		*Server::getChannelByName(std::string const &name) {
	std::vector<Channel *>::iterator it;
	
	for (it = this->channels.begin(); it != this->channels.end(); ++it)
		if ((*it)->getName() == name)
			return ((*it));
	return (NULL);
}

std::string Server::getPrefix() {
	return this->hostname;
	
	struct hostent *hostEntry = gethostbyname(this->hostname.c_str());
	if (!hostEntry) {
		throw std::runtime_error("Error: gethostbyname failed");
	}
	return hostEntry->h_name;
}

/* -------------------------------------------------------------------------- */
/*                             SerachFor functions                            */
/* -------------------------------------------------------------------------- */

pollFdIt Server::searchForFd(int fd) {
	pollFdIt it;
	for (it = this->pollFds.begin(); it != this->pollFds.end(); ++it) {
		if (it->fd == fd)
			return (it);
	}
	return (it);
}

parserIt	Server::searchForCmd(std::string cmd) {
	parserIt it;
	for (it = this->parsedMessages.begin(); it != this->parsedMessages.end(); ++it) {
		if (it->command == cmd)
			return (it);
	}
	return (it);
}

clientsIt Server::searchForClient(Client *client) {
	clientsIt it;
	for (it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (it->second == client)
			return (it);
	}
	return (it);
}

channelIt 	Server::searchForChannel(Channel *channel) {
	channelIt it;
	for (it = this->channels.begin(); it != this->channels.end(); ++it) {
		if (*it == channel)
			return (it);
	}
	return (it);
}

/* -------------------------------------------------------------------------- */
/*                                    Utils                                   */
/* -------------------------------------------------------------------------- */

std::set<Client *> Server::getChanCommonUsers(Client *client) {
	std::set<Client *> common;
	std::vector<Client *> cli;
	std::vector<Channel *>::iterator chanIt;

	for (chanIt = this->channels.begin(); chanIt != this->channels.end(); ++chanIt) {
		if ((*chanIt)->isClientInChan(client) == true) {
			cli = (*chanIt)->getClients();
			common.insert(cli.begin(), cli.end());
		}
	}
	return (common);
}

void	Server::sendMessToAllCommonUsers(Client *client, std::string message) {
	std::set<Client *> commonUsers = this->getChanCommonUsers(client);
	for (std::set<Client *>::iterator it = commonUsers.begin(); it != commonUsers.end(); ++it) {
		if ((*it) != client)
			(*it)->write(message);
	}
	commonUsers.clear();
}

void Server::parseMess(std::string message) {
	size_t		pos = 0;
	std::string	res;
	Parser		parsed;

	if (!this->parsedMessages.empty())
		this->parsedMessages.clear();

	message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
	while (getStringUntil(message, res, '\n', pos)) {
		if (res != "") {
			parsed.parseMessage(res);
			this->parsedMessages.push_back(parsed);
		}
		res = "";
	}
}