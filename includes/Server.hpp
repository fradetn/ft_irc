/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:37:11 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/23 17:26:31 by nfradet          ###   ########.fr       */
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
	void		handleCommands(Client *client);
	void		shutDown();
	void		disconectClient(Client *client);
	void		rmCliFromAllChan(Client *client);
	void 		respond(Client *client, std::string message);
	
	
	//=====================//
	//	   Server Utils	   //
	//=====================//
	
	std::set<Client *> getChanCommonUsers(Client *client);
	void		sendMessToAllCommonUsers(Client *client, std::string message);

	Client		*getClientByNick(std::string const &nickname);
	Client		*getClientByUser(std::string const &username);
	Channel		*getChannelByName(std::string const &name);
	std::string getPrefix();
	
	clientsIt 	searchForClient(Client *client);
	channelIt 	searchForChannel(Channel *channel);
	parserIt	searchForCmd(std::string cmd);
	pollFdIt 	searchForFd(int fd);
	
	void		parseMess(std::string message);

public:

	static bool					isRunning;

	Server();
	Server(int _port, std::string _passWord);
	~Server();

	void run();
	
	//============//
	//  COMMANDS  //
	//============//

	void cmdPass(Client *client, Parser cmd);
	void cmdNick(Client *client, Parser cmd);
	void cmdUser(Client *client, Parser cmd);
	void cmdQuit(Client *client, Parser cmd);
	void cmdJoin(Client *client, Parser cmd);
	void cmdPart(Client *client, Parser cmd);

};

#endif // SERVER_HPP
