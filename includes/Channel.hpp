/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asangerm <asangerm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:42:59 by nfradet           #+#    #+#             */
/*   Updated: 2025/01/16 17:20:22 by asangerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "includes.hpp"

class Client;

class Channel
{
private:
	std::string name;
	std::string topic;

	//		 client  , isAdmin
	std::map<Client *, bool>	clientList;
	std::vector<Client *>		banned;
	std::set<char>				mods;

	std::string key;
	size_t		limit;

public:
	Channel();
	Channel(Client *first, std::string _name);
	Channel(Client *first, std::string _name, std::string _key);
	~Channel();
	
	std::string getName() const;
	std::string getTopic() const;
	std::vector<Client *> getClients() const;
	std::string getKey() const;
	std::set<char> getMods() const;
	std::string getModsForReply() const;

	void	setTopic(std::string newTopic);
	void	setMods(Client *client, char sign, char mod);
	void	setKey(Client *client, char sign, std::string _key);
	void	setLimit(Client *client, char sign, std::string _limit);
	void	setOperator(Client *client, Client *target, char sign);

	std::string getListMessage(Client *client);

	void writeInChan(Client *client, std::string message, bool sendToMe);
	
	bool removeClient(Client *client);
	bool addNewClient(Client *newClient, std::string key);
	bool isClientInChan(Client *client);
	bool isClientAdmin(Client *client);
	bool isClientBanned(Client *client);
	bool isOneAdminInChan();
};

#endif // CHANNEL_HPP