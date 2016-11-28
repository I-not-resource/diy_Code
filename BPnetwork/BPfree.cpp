#include "BPfree.h"
//�����紴��
void CreatBPnetwork(int _level, ...)
{
	int *plevel = &_level;
	int i, j, l, a, b;
	CellControl.efficiency = 0.2;                //��������С��1
	CellControl.precision = 0.05;                //��������С��1
	CellControl.Levelnum = _level;
	CellControl.Cellnum = (int *)malloc(_level * sizeof(int));
	CellControl.Weight = (double **)malloc((_level - 1)* sizeof(double *));
	CellControl.transmit = (double **)malloc((2 * _level - 3) * sizeof(double *));                    //���ݲ����������+ num������+���㼤�* �������
	CellControl.OutputResult = (double *)malloc((*(plevel + _level))*sizeof(double));

	for (i = 0; i < _level; i++)                                                            //Ϊÿ�������ʵĿռ䣬�����ɸ���Ȩֵ
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

	for (i = 0; i < (_level - 1); i++)                                                        //Ϊ������Ԫ���������ٿռ�
	{
		CellControl.transmit[(2*i+1)] = (double *)malloc((CellControl.Cellnum[(i + 1)] * sizeof(double)));
		CellControl.transmit[(2*i+2)] = (double *)malloc((CellControl.Cellnum[(i + 1)] * sizeof(double)));
	}
	CellControl.transmit[0] = (double *)malloc((CellControl.Cellnum[0] * sizeof(double)));

	srand((unsigned)time(NULL));                                                           //Ϊ��Ȩֵ�������ֵ
	for (l = 0; l<_level - 1; l++)
		for (j = 0; j<CellControl.Cellnum[(l + 1)]; j++)
		{
			for (i = 0; i<CellControl.Cellnum[l]; i++)
			{
				CellControl.Weight[l][(j*CellControl.Cellnum[l] + i)] = rand() / (double)(RAND_MAX);
			}
		}
}
//������Ԫ�����
void stealth()
{
	int i, j,h;
	for (h = 0; h < HIDELEVELNUM; h++)                               //�������
	{
		for (j = 0; j < CellControl.Cellnum[(h+1)]; j++)             //ÿ��������Ԫ��
		{
		  CellControl.transmit[(2 * h + 1)][j] = 0;
		  for (i = 0; i < CellControl.Cellnum[h]; i++)               //ÿ��������Ԫ�Ľ���ֵ
		  {
			  CellControl.transmit[(2 * h + 1)][j] += CellControl.Weight[h][(j*CellControl.Cellnum[h] + i)] * CellControl.transmit[(2 * h)][i];     //��������� = ǰһ���������ۼ�
		  }
		  CellControl.transmit[(2 * h + 2)][j] = activate(CellControl.transmit[(2 * h + 1)][j]);                                  //��������� = activate���� ��������� ����
	    }
	}
}
//������Ԫ�����
double activate(double _Data)
{
		return  (1 / (1 + exp(-_Data)));
}
//�������Ԫ���
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
//������
void getErrot(double *_data)
{
	int l;
	for (l = 0; l < OUTPUTCELLNUM; l++)
	{
		CellControl.Error[l] = _data[l] - CellControl.OutputResult[l];
	}
}
//��������
void InportDta(double *_in, double *_out,int _num)
{
	int i,j;
	for (i = 0; i < INPUTCELLNUM; i++)
		CellControl.transmit[0][i] = _in[(_num*INPUTCELLNUM + i)];
	for (j = 0; j < OUTPUTCELLNUM; j++)
		CellControl.OutputResult[j] = _out[_num*OUTPUTCELLNUM+j];
}
//����ά����_weight���Ƶ���ά����weight
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
//bp����ѧϰ
void TrainBPnetwork(double*_Input, double *_Outout, int _Dtacount)
{
		double **weight_0,
			   weight_changle, weight_ts;
		int i, j, l, k,num,Lnum;
		long count;
		CellControl.Error = (double *)malloc((OUTPUTCELLNUM + 1)*sizeof(double));               //Ϊ�˱�����ɶ��ƻ������ٵ��ڴ�ռ��һ
		weight_0 = (double **)malloc((CellControl.Levelnum) * sizeof(double *));
		for (i = 0; i < (CellControl.Levelnum - 1); i++)                                        //����һ���ݴ�ռ䣬������Ÿ�����ԪȨֵ
			weight_0[i] = (double *)malloc((CellControl.Cellnum[i] * CellControl.Cellnum[i + 1])*sizeof(double));
		weight_0[(CellControl.Levelnum - 1)] = (double *)malloc((CellControl.Cellnum[i] * CellControl.Cellnum[(CellControl.Levelnum - 2)])*sizeof(double));
		cellcopy(CellControl.Weight, weight_0);

		count = 0;
		while (count < 1000000 && (CellControl.Error[0]+CellControl.Error[1]) != 0)
		{
			count++;
			for (num = 0; num < _Dtacount; num++)                                               //��ʼ�����������
			{
				InportDta(_Input, _Outout, num);
				stealth();
				outdata();
				getErrot((_Outout + num*OUTPUTCELLNUM));                                        //�ۼ���������һ�μ������
				CellControl.Error_index = 0;                                                    //�ۼ��������
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
					for (l = (CellControl.Levelnum - 2); l > 0; l--)                      //lΪʣ��δѵ������
						for (k = 0; k < CellControl.Cellnum[(l + 1)]; k++)                   //k��һ����Ԫ����     x+1
						{
							for (j = 0; j < CellControl.Cellnum[l]; j++)                       //jΪ��ǰ����Ԫ����   x
							{
								for (i = 0; i < CellControl.Cellnum[(l - 1)]; i++)              //iΪ��һ����Ԫ����   x-1
								{
									weight_changle = CellControl.efficiency * CellControl.Error_index       //ѧϰЧ�� ���� �����
										* CellControl.Weight[l][(k*CellControl.Cellnum[l] + j)]       //��l�㣬��k*j����ԪȨֵ
										* CellControl.transmit[l + 1][j] * (1 - CellControl.transmit[l + 1][j]) * CellControl.transmit[(l - 1)][i]; //��һ�㴫���� *��1-��һ�㴫������ *��һ�㴫����

									weight_ts = CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)]
										+ weight_changle
										+ CellControl.precision * (CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)] - weight_0[l - 1][(j*CellControl.Cellnum[(l - 1)] + i)]);

									weight_0[l - 1][(j*CellControl.Cellnum[(l - 1)] + i)] = CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)];
									CellControl.Weight[(l - 1)][(j*CellControl.Cellnum[(l - 1)] + i)] = weight_ts;
								}
							}
						}
*/
					for (l = (CellControl.Levelnum - 2); l > 0;l--)                            //lΪ������l�㣬����ȥ�������
					for (i = 0; i < CellControl.Cellnum[l];i++)                                //iΪl���j����Ԫ
					for (j = 0; j < CellControl.Cellnum[(l - 1)]; j++)                         //jΪl+1���i����Ԫ
					{
						weight_changle = 0;
						for (Lnum = 0; Lnum < OUTPUTCELLNUM; Lnum++)           //�ۼӱ仯��   ��l+1����Ԫ��
						{
							weight_changle += CellControl.Error[Lnum]                          //��Lnum��������
							* CellControl.Weight[l][(j*CellControl.Cellnum[(l + 1)] + Lnum)]   //��l+1��j-Lnum��Ȩֵ
							* CellControl.transmit[(2 * l)][j] * (1 - CellControl.transmit[(2 * l)][j]) * CellControl.transmit[(2 * l - 2)][i];//������� *��1-���������* ��������
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
			printf("ѧϰ������%ld ѧϰ��%.4f\r", count,CellControl.Error[0]);
		}
		printf_s("������%.4lf %.4lf %.4lf\n���1 %.4lf ���2��%.4f\n", CellControl.transmit[0][0], CellControl.transmit[0][1], CellControl.transmit[0][2], CellControl.OutputResult[0], CellControl.OutputResult[1]);
        printf("���1��%lf ���2�� %lf\n", CellControl.Error[0], CellControl.Error[1]);
		printf("ѧϰ������%d\n",count);

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

//bp����ʹ��
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