/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:37:11 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/08 19:27:58 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"

class Client;
class Parser;


class Server
{
private:
	int 					serverFd;
	std::string 			passWord;
	unsigned int 			port;
	std::vector<pollfd> 	pollFds;
	std::map<int, Client*>	clients;

	std::vector<Parser>		parsedMessages;

	bool					isRunning;
	
	void	createSocket(void);
	void	handleEvent(size_t &i);
	Parser	searchForCmd(std::string cmd);
	void	parseMess(std::string message);
	void	handleClientMessage(Client *client, std::string const &message);

public:

	Server();
	Server(int _port, std::string _passWord);
	~Server();

	void run();

};
