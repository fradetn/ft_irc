#include "includes.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "Client.hpp"

void Server::cmdNick(Client *client, Parser cmd) {
	if (cmd.params.empty()) { // si aucun parametres
			std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
		client->respond(ERR_NEEDMOREPARAMS(client->getNickName(), cmd.command));
	}
	else if (cmd.params[0].empty()) {// si le premier parametre est vide
		std::cout << "ERR_NONICKNAMEGIVEN" << std::endl;
		client->respond(ERR_NONICKNAMEGIVEN(client->getNickName()));
	}
	else {
		if (this->getClient(cmd.params[0]) == NULL) // si aucun client n'a deja ce NickName
			client->setNickName(cmd.params[0]);
		else {
			std::cout << "ERR_NICKNAMEINUSE" << std::endl;
			client->respond(ERR_NICKNAMEINUSE(client->getNickName(), cmd.params[0]));
		}
	}
}
// void Server::cmdUser(Client *client, Parser cmd) {

// }
// void Server::cmdQuit(Client *client, Parser cmd) {

// }
// void Server::cmdJoin(Client *client, Parser cmd) {

// }