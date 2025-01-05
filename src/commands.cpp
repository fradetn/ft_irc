#include "includes.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::handleCommands(Client *client) {
	parserIt it = this->parsedMessages.begin();
	std::string commandsStr[NB_CMD] = {"PASS", "NICK", "USER", "QUIT", "JOIN", "PART", "TOPIC", "PRIVMSG"};
	cmdFunc_t	commandsFunc[NB_CMD] = {&Server::cmdPass, &Server::cmdNick, &Server::cmdUser, &Server::cmdQuit, &Server::cmdJoin, &Server::cmdPart, &Server::cmdTopic, &Server::cmdPriv};

	while (this->parsedMessages.size() >= 1) {
		int i;
		for (i = 0; i < NB_CMD && commandsStr[i] != (*it).command; i++){}
		// std::cout << "i = " << i << std::endl;
		if ((i == 1 || i == 2) && !client->getIsLog()) {
			std::cout << RED"ERR_NOTREGISTERED"DEFAULT << std::endl;
			this->respond(client, ERR_NOTREGISTERED((*it).command));
		}
		else if (i > 2 && client->getIsAuth() == false) {
			std::cout << RED"ERR_NOTREGISTERED"DEFAULT << std::endl;
			this->respond(client, ERR_NOTREGISTERED((*it).command));
		}
		else if (i == NB_CMD) {
			std::cout << RED"ERR_UNKNOWNCOMMAND"DEFAULT << std::endl;
			this->respond(client, ERR_UNKNOWNCOMMAND(client->getNickName(), (*it).command));
		}
		else {
			// std::cout << commandsStr[i] << std::endl;
			(this->*commandsFunc[i])(client, *it);
		}
		this->parsedMessages.erase(it);
		std::cout << std::endl;
	}
}

void Server::cmdPass(Client *client, Parser cmd) {
	if (client->getIsLog() == true) {
		std::cout << RED"ERR_ALREADYREGISTRED"DEFAULT << std::endl;
		return (this->respond(client, ERR_ALREADYREGISTRED(client->getNickName())));
	}
	if (cmd.params[0] == this->passWord || cmd.trailing == this->passWord) {
		client->setIsLog(true);
		std::cout << GREEN"Client logged: "DEFAULT << client->getFd() << std::endl;
	}
	else {
		std::cout << "Wrong password from: " << client->getFd() << std::endl;
		this->respond(client, ERR_PASSWDMISMATCH(client->getNickName()));
		this->clients.erase(searchForClient(client));
		client->setToBeDeleted(true);
	}
}

/**
 * @brief Command: NICK
 *
 * Parameters: <nickname>
 * 
 * @param client Pointer to client
 * @param cmd Parsed command line
 */
void Server::cmdNick(Client *client, Parser cmd) {
	std::string oldNick = client->getNickName();
	if (cmd.params.empty()) { // si aucun parametres
		std::cout << RED"ERR_NEEDMOREPARAMS"DEFAULT << std::endl;
		this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	if (cmd.params[0].empty() == true) {// si le premier parametre est vide
		std::cout << RED"ERR_NONICKNAMEGIVEN"DEFAULT << std::endl;
		this->respond(client, ERR_NONICKNAMEGIVEN(client->getNickName()));
		return;
	}
	else {
		if (this->getClientByNick(cmd.params[0]) == NULL) {// si aucun client n'a deja ce NickName
			client->setNickName(cmd.params[0]);
			std::cout << GREEN"New nickname set: "DEFAULT << client->getNickName() << std::endl;
			if (!client->getUserName().empty() && !client->getIsAuth()) {
				std::cout << GREEN"Client on socket "DEFAULT << client->getFd() << GREEN" is now authentificated"DEFAULT << std::endl;
				this->respond(client, RPL_WELCOME(client->getNickName(), client->getPrefix()));
				client->setIsAuth(true);
			}
		else
			this->sendMessToAllCommonUsers(client, RPL_NICK(oldNick, client->getNickName()));
		}
		else {
			std::cout << RED"ERR_NICKNAMEINUSE"DEFAULT << std::endl;
			this->respond(client, ERR_NICKNAMEINUSE(client->getNickName(), cmd.params[0]));
		}
	}
}

/**
 * @brief Command: USER
 *
 * Parameters: <user> <mode> <unused> :<realname>
 * 
 * @param client Pointer to client
 * @param cmd Parsed command line
 */
void Server::cmdUser(Client *client, Parser cmd) {
	// std::cout << "first param: " << cmd.params[0] << std::endl;
	if (cmd.params.size() < 3 || !cmd.hasTrailing) {
		std::cout << RED"ERR_NEEDMOREPARAMS"DEFAULT << std::endl;
		this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	else if (client->getUserName().empty() == false){
		std::cout << RED"ERR_ALREADYREGISTRED"DEFAULT << std::endl;
		this->respond(client, ERR_ALREADYREGISTRED(client->getNickName()));
		return;
	}
	client->setUserName(cmd.params[0]);
	std::cout << GREEN"New username set: "DEFAULT << client->getUserName() << std::endl;
	if (!client->getNickName().empty() && !client->getIsAuth()) {
		std::cout << GREEN"Client on socket "DEFAULT << client->getFd() << GREEN" is now authentificated"DEFAULT << std::endl;
		this->respond(client, RPL_WELCOME(client->getNickName(), client->getPrefix()));
		client->setIsAuth(true);
	}
}

void Server::cmdQuit(Client *client, Parser cmd) {
	std::cout << cmd.trailing << std::endl;

	this->sendMessToAllCommonUsers(client, RPL_QUIT(client->getPrefix(), cmd.trailing));
	this->clients.erase(searchForClient(client));
	client->setToBeDeleted(true);
}

/**
 * @brief 
 * Command: JOIN
 * 
 * Parameters: ( #<channel> *( "," #<channel> ) [ <key> *( "," <key> ) ] )
 * 
 * 				OR "0"
 * 
 * @param client pointer to client
 * @param cmd parsed command line
 */
void Server::cmdJoin(Client *client, Parser cmd) {
	bool isJoined = false;

	if (cmd.params.size() == 1 && cmd.params[0] == "0") {
		std::cout << RED"Removing client from all Channels"DEFAULT << std::endl;
		std::vector<Channel *>::iterator it;
		for (it = this->channels.begin(); it != this->channels.end(); ++it) {
			if ((*it)->isClientInChan(client))
				(*it)->writeInChan(client, RPL_PART((*it)->getName(), "Leaving"));
		}
		this->rmCliFromAllChan(client);

		std::cout << MAGENTA"Channels list: "DEFAULT << std::endl;
		std::vector<Channel *>::iterator ite;
		for (ite = this->channels.begin(); ite != this->channels.end(); ++ite)
			std::cout << (*ite)->getName() << std::endl;
		return;
	}
	else if (cmd.params.size() < 1 || cmd.params.size() > 2) {
		std::cout << RED"ERR_NEEDMOREPARAMS"DEFAULT << std::endl;
		this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	std::vector<std::string> chans = split(cmd.params[0], ',');
	std::vector<std::string> keys;
	if (cmd.params.size() == 2)
		keys = split(cmd.params[1], ',');
	else
		keys = split("", ' ');
	std::vector<std::string>::iterator keysIt = keys.begin();
	std::vector<std::string>::iterator chanIt;

	for (chanIt = chans.begin(); chanIt != chans.end(); ++chanIt) {
		std::string channelName = (*chanIt);
		if (channelName[0] != '#') {
			std::cout << RED"ERR_NOSUCHCHANNEL"DEFAULT << std::endl;
			return (this->respond(client, ERR_NOSUCHCHANNEL(channelName)));
		}
		Channel *channel = this->getChannelByName(channelName);
		if (channel == NULL) { // si le channel n'existe pas, on le créer avec le client comme admin
			if (keysIt != keys.end())
				this->channels.push_back(new Channel(client, channelName, *keysIt));
			else
				this->channels.push_back(new Channel(client, channelName));
			isJoined = true;
		}
		else {
			isJoined |= channel->addNewClient(client, *keysIt);
		}
		if (isJoined) {
			channel = this->getChannelByName(channelName);
			// ecrire un message dans le channel
			channel->writeInChan(client, RPL_JOIN(channelName));
			if (!channel->getTopic().empty())
				this->respond(client, RPL_TOPIC( client->getNickName(), channelName, channel->getTopic()));
			else
				this->respond(client, RPL_NOTOPIC(channelName));
			this->respond(client, channel->getListMessage(client));
			this->respond(client, RPL_ENDOFNAMES(client->getNickName(), channelName));
		}
		if (keysIt != keys.end())
			keysIt++;
	}

	std::cout << MAGENTA"Channels list: "DEFAULT << std::endl;
	std::vector<Channel *>::iterator it;
	for (it = this->channels.begin(); it != this->channels.end(); ++it)
		std::cout << (*it)->getName() << std::endl;
}

/**
 * @brief 
 * Command: PART
 * 
 * Parameters: ( #<channel> *( "," #<channel> ) [ :<Part Message> ] )
 * 
 * @param client pointer to client
 * @param cmd parsed command line
 */
void Server::cmdPart(Client *client, Parser cmd) {
	if (cmd.params.empty() || cmd.params.size() > 1) {
		std::cout << RED"ERR_NEEDMOREPARAMS"DEFAULT << std::endl;
		return this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
	}

	std::vector<std::string> chans = split(cmd.params[0], ',');
	std::vector<std::string>::iterator chanIt;

	for (chanIt = chans.begin(); chanIt != chans.end(); ++chanIt) {
		std::string channelName = (*chanIt);
		Channel *channel = getChannelByName(channelName);
		if (channelName.empty() || channel == NULL) {
			std::cout << RED"ERR_NOSUCHCHANNEL"DEFAULT << std::endl;
			this->respond(client, ERR_NOSUCHCHANNEL((*chanIt)));
		}
		else if (channel != NULL && !channel->isClientInChan(client)) {
			std::cout << RED"ERR_NOTONCHANNEL"DEFAULT << std::endl;
			this->respond(client, ERR_NOSUCHCHANNEL((*chanIt)));
		}
		else {
			channel->writeInChan(client, RPL_PART(channel->getName(), cmd.trailing));
			if (channel->removeClient(client) == false) {
				this->channels.erase(this->searchForChannel(channel));
				delete (channel);
			}

		}
	}
}

/**
 * @brief 
 * Command: TOPIC
 * 
 * Parameters: ( #<channel> [ <topic> ] )
 * 
 * @param client pointer to client
 * @param cmd parsed command line2
 */
void Server::cmdTopic(Client *client, Parser cmd) {
	if (cmd.params.size() < 1) {
		std::cout << RED"ERR_NEEDMOREPARAMS"DEFAULT << std::endl;
		return this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
	}
	Channel *channel = this->getChannelByName(cmd.params[0]);
	if (channel != NULL) {
		if (channel->isClientInChan(client)) {
			if (!cmd.hasTrailing) {
				if (!channel->getTopic().empty())
					return this->respond(client, RPL_TOPIC(client->getNickName(), channel->getName(), channel->getTopic()));
				else
					return this->respond(client, RPL_NOTOPIC(channel->getName()));
			}
			else {
				if (channel->isClientAdmin(client)) {
					channel->setTopic(cmd.trailing);
					channel->writeInChan(client, RPL_TOPIC(client->getNickName(), channel->getName(), channel->getTopic()));
				}
				else {
					std::cout << RED"ERR_CHANOPRIVSNEEDED"DEFAULT << std::endl;
					return this->respond(client, ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName()));
				}
			}
		}
		else {
			std::cout << RED"ERR_NOTONCHANNEL"DEFAULT << std::endl;
			return this->respond(client, ERR_NOTONCHANNEL(client->getNickName(), channel->getName()));
		}
	}
}

/**
 * @brief Command: PRIVMSG
 *
 * Parameters: <target> :<message>
 * 
 * @param client Pointer to client
 * @param cmd Parsed command line
 */
void Server::cmdPriv(Client *client, Parser cmd) {
	if (cmd.params[0].empty())
	{
		std::cout << RED"ERR_NORECIPIENT"DEFAULT << std::endl;
		this->respond(client, ERR_NORECIPIENT(client->getNickName(), cmd.command));
		return;
	}

	if (cmd.params.size() != 1) {
		std::cout << RED"ERR_NEEDMOREPARAMS"DEFAULT << std::endl;
		this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}

	if (!cmd.hasTrailing){
		std::cout << RED"ERR_NOTEXTTOSEND"DEFAULT << std::endl;
		this->respond(client, ERR_NOTEXTTOSEND(client->getNickName(), cmd.command));
		return;
	}

	if (cmd.params[0].find(',') != std::string::npos){
		std::cout << RED"ERR_TOOMANYTARGETS"DEFAULT << std::endl;
		this->respond(client, ERR_TOOMANYTARGETS(client->getNickName(), cmd.command));
		return;
	}

	if (cmd.params[0][0] == '#'){
		Channel *channel = this->getChannelByName(cmd.params[0]);
		if (channel == NULL){
			std::cout << RED"Channel " << cmd.params[0] << " does not exists." DEFAULT << std::endl;
		}
		else{
			channel->writeInChan(client, cmd.trailing);
		}
		return;
	}

	Client *clienttest = this->getClientByNick(cmd.params[0]);
	if (clienttest == NULL){
		std::cout << RED"ERR_NOSUCHNICK"DEFAULT << std::endl;
		this->respond(client, ERR_NOSUCHNICK(client->getNickName(), cmd.command));
		return;
	}
	else{
		clienttest->write(":" + client->getPrefix() + " :" + cmd.trailing);
	}
}