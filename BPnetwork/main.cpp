#include "BPfree.h"
#include "time.h"

//////////////////////////////////////
//BP��������԰�V1.0
//ʹ��˵����Ŀǰ֧�ֶ������
//          CreatBPnetwork(num1,data1,data2,data3);  
//          num1��д������Ŀǰ��д��㣬��data1Ϊ�������������Ϊ3��data2Ϊ������������Ϊ5��data3Ϊ�������������Ϊ1
//          void TrainBPnetwork(double*_Input, double *_Outout, int _Dtacount)
//          ����1Ϊ��������������2Ϊ�������������3Ϊ����������PS�������������������˳����һ��
//���ߣ�ZPW
//�������汾��Visual Studio 2013
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
	printf("����ѵ��ʹ��ʱ�䣺%f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
	while (1)
	{
		printf_s("Please iput 3 num:\n");
		scanf_s("%lf %lf %lf", &input_0[0], &input_0[1], &input_0[2]);
		BPnetworkuse(input_0, output);
		printf_s("The result:%.4lf-%.4lf\n", output[0], output[1]);
	}
}