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
	* domain: défini la famille de protocole a utiliser (AF_INET: IPV4, AF_INET6: IPV6) *type: indique le type de socket utilisé (SOCK_STREAM) *protocol: 0

https://linux.die.net/man/2/close </br>
**close(int fd)**: </br>
  ferme un descripteur de fichier 
	Retourne Zero en cas de succes, -1 s'il echoue en instanciant errno correctement.

https://man7.org/linux/man-pages/man2/getsockname.2.html </br>
**getsockname(int sockfd, struct sockaddr *restriction addr, socklen_t *restriction addrlen)**:</br>
	Stocke l'adresse a laquelle est lié sockfd dans addr.
	addrlen: indique la quantité d'espace (en octets) pointé par addr

setsockopt</br>
getprotobyname</br>
gethostbyname</br>
getaddrinfo</br>
freeaddrinfo</br>
bind</br>
connect</br>
listen</br>
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
