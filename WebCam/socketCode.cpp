#include "socketCode.h"


/**
*    创建socket并绑定端口
*/
SocketServer::SocketServer(int port=1210)
{
	DataSize = 1024 * 1024;
	Request = new char[DataSize];
	Filebuffer = new char[DataSize];
	/** 
	*   创建socket链接
	*   int socket(int domain, int type, int protocol);
	*   domain    协议域     AF_INET——ipv4  AF_INET6——ipv6  AF_LOCAL——一个绝对路劲名
	*   type      socket类型 SOCK_STREAM——数据流套接口(TCP) SOCK_DGRAM——数据报套接口(UDP) SOCK_RAM——原始套接口 SOCK_RACKET SOCK——与网络驱动程序直接通信_SEQPACKET——提供连续可靠的数据包连接
	*    protocol 协议       IPPROTO_TCP——TCP传输协议 IPPRPOTO_UDP——UDP传输协议 IPPROTO_SCTP——SCTP传输协议 IPPROTO_TIPC——TIPC传输协议
	*/
	if((ListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		cout << "socket() failed" << endl;
		exit(1);
	}
	
	/**
	*    清除Localaddr中的内容
	*/
	memset(&Localaddr,0,sizeof(Localaddr));
	Localaddr.sin_family = AF_INET;                                  //使用IPV4通信域
	Localaddr.sin_port   = htons(port);                              //端口转换为网络字节序
	Localaddr.sin_addr.s_addr = INADDR_ANY;                          //服务器可以接受任意地址
	
	/**
	*    绑定端口
	*    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	*    sockfd    socket链接描述符
	*    addr      指向要绑定给socket的协议地址
	*    addrlen   对应的地址长度
	*/
	if(bind(ListenSocket,(sockaddr *)&Localaddr,sizeof(Localaddr)) == -1)
	{
		cout << "bind() failed" << endl;
		exit(1);		
	}
	
	/**
	*    监听socket链接
	*    int listen(int sockfd, int backlog);
	*    sockfd    监听的socket描述符
	*    backlog   排队的最大连接数
	*/
	if(listen(ListenSocket,100) == -1)
	{
		cout << "listen() failed" << endl;
		exit(1);
	}
	
	sleep(1);
	
}

SocketServer::~SocketServer()
{
	CloseServer();
}


int SocketServer::HTTP_Server()
{
	int acceptsocket;
	string responser;
    HTMLProcess httpsever;
	sockaddr_in clientaddr;
	socklen_t sin_size;
	sin_size=sizeof(sockaddr_in);
	//pthread_t ThreadID;
	
		/**
		*    接收请求
		*    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		*    sockfd    监听的端口
		*    addr      接收返回的客户端地址
		*    addrlen   用来接收客户端地址的大小
		*/
		if(acceptsocket = accept(ListenSocket,(sockaddr *)&clientaddr,&sin_size) == -1)
		{
			cout << "accept failed" << endl;
			//continue;
		}
		
		/**
		*    接收客户端发来的信息
		*    int  recv(int sock, void FAR *buf, size_t len, int flags);
		*    sock    接收端监听的端口
		*    buf     消息接收缓冲区
		*    len     缓冲区容量
		*    flags   接收状态    MSG_DONTWAIT——操作不会被阻塞  MSG_ERRQUEUE——从套接字的错误队列上接收错误值  MSG_PEEK——数据接收后，在接收队列中保留原数据  MSG_TRUNC——返回封包的实际长度  MSG_WAITAL——阻塞操作
		*
		*/
		recv(acceptsocket,Request,DataSize,0);
		httpsever.RequestHTML(Request,Filebuffer);
		responser = httpsever.ResponserHTML();
		
		/**
		*    向客户端发送信息
		*    int send( SOCKET s,const char *buf,nt len,int flags ); 
		*    s      接收端监听的端口
		*    buf    要发送数据的缓冲区
		*    len    实际要发送的字节数
		*    flags  
		*/
		send(acceptsocket,responser.data(),responser.length(),0);
		send(acceptsocket,Filebuffer,httpsever.ResponserFileLength(),0);

    //pthread_create(&ThreadID,NULL,SocketThread,(void*)&acceptsocket);
	//sleep(1);

	close(acceptsocket);

}

void SocketServer::CloseServer()
{
	close(ListenSocket);
	delete Filebuffer;
	Filebuffer = NULL;
	delete Request;
	Request = NULL;
}
/*
void* SocketThread(void* acceptsocketID)
{
	int ID = *(int*)acceptsocketID;
	int rsquestsize = 1024*1024;
	string responser;
    char *request;
	char *Filebuffer;
	request = new char[rsquestsize];
	HTMLProcess httpsever;
	
	recv(ID,request,rsquestsize,0);
	cout << "HTTP Message:" << httpsever.RequestHTML(request,Filebuffer) << endl;
	responser = httpsever.ResponserHTML();
	send(ID,responser.data(),responser.length(),0);
	send(ID,Filebuffer,httpsever.ResponserFileLength(),0);
	
	close(ID);
	delete request;
	request = NULL;
	delete Filebuffer;
	Filebuffer = NULL;
}
*/

