#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <map>
#include <vector>
#include <poll.h>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"

#ifndef BUFFUR_SIZE
# define BUFFUR_SIZE 512
#endif

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 999
#endif

typedef std::vector<Parser>::iterator parserIt;

void 						makeSocketNonBlock(int fd);
std::vector<std::string>	split(std::string toSplit, char delim);
bool getStringUntil(const std::string& input, std::string& result, char delimiter, size_t& startPos);

#endif