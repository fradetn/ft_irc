#include "includes.hpp"
#include "Channel.hpp"
#include "Client.hpp"

/* -------------------------------------------------------------------------- */
/*                          Constructors & destructor                         */
/* -------------------------------------------------------------------------- */

Channel::Channel() {
	this->key = "";
	this->limit = 0;
}

Channel::Channel(Client *first, std::string _name) {
	this->name = _name;
	this->topic = "default";
	this->clientList[first] = true;
	this->key = "";
	this->limit = 0;
}

Channel::Channel(Client *first, std::string _name, std::string _key) {
	this->name = _name;
	this->topic = "default";
	this->clientList[first] = true;
	this->key = _key;
	this->limit = 0;
}

Channel::~Channel() {}

/* -------------------------------------------------------------------------- */
/*                                  Functions                                 */
/* -------------------------------------------------------------------------- */

std::string Channel::getName() const {
	return (this->name);
}

std::string Channel::getTopic() const {
	return (this->topic);
}

std::vector<Client *> Channel::getClients() const{
	std::vector<Client *> clients;
	std::map<Client *, bool>::const_iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); ++it) {
		clients.push_back((*it).first);
	}
	return (clients);
}

std::string Channel::getListMessage(Client *client) {
	std::string list = "353 " + client->getNickName();
	if (true) // verifier l'état du channel
		list += " = " + this->getName() + " :";
	
	std::map<Client *, bool>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); ++it) {
		if (it->second == true)
			list += "@";
		list += it->first->getNickName() + " ";
	}
	list.erase(list.size() - 1);
	return (list);
}

bool Channel::isClientInChan(Client *client) {
	std::map<Client *, bool>::iterator it;

	for (it = this->clientList.begin(); it != this->clientList.end(); ++it)
		if (client == (*it).first)
			return (true);
	return (false);
}

bool Channel::isClientBanned(Client *client) {
	std::vector<Client *>::iterator it;

	for (it = this->banned.begin(); it != this->banned.end(); ++it)
		if (client == (*it))
			return (true);
	return (false);
}

bool Channel::isOneAdminInChan() {
	std::map<Client *, bool>::iterator it;

	for (it = this->clientList.begin(); it != this->clientList.end(); ++it)
		if ((*it).second == true)
			return (true);
	return (false);
}

bool Channel::isClientAdmin(Client *client) {
	std::map<Client *, bool>::iterator it;

	for (it = this->clientList.begin(); it != this->clientList.end(); ++it)
		if (client == (*it).first && (*it).second == true)
			return (true);
	return (false);
}

bool Channel::isClientInvited(Client *client) {
	std::vector<Client *>::iterator it;

	for (it = this->invited.begin(); it != this->invited.end(); ++it)
		if (client == (*it))
			return (true);
	return (false);
}

void	Channel::setTopic(std::string newTopic) {
	this->topic = newTopic;
}

void	Channel::setMods(Client *client, char sign, char mod) {
	if (sign == '-') {
		if (mod == 't')
			this->writeInChan(client, RPL_MODE(this->name, "-t "), true);
		if (mod == 'i')
			this->writeInChan(client, RPL_MODE(this->name, "-i "), true);
		this->mods.erase(mod);
	}
	else {
		if (mod == 't')
			this->writeInChan(client, RPL_MODE(this->name, "+t "), true);
		if (mod == 'i')
			this->writeInChan(client, RPL_MODE(this->name, "+i "), true);
		this->mods.insert(mod);
	}
}

void	Channel::setKey(Client *client,  char sign, std::string _key) {
	if (sign == '-') {
		if (_key == this->key && this->mods.count('k') == 1) {
			this->key.clear();
			this->setMods(client, sign, 'k');
			this->writeInChan(client, RPL_MODE(this->name, "-k "), true);
		}
	}
	else {
		if (_key != this->key) {
			this->key = _key;
			this->setMods(client, sign, 'k');
			this->writeInChan(client, RPL_MODE(this->name, "+k " + _key), true);
		}
	}
}

void	Channel::setLimit(Client *client, char sign, std::string _limit) {
	if (sign == '-') {
		std::cout << "test 1" << std::endl;
		if (this->mods.count('l') == 1)
		{
			std::cout << "test 2" << std::endl;
			this->limit = 1;
			this->setMods(client, sign, 'l');
			std::cout << "test 3" << std::endl;
			this->writeInChan(client, RPL_MODE(this->name, "-l "), true);
		}
	}
	else {
		if (isDigits(_limit)) {
			size_t l = atoi(_limit.c_str());
			if (l != this->limit) {
				this->limit = l;
				this->setMods(client, sign, 'l');
				this->writeInChan(client, RPL_MODE(this->name, "+l " + _limit), true);
			}
		}
	}
}

void	Channel::setOperator(Client *client, Client *target, char sign) {
	std::map<Client *, bool>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); ++it) {
		if (it->first == target) {
			if (sign == '-' && it->second == true) {
				it->second = false;
				this->writeInChan(client, RPL_MODE(this->name, "-o " + target->getNickName()), true);
			}
			else if (sign == '+' && it->second == false) {
				it->second = true;
				this->writeInChan(client, RPL_MODE(this->name, "+o " + target->getNickName()), true);
			}
		}
	}
}

bool Channel::addNewClient(Client *newClient, std::string _key) {
	if (this->isClientBanned(newClient)) {
		std::cout << RED"ERR_BANNEDFROMCHAN"DEFAULT << std::endl;
		newClient->respond(ERR_BANNEDFROMCHAN(newClient->getNickName(), this->name));
		return (false);
	}
	if (this->limit != 0 && this->clientList.size() >= this->limit) {
		std::cout << RED"ERR_CHANNELISFULL"DEFAULT << std::endl;
		newClient->respond(ERR_CHANNELISFULL(newClient->getNickName(), this->name));
		return (false);
	}
	if (!this->isClientInChan(newClient)) {
		if (this->mods.count('i') > 0) {
			if (this->isClientInvited(newClient)) {
				this->clientList[newClient] = false;
				return (true);
			}
			else {
				std::cout << RED"ERR_INVITEONLYCHAN"DEFAULT << std::endl;
				newClient->respond(ERR_INVITEONLYCHAN(this->name));
				return (false);
			}
		}
		if (this->key.empty()) {
			this->clientList[newClient] = false;
			return (true);
		}
		else if (this->key == _key) {
			this->clientList[newClient] = false;
			return (true);
		}
		else {
			std::cout << RED"ERR_BADCHANNELKEY"DEFAULT << std::endl;
			newClient->respond(ERR_BADCHANNELKEY(newClient->getNickName(), this->name));
			return (false);
		}
	}
	return (false);
}

void Channel::banClient(Client *client) {
	if (this->isClientInChan(client) == true) {
		this->clientList.erase(client);
		this->banned.push_back(client);
	}
}

bool Channel::removeClient(Client *client) {
	if (this->isClientInChan(client) == true) {
		this->clientList.erase(client);
		if (this->clientList.empty())
			return (false); // Return false si le client etait le dernier du channel
	}
	return (true);
}

void Channel::writeInChan(Client *client, std::string message, bool sendToMe) {
	// avertir tous les clients
	std::map<Client *, bool>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); ++it) {
		if (client != it->first || sendToMe)
			it->first->write(":" + client->getPrefix() + " " + message);
	}
}