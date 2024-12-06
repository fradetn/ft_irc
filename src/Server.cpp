/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:04:59 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/06 14:19:05 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : port(8080), passWord("serveur42") {
	this->serverFd = -1;
}

Server::Server(int _port, std::string _passWord) : port(_port), passWord(_passWord) {
	this->serverFd = -1;
}

Server::~Server() {
}

void Server::makeSocketNonBlock() {
	int flags = fcntl(this->serverFd, F_GETFL, 0);
	fcntl(this->serverFd, F_SETFL, flags | O_NONBLOCK);
}

void Server::createSocket(void) {
	this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
	 if (this->serverFd < 0) {
		throw std::runtime_error("Error: socket creation failed");
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
	this->makeSocketNonBlock();
}