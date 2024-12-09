# ft_irc

## Différents client IRC existants:
  * https://hexchat.github.io/ : Interface graphique conviviale </br>
  * https://weechat.org/       : Client IRC sous forme de terminal </br>
  * https://www.mirc.com/      : Client Windows très connu. </br>
 </br>

## Utilisation avec hexchat

### Commandes utilisateurs

* PASS : Usage: /pass \<password>, permet de se connecter au serveur avec le mot de passe
* NICK : Usage: /nick \<nickname>, défini votre nickname
* USER : /user \<username> \<mode> \<unused> :\<realname>, Commande pour définir un identifiant utilisateur
* QUIT : Usage: /quit [\<reason>], Déconnecte du serveur actuel
* JOIN : Usage: /join \<channel>, Rejoins le channel
* PRIVMSG  : Usage: /msg \<nick> \<message>, Envoie un message privé, message "."  à envoyer au dernier pseudo ou préfixe (avec "=" pour dcc chat)

## Commandes opérateurs

*  KICK   : Usage: /kick \<nick> [reason], Ejecter un client du channel
*  INVITE : Usage: /invite \<nick> [\<channel>], Inviter un client au channel, par défaut le channel actuel (needs chanop)
*  TOPIC  : Usage: /topic [\<topic>], Modifie le thème du channel s'il est donné, l'affiche sinon 
*  MODE   : Usage: /mode \<channel> \<+|->\<modes> [\<params>], Changer le mode du channel
   * i : Définir/supprimer le canal sur invitation uniquement
   * t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
   * k : Définir/supprimer la clé du canal (mot de passe)
   * o : Donner/retirer le privilège de l’opérateur de canal
   * l : Définir/supprimer la limite d’utilisateurs pour le canal

## Quelques fonctions utiles

### Pour les sockets

https://man7.org/linux/man-pages/man2/socket.2.html </br>
**socket(int domain, int type, int protocol)**: </br>
	</t>Créer un endpoint pour la communication.
	Retourne un descripteur de fichier qui reference cet endpoint
	 </br>*domain*: défini la famille de protocole a utiliser (AF_INET: IPV4, AF_INET6: IPV6) </br>*type*: indique le type de socket utilisé (SOCK_STREAM: TCP) </br>*protocol*: 0

https://linux.die.net/man/3/htons</br>
**htons(uint16_t hostshort)**:</br>
	Convertit l'entier unsigned short int hostshort de l'ordre des octets de l'hôte à l'ordre des octets du réseau.

https://man7.org/linux/man-pages/man2/bind.2.html</br>
**bind(int sockfd, const struct sockaddr \*addr, socklen_t addrlen)** :</br>
	Permet d'assigner l'adresse specifée par *addr* à la socket réferencée par *sockfd* le descripteur de fichier de la socket. *addrlen* spécifie la taille en bits, de l'adresse de la structure pointée par *addr*

https://man7.org/linux/man-pages/man2/listen.2.html </br>
**listen(int sockfd, int backlog)**: </br>
le socket *sockfd* est marqué comme passif, </br>
il sera utilisé pour accepter des connections entrantes avec <a href="https://man7.org/linux/man-pages/man2/accept.2.html">accept(2)</a>.

https://man7.org/linux/man-pages/man2/accept.2.html </br>
**accept(int sockfd, struct sockaddr \*_Nullable restrict addr, socklen_t \*_Nullable restrict addrlen)** :</br>
The accept() system call is used with connection-based socket
       types (SOCK_STREAM).  It extracts the first
       connection request on the queue of pending connections for the
       listening socket, sockfd, creates a new connected socket, and
       returns a new file descriptor referring to that socket.

https://man7.org/linux/man-pages/man2/poll.2.html </br>
**poll(struct pollfd \*fds, nfds_t nfds, int timeout)** :</br>
poll() performs a similar task to <a href="https://man7.org/linux/man-pages/man2/select.2.html">select(2)</a></br> 
it waits for one of a set of file descriptors to become ready to perform I/O

https://man7.org/linux/man-pages/man2/getsockname.2.html </br>
**getsockname(int sockfd, struct sockaddr *restriction addr, socklen_t *restriction addrlen)**:</br>
	Stocke l'adresse a laquelle est lié *sockfd* dans *addr*.
	</br>*addrlen*: indique la quantité d'espace (en octets) pointé par *addr*


https://linux.die.net/man/2/close </br>
**close(int fd)**: </br>
  ferme un descripteur de fichier 
	Retourne Zero en cas de succes, -1 s'il echoue en instanciant errno correctement.


setsockopt</br>
getprotobyname</br>
gethostbyname</br>
getaddrinfo</br>
freeaddrinfo</br>
connect</br>
ntohs</br>
inet_addr</br>
inet_ntoa</br>
send</br>
recv</br>
signal</br>
sigaction</br>
lseek</br>
fstat</br>
fcntl</br>

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

### Permettre la connection non bloquante des clients

Fonction makeSocketNonBlock et explication:
``` c++
void makeSocketNonBlock(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}
```
1. **int flags = fcntl(socket, F_GETFL, 0);** </br>
* La fonction *fcntl* permet de manipuler des file descriptors. </br>
* *socket*: le descripteur de fichier de la socket; </br>
* **F_GETFL**: permet de recuperrer les flags associés au socket. </br>
* 0: valeur par defaut car F_GETFL n'utilise pas ce parametre. </br>
2. fcntl(socket, F_SETFL, flags | O_NONBLOCK); </br>
 * *socket*: le descripteur de fichier de la socket; </br>
 * **F_SETFL**: permet de d'attribuer des flags au socket. </br>
 * **O_NONBLOCK**: Flag existant qui indique que le socket doit fonctionner en mode non-bloquant . </br>
 * **flags | O_NONBLOCK**: On ajoute aux flags de la socket, le flag O_NONBLOCK s'il n'y est pas. </br>

