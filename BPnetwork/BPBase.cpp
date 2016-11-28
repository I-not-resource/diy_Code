#include "BPBase.h"

/*__________________________________________________*/
//
//
//
/*_________________________________________________*/




// 神经元初始化
void cell_int()
{
	int i, j;
	cell.efficiency = 0.6;
	cell.precision = 0.2;

	srand((unsigned)time(NULL));

	for (i = 0; i < IN_LEVE; i++)
		for (j = 0; j < HIDE_LEVE; j++)
			cell.weight_in[i][j] = rand() / (double)(RAND_MAX);
	for (i = 0; i < HIDE_LEVE; i++)
		for (j = 0; j < OUT_LEVE; j++)
			cell.weight_out[i][j] = rand() / (double)(RAND_MAX);

}
//隐层神经元输入端
void stealth()           
{
	int i,j;
	for (j = 0; j < HIDE_LEVE; j++)
	{
		cell.input_next[j] = 0;
		for (i = 0; i < IN_LEVE; i++)
			cell.input_next[j] += cell.weight_in[i][j] * cell.input[i];
	}
}
//隐层神经元激活函数
void activate()        
{
	int j;
	for (j = 0; j < HIDE_LEVE;j++)
	cell.transmit[j] = 1 / (1 + exp((-cell.input_next[j])));
}
//输出层神经元结果
void outdata()       
{
	int j,l;
	for (l = 0; l < OUT_LEVE; l++)
	{
		cell.output_result[l] = 0;
		for (j = 0; j < OUT_LEVE; j++)
			cell.output_result[l] += cell.weight_out[j][l] * cell.transmit[j];
	}
}
//输出误差
void getErrot()    
{
	int l;
	for (l = 0; l < OUT_LEVE;l++)
		cell.Error[l] = cell.output_expect[l] - cell.output_result[l];
}
//将二维数组_weight复制到二维数组weight
void cellcopy(int firstdata, int seconddata, double *_weight, double *weight)
{
	int x, y;
	for (x = 0; x < firstdata; ++x)
		for (y = 0; y < seconddata; ++y)
			weight[(x*seconddata + y)] = _weight[(x*seconddata + y)];
}
//样本导入
void InportDta(double(*_in)[IN_LEVE], int _x, double(*_out)[OUT_LEVE], int _y, int _num)
{
	int i, j;
	for (i = 0; i < _x; i++)
		cell.input[i] = _in[_num][i];
	for (j = 0; j < _y; j++)
		cell.output_expect[j] = _out[_num][j];
}
//bp网络学习
void celltrain(double(*_in)[IN_LEVE], int _x, double(*_out)[OUT_LEVE], int _y, int _num)
{
	double weight_i0[IN_LEVE][HIDE_LEVE]  = {0},
		   weight_o0[HIDE_LEVE][OUT_LEVE] = {0},
		   weight_changle,weight_ts;
	int i, j, l,count,num=1;
	cellcopy(IN_LEVE, HIDE_LEVE, (double *)cell.weight_in, (double *)weight_i0);
	cellcopy(HIDE_LEVE, OUT_LEVE, (double *)cell.weight_out, (double *)weight_o0);

	for (count = 0; count <50000; count++)
	{
		InportDta(_in, _x, _out, _y, num);
		stealth();
		activate();
		outdata();
		getErrot();

		for (l = 0; l < OUT_LEVE; l++)
			for (j = 0; j < HIDE_LEVE; j++)
			{
				weight_changle        = cell.efficiency * cell.Error[l] * cell.transmit[j];
				weight_ts             = cell.weight_out[j][l] + weight_changle + cell.precision * (cell.weight_out[j][l] - weight_o0[j][l]);
				weight_o0[j][l]       = cell.weight_out[j][l];
				cell.weight_out[j][l] = weight_ts;

				for (i = 0; i < IN_LEVE; i++)
				{
					weight_changle       = cell.efficiency * cell.Error[l] * cell.weight_out[j][l] * cell.transmit[j] * (1 - cell.transmit[j]) * cell.input[i];
					weight_ts            = cell.weight_in[i][j] + weight_changle + cell.precision * (cell.weight_in[i][j] - weight_i0[i][j]);
					weight_i0[i][j]      = cell.weight_in[i][j];
					cell.weight_in[i][j] = weight_ts;
				}
			}
		num < (_num-1) ? num++ : num=0;
		printf_s("%.4lf %.4lf %.4lf %.4lf\n", cell.input[0], cell.input[1], cell.input[2],cell.output_result[0]);
	}
}
//bp网络使用
double *BPuse(double *Pdata,int num)
{
	int _i;
	for (_i = 0; _i < num; _i++)
		cell.input[_i] = Pdata[_i];
	stealth();
	activate();
	outdata();
	return cell.output_result;
}
