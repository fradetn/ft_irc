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
	this->isLog = false;
	this->toBeDeleted = false;
	this->buffer = "";
}

Client::Client(int _fd, std::string const &_hostname, pollfd *_pfd) {
	this->fd = _fd;
	this->nickName = "";
	this->userName = "";
	this->hostName = _hostname;
	this->pfd = _pfd;
	this->isAuth = false;
	this->isLog = false;
	this->toBeDeleted = false;
	this->buffer = "";
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
bool Client::getIsLog() const {
	return (this->isLog);
}
std::string Client::getHostName() const {
	return (this->hostName);
}
std::string Client::getBuffer() const {
	return (this->buffer);
}
bool Client::getToBeDeleted() const {
	return (this->toBeDeleted);
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
void Client::setIsLog(bool _isLog) {
	this->isLog = _isLog;
}
void Client::setToBeDeleted(bool _toBeDeleted) {
	this->toBeDeleted = _toBeDeleted;
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
	if ((this->pfd->revents & POLLOUT) == POLLOUT) {
		std::string buf = message + "\r\n";
		if (send(this->fd, buf.c_str(), buf.length(), 0) == -1)
			throw std::runtime_error("Error: sending a message failed");
	}
}

void Client::respond(std::string const &message) const {
	std::string prefix = this->getPrefix();

	if (prefix.empty())
		this->write(message);
	else
		this->write(":" + prefix + " " + message);
}

void Client::appendToBuffer(const char *receiveBuffer, size_t length) {
	this->buffer.append(receiveBuffer, length);
}

std::string Client::extractNextMessage() {
	size_t pos = this->buffer.find("\n");
	if (pos != std::string::npos) {
		std::string message = this->buffer.substr(0, pos);
		this->buffer.erase(0, pos + 1);
		return message;
	}
	return "";
}
