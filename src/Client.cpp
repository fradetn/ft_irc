// #include "includes.hpp"
#include "Client.hpp"

/* -------------------------------------------------------------------------- */
/*                         Constructors & destructor                          */
/* -------------------------------------------------------------------------- */

Client::Client() {
	this->fd = -1;
	this->nickName = "";
	this->userName = "";
	this->isAuth = false;
}

Client::Client(int _fd, std::string const &_hostname) {
	this->fd = _fd;
	this->nickName = "";
	this->userName = "";
	this->hostName = _hostname;
	this->isAuth = false;
}

Client::~Client() {
}

/* -------------------------------------------------------------------------- */
/*                              Getters & setters                             */
/* -------------------------------------------------------------------------- */

int Client::getFd() const {
	return (this->fd);
}
std::string Client::getNickName() const {
	return (this->nickName);
}
std::string Client::getUserName() const {
	return (this->userName);
}
bool Client::getIsAuth() const {
	return (this->isAuth);
}

std::string Client::getHostName() const {
	return (this->hostName);
}

void Client::setFd(int _fd) {
	this->fd = _fd;
}
void Client::setNickName(std::string _nickName) {
	this->nickName = _nickName;
}
void Client::setUserName(std::string _userName) {
	this->userName = _userName;
}
void Client::setIsAuth(bool _isAuth) {
	this->isAuth = _isAuth;
}

/* -------------------------------------------------------------------------- */
/*                                  Functions                                 */
/* -------------------------------------------------------------------------- */

std::string Client::getPrefix() const {
	std::string username = this->userName.empty() ? "" : "!" + this->userName;
	std::string hostname = this->hostName.empty() ? "" : "@" + this->hostName;

	return (this->nickName + username + hostname);
}

void Client::write(std::string const &message) const {
	std::string buf = message + "\r\n";
	if (send(this->fd, buf.c_str(), buf.length(), 0) == -1)
		throw std::runtime_error("Error: sending a message failed");
}

void Client::respond(std::string const &message) const {
	this->write(":" + this->getPrefix() + " " + message);
}