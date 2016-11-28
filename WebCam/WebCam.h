#ifndef __WEBCAM_H__
#define __WEBCAM_H__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>  
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "ShareData.h"

//jpeg库
#include <jpeglib.h>

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace::std;


#define OUTDATA 1u
#define DEV_NAME "/dev/video0"

struct CameraStream
{
void* start;
unsigned int length;
};



class ImageProcess
{
public:
    ImageProcess();
	~ImageProcess();
	void JPEGClose();
	void CameraClose();
	int ShareMemoryClose();
	void CameraGo();

private:
	FILE *CacheFile;
	int JPEGSize;                   //压缩成JPEG后的大小
	jpeg_compress_struct JPEGCS;    //定义一个压缩对象，这个对象用于处理主要的功能 
	jpeg_error_mgr       JPEGErr;   //用于错误信息 
	
	int Camerafd;
	v4l2_capability     CameraCap;
	v4l2_fmtdesc        Camerades;
	v4l2_format         CameraFormat;
	v4l2_requestbuffers CameraREQ;
	v4l2_buffer         CameraBuffer;
	v4l2_buf_type       CameraGoType;
	CameraStream        *CameraData;
	char                *RGBbuffer;
	
	ShareData_Typedef   *ShareImageData;
	int ShareMemoryID;
	void* ShareMemoryAddress;
	
    void YUY2toRGB(unsigned char* srcdata,unsigned char* rgbdata,int srcsize);
	void JPEGInit(int image_width,int image_height,int quality);
	int RGB2JPEG(unsigned char* srcdata,char* jpegdata);
	void CameraInit();
	int ShareMemoryInit(key_t key);
};

#endif
