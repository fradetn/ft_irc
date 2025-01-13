/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:42:59 by nfradet           #+#    #+#             */
/*   Updated: 2025/01/13 17:01:19 by nfradet          ###   ########.fr       */
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
	std::map<Client *, bool> clientList;
	std::vector<Client *> banned;

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

	void	setTopic(std::string newTopic);

	std::string getListMessage(Client *client);

	void writeInChan(Client *client, std::string message);
	
	void banClient(Client *client);
	bool removeClient(Client *client);
	bool addNewClient(Client *newClient, std::string key);
	bool isClientInChan(Client *client);
	bool isClientAdmin(Client *client);
	bool isClientBanned(Client *client);
	bool isOneAdminInChan();
};

#endif // CHANNEL_HPP