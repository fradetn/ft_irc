#include "includes.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::handleCommands(Client *client) {
	parserIt it = this->parsedMessages.begin();
	while (this->parsedMessages.size() >= 1) {
		switch (getCmdType((*it).command)) {
			case CMD_NICK:
				std::cout << "NICK" << std::endl;
				this->cmdNick(client, *it);
				break;
			case CMD_USER:
				std::cout << "USER" << std::endl;
				this->cmdUser(client, *it);
				break;
			case CMD_QUIT:
				std::cout << "QUIT" << std::endl;
				this->cmdQuit(client, *it);
				break;
			case CMD_JOIN:
				std::cout << "JOIN" << std::endl;
				this->cmdJoin(client, *it);
				break;
			case CMD_UNKNOWN:
				std::cout << "UNKNOWN" << std::endl;
				this->respond(client, ERR_UNKNOWNCOMMAND(client->getNickName(), (*it).command));
				break;	
		}
		this->parsedMessages.erase(it);
		std::cout << std::endl;
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
	if (cmd.params.empty()) { // si aucun parametres
		std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
		this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	std::cout << "first param: " << cmd.command << std::endl;
	if (cmd.params[0].empty() == true) {// si le premier parametre est vide
		std::cout << "ERR_NONICKNAMEGIVEN" << std::endl;
		this->respond(client, ERR_NONICKNAMEGIVEN(client->getNickName()));
		return;
	}
	else {
		if (this->getClientByNick(cmd.params[0]) == NULL) // si aucun client n'a deja ce NickName
			client->setNickName(cmd.params[0]);
		else {
			std::cout << "ERR_NICKNAMEINUSE" << std::endl;
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
	std::cout << "first param: " << cmd.params[0] << std::endl;
	if (cmd.params.size() < 3 || cmd.trailing.empty()) {
			std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
		this->respond(client, ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	else if (client->getUserName().empty() == false){
		std::cout << "ERR_ALREADYREGISTRED" << std::endl;
		this->respond(client, ERR_ALREADYREGISTRED(client->getNickName()));
		return;
	}
	else if (this->getClientByUser(cmd.params[0]) != NULL) { // si ce UserName est deja pris
		std::cout << "ERR_ALREADYREGISTRED" << std::endl;
		this->respond(client, ERR_ALREADYREGISTRED(client->getNickName()));
		return;
	}

	client->setUserName(cmd.params[0]);
}

void Server::cmdQuit(Client *client, Parser cmd) {
	std::cout << cmd.trailing << std::endl;
	this->disconectClient(client);
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
	bool isJoined;

	if (cmd.params.size() == 1 && cmd.params[0] == "0") {
		std::cout << "Removing client from all Channels" << std::endl; 
		this->rmCliFromAllChan(client);
		return;
	}
	else if (cmd.params.size() < 1 || cmd.params.size() > 2) {
		std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
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
		std::string channelName = (*chanIt)[0] == '#' ? (*chanIt).substr(1) : "";
		if (channelName == "") {
			std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
			return (client->respond(ERR_NOSUCHCHANNEL(*chanIt)));
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
			// envoyer confirmation
			client->respond("JOIN #" + channelName);
		}
		if (keysIt != keys.end())
			keysIt++;
	}

	std::cout << "channels: " << std::endl;
	std::vector<Channel *>::iterator it;

	for (it = this->channels.begin(); it != this->channels.end(); ++it)
		std::cout << (*it)->getName() << std::endl;
}
