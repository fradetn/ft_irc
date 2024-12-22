/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:37:11 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/22 21:55:34 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP


#include "includes.hpp"

class Client;
class Parser;
class Channel;

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
	std::vector<Channel*>	channels;
	std::string				hostname;
	
	std::vector<Parser>		parsedMessages;

	
	//=====================//
	//  Private Functions  //
	//=====================//

	void		createSocket(void);
	void		handleEvent(size_t &i);
	void		handleClientMessage(Client *client, std::string const &message);
	void		handleCommands(Client *client);
	// void 		newHandleCommands(Client *client);
	void		rmCliFromAllChan(Client *client);
	void		shutDown();
	
	std::set<Client *> getChanCommonUsers();
	Client		*getClientByNick(std::string const &nickname);
	Client		*getClientByUser(std::string const &username);
	Channel		*getChannelByName(std::string const &name);
	clientsIt 	searchForClient(Client *client);
	parserIt	searchForCmd(std::string cmd);
	pollFdIt 	searchForFd(int fd);
	void		parseMess(std::string message);
	void		disconectClient(Client *client);
	
	void 		respond(Client *client, std::string message);
	std::string getPrefix();

public:

	static bool					isRunning;

	Server();
	Server(int _port, std::string _passWord);
	~Server();

	void run();
	
	//============//
	//  COMMANDS  //
	//============//
	
	void cmdNick(Client *client, Parser cmd);
	void cmdUser(Client *client, Parser cmd);
	void cmdQuit(Client *client, Parser cmd);
	void cmdJoin(Client *client, Parser cmd);

};

#endif // SERVER_HPP
