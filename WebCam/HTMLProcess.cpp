/**
*    HTMLProcess
*    Version 0.1
*    ps:待完善 sse服务 和 表单处理
*/
#include "HTMLProcess.h"

//#define Debug i

HTMLProcess::HTMLProcess()
{
	HTMLStream.clear();
	HTMLLength = 0;
	RequestPath = "www/index.html";
	ShareMemoryInit(1210);
}

HTMLProcess::~HTMLProcess()
{
	ShareMemoryClose();
}

/**
*    创建一个相应报文
*
*/
void HTMLProcess::CreatResponseHeadrs(string content,int code=200)
{
	time_t systime;
	char len[20];
	ResponseHeadrs.clear();
	
	ResponseHeadrs +="HTTP/1.1 ";
	ResponseHeadrs += code;
	switch(code)
	{
		case 200:ResponseHeadrs += " OK";break;
		case 403:ResponseHeadrs += " Forbidden";cout << "the Forbidden" << endl;break;
		case 404:ResponseHeadrs += " NOT FOUND";break;
		case 500:ResponseHeadrs += " SEVER ERROR";break;
		default:break;
	}
	ResponseHeadrs += "\r\n";
	
	ResponseHeadrs += "Date:";
	time(&systime);                                   //获得系统时间
	ResponseHeadrs += asctime(gmtime(&systime));
	ResponseHeadrs += "Server:Air Server/0.1\r\n";
	ResponseHeadrs += "Cache-Control: no-cache\r\n";
	ResponseHeadrs += "Content-type:";
	ResponseHeadrs += content;
	if(content == "text/html")
	{
	    ResponseHeadrs += ";charset=utf8";
	    ResponseHeadrs += "\r\n";
		ResponseHeadrs += "Connection: Keep-Alive\r\n";
	}
	else if(content == "text/css")
	{
		ResponseHeadrs += "\r\n";
		ResponseHeadrs += "Content-Encoding: gzip\r\n";
		ResponseHeadrs += "Connection: Keep-Alive\r\n";
	}
	else
	{
        ResponseHeadrs += "\r\n";
		ResponseHeadrs += "Connection: close\r\n";
	}
	ResponseHeadrs += "Accept-Ranges: bytes\r\n";
	ResponseHeadrs += "Content-Length:";
	sprintf(len,"%d",HTMLLength);
	ResponseHeadrs += len;
	
	ResponseHeadrs += "\r\n\r\n";

#if Debug	
	cout << "responseHears" << endl;
	cout << ResponseHeadrs << len << endl << endl;
#endif
}

/**
*    根据html请求的类型，获得相应的文件
*/
int HTMLProcess::Http_Read_FILE(char* &Byte,string content="text/html")
{
	ifstream HTMLFile;
	struct stat filestat;
	int flag = 0,code=200;
	void *Filed;
    HTMLLength = 0;

#if Debug	
	cout << RequestPath.data() << "/" << endl;
#endif
	//如果获取文件状态失败，表示文件不存的
	if(stat(RequestPath.data(),&filestat))
	{
		code = 404;
		cout << "get file error" << endl;
		flag = -1;
	}
	//如果不是文件或者无读权限，发送无法读取文件 
	else if( ! (S_ISREG(filestat.st_mode)) || ! (S_IRUSR & filestat.st_mode))
	{
		code = 403;
		flag = -2;
	}
	else
	{
		HTMLFile.open(RequestPath.data(),ios::in|ios::binary);
		if(HTMLFile.is_open())
		{
			HTMLFile.seekg(0,ios::end);
			HTMLLength = HTMLFile.tellg();
			HTMLFile.seekg(0,ios::beg);
			HTMLFile.read(Byte,HTMLLength);
#if Debug
			cout << "Byte:" << Byte << endl;
#endif
		}
		else
		{
			code = 404;
			flag = -1;
		}
	}
	
	CreatResponseHeadrs(content,code);
	HTMLStream = ResponseHeadrs;

	HTMLFile.close();
	ShareImageData->Status = COMMON_FREE;
	return flag;
}

/**
*    响应HTML的请求
*    返回处理完成的数据
*/
string HTMLProcess::ResponserHTML()
{
	return HTMLStream;
}

int HTMLProcess::ResponserFileLength()
{
	return HTMLLength;
}

/**
*    html请求处理
*    _info 请求报文信息
*
*/
int HTMLProcess::RequestHTML(const char *_info,char* &_byte)
{
	RequestHeaders = _info;    //将 char* 转为 string 便于处理字符串
	int pos;
	int pos_mothd,pos_http,pos_accept,pos_newline,pos_point;
	string AcceptIfo;          //请求 服务器的信息
	string AcceptContent;
	string ContentType;

#if Debug	
	cout << "RequestHeaders" << endl;
	cout << RequestHeaders << endl <<endl;
#endif
	
	pos_http = RequestHeaders.find(" HTTP/1.1");
	if((pos_mothd = RequestHeaders.find("GET ")) != -1)    //GET方式
	{
		if(pos_http - pos_mothd > 5)     //获得信息为路径信息
		{
			if(RequestPath.find("http://") == -1)
			    RequestPath = RequestHeaders.substr(5,(pos_http-5));

			if((RequestPath.find("?")) != -1)
			{
				if(RequestPath.find("?image") != -1)     //实时视频请求
			    {
					ShareImageData->Wait = false;
					ShareImageData->Go   = true;
					while(ShareImageData->Status == COMMON_MASTER);
					ShareImageData->Status = COMMON_SERVITOR;
				    HTMLLength = ShareImageData->Length;
					int count = 0;
					while(count<HTMLLength)
					{
						_byte[count] = ShareImageData->Data[count];
						count++;
					}
					ShareImageData->Status = COMMON_FREE;
					CreatResponseHeadrs("image/jpeg",200);
				    return 0;
			    }
				//待完善
				CreatResponseHeadrs("",500);
				HTMLStream = ResponseHeadrs;
				return 0;
			}
            else if((RequestHeaders.find("text/event-stream")) != -1)  //SSE请求
			{
				//待完善
				CreatResponseHeadrs("",500);
				HTMLStream = ResponseHeadrs;
				return 0;
			}
			else if(RequestPath.find(".ico") != -1)
			{
				//待完善
				CreatResponseHeadrs("",500);
				HTMLStream = ResponseHeadrs;
				return 0;
			}
			if(RequestPath.find("Cache.jpg") != -1)
			{
				RequestPath = "Cache.jpg";
				while(ShareImageData->Status == COMMON_MASTER);
				ShareImageData->Status = COMMON_SERVITOR;
			}
			pos_point = RequestPath.find(".");
			ContentType = RequestPath.substr((pos_point+1),(RequestPath.length()-pos_point));
			if(RequestPath.find("jpg") != -1)
				ContentType = "jpeg";
			pos_accept    = RequestHeaders.find("Accept: ",pos_http);
			pos_newline   = RequestHeaders.find("\r\n",pos_accept);
			AcceptIfo     = RequestHeaders.substr((pos_accept+8),(pos_newline-1));
			AcceptContent = AcceptIfo.substr(0,(AcceptIfo.find("/")+1));
			AcceptContent += ContentType;
			return Http_Read_FILE(_byte,AcceptContent);
		}
		
	}
	else if((pos_mothd = RequestHeaders.find("POST ")) != -1)  //POST方式
	{
		pos = RequestHeaders.find("\r\n\r\n");
		RequestHeaders = RequestHeaders.substr(0,pos);
		if(pos_http - pos_mothd > 6)
		{
			RequestPath = RequestHeaders.substr(5,pos_http);
			if(RequestPath == "/?control")
			{
				//待续
				return 0;
			}
		}
	}
	
	return Http_Read_FILE(_byte);          //html首页或其他html请求
}

int HTMLProcess::ShareMemoryInit(int key)
{
	//创建共享内存
	ShareMemoryID = shmget((key_t)key,sizeof(ShareData_Typedef),0666|IPC_CREAT);
	if(ShareMemoryID == -1)
	{
		cout << "HTML shmget failed" << endl;
		return -1;
	}
	//将共享内存连接到当前进程的地址空间
	ShareMemoryAddress = shmat(ShareMemoryID,NULL,0);
	if(ShareMemoryAddress == (void*)-1)
	{
		cout << "HTML shmat failed" << endl;
		return -1;		
	}
	//映射共享内存地址
	ShareImageData = (ShareData_Typedef*)ShareMemoryAddress;
	ShareImageData->Status = COMMON_FREE;
	return 0;
}

int HTMLProcess::ShareMemoryClose()
{
	//把共享内存从当前进程中分离
	if(shmdt(ShareMemoryAddress) == -1)
	{
		cout << "HTML shmdt failed" << endl;
		return -1;
	}
	//删除共享内存  
	if(shmctl(ShareMemoryID,IPC_RMID,0) == -1)
	{
		cout << "HTML shmctl failed" << endl;
		return -1;
	}
	return 0;
}








