/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:34:46 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/09 17:16:34 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

void makeSocketNonBlock(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

bool getStringUntil(const std::string& input, std::string& result, char delimiter, size_t& startPos) {
   // Vérifier si startPos dépasse la longueur de la chaîne
    if (startPos >= input.length()) {
        return false;
    }
    // Trouver la position de départ effective en sautant les espaces
    size_t begin = startPos;
    while (begin < input.length() && input[begin] == delimiter) {
        ++begin;
    }
    // Si tout est des espaces ou que la chaîne est vide
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
	std::string res;
	size_t pos = 0;

	while (getStringUntil(toSplit, res, delim, pos)) {
		splited.push_back(res);
	}

	return (splited);
}

e_cmdType getCmdType(const std::string& command) {
    if (command == "NICK") return CMD_NICK;
    if (command == "USER") return CMD_USER;
    if (command == "QUIT") return CMD_QUIT;
    return CMD_UNKNOWN;
}