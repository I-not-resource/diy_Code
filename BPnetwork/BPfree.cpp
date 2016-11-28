#include "BPfree.h"
//神经网络创建
void CreatBPnetwork(int _level, ...)
{
	int *plevel = &_level;
	int i, j, l, a, b;
	CellControl.efficiency = 0.2;                //参数设置小于1
	CellControl.precision = 0.05;                //参数设置小于1
	CellControl.Levelnum = _level;
	CellControl.Cellnum = (int *)malloc(_level * sizeof(int));
	CellControl.Weight = (double **)malloc((_level - 1)* sizeof(double *));
	CellControl.transmit = (double **)malloc((2 * _level - 3) * sizeof(double *));                    //传递层数：输入层+ num（隐层+隐层激活）* 隐层层数
	CellControl.OutputResult = (double *)malloc((*(plevel + _level))*sizeof(double));

	for (i = 0; i < _level; i++)                                                            //为每层分配合适的空间，以容纳各个权值
	{
		CellControl.Cellnum[i] = a = *(++plevel);
		if (i >= _level - 1)
			continue;
		else
		{
			b = *(plevel + 1);
			CellControl.Weight[i] = (double *)malloc((a*b)*sizeof(double));
		}
	}

	for (i = 0; i < (_level - 1); i++)                                                        //为各层神经元传递数开辟空间
	{
		CellControl.transmit[(2*i+1)] = (double *)malloc((CellControl.Cellnum[(i + 1)] * sizeof(double)));
		CellControl.transmit[(2*i+2)] = (double *)malloc((CellControl.Cellnum[(i + 1)] * sizeof(double)));
	}
	CellControl.transmit[0] = (double *)malloc((CellControl.Cellnum[0] * sizeof(double)));

	srand((unsigned)time(NULL));                                                           //为各权值分配随机值
	for (l = 0; l<_level - 1; l++)
		for (j = 0; j<CellControl.Cellnum[(l + 1)]; j++)
		{
			for (i = 0; i<CellControl.Cellnum[l]; i++)
			{
				CellControl.Weight[l][(j*CellControl.Cellnum[l] + i)] = rand() / (double)(RAND_MAX);
			}
		}
}
//隐层神经元输入端
void stealth()
{
	int i, j,h;
	for (h = 0; h < HIDELEVELNUM; h++)                               //隐层层数
	{
		for (j = 0; j < CellControl.Cellnum[(h+1)]; j++)             //每层隐层神经元数
		{
		  CellControl.transmit[(2 * h + 1)][j] = 0;
		  for (i = 0; i < CellControl.Cellnum[h]; i++)               //每个隐层神经元的接受值
		  {
			  CellControl.transmit[(2 * h + 1)][j] += CellControl.Weight[h][(j*CellControl.Cellnum[h] + i)] * CellControl.transmit[(2 * h)][i];     //隐层输入层 = 前一层输出层的累加
		  }
		  CellControl.transmit[(2 * h + 2)][j] = activate(CellControl.transmit[(2 * h + 1)][j]);                                  //隐层输出层 = activate函数 隐层输入层 激活
	    }
	}
}
//隐层神经元激活函数
double activate(double _Data)
{
		return  (1 / (1 + exp(-_Data)));
}
//输出层神经元结果
void outdata()
{
	int j, l;
	for (l = 0; l < OUTPUTCELLNUM; l++)
	{
		CellControl.OutputResult[l] = 0;
		for (j = 0; j < HIDEENDCELLNUM; j++)
		{
			CellControl.OutputResult[l] += CellControl.Weight[(CellControl.Levelnum - 2)][(l*HIDEENDCELLNUM + j)] * CellControl.transmit[(2 * CellControl.Levelnum - 4)][j];
		}
	}
}
//输出误差
void getErrot(double *_data)
{
	int l;
	for (l = 0; l < OUTPUTCELLNUM; l++)
	{
		CellControl.Error[l] = _data[l] - CellControl.OutputResult[l];
	}
}
//样本导入
void InportDta(double *_in, double *_out,int _num)
{
	int i,j;
	for (i = 0; i < INPUTCELLNUM; i++)
		CellControl.transmit[0][i] = _in[(_num*INPUTCELLNUM + i)];
	for (j = 0; j < OUTPUTCELLNUM; j++)
		CellControl.OutputResult[j] = _out[_num*OUTPUTCELLNUM+j];
}
//将二维数组_weight复制到二维数组weight
void cellcopy(double **_weight, double **weight)
{
	int x, y, z;
	for (z = 0; z < (CellControl.Levelnum - 1); z++)
		for (x = 0; x < CellControl.Cellnum[z + 1]; ++x)
			for (y = 0; y < CellControl.Cellnum[z]; ++y)
			{
				weight[z][(x*CellControl.Cellnum[z] + y)] = _weight[z][(x*CellControl.Cellnum[z] + y)];
			}
}
//bp网络学习
void TrainBPnetwork(double*_Input, double *_Outout, int _Dtacount)
{
		double **weight_0,
			   weight_changle, weight_ts;
		int i, j, l, k,num,Lnum;
		long count;
		CellControl.Error = (double *)malloc((OUTPUTCELLNUM + 1)*sizeof(double));               //为了避免造成堆破坏，开辟的内存空间加一
		weight_0 = (double **)malloc((CellControl.Levelnum) * sizeof(double *));
		for (i = 0; i < (CellControl.Levelnum - 1); i++)                                        //开辟一个暂存空间，用来存放各个神经元权值
			weight_0[i] = (double *)malloc((CellControl.Cellnum[i] * CellControl.Cellnum[i + 1])*sizeof(double));
		weight_0[(CellControl.Levelnum - 1)] = (double *)malloc((CellControl.Cellnum[i] * CellControl.Cellnum[(CellControl.Levelnum - 2)])*sizeof(double));
		cellcopy(CellControl.Weight, weight_0);

		count = 0;
		while (count < 1000000 && (CellControl.Error[0]+CellControl.Error[1]) != 0)
		{
			count++;
			for (num = 0; num < _Dtacount; num++)                                               //开始计算样本误差
			{
				InportDta(_Input, _Outout, num);
				stealth();
				outdata();
				getErrot((_Outout + num*OUTPUTCELLNUM));                                        //累计所有样本一次计算错误
				CellControl.Error_index = 0;                                                    //累计误差置零
//				for (i = 0; i < OUTPUTCELLNUM; i++)
//				{
//					CellControl.Error_index += CellControl.Error[i] * CellControl.Error[i];
//				}
//				CellControl.Error_index /= 2;

				for (Lnum = 0; Lnum < OUTPUTCELLNUM; Lnum++)
				{
					for (j = 0; j < HIDEENDCELLNUM; j++)
					{
						weight_changle = CellControl.efficiency * CellControl.Error[Lnum] * CellControl.transmit[(2 * CellControl.Levelnum - 4)][j];
						weight_ts = CellControl.Weight[(CellControl.Levelnum - 2)][(Lnum*HIDEENDCELLNUM + j)]
							+ weight_changle
							+ CellControl.precision * (CellControl.Weight[(CellControl.Levelnum - 2)][(Lnum*HIDEENDCELLNUM + j)] - weight_0[(CellControl.Levelnum - 2)][(Lnum*HIDEENDCELLNUM + j)]);
						weight_0[(CellControl.Levelnum - 2)][(Lnum*HIDEENDCELLNUM + j)] = CellControl.Weight[(CellControl.Levelnum - 2)][(Lnum*HIDEENDCELLNUM + j)];
						CellControl.Weight[(CellControl.Levelnum - 2)][(Lnum*HIDEENDCELLNUM + j)] = weight_ts;
					}
				}
/*
					for (l = (CellControl.Levelnum - 2); l > 0; l--)                      //l为剩余未训练层数
						for (k = 0; k < CellControl.Cellnum[(l + 1)]; k++)                   //k上一层神经元数量     x+1
						{
							for (j = 0; j < CellControl.Cellnum[l]; j++)                       //j为当前层神经元数量   x
							{
								for (i = 0; i < CellControl.Cellnum[(l - 1)]; i++)              //i为下一层神经元数量   x-1
								{
									weight_changle = CellControl.efficiency * CellControl.Error_index       //学习效率 乘以 总误差
										* CellControl.Weight[l][(k*CellControl.Cellnum[l] + j)]       //第l层，第k*j个神经元权值
										* CellControl.transmit[l + 1][j] * (1 - CellControl.transmit[l + 1][j]) * CellControl.transmit[(l - 1)][i]; //上一层传导数 *（1-上一层传导数） *下一层传导数

									weight_ts = CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)]
										+ weight_changle
										+ CellControl.precision * (CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)] - weight_0[l - 1][(j*CellControl.Cellnum[(l - 1)] + i)]);

									weight_0[l - 1][(j*CellControl.Cellnum[(l - 1)] + i)] = CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)];
									CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)] = weight_ts;
								}
							}
						}
*/
					for (l = (CellControl.Levelnum - 2); l > 0;l--)                            //l为倒数第l层，其中去掉输出层
					for (i = 0; i < CellControl.Cellnum[l];i++)                                //i为l层第j个神经元
					for (j = 0; j < CellControl.Cellnum[(l - 1)]; j++)                         //j为l+1层第i个神经元
					{
						weight_changle = 0;
						for (Lnum = 0; Lnum < OUTPUTCELLNUM; Lnum++)           //累加变化量   第l+1层神经元数
						{
							weight_changle += CellControl.Error[Lnum]                          //第Lnum个输出误差
							* CellControl.Weight[l][(j*CellControl.Cellnum[(l + 1)] + Lnum)]   //第l+1层j-Lnum的权值
							* CellControl.transmit[(2 * l)][j] * (1 - CellControl.transmit[(2 * l)][j]) * CellControl.transmit[(2 * l - 2)][i];//隐层输出 *（1-隐层输出）* 隐层输入
//							printf("l = %d i = %d\n%.4f -- %.4f\n", l, i, weight_changle, CellControl.Weight[l][(j*CellControl.Cellnum[(l + 1)] + Lnum)]);
						}

						weight_changle = CellControl.efficiency * weight_changle;

						weight_ts      = CellControl.Weight[(l - 1)][(i*CellControl.Cellnum[(l - 1)] + j)]
							           + weight_changle
							           + CellControl.precision * (CellControl.Weight[(l - 1)][(i*CellControl.Cellnum[(l - 1)] + j)] - weight_0[l - 1][(i*CellControl.Cellnum[(l - 1)] + j)]);

						weight_0[l - 1][(i*CellControl.Cellnum[(l - 1)] + j)] = CellControl.Weight[(l - 1)][(i*CellControl.Cellnum[(l - 1)] + j)];

						CellControl.Weight[(l - 1)][(i*CellControl.Cellnum[(l - 1)] + j)] = weight_ts;
//						printf("%.4f -- %.4f\n",weight_changle, weight_ts);
//						printf("%.4f\n\n", CellControl.Error[0]);
					}

			}
			printf("学习次数：%ld 学习误差：%.4f\r", count,CellControl.Error[0]);
		}
		printf_s("样本：%.4lf %.4lf %.4lf\n结果1 %.4lf 结果2：%.4f\n", CellControl.transmit[0][0], CellControl.transmit[0][1], CellControl.transmit[0][2], CellControl.OutputResult[0], CellControl.OutputResult[1]);
        printf("误差1：%lf 误差2： %lf\n", CellControl.Error[0], CellControl.Error[1]);
		printf("学习次数：%d\n",count);

//   if (OUTPUTCELLNUM > 1)
//	free(CellControl.Error);
//	CellControl.Error = NULL;
		for (i = 0; i < (CellControl.Levelnum - 1); i++)
		{
			free(weight_0[i]);
			weight_0[i] = NULL;
		}
	free(weight_0);
		weight_0 = NULL;
}

//bp网络使用
void BPnetworkuse(double *Pdata,double *Outdata)
{
	int _i;
	for (_i = 0; _i < INPUTCELLNUM; _i++)
		CellControl.transmit[0][_i] = Pdata[_i];
	stealth();
	outdata();
	for (_i = 0; _i < OUTPUTCELLNUM; _i++)
		Outdata[_i] = CellControl.OutputResult[_i];
}