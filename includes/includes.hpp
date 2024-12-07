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
#include <sys/socket.h>
#include <netinet/in.h>

#include "Server.hpp"
#include "Client.hpp"

#ifndef BUFFUR_SIZE
# define BUFFUR_SIZE 512
#endif

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 999
#endif

void makeSocketNonBlock(int fd);

#endif