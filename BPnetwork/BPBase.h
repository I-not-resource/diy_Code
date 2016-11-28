#include "math.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"

#define IN_LEVE   3
#define HIDE_LEVE 7
#define OUT_LEVE  1

struct cell_base                 //神经元结构体
{
	double input[IN_LEVE],                           //神经元输入
		input_next[HIDE_LEVE],                    //隐层神经元输入or输入层神经元加权输出和
		transmit[HIDE_LEVE];                      //隐层神经元输出

	double output_result[OUT_LEVE],                  //神经元输出结果
		output_expect[OUT_LEVE];                  //输出结果期望

	double weight_in[IN_LEVE][HIDE_LEVE],            //神经元输入端权值
		weight_out[HIDE_LEVE][OUT_LEVE];          //神经元输出端权值

	double Error[OUT_LEVE];                          //输出误差

	double Error_index;

	double efficiency;                               //神经网络学习效率

	double precision;                                //动量因子
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