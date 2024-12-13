/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:42:59 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/13 19:34:52 by nfradet          ###   ########.fr       */
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
	
	bool removeClient(Client *client);
	bool addNewClient(Client *newClient, std::string key);
	bool isClientInChan(Client *client);
	bool isClientBanned(Client *client);
	bool isOneAdminInChan();
};

#endif // CHANNEL_HPP