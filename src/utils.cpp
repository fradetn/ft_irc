/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asangerm <asangerm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:34:46 by nfradet           #+#    #+#             */
/*   Updated: 2025/01/07 19:18:10 by asangerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Server.hpp"

void makeSocketNonBlock(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

bool getStringUntil(const std::string& input, std::string& result, char delimiter, size_t& startPos) {
	// Vérifier si startPos dépasse la longueur de la chaîne
	if (startPos >= input.length()) {
		return false;
	}
	// Trouver la position de départ effective en sautant les delim
	size_t begin = startPos;
	while (begin < input.length() && input[begin] == delimiter) {
		++begin;
	}
	// Si tout est des delim ou que la chaîne est vide
	if (begin >= input.length()) {
		return false;
	}
	// Trouver la position du délimiteur ou la fin de la chaîne
	size_t end = begin;
	while (end < input.length() && input[end] != delimiter) {
		++end;
	}
	// Exclure les espaces en fin de mot
	size_t wordEnd = end;
	while (wordEnd > begin && input[wordEnd - 1] == delimiter) {
		--wordEnd;
	}
	// Extraire le résultat
	result = input.substr(begin, wordEnd - begin);
	// Mettre à jour startPos
	startPos = (end < input.length()) ? end + 1 : end;
	return !result.empty();
}

std::vector<std::string> split(std::string toSplit, char delim) {
	std::vector<std::string> splited;
	std::string res = "";
	size_t pos = 0;

	if (toSplit.empty())
		splited.push_back("");
	else {
		while (getStringUntil(toSplit, res, delim, pos)) {
			splited.push_back(res);
		}
	}
	
	return (splited);
}

void handle_shutdown(int sig) {
	(void) sig;
	if (Server::isRunning)
		Server::isRunning = false;
}

bool isDigits(const std::string& str) {
    if (str.empty()) return false; // Vérifie si la chaîne est vide
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it)) return false;
    }
    return true;
}
