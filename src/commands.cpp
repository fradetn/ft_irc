#include "includes.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
	if (cmd.params.size() < 1 || cmd.params.size() > 2) {
			std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
		client->respond(ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
		return;
	}
	std::vector<std::string> chans = split(cmd.params[0], ',');
	std::vector<std::string> keys;
	std::vector<std::string>::iterator keysIt;
	if (!cmd.params[1].empty()) {
		keys = split(cmd.params[1], ',');
	}
	keysIt = keys.begin();
	std::vector<std::string>::iterator chanIt;
	for (chanIt = chans.begin(); chanIt != chans.end(); ++chanIt) {
		Channel *channel = this->getChannelByName(*chanIt);
		if (channel == NULL) {
			this->channels.push_back(new Channel(client, *chanIt));
		}
		else {
			channel->addNewClient(client, *keysIt);
		}
	}
}