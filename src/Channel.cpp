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

void	Channel::setTopic(std::string newTopic) {
	this->topic = newTopic;
}

bool Channel::addNewClient(Client *newClient, std::string _key) {
	if (this->isClientBanned(newClient)) {
		newClient->respond(ERR_BANNEDFROMCHAN(this->name));
		return (false);
	}
	if (this->limit != 0 && this->clientList.size() >= this->limit) {
		newClient->respond(ERR_CHANNELISFULL(this->name));
		return (false);
	}
	if (!this->isClientInChan(newClient)) {
		if (this->key.empty()) {
			this->clientList[newClient] = false;
			return (true);
		}
		else if (this->key == _key) {
			this->clientList[newClient] = false;
			return (true);
		}
		else {
			newClient->respond(ERR_BADCHANNELKEY(this->name));
			return (false);
		}
	}
	return (false);
}

bool Channel::removeClient(Client *client) {
	if (this->isClientInChan(client) == true) {
		this->clientList.erase(client);
		if (this->clientList.empty())
			return (false); // Return false si le client etait le dernier du channel
	}
	return (true);
}

void Channel::writeInChan(Client *client, std::string message) {
	// avertir tous les clients
	std::map<Client *, bool>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); ++it) {
		// if (client != it->first)
		it->first->write(":" + client->getPrefix() + " " + message);
	}
}