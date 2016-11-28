#include "socketCode.h"


/**
*    ����socket���󶨶˿�
*/
SocketServer::SocketServer(int port=1210)
{
	DataSize = 1024 * 1024;
	Request = new char[DataSize];
	Filebuffer = new char[DataSize];
	/** 
	*   ����socket����
	*   int socket(int domain, int type, int protocol);
	*   domain    Э����     AF_INET����ipv4  AF_INET6����ipv6  AF_LOCAL����һ������·����
	*   type      socket���� SOCK_STREAM�����������׽ӿ�(TCP) SOCK_DGRAM�������ݱ��׽ӿ�(UDP) SOCK_RAM����ԭʼ�׽ӿ� SOCK_RACKET SOCK������������������ֱ��ͨ��_SEQPACKET�����ṩ�����ɿ������ݰ�����
	*    protocol Э��       IPPROTO_TCP����TCP����Э�� IPPRPOTO_UDP����UDP����Э�� IPPROTO_SCTP����SCTP����Э�� IPPROTO_TIPC����TIPC����Э��
	*/
	if((ListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		cout << "socket() failed" << endl;
		exit(1);
	}
	
	/**
	*    ���Localaddr�е�����
	*/
	memset(&Localaddr,0,sizeof(Localaddr));
	Localaddr.sin_family = AF_INET;                                  //ʹ��IPV4ͨ����
	Localaddr.sin_port   = htons(port);                              //�˿�ת��Ϊ�����ֽ���
	Localaddr.sin_addr.s_addr = INADDR_ANY;                          //���������Խ��������ַ
	
	/**
	*    �󶨶˿�
	*    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	*    sockfd    socket����������
	*    addr      ָ��Ҫ�󶨸�socket��Э���ַ
	*    addrlen   ��Ӧ�ĵ�ַ����
	*/
	if(bind(ListenSocket,(sockaddr *)&Localaddr,sizeof(Localaddr)) == -1)
	{
		cout << "bind() failed" << endl;
		exit(1);		
	}
	
	/**
	*    ����socket����
	*    int listen(int sockfd, int backlog);
	*    sockfd    ������socket������
	*    backlog   �Ŷӵ����������
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
		*    ��������
		*    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		*    sockfd    �����Ķ˿�
		*    addr      ���շ��صĿͻ��˵�ַ
		*    addrlen   �������տͻ��˵�ַ�Ĵ�С
		*/
		if(acceptsocket = accept(ListenSocket,(sockaddr *)&clientaddr,&sin_size) == -1)
		{
			cout << "accept failed" << endl;
			//continue;
		}
		
		/**
		*    ���տͻ��˷�������Ϣ
		*    int  recv(int sock, void FAR *buf, size_t len, int flags);
		*    sock    ���ն˼����Ķ˿�
		*    buf     ��Ϣ���ջ�����
		*    len     ����������
		*    flags   ����״̬    MSG_DONTWAIT�����������ᱻ����  MSG_ERRQUEUE�������׽��ֵĴ�������Ͻ��մ���ֵ  MSG_PEEK�������ݽ��պ��ڽ��ն����б���ԭ����  MSG_TRUNC�������ط����ʵ�ʳ���  MSG_WAITAL������������
		*
		*/
		recv(acceptsocket,Request,DataSize,0);
		httpsever.RequestHTML(Request,Filebuffer);
		responser = httpsever.ResponserHTML();
		
		/**
		*    ��ͻ��˷�����Ϣ
		*    int send( SOCKET s,const char *buf,nt len,int flags ); 
		*    s      ���ն˼����Ķ˿�
		*    buf    Ҫ�������ݵĻ�����
		*    len    ʵ��Ҫ���͵��ֽ���
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

