/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:16:36 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/22 12:45:27 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Parser.hpp"
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
		(void) argc;
		checkInput(argc, argv);
		Server serv(std::atoi(argv[1]), argv[2]);
		serv.run();
		// Parser parse;
		
		// parse.parseMessage(argv[1]);
		// std::string res;
		// size_t pos = 0;
		// while(getStringUntil(argv[1], res, '\n', pos)) {
		// 	parse.parseMessage(res);
		// }

	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
