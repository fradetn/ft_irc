#include "includes.hpp"

Parser::Parser(/* args */) : prefix(""), command("") {
}

Parser::~Parser() {
}

std::vector<std::string> split(std::string toSplit, char delim) {
	std::vector<std::string> splited;
	std::string res;
	size_t pos = 0;

	while (getStringUntil(toSplit, res, delim, pos)) {
		splited.push_back(res);
	}

	return (splited);
}

void affStr(std::string str) {
	std::cout << "'" << str << "'" << std::endl;
}

void Parser::parseMessage(std::string message) {
	size_t pos = 0;

	if (message[0] == ':') {
		pos = message.find(' ');
		this->prefix = message.substr(1, pos - 1);
		message = message.substr(pos + 1);
	}
	pos = message.find(' ');
	this->command = message.substr(0, pos);
	message = message.substr(pos + 1);

	pos = message.find(" :");
	if (pos != std::string::npos) {
		std::string trailing = message.substr(pos + 2);
		message = message.substr(0, pos);
		this->params = split(message, ' ');
        this->params.push_back(trailing);
	}
	else{
		this->params = split(message, ' ');

	}

	// std::cout << "prefix: '" << this->prefix << "'" << std::endl;
	// std::cout << "command: '" << this->command << "'" << std::endl;
	// std::cout << "params: " << 
	// 	for_each(this->params.begin(), this->params.end(), affStr)
	// 	<< std::endl;
	// std::cout << std::endl;
}