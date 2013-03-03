#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef SOCKWRAPH
#define SOCKWRAPH

class Server {
	SOCKET s;
	int port;
	struct sockaddr_in addr;
	u_long Block, nonBlock;
	int blen;
public:
	Server(int port, int buflen);
	~Server();
	void Init(int port, int buflen);
	int Send(char *msg, struct sockaddr_in caddr);
	int Recv(char *buf, struct sockaddr_in *raddr);
	void SetBlocking(bool);
	bool WouldBlock();
};

Server::Server(int port, int buflen) {
	Init(port, buflen);
}

void Server::Init(int port, int buflen) {
	WSAData w;
	WSAStartup(0x0202, &w);
	s = socket(AF_INET, SOCK_DGRAM, 0);
	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if(bind(s, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != 0)
		printf("%i\n", WSAGetLastError());
	Block = 0;
	nonBlock = 1;
	blen = buflen;
}

int Server::Send(char *msg, struct sockaddr_in caddr) {
	return sendto(s, msg, blen, 0, (struct sockaddr *)&caddr, sizeof(caddr));
}

int Server::Recv(char *buf, struct sockaddr_in *raddr) {
	int raddrs = sizeof(struct sockaddr_in);
	return recvfrom(s, buf, blen, 0, (struct sockaddr *)raddr, &raddrs);
}

void Server::SetBlocking(bool ean) {
	u_long toggle = !ean;
	ioctlsocket(s, FIONBIO, &toggle);
}

bool Server::WouldBlock() {
	return WSAGetLastError() == WSAEWOULDBLOCK;
}

Server::~Server() {
	closesocket(s);
	WSACleanup();
}

class Client {
	typedef void (*listenFunc)(char*);
	SOCKET s;
	int port;
	struct sockaddr_in saddr, laddr;
	unsigned int ltid;
	listenFunc lf;
	static void ListenThread(void*);
	bool tFlag;
	u_long Block, nonBlock;
	int blen;
public:
	Client(char *ip, int port, int buflen, listenFunc l);
	~Client();
	void Init(char *ip, int port, int buflen, listenFunc l);
	void StartListenThread();
	void StopListenThread();
	int Send(char *msg);
};

Client::Client(char *ip, int port, int buflen, listenFunc l) {
	Init(ip, port, buflen, l);
}

void Client::Init(char *ip, int port, int buflen, listenFunc l) {
	WSAData w;
	WSAStartup(0x0202, &w);
	s = socket(AF_INET, SOCK_DGRAM, 0);
	memset((void *)&saddr, 0, sizeof(saddr));
	saddr.sin_addr.S_un.S_addr = inet_addr(ip);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	memset((void *)&laddr, 0, sizeof(laddr));
	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(0);
	laddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if(bind(s, (struct sockaddr*)&laddr, sizeof(struct sockaddr_in)) != 0)
		printf("%i\n", WSAGetLastError());
	lf = l;
	ltid = NULL;
	tFlag = false;
	Block = 0;
	nonBlock = 1;
	blen = buflen;
}

void Client::StartListenThread() {
	if(ltid == NULL)
		ltid = _beginthread(this->ListenThread, 0, (void *)this);
}

void Client::ListenThread(void *a) {
	Client *sw = (Client*)a;
	ioctlsocket(sw->s, FIONBIO, &sw->nonBlock);
	char *buf = new char[sw->blen];
	int slen = sizeof(sw->saddr);
	while(sw->tFlag == false) {
		recvfrom(sw->s, buf, sw->blen, 0, (struct sockaddr *)&sw->saddr, &slen);
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			sw->lf(buf);
	}
	sw->tFlag = false;
	_endthread();
}

void Client::StopListenThread() {
	if(ltid != NULL) {
		tFlag = true;
		ltid = NULL;
	}
}

int Client::Send(char *msg) {
	return sendto(s, msg, blen, 0, (struct sockaddr *)&saddr, sizeof(saddr));
}

Client::~Client() {
	tFlag = true;
	closesocket(s);
	WSACleanup();
}

#endif