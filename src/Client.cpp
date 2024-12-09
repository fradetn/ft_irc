// #include "includes.hpp"
#include "Client.hpp"

Client::Client() {
	this->fd = -1;
	this->nickName = "";
	this->userName = "";
	this->isAuth = false;
}

Client::Client(int _fd) {
	this->fd = _fd;
	this->nickName = "defaultNickname";
	this->userName = "defaultUsername";
	this->isAuth = false;
}

Client::~Client() {
}

/* -------------------------------------------------------------------------- */
/*                              Getters & setters                             */
/* -------------------------------------------------------------------------- */

int Client::getFd() {
	return (this->fd);
}
std::string Client::getNickName() {
	return (this->nickName);
}
std::string Client::getUserName() {
	return (this->userName);
}
bool Client::getIsAuth() {
	return (this->isAuth);
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