/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:04:59 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/12 17:09:59 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include "Channel.hpp"

Server::Server() : passWord("serveur42"), port(8080) {
	this->serverFd = -1;
	this->createSocket();
}

Server::Server(int _port, std::string _passWord) : passWord(_passWord), port(_port) {
	this->serverFd = -1;
	this->createSocket();
}

Server::~Server() {
	
}

/* -------------------------------------------------------------------------- */
/*                              Private methods                               */
/* -------------------------------------------------------------------------- */

void Server::createSocket(void) {
	this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
	 if (this->serverFd < 0) {
		throw std::runtime_error("Error: socket creation failed");
    }

	int opt = 1;
	if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(this->serverFd);
		throw std::runtime_error("Error: setting socket options failed");
	}

	sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(this->port);

	if (bind(this->serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		close(this->serverFd);
		throw std::runtime_error("Error: bind failed");
	}
	if (listen(this->serverFd, MAX_CLIENTS) < 0) {  // MAX_CLIENTS : nombre maximal de connexions en file d'attente
		close(this->serverFd);
		throw std::runtime_error("Error: listen failed");
	}

	makeSocketNonBlock(this->serverFd);
	pollfd pfd = {this->serverFd, POLLIN, 0};
	this->pollFds.push_back(pfd);
}

void Server::run() {
    std::cout << "Server listening on port " << this->port << std::endl;
	this->isRunning = true;
	while (this->isRunning) {
		int polCount = poll(this->pollFds.data(), this->pollFds.size(), -1);
		if (polCount < 0) {
			close(this->serverFd);
			throw std::runtime_error("Error: poll failed");
		}
		for (size_t i = 0; i < this->pollFds.size(); i++) {
			if (this->pollFds[i].revents == 0) 
				continue;
			if ((this->pollFds[i].revents & POLLIN) == POLLIN) {
				std::cout << "event happening on: " << this->pollFds[i].revents << std::endl;
				this->handleEvent(i);
				std::cout << std::endl;
			}
		}
	}
}

void Server::handleEvent(size_t &i) {
	if (this->pollFds[i].fd == this->serverFd) {
		// Nouvelle connexion
		sockaddr_in addr = {};
		socklen_t addrlen = sizeof(addr);

		int clientFd = accept(this->serverFd, (sockaddr *) &addr, &addrlen);
		if (clientFd < 0)
			throw std::runtime_error("Error: accepting new client failed");
		if (clientFd >= 0) {
			// char *ip = inet_ntoa(addr.sin_addr);
			struct hostent *host = gethostbyaddr(&(addr.sin_addr), sizeof(addr.sin_addr), AF_INET);
    
			makeSocketNonBlock(clientFd);
			pollfd pfd = {clientFd, POLLIN, 0};
			this->pollFds.push_back(pfd);
			this->clients[clientFd] = new Client(clientFd, host->h_name);
			std::cout << "New client connected: " << clientFd << std::endl;
		}
	}
	else {
		// Message recu d'un client
		char buffer[BUFFUR_SIZE];
		int byteRead = recv(this->pollFds[i].fd, buffer, sizeof(buffer) - 1, 0);
		if (byteRead <= 0) {
			//Déconnexion
			std::cout << "Déconnexion: " << this->pollFds[i].fd << std::endl;
			this->disconectClient(this->clients[this->pollFds[i].fd]);
			// --i;
		}
		else {
			buffer[byteRead] = '\0';
			std::cout << "Message from: " << this->pollFds[i].fd << std::endl;
			this->handleClientMessage(this->clients[this->pollFds[i].fd], buffer);
		}
	}
}

void Server::handleClientMessage(Client *client, std::string const &message) {
	std::cout << "message: '" << message << "'" << std::endl;
	this->parseMess(message);
	if (client->getIsAuth() == false) {
		parserIt pass = this->searchForCmd("PASS");
		// std::cout << "pass: '" << this->passWord << "'" << std::endl;
		if (pass != this->parsedMessages.end() ) {
			if ((*pass).params[0] == this->passWord) {
				client->setIsAuth(true);
				std::cout << "Client authentificated: " << client->getFd() << std::endl;
				this->parsedMessages.erase(pass);
				this->handleCommands(client);				
			}
			else {
				std::cout << "Wrong password from: " << client->getFd() << std::endl;
				client->respond(ERR_PASSWDMISMATCH(client->getNickName()));
				this->disconectClient(client);
			}
		}
		else {
			client->respond(ERR_NOTREGISTERED(this->parsedMessages[0].command));
		}
	}
	else {
		// Gérer les commandes
		this->handleCommands(client);
	}
}

/* -------------------------------------------------------------------------- */
/*                                    Utils                                   */
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

void Server::disconectClient(Client *client) {
	pollFdIt pfdIt = searchForFd(client->getFd());
	int fdSaved = client->getFd();
	close(fdSaved);
	
	delete this->clients[client->getFd()];
	this->clients.erase(fdSaved);
	if (pfdIt != this->pollFds.end())
		this->pollFds.erase(pfdIt);
}

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