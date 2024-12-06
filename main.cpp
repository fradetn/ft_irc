#include "Server.hpp"

void checkInput(int ac, char **av) {
	if (ac != 3)
		throw std::runtime_error("Error: Usage: ./ircserv <port> <password>");

	std::string str = av[1];
	for (size_t i = 0; i < str.size(); i++) {
		if (std::isdigit(str[i]) == 0)
			throw std::runtime_error("Error: <port> have to be numeric values");
	}
}

int main(int argc, char **argv) {
	try {
		checkInput(argc, argv);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
