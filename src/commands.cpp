#include "includes.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "Client.hpp"

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
	else if (this->getClientByUser(cmd.params[0]) != NULL) { // si aucun client n'a deja ce UserName
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

// void Server::cmdJoin(Client *client, Parser cmd) {

// }