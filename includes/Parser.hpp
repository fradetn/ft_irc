/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:33:28 by nfradet           #+#    #+#             */
/*   Updated: 2025/01/02 20:28:12 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include "includes.hpp"

class Parser
{
private:

public:
	std::string fullCmd;
	std::string prefix;
	std::string command;
    std::vector<std::string> params;
	std::string trailing;

	bool hasTrailing;

	Parser(/* args */);
	~Parser();

	void parseMessage(std::string message);
};

#endif // PARSER_HPP