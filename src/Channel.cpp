#include "includes.hpp"
#include "Channel.hpp"
#include "Client.hpp"

/* -------------------------------------------------------------------------- */
/*                          Constructors & destructor                         */
/* -------------------------------------------------------------------------- */

Channel::Channel() {
	this->key = "";
	this->limit = -1;
}

Channel::Channel(Client *first, std::string _name) {
	this->name = _name;
	this->clientList[first] = true;
	this->key = "";
	this->limit = -1;
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

void Channel::addNewClient(Client *newClient, std::string key) {
	if (this->isClientInChan(newClient) == false)
		this->clientList[newClient] = false;
}