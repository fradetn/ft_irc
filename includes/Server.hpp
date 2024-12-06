#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

class Server
{
private:
	std::string passWord;
	unsigned int port;

	int serverFd;
	
public:
	Server();
	Server(int _port, std::string _passWord);
	~Server();

	void createSocket(void);
	void makeSocketNonBlock();
};

