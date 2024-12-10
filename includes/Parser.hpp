/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:33:28 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/10 20:59:06 by nfradet          ###   ########.fr       */
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

	Parser(/* args */);
	~Parser();

	void parseMessage(std::string message);
};

#endif // PARSER_HPP