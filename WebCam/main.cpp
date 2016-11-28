#include "main.h"

void Stop(int signo)
{
	Wait = true;
	sleep(1);
	Run = false;
}

int main(int argv,char*argc[])
{
	int i=0;
	Run = true;
	pid_t fpid = 0;
	signal(SIGINT,Stop); 
	
	fpid = fork();
	if(fpid == -1)
	{
		cout << "fork failed" << endl;
		return -1;
	}
	else if(fpid == 0)
	{
		SocketServer httpserver(1210);
		while(Run)
		{
			cout << i << endl;
			httpserver.HTTP_Server();
			i++;
		}
		httpserver.CloseServer();
	}
	else
	{
		cout << "Yept0" << endl;
		ImageProcess WebCamera;
		while(Run)
		{
			WebCamera.CameraGo();
		}
		WebCamera.JPEGClose();
		WebCamera.CameraClose();
		WebCamera.ShareMemoryClose();
	}
	return 0;
	
}