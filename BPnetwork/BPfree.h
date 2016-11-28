#ifndef __BPFREE_H__
#define __BPFREE_H__
#include "math.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
 
#define MODE_COUNT
//#define MODE_ERROR


#define HIDELEVELNUM  (CellControl.Levelnum-2)                           //隐层层数
#define INPUTLEVEL    0                                                  //输入层
#define OUTPUTCELLNUM (CellControl.Cellnum[(CellControl.Levelnum - 1)])  //输出层神经元数
#define HIDEENDCELLNUM (CellControl.Cellnum[CellControl.Levelnum - 2])   //最后一层隐层神经元数
#define INPUTCELLNUM (CellControl.Cellnum[INPUTLEVEL])                   //输入层神经元数

struct _CellControl                           //神经元控制中心
{
	int *Cellnum;                             //某层神经元数
	int Levelnum;                             //神经网络层数
	double **Weight;                          //神经元权值
	double **transmit;                        //传导数
	double *OutputResult;                     //输出结果
	double *Error;                            //神经网络每个输出口的输出结果误差
	double Error_index;                       //每个输出结构一遍学习误差累计
	double efficiency;                        //神经网络学习效率
	double precision;                         //动量因子
};
static struct _CellControl CellControl;
void CreatBPnetwork(int _level, ...);
void stealth();
double activate(double _Data);
void outdata();
void getErrot(double *_data);
void InportDta(double *_in, double *_out, int _num);
void cellcopy(double **_weight, double **weight);
void TrainBPnetwork(double*_Input, double *_Outout, int _Dtacount);
void BPnetworkuse(double *Pdata, double *Outdata);




#endif