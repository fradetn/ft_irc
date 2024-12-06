# ft_irc

## Différents client IRC existants:
  * https://hexchat.github.io/ : Interface graphique conviviale </br>
  * https://weechat.org/       : Client IRC sous forme de terminal </br>
  * https://www.mirc.com/      : Client Windows très connu. </br>
 </br>

## Quelques fonctions utiles

https://man7.org/linux/man-pages/man2/socket.2.html </br>
**socket(int domain, int type, int protocol)**: </br>
	</t>Créer un endpoint pour la communication.
	Retourne un descripteur de fichier qui reference cet endpoint
	 </br>*domain*: défini la famille de protocole a utiliser (AF_INET: IPV4, AF_INET6: IPV6) </br>*type*: indique le type de socket utilisé (SOCK_STREAM: TCP) </br>*protocol*: 0

https://man7.org/linux/man-pages/man2/bind.2.html</br>
**bind(int sockfd, const struct sockaddr \*addr, socklen_t addrlen)** :</br>
	permet d'assigner l'adresse specifée par *addr* à la socket réferencée par *sockfd* le descripteur de fichier de la socket. *addrlen* spécifie la taille en bits, de l'adresse de la structure pointée par *addr*

https://man7.org/linux/man-pages/man2/listen.2.html </br>
**listen(int sockfd, int backlog)**: </br>
le socket *sockfd* est marqué comme passif, </br>
il sera utilisé pour accepter des connections entrantes avec <a href="https://man7.org/linux/man-pages/man2/accept.2.html">accept(2)</a>.


https://linux.die.net/man/2/close </br>
**close(int fd)**: </br>
  ferme un descripteur de fichier 
	Retourne Zero en cas de succes, -1 s'il echoue en instanciant errno correctement.

https://man7.org/linux/man-pages/man2/getsockname.2.html </br>
**getsockname(int sockfd, struct sockaddr *restriction addr, socklen_t *restriction addrlen)**:</br>
	Stocke l'adresse a laquelle est lié *sockfd* dans *addr*.
	</br>*addrlen*: indique la quantité d'espace (en octets) pointé par *addr*
 
setsockopt</br>
getprotobyname</br>
gethostbyname</br>
getaddrinfo</br>
freeaddrinfo</br>
connect</br>
accept</br>
htons</br>
htonl</br>
ntohs</br>
ntohl</br>
inet_addr</br>
inet_ntoa</br>
send</br>
recv</br>
signal</br>
sigaction</br>
lseek</br>
fstat</br>
fcntl</br>
poll</br>

## Étapes de base pour utiliser des sockets en C++ (coté serveur)

### 1. Inclure les en-têtes nécessaires
``` c++
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
```
### 2. Création du socket

* Utiliser la fonction socket() pour créer un socket.
* Spécifiez le domaine, le type, et le protocole :
	* *Domaine* : AF_INET pour IPv4, AF_INET6 pour IPv6.
	* *Type* : SOCK_STREAM (TCP) ou SOCK_DGRAM (UDP).
	* *Protocole* : Généralement 0 (par défaut pour TCP/UDP).

``` cpp
int serverFd = socket(AF_INET, SOCK_STREAM, 0);
if (serverFd < 0) {
	throw std::runtime_error("Error: socket creation failed");
}
```
### 3. Lier le socket à une adresse (serveur uniquement)

* Utiliser bind() pour associer le socket à une adresse IP et à un port.
* Une structure sockaddr_in est utilisée pour définir l'adresse.

``` cpp
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);  // Port 8080
server_addr.sin_addr.s_addr = INADDR_ANY; // Accepte toutes les connexions entrantes

if (bind(serverFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
	close(serverFd);
	throw std::runtime_error("Error: bind failed");
}
```

### 4. Écouter et accepter des connexions (serveur)

* Utiliser listen() pour indiquer que le serveur attend des connexions.
* Utiliser accept() pour accepter une connexion entrante.

``` cpp
if (listen(sock, 5) < 0) {  // 5 : nombre maximal de connexions en file d'attente
	close(serverFd);
	throw std::runtime_error("Error: listen failed");
}

int client_sock;
struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);

client_sock = accept(serverFd, (struct sockaddr*)&client_addr, &client_len);
if (client_sock < 0) {
	close(serverFd);
	throw std::runtime_error("Error: accept failed");
}

std::cout << "Connexion acceptée\n";
```
