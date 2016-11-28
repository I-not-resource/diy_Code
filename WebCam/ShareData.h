#ifndef __SHAREDATA_H__
#define __SHAREDATA_H__

struct ShareData_Typedef
{
	char Data[1024*1024];
    int Length;	
	int Status;
	bool Wait,Go;
};
#define COMMON_MASTER   0X02
#define COMMON_SERVITOR 0X01
#define COMMON_FREE     0X00

static bool Run = true,Wait=true,Go=false;



#endif