/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:04:59 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/09 17:36:55 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "includes.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"

Server::Server() : passWord("serveur42"), port(8080) {
	this->serverFd = -1;
	this->createSocket();
}

Server::Server(int _port, std::string _passWord) : passWord(_passWord), port(_port) {
	this->serverFd = -1;
	this->createSocket();
}

Server::~Server() {}

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
		int clientFd = accept(this->serverFd, NULL, NULL);
		if (clientFd >= 0) {
			makeSocketNonBlock(clientFd);
			pollfd pfd = {clientFd, POLLIN, 0};
			this->pollFds.push_back(pfd);
			this->clients[clientFd] = new Client(clientFd);
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
			close(this->pollFds[i].fd);
			delete this->clients[this->pollFds[i].fd];
			this->clients.erase(this->pollFds[i].fd);
			this->pollFds.erase(this->pollFds.begin() + i);
			--i;
		}
		else {
			buffer[byteRead] = '\0';
			std::cout << "Message from: " << this->pollFds[i].fd << std::endl;
			this->handleClientMessage(this->clients[this->pollFds[i].fd], buffer);
		}
	}
}

void Server::handleClientMessage(Client *client, std::string const &message) {
	// std::cout << "message: '" << message << "'" << std::endl;
	this->parseMess(message);
	if (client->getIsAuth() == false) {
		parserIt pass = this->searchForCmd("PASS");
		// std::cout << "pass: '" << this->passWord << "'" << std::endl;
		if (pass != this->parsedMessages.end() && (*pass).params[0] == this->passWord) {
				client->setIsAuth(true);
				std::cout << "Client authentificated: " << client->getFd() << std::endl;
				this->parsedMessages.erase(pass);
				this->handleCommands();
		}
		else {
			std::cout << "Wrong password from: " << client->getFd() << std::endl;
			close(client->getFd());
		}
	}
	else {
		// Gérer les commandes
		this->handleCommands();
	}
}

void Server::handleCommands() {
	parserIt it = this->parsedMessages.begin();
	while (this->parsedMessages.size() >= 1) {
		switch (getCmdType((*it).command)) {
			case CMD_NICK:
				std::cout << "NICK" << std::endl;
				break;
			case CMD_USER:
				std::cout << "USER" << std::endl;
				break;
			case CMD_QUIT:
				std::cout << "QUIT" << std::endl;
				break;
			case CMD_UNKNOWN:
				std::cout << "UNKNOWN" << std::endl;
				break;
		}
		this->parsedMessages.erase(it);
	}
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

	message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
	while (getStringUntil(message, res, '\n', pos)) {
		if (res != "") {
			parsed.parseMessage(res);
			this->parsedMessages.push_back(parsed);
		}
		res = "";
	}
}