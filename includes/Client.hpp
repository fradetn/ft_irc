/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfradet <nfradet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:52:41 by nfradet           #+#    #+#             */
/*   Updated: 2024/12/07 11:51:13 by nfradet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"

class Client
{
private:
	int 		fd;
	std::string	nickName;
	std::string	userName;
	bool		isAuth;

public:
	Client();
	Client(int fd);
	~Client();

	int getFd();
	std::string getNickName();
	std::string getUserName();
	bool getIsAuth();

	void setFd(int _fd);
	void setNickName(std::string _nickName);
	void setUserName(std::string _userName);
	void setIsAuth(bool _isAuth);
};

