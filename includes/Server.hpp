/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:37:11 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/10 22:28:37 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP


#include "includes.hpp"

class Client;
class Parser;

class Server
{
private:

	//=====================//
	//  Private attributs  //
	//=====================//

	int 					serverFd;
	std::string 			passWord;
	unsigned int 			port;
	std::vector<pollfd> 	pollFds;
	std::map<int, Client*>	clients;

	std::vector<Parser>		parsedMessages;

	bool					isRunning;
	
	//=====================//
	//  Private Functions  //
	//=====================//
	
	Client		*getClient(std::string const &nickname);
	parserIt	searchForCmd(std::string cmd);
	void		parseMess(std::string message);

	void		createSocket(void);
	void		handleEvent(size_t &i);
	void		handleClientMessage(Client *client, std::string const &message);
	void		handleCommands(Client *client);

public:

	Server();
	Server(int _port, std::string _passWord);
	~Server();

	void run();
	
	//============//
	//  COMMANDS  //
	//============//
	
	void cmdNick(Client *client, Parser cmd);
	// void cmdUser(Client *client, Parser cmd);
	// void cmdQuit(Client *client, Parser cmd);
	// void cmdJoin(Client *client, Parser cmd);

};

#endif // SERVER_HPP
