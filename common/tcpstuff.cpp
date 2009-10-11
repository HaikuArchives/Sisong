
#include <OS.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <netdb.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../common/basics.h"
#include "tcpstuff.fdh"

struct sockaddr_in sain;
bool AbortConnect = false;


int senddata(short sock, char *packet, int len)
{
	while(!chkwrite(sock)) { snooze(10); }
	return send(sock, packet, len, 0);
}

int sendstr(short sock, char *str)
{
	return senddata(sock, str, strlen(str));
}


int chkread(short sock)
{
fd_set readfds;
struct timeval poll;
int sr;

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);

	memset((char *)&poll, 0, sizeof(poll));
	sr = select(sock+1, &readfds, (fd_set *)0, (fd_set *)0, &poll);
	return sr;
}

int chkwrite(short sock)
{
fd_set writefds;
struct timeval poll;
int sr;

	FD_ZERO(&writefds);
	FD_SET(sock, &writefds);

	memset((char *)&poll, 0, sizeof(poll));
	sr = select(sock+1, (fd_set *)0, &writefds, (fd_set *)0, &poll);
	return sr;
}

/*
void c------------------------------() {}
*/

char *decimalip(unsigned int ip)
{
static char buffer[800];
	sprintf(buffer, "%d.%d.%d.%d",
				(ip>>24)&255, (ip>>16)&255, (ip>>8)&255, ip&255);
	return buffer;
}

//uint net_ipfromstring(char *ip)	{ return inet_addr(ip); }


unsigned long net_dnslookup(const char *host)
{
struct hostent *hosten;
unsigned long ip;

	if (host)
		stat("Resolving '%s' via DNS...", host);

	// attempt to resolve the hostname via DNS
	hosten = gethostbyname(host);
	if (hosten == NULL)
	{
		staterr("dnslookup: failed to resolve host: '%s'.", host);
		return 0;
	}
	else
	{
		memcpy((char*)&ip, hosten->h_addr_list[0], 4);
		return ntohl(ip);
	}
}

/*
void c------------------------------() {}
*/

// attempt to connect to ip:port, and return the socket number if successful (else 0)
uint connect_tcp(uint ip, ushort port, int timeout_us)
{
long arg;
int conn_socket;
bool connected;

	if (!(conn_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		staterr("connect_tcp: Failed to create socket.");
		return 1;
	}

	// Set non-blocking
	if((arg = fcntl(conn_socket, F_GETFL, NULL)) < 0)
	{
		staterr("connect_tcp: Error fcntl(..., F_GETFL) (%s)", strerror(errno));
		return 0;
	}
	arg |= O_NONBLOCK;
	if( fcntl(conn_socket, F_SETFL, arg) < 0)
	{
		staterr("connect_tcp: Error fcntl(..., F_SETFL) (%s)", strerror(errno));
		return 0;
	}

	sain.sin_addr.s_addr = htonl(ip);
	sain.sin_port = htons(port);
	sain.sin_family = AF_INET;

	stat("entering connect loop...");

	#define TICK_BASE	(10 * 1000)
	connected = false;
	AbortConnect = false;

	while(!AbortConnect)
	{
		int result = connect(conn_socket, (struct sockaddr *)&sain, sizeof(struct sockaddr_in));

		if (errno == EISCONN || result != -1)
		{
			connected = true;
			break;
		}

		if (errno == EINTR) break;

		if (timeout_us >= 0)
		{
			usleep(TICK_BASE);
			timeout_us -= TICK_BASE;
		}
		else break;
    }

	AbortConnect = false;

    if (connected)
	{
		return conn_socket;
	}

	staterr("connect_tcp: Connect timeout, abort, or failure connecting to %08x:%d\n", ip, port);
	close(conn_socket);
	return 0;
}

void abort_connect()
{
	AbortConnect = true;
}

/*
void c------------------------------() {}
*/

// create a socket and have it listen on all available interfaces.
uint GetTCPServerSocket(ushort port)
{
int sock;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!sock)
	{
		staterr("GetTCPServerSocket: failed to create socket!");
		return 0;
	}

	if (net_listen(sock, INADDR_ANY, port))
	{
		close(sock);
		return 0;
	}

	return sock;
}

// set to socket to listen on the given IP and port
char net_listen(uint sock, uint listen_ip, ushort listen_port)
{
sockaddr_in thesock;

	thesock.sin_addr.s_addr = htonl(listen_ip);
	thesock.sin_port = htons(listen_port);
	thesock.sin_family = AF_INET;

	if (bind(sock, (struct sockaddr *)&thesock, sizeof(sockaddr_in)))
	{
		staterr("bind failed to %s:%d!", decimalip(listen_ip), listen_port);
		return 1;
	}

	if (listen(sock, 50))
	{
		staterr("listen failed!");
		return 1;
	}

	stat("bind success to %s:%d", decimalip(listen_ip), listen_port);
	return 0;
}

// accepts an incoming connection on a socket and returns the new socket.
// if remote_ip is non-NULL, it is written with the IP of the connecting host.
uint net_accept(uint s, uint *connected_ip)
{
uint newsocket;
struct sockaddr_in sockinfo;
socklen_t sz = sizeof(sockinfo);

	newsocket = accept(s, (struct sockaddr *)&sockinfo, &sz);

	if (connected_ip)
	{
		if (newsocket)
		{
			*connected_ip = ntohl(sockinfo.sin_addr.s_addr);
		}
		else
		{
			*connected_ip = 0;
		}
	}

	return newsocket;
}

/*
void c------------------------------() {}
*/


