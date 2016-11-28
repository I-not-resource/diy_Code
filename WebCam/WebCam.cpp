#include "WebCam.h"

ImageProcess::ImageProcess()
{
	CameraInit();
	JPEGInit(CameraFormat.fmt.pix.width,CameraFormat.fmt.pix.height,70);
	ShareMemoryInit(1210);
	
}

ImageProcess::~ImageProcess()
{
	JPEGClose();
	CameraClose();
	ShareMemoryClose();

}

/**
*    YUY2转RGB
*    srcdata    YUY2源数据
*    rgbdata    转换后RGB数据
*    srcsize    源数据大小
*/
void ImageProcess::YUY2toRGB(unsigned char* srcdata,unsigned char* rgbdata,int srcsize)
{
	/**
	*    YUY2转RGB公式
	*    B = 1.164(Y - 16) + 2.018(U - 128)    
    *    G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)    
    *    R = 1.164(Y - 16) + 1.596(V - 128)
    */	
	unsigned char* orgRGBData = rgbdata;
	
	for(int count = 0;count < srcsize;count += 4)
	{
		float Y0 = *srcdata;
		float U  = *(++srcdata);
		float Y1 = *(++srcdata);
		float V  = *(++srcdata);
		++srcdata;
        *(rgbdata) =   (unsigned char)(Y0 + (1.370705 * (V-128)));              
        *(++rgbdata) = (unsigned char)(Y0 - (0.698001 * (V-128)) - (0.337633 * (U-128)));      
        *(++rgbdata) = (unsigned char)(Y0 + (1.732446 * (U-128)));            
        *(++rgbdata) = (unsigned char)(Y1 + (1.370705 * (V-128)));              
        *(++rgbdata) = (unsigned char)(Y1 - (0.698001 * (V-128)) - (0.337633 * (U-128)));      
        *(++rgbdata) = (unsigned char)(Y1 + (1.732446 * (U-128)));            
        ++rgbdata; 	
	}
}

/**
*    JPEG压缩初始化
*    image_width    图片的宽度
*    image_height   图片的高度
*    quality        图片的质量 (1~100)
*/
void ImageProcess::JPEGInit(int image_width,int image_height,int quality)
{
	if(quality>100)
		quality = 100;
	else if(quality < 1)
		quality = 1;
	
	JPEGCS.err = jpeg_std_error(&JPEGErr);                //错误输出在绑定 
	jpeg_create_compress(&JPEGCS);                         //初始化压缩对象    
	/**    压缩参数设置    **/
	JPEGCS.image_width      = image_width;
	JPEGCS.image_height     = image_height;
	JPEGCS.input_components = 3;
	JPEGCS.in_color_space   = JCS_RGB;
	/**    参数默认设置    **/
	jpeg_set_defaults(&JPEGCS);
	
	jpeg_set_quality(&JPEGCS,quality,TRUE);               // 设定编码jpeg压缩质量 
}

/**
*    RGB压缩成JPEG
*    srcdata    要压缩的RGB源数据
*    jpegdata   压缩后的JPEG数据
*
*    返回压缩后图片的大小
*/
int ImageProcess::RGB2JPEG(unsigned char* srcdata,char* jpegdata=NULL)
{
	
	//jpeg_stdio_dest(&JPEGCS,jpegdata,&JPEGSize);               //绑定输出
	//fseek(CacheFile,0,SEEK_SET);
	CacheFile = fopen("Cache.jpg","w");
	jpeg_stdio_dest(&JPEGCS,CacheFile);
	
	jpeg_start_compress(&JPEGCS,TRUE);                          //开始压缩。执行这一行数据后，无法再设置参数了！
	
	JSAMPROW row_point[1];
	
	//从上到下，设置图片中每一行的像素值
	for(int i=0;i<JPEGCS.image_height;i++)
	{
		row_point[0] = srcdata + i * JPEGCS.image_width * 3;
		jpeg_write_scanlines(&JPEGCS,row_point,1);
	}
	
	jpeg_finish_compress(&JPEGCS);                              //结束压缩
	//fseek(CacheFile,0,SEEK_END);
	//JPEGSize = ftell(CacheFile);
	//fseek(CacheFile,0,SEEK_SET);
	//fread(ShareImageData->Data,sizeof(char),JPEGSize,CacheFile);
	fclose(CacheFile);
	return JPEGSize;
}

/**
*    结束jpeg压缩
*/
void ImageProcess::JPEGClose()
{
	jpeg_destroy_compress(&JPEGCS);                               //清空对象 
}

/**
*    摄像头初始化
*/
void ImageProcess::CameraInit()
{
	Camerades.index   = 0;
	Camerades.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	CameraFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	CameraREQ.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	CameraREQ.memory  = V4L2_MEMORY_MMAP;
	CameraREQ.count   = 4;
	CameraGoType      = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	Camerafd = open(DEV_NAME,O_RDWR,0);
	if(Camerafd<0)
		cout << "open video0 fialed!" << endl;
	else
	{
		ioctl(Camerafd,VIDIOC_QUERYCAP,&CameraCap);
		while(ioctl(Camerafd,VIDIOC_ENUM_FMT,&Camerades) != -1)
		{
			Camerades.index++;
		}
		if(ioctl(Camerafd,VIDIOC_G_FMT,&CameraFormat) == -1)
			cout << "Get format failed." << endl;
		/**    摄像头参数设置    **/
		CameraFormat.fmt.pix.width       = 640;
		CameraFormat.fmt.pix.height      = 480;
		CameraFormat.fmt.pix.field       = V4L2_FIELD_INTERLACED;
		CameraFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
		CameraFormat.fmt.pix.priv        = 1;
		if(ioctl(Camerafd,VIDIOC_S_FMT,&CameraFormat) == -1)
			cout << "Set format failed." << endl;
		
		if(ioctl(Camerafd,VIDIOC_G_FMT,&CameraFormat) == -1)
			cout << "Get format failed." << endl;
		
		CameraData = (CameraStream*)calloc(CameraREQ.count,sizeof(*CameraData));
		if(!CameraData)
		{
			cout << "CameraData failed by out of memory." << endl;
			close(Camerafd);
			exit(EXIT_FAILURE);
		}
		if (ioctl(Camerafd,VIDIOC_REQBUFS,&CameraREQ) < 0)
		{
			cout << "VIDIOC_REQBUFS failed" << endl;
			close(Camerafd);
			exit(-1);
		}
		for(int i=0;i<CameraREQ.count;++i)
		{
			CameraBuffer.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			CameraBuffer.memory = V4L2_MEMORY_MMAP;
			CameraBuffer.index  = i;
			if(ioctl(Camerafd,VIDIOC_QUERYBUF,&CameraBuffer) == -1)
			{
				cout << i << " - " << "VIDIOC_QUERYBUF failed" << endl;
				close(Camerafd);
				exit(-1);
			}
			CameraData[i].length = CameraBuffer.length;
			CameraData[i].start  = mmap(NULL , CameraBuffer.length , PROT_READ | PROT_WRITE , MAP_SHARED , Camerafd , CameraBuffer.m.offset);
			if(CameraData[i].start == MAP_FAILED)
			{
				cout << "MAP_FAILED" << endl;
				close(Camerafd);
				exit(-1);
			}
			if(ioctl(Camerafd,VIDIOC_QBUF,&CameraBuffer))
			{
				cout << "VIDIOC_QBUF failed" << endl;
				close(Camerafd);
				exit(-1);
			}
		}
	    if(ioctl(Camerafd,VIDIOC_STREAMON,&CameraGoType) == -1)
		{
			cout << "VIDIOC_STREAMON failed" << endl;
			close(Camerafd);
			exit(-1);
		}
	}
}

/**
*    开始采集摄像头
*/
void ImageProcess::CameraGo()
{
	if(ShareImageData->Status == COMMON_SERVITOR)
		return;
	else
		ShareImageData->Status = COMMON_MASTER;
	int rgbsize = CameraData[CameraBuffer.index].length * 3 / 2;
	if(ioctl(Camerafd,VIDIOC_DQBUF,&CameraBuffer) == -1)
	{
		cout << "VIDIO_DQBUF failed" << endl;
		close(Camerafd);
		return;
	}
	RGBbuffer = new  char[rgbsize];
	YUY2toRGB((unsigned char*)CameraData[CameraBuffer.index].start,(unsigned char*)RGBbuffer,rgbsize);
	ShareImageData->Length = RGB2JPEG((unsigned char*)RGBbuffer,ShareImageData->Data);
	ShareImageData->Status = COMMON_FREE;
	delete [] RGBbuffer;
	RGBbuffer     = NULL;
	if(ioctl(Camerafd,VIDIOC_QBUF,&CameraBuffer) == -1)
	{
		cout << "VIDIOC_QBUF failed" << endl;
		close(Camerafd);
		return;
	}
}

/**
*    关闭摄像头
*/
void ImageProcess::CameraClose()
{
	ioctl(Camerafd,VIDIOC_STREAMOFF,&CameraGoType);
	close(Camerafd);
}

/**
*
*/
int ImageProcess::ShareMemoryInit(key_t key)
{
	//创建共享内存
	ShareMemoryID = shmget(key,sizeof(ShareData_Typedef),0666|IPC_CREAT);
	if(ShareMemoryID == -1)
	{
		cout << "Camera shmget failed" << endl;
		return -1;
	}
	//将共享内存连接到当前进程的地址空间 
	ShareMemoryAddress = shmat(ShareMemoryID,NULL,0);
	if(ShareMemoryAddress == (void*)-1)
	{
		cout << "Camera shmat failed" << endl;
		return -1;		
	}
	//映射共享内存地址
	ShareImageData = (ShareData_Typedef*)ShareMemoryAddress;
	ShareImageData->Status = COMMON_FREE;
	return 0;
}

/**
*
*/
int ImageProcess::ShareMemoryClose()
{
	if(shmdt(ShareMemoryAddress) == -1)
	{
		cout << "Camera shmdt failed" << endl;
		return -1;
	}
	return 0;
}















