#ifndef __HTMLPROCESS_H__
#define __HTMLPROCESS_H__

#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <sys/mman.h>
#include <sys/shm.h>
#include "ShareData.h"

using namespace::std;


class HTMLProcess
{
public:
    HTMLProcess();
	~HTMLProcess();
    string ResponserHTML();
	
	int ResponserFileLength();
	
	int RequestHTML(const char* _info,char* &_byte);

private:
    string ResponseHeadrs,RequestHeaders;
    string HTMLStream;
    string RequestPath;
    int HTMLLength;
   
    ShareData_Typedef *ShareImageData;
    int ShareMemoryID;
	void* ShareMemoryAddress;
	
    void CreatResponseHeadrs(string content,int code);
    int Http_Read_FILE(char* &Byte,string content);
   	int ShareMemoryInit(int key);
	int ShareMemoryClose();
   
};




#endif
