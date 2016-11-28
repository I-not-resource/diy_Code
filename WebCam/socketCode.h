#ifndef __SOCKETCODE_H__
#define __SOCKETCODE_H__

#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <cstdlib>
#include <string.h>
#include <pthread.h>


#include "HTMLProcess.h"

using namespace::std;

//void* SocketThread(void* acceptsocketID);

class SocketServer
{
public:
    SocketServer(int port);
	~SocketServer();
	
	int HTTP_Server();
	void CloseServer();

private:
    int ListenSocket;
	sockaddr_in Localaddr;
	char *Filebuffer;
	char *Request;
	long  DataSize;
	

};










#endif
