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
				client->respond(ERR_UNKNOWNCOMMAND(client->getNickName(), (*it).command));
				break;	
		}
		this->parsedMessages.erase(it);
	}
}

void Server::cmdNick(Client *client, Parser cmd) {
	if (cmd.params.empty()) { // si aucun parametres ou trop
			std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
		client->respond(ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	else if (cmd.params[0].empty()) {// si le premier parametre est vide
		std::cout << "ERR_NONICKNAMEGIVEN" << std::endl;
		client->respond(ERR_NONICKNAMEGIVEN(client->getNickName()));
		return;
	}
	else {
		if (this->getClientByNick(cmd.params[0]) == NULL) // si aucun client n'a deja ce NickName
			client->setNickName(cmd.params[0]);
		else {
			std::cout << "ERR_NICKNAMEINUSE" << std::endl;
			client->respond(ERR_NICKNAMEINUSE(client->getNickName(), cmd.params[0]));
		}
	}
}

void Server::cmdUser(Client *client, Parser cmd) {
	std::cout << "size: " << cmd.params.size() << std::endl;
	if (cmd.params.size() < 3 || cmd.trailing.empty()) {
			std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
		client->respond(ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	else if (client->getUserName().empty() == false){
		std::cout << "ERR_ALREADYREGISTRED" << std::endl;
		client->respond(ERR_ALREADYREGISTRED(client->getNickName()));
		return;
	}
	else if (this->getClientByUser(cmd.params[0]) != NULL) { // si ce UserName est deja pris
		std::cout << "ERR_ALREADYREGISTRED" << std::endl;
		client->respond(ERR_ALREADYREGISTRED(client->getNickName()));
		return;
	}

	client->setUserName(cmd.params[0]);
}

void Server::cmdQuit(Client *client, Parser cmd) {
	std::cout << cmd.trailing << std::endl;
	this->disconectClient(client);
}

void Server::cmdJoin(Client *client, Parser cmd) {
	if (cmd.params.size() == 1 && cmd.params[0] == "0") {
		std::cout << "Removing client from all Channels" << std::endl; 
		std::vector<Channel *>::iterator it;
		for (it = this->channels.begin(); it != this->channels.end(); ++it) {
			if ((*it)->removeClient(client) == false) {
				// Delete channel
				delete (*it);
				this->channels.erase(it);
				it--;
			}
		}
	}
	else if (cmd.params.size() < 1 || cmd.params.size() > 2) {
			std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
		client->respond(ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
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
		Channel *channel = this->getChannelByName(*chanIt);
		if (channel == NULL) { // si le channel n'existe pas, on le créer avec le client comme admin
			this->channels.push_back(new Channel(client, *chanIt));
		}
		else {
			channel->addNewClient(client, *keysIt);
		}
		if (keysIt != keys.end())
			keysIt++;
	}
	// std::cout << "channels: "
}