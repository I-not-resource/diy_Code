#include "math.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"

#define IN_LEVE   3
#define HIDE_LEVE 7
#define OUT_LEVE  1

struct cell_base                 //��Ԫ�ṹ��
{
	double input[IN_LEVE],                           //��Ԫ����
		input_next[HIDE_LEVE],                    //������Ԫ����or�������Ԫ��Ȩ�����
		transmit[HIDE_LEVE];                      //������Ԫ���

	double output_result[OUT_LEVE],                  //��Ԫ������
		output_expect[OUT_LEVE];                  //����������

	double weight_in[IN_LEVE][HIDE_LEVE],            //��Ԫ�����Ȩֵ
		weight_out[HIDE_LEVE][OUT_LEVE];          //��Ԫ�����Ȩֵ

	double Error[OUT_LEVE];                          //������

	double Error_index;

	double efficiency;                               //������ѧϰЧ��

	double precision;                                //��������
};



static struct cell_base cell = {0};

void cell_int();
void stealth();
void activate();
void outdata();
void getErrot();
void InportDta(double(*_in)[IN_LEVE], int _x, double(*_out)[OUT_LEVE], int _y, int _num);
void cellcopy(int firstdate, int seconddate, double *_weight, double *weight);
void celltrain(double(*_in)[IN_LEVE], int _x, double(*_out)[OUT_LEVE], int _y, int _num);
double *BPuse(double *Pdate, int num);