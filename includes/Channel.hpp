/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:42:59 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/11 19:23:25 by nfradet          ###   ########.fr       */
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
	// std::vector<Client *> admins;
	std::map<Client *, bool> clientList;

	std::string key;
	size_t		limit;

public:
	Channel();
	Channel(Client *first, std::string _name);
	~Channel();
	
	std::string getName() const;
	
	void addNewClient(Client *newClient, std::string key);
	bool isClientInChan(Client *client);
};

#endif // CHANNEL_HPP