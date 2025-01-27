/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:37:11 by nfradet           #+#    #+#             */
/*   Updated: 2025/01/27 19:52:08 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "includes.hpp"

class Server
{
private:

	//=====================//
	//  Private attributs  //
	//=====================//

	int 					serverFd;
	std::string 			passWord;
	unsigned int 			port;
	std::deque<pollfd> 		pollFds;
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
	
	std::string			getPrefix();
	void				parseMess(std::string message);
	std::set<Client *>	getChanCommonUsers(Client *client);
	void				sendMessToAllCommonUsers(Client *client, std::string message);

	/* ------------ Getters that return pointers ----------- */
	Client		*getClientByNick(std::string const &nickname);
	Client		*getClientByUser(std::string const &username);
	Channel		*getChannelByName(std::string const &name);
	
	/* ----------- Getters that return iterators ----------- */
	clientsIt 	searchForClient(Client *client);
	channelIt 	searchForChannel(Channel *channel);
	parserIt	searchForCmd(std::string cmd);
	pollFdIt 	searchForFd(int fd);

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
	void cmdCap(Client *client, Parser cmd);
	void cmdNick(Client *client, Parser cmd);
	void cmdUser(Client *client, Parser cmd);
	void cmdQuit(Client *client, Parser cmd);
	void cmdJoin(Client *client, Parser cmd);
	void cmdPart(Client *client, Parser cmd);
	void cmdTopic(Client *client, Parser cmd);
	void cmdPriv(Client *client, Parser cmd);
	void cmdNotice(Client *client, Parser cmd);
	void cmdMode(Client *client, Parser cmd);
	void cmdKick(Client *client, Parser cmd);
	void cmdInvite(Client *client, Parser cmd);

};

#endif // SERVER_HPP
