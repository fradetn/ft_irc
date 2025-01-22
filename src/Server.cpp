/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:04:59 by nfradet           #+#    #+#             */
/*   Updated: 2025/01/20 11:41:13 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include "Channel.hpp"

bool Server::isRunning = true;

Server::Server() : passWord("serveur42"), port(8080) {
	this->serverFd = -1;
	this->createSocket();
	signal(SIGINT, handle_shutdown);
}

Server::Server(int _port, std::string _passWord) : passWord(_passWord), port(_port) {
	this->serverFd = -1;
	this->createSocket();
	signal(SIGINT, handle_shutdown);
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
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(this->port);

	if (bind(this->serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		close(this->serverFd);
		throw std::runtime_error("Error: bind failed");
	}	
	if (listen(this->serverFd, MAX_CLIENTS) < 0) {  // MAX_CLIENTS : nombre maximal de connexions en file d'attente
		close(this->serverFd);
		throw std::runtime_error("Error: listen failed");
	}
	// Récupérer l'adresse réelle liée au socket
	sockaddr_in actualAddr;
	socklen_t addrLen = sizeof(actualAddr);
	if (getsockname(this->serverFd, (struct sockaddr *)&actualAddr, &addrLen) == -1) {
		close(this->serverFd);
		throw std::runtime_error("Error: getsockname failed");
	}

	// Résolution du nom de l'hôte
	char hostBuffer[NI_MAXHOST];
	int ret = getnameinfo((struct sockaddr *)&actualAddr, sizeof(actualAddr),
							hostBuffer, sizeof(hostBuffer), NULL, 0, 0);
	if (ret != 0) {
		close(this->serverFd);
		throw std::runtime_error(std::string("Error: ") + gai_strerror(ret));
	}

	this->hostname = std::string(hostBuffer);

	makeSocketNonBlock(this->serverFd);
	pollfd pfd = {this->serverFd, POLLIN, 0};
	this->pollFds.push_back(pfd);
}

void Server::run() {
	std::cout << GREEN"Server listening on port "DEFAULT << this->port << std::endl;
	while (Server::isRunning) {
		int polCount = poll(this->pollFds.data(), this->pollFds.size(), -1);
		if (!Server::isRunning)
			continue;
		if (polCount < 0) {
			close(this->serverFd);
			throw std::runtime_error("Error: poll failed");
		}
		for (size_t i = 0; i < this->pollFds.size(); i++) {
			if (this->pollFds[i].revents == 0) 
				continue;
			if ((this->pollFds[i].revents & POLLIN) == POLLIN) {
				std::cout << std::endl << YELLOW"event happening on socket: "DEFAULT << this->pollFds[i].revents << std::endl;
				this->handleEvent(i);
				std::cout << std::endl;
			}
		}
	}
	this->shutDown();
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
			struct hostent *host = gethostbyaddr(&(addr.sin_addr), sizeof(addr.sin_addr), AF_INET);

			makeSocketNonBlock(clientFd);
			pollfd pfd = {clientFd, POLLIN | POLLOUT, 0};
			this->pollFds.push_back(pfd);
			this->clients[clientFd] = new Client(clientFd, host->h_name, &this->pollFds.back());
			std::cout << GREEN"New connection accepted"DEFAULT", socketFd: " << clientFd << std::endl;
		}
	}
	else {
		// Message recu d'un client
		
		Client *client = this->clients[this->pollFds[i].fd];
		char buffer[BUFFER_SIZE];
		int byteRead = recv(this->pollFds[i].fd, buffer, sizeof(buffer) - 1, 0);
		std::cout << GREEN"byteRead: '"DEFAULT << byteRead << GREEN"'"DEFAULT << std::endl;
		
		if (byteRead == -1)
			throw std::runtime_error("Error: recv failed");
		if (byteRead == 0) {
			//Déconnexion;
			this->sendMessToAllCommonUsers(client, RPL_QUIT(client->getPrefix(), "Leaving"));
			this->clients.erase(searchForClient(client));
			this->disconectClient(client);
			return;
		}
		buffer[byteRead] = '\0';

		client->appendToBuffer(buffer, byteRead);

		std::cout << GREEN"Message received from socket: "DEFAULT << this->pollFds[i].fd << std::endl;
		std::cout << GREEN"Message: '"DEFAULT << buffer << GREEN"'"DEFAULT << std::endl;
		std::cout << GREEN"Buffer: '"DEFAULT << client->getBuffer() << GREEN"'"DEFAULT << std::endl;
		std::string message;
		while (!client->getToBeDeleted() && (message = client->extractNextMessage()) != "") {
			std::cout << GREEN"Message complet: '"DEFAULT << message << GREEN"'"DEFAULT << std::endl;
			this->parseMess(message);
			this->handleCommands(client);
			std::cout << client << std::endl;
		}
		if (client->getToBeDeleted())
			this->disconectClient(client);
	}
}

void Server::shutDown() {
	std::map<int, Client *>::iterator it;
	for (it = this->clients.begin(); it != this->clients.end(); ++it) {
		it->second->write(ERR_SHUTDOWN);
		this->disconectClient(it->second);
	}
	close(this->serverFd);
	this->clients.clear();
	std::cout << std::endl;
}

void Server::disconectClient(Client *client) {
	pollFdIt pfdIt = searchForFd(client->getFd());
	std::cout << RED"Disconnecting client on socket: "DEFAULT << client->getFd() << std::endl;
	this->rmCliFromAllChan(client);
	close(client->getFd());
	delete client;
	if (pfdIt != this->pollFds.end())
		this->pollFds.erase(pfdIt);
}

void Server::rmCliFromAllChan(Client *client) {
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

void Server::respond(Client *client, std::string message) {
	std::string prefix = this->getPrefix();
	if (prefix.empty())
		client->write(message);
	else
		client->write(":" + prefix + " " + message);
}
