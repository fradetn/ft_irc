/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asangerm <asangerm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:42:59 by nfradet           #+#    #+#             */
/*   Updated: 2025/01/05 18:51:59 by asangerm         ###   ########.fr       */
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

	std::string getListMessage(Client *client);

	void writeInChan(Client *client, std::string message, bool sendToMe);
	
	bool removeClient(Client *client);
	bool addNewClient(Client *newClient, std::string key);
	bool isClientInChan(Client *client);
	bool isClientBanned(Client *client);
	bool isOneAdminInChan();
};

#endif // CHANNEL_HPP