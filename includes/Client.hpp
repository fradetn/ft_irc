/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:52:41 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/11 17:47:42 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "includes.hpp"

class Client
{

private:
	int 		fd;
	std::string	nickName;
	std::string	userName;
	std::string	hostName;
	bool		isAuth;

public:
	Client();
	Client(int _fd, std::string const &_hostname);
	~Client();

	int getFd() const;
	std::string getNickName() const;
	std::string getUserName() const;
	std::string getHostName() const;
	bool getIsAuth() const;

	void setFd(int _fd);
	void setNickName(std::string _nickName);
	void setUserName(std::string _userName);
	// void setHostName(std::string _hostname);
	void setIsAuth(bool _isAuth);
	
	std::string getPrefix() const;
	void write(std::string const &message) const;
	void respond(std::string const &message) const;
};

#endif // CLIENT_HPP