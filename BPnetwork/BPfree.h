#ifndef __BPFREE_H__
#define __BPFREE_H__
#include "math.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
 
#define MODE_COUNT
//#define MODE_ERROR


#define HIDELEVELNUM  (CellControl.Levelnum-2)                           //�������
#define INPUTLEVEL    0                                                  //�����
#define OUTPUTCELLNUM (CellControl.Cellnum[(CellControl.Levelnum - 1)])  //�������Ԫ��
#define HIDEENDCELLNUM (CellControl.Cellnum[CellControl.Levelnum - 2])   //���һ��������Ԫ��
#define INPUTCELLNUM (CellControl.Cellnum[INPUTLEVEL])                   //�������Ԫ��

struct _CellControl                           //��Ԫ��������
{
	int *Cellnum;                             //ĳ����Ԫ��
	int Levelnum;                             //���������
	double **Weight;                          //��ԪȨֵ
	double **transmit;                        //������
	double *OutputResult;                     //������
	double *Error;                            //������ÿ������ڵ����������
	double Error_index;                       //ÿ������ṹһ��ѧϰ����ۼ�
	double efficiency;                        //������ѧϰЧ��
	double precision;                         //��������
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