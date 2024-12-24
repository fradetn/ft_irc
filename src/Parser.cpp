#include "includes.hpp"
#include "Parser.hpp"

Parser::Parser(/* args */) : prefix(""), command("") {
}

Parser::~Parser() {
}

void affStr(std::string str) {
	std::cout << "'" << str << "'" << std::endl;
}

void Parser::parseMessage(std::string message) {
	size_t pos = 0;

	// std::cout << "one message: '" << message << "'" << std::endl;
	this->fullCmd = message;
	if (message[0] == ':') {
		pos = message.find(' ');
		this->prefix = message.substr(1, pos - 1);
		message = message.substr(pos + 1);
	}
	pos = message.find(' ');
	if (pos == std::string::npos) {
		this->command = message.substr(0, message.find('\n'));
		message = "";
	}
	else {
		this->command = message.substr(0, pos);
		message = message.substr(pos + 1);
	}

	pos = message.find(":");
	if (pos != std::string::npos) {
		this->trailing = message.substr(pos + 1);
		message = message.substr(0, pos);
		this->params = split(message, ' ');
        // this->params.push_back(trailing);
	}
	else{
		this->params = split(message, ' ');
	}

	std::cout << "prefix: '" << this->prefix << "'" << std::endl;
	std::cout << "command: '" << this->command << "'" << std::endl;
	std::cout << "params: " << 
		for_each(this->params.begin(), this->params.end(), affStr)
		<< std::endl;
		std::cout << "trailing: '" << this->trailing << "'" << std::endl;
	std::cout << std::endl;
}
