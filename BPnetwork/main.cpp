#include "BPfree.h"
#include "time.h"

//////////////////////////////////////
//BP神经网络测试版V1.0
//使用说明：目前支持多层网络
//          CreatBPnetwork(num1,data1,data2,data3);  
//          num1填写层数，目前可写多层，；data1为输入层数，例程为3；data2为隐层数，例程为5；data3为输出层数，例程为1
//          void TrainBPnetwork(double*_Input, double *_Outout, int _Dtacount)
//          参数1为输入样本，参数2为输出样本，参数3为样本数量；PS：输入样本和输出样本顺序需一致
//作者：ZPW
//调试器版本：Visual Studio 2013
/////////////////////////////////////

int main()
{
	double xx[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1 },
		   yy[] = {
		1,0,0.5,0.5,0,1};
//  yy[] = { 1,
//	0.5,
//	0,};
	double input_0[3];
	double *output;
	clock_t start, finish;
	output = (double *)malloc(2*sizeof(double));
	start = clock();
	CreatBPnetwork(3,3,5,2);
	TrainBPnetwork(xx, yy,3);
	finish = clock();
	printf("网络训练使用时间：%f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
	while (1)
	{
		printf_s("Please iput 3 num:\n");
		scanf_s("%lf %lf %lf", &input_0[0], &input_0[1], &input_0[2]);
		BPnetworkuse(input_0, output);
		printf_s("The result:%.4lf-%.4lf\n", output[0], output[1]);
	}
}