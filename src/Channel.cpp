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
	this->clientList[first] = true;
	this->key = "";
	this->limit = 0;
}

Channel::~Channel() {}

/* -------------------------------------------------------------------------- */
/*                                  Functions                                 */
/* -------------------------------------------------------------------------- */

std::string Channel::getName() const {
	return (this->name);
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

void Channel::addNewClient(Client *newClient, std::string _key) {
	if (this->isClientBanned(newClient)) {
		newClient->respond(ERR_BANNEDFROMCHAN(this->name));
		return;
	}
	if (this->limit != 0 && this->clientList.size() >= this->limit) {
		newClient->respond(ERR_CHANNELISFULL(this->name));
		return;
	}
	if (!this->isClientInChan(newClient)) {
		if (this->key.empty())
			this->clientList[newClient] = false;
		else if (this->key == _key) {
			this->clientList[newClient] = false;
		}
		else
			newClient->respond(ERR_BADCHANNELKEY(this->name));
	}
}

bool Channel::removeClient(Client *client) {
	if (this->isClientInChan(client) == true) {
		this->clientList.erase(client);
		if (this->isOneAdminInChan() == false)
			return (false); // Return false si le client etait le seul admin du channel
	}
	return (true);
}
