#ifndef __MALLOC_H_
#define __MALLOC_H_
#include "Sys.h"
#if __MALLOC_ENABLE

//////////////////////////////////////////
//�ڴ� ���
#define MEM_BLOCK_SIZE       32                                     //�ڴ���С
#define MEM1_MAX_SIZE        (100*1024)                             //��100K��С�ڲ��ڴ�
#define MEM2_MAX_SIZE        (960*1024)                             //��960K��С�ڲ��ڴ�
#define MEM3_MAX_SIZE        (60*1024)                              //��60K��С�ڲ��ڴ�
#define MEM1_ALLOC_TABLE_IZE (MEM1_MAX_SIZE/MEM_BLOCK_SIZE/8)       //�ڴ��1��С
#define MEM2_ALLOC_TABLE_IZE (MEM2_MAX_SIZE/MEM_BLOCK_SIZE/8)       //�ڴ��2��С
#define MEM3_ALLOC_TABLE_IZE (MEM3_MAX_SIZE/MEM_BLOCK_SIZE/8)       //�ڴ��3��С
#define MEM1_SIDE            (255 * 12 + (u32)&memory1)             //�ڲ��ڴ����߽�
#define MEM2_SIDE            (255 * 12 + 0X680F0F00)                //�ⲿ�ڴ����߽�
//////////////////////////////////////////
//�ڴ�ʹ�÷���
#define MEM_NUB  3   //�ڴ���������
#ifndef NULL
#define NULL     0   //��
#endif
#define MEM_USE  0   //�ڴ�����ʹ��
#define MEM_OFF  0   //δ����
#define MEM_ON   1   //����
#define MEM_OK   1   //�ڴ�״̬OK
#define MEM_NONE 2   //�ڴ�δ��ʹ��
#define MEM_FREE 3   //�ڴ��ͷ����
#define MEM_OVER 4   //�����ڴ�
#define MEM_END  10  //�ڴ���������
//////////////////////////////////////////
//�ڴ�������
#define ERR_NONE     0XFF000000
#define ERR_UNNEED   0XFF000001    //����Ҫ�ڴ�
#define ERR_SHORTAGE 0XFF000002    //�ڴ治��
//////////////////////////////////////////
//���������ڴ��
#define SRAMIN	 0		//�ڲ��ڴ��
#define SRAMEX   1		//�ⲿ�ڴ��
#define SRAMCCM  2		//CCM�ڴ��(�˲���SRAM����CPU���Է���!!!)
//////////////////////////////////////////
//�ڴ�ʹ�������¼ ռ12�ֽ�
struct _m_memory
{
  void **Address;         //��ַ
	u16 Size;               //��Ҫ�ڴ���С
	u8  User;               //�ڴ�ʹ�ñ��
	struct _m_memory *next; //��һ���ڴ�ʹ�����
};
extern struct _m_memory memory1 __attribute__((aligned(4)));
extern struct _m_memory memory2 __attribute__((aligned(4))) __attribute__((at(0X68000000+0X000F0000+0X00000F00)));

//////////////////////////////////////////
//�ڴ���������
struct _m_memory_dev
{
  struct _m_memory *p_memory[MEM_NUB];  //�ڴ�ʹ�������
	u8  (*tab)(u8,u32);                   //�ڴ��ʹ��״̬
	u8  *mem_tab[MEM_NUB];                //�ڴ�ʹ�������߽磬��
	u32 MEM_SIDE[MEM_NUB];                //�ڴ�ʹ�������߽磬��
	u8  status[MEM_NUB];                  //�ڴ���״̬
};
extern struct _m_memory_dev memory_dev;
////////////////////////////////////////////////////////////////////////////////
//���塪������
u8 men_tab(u8 memx,u32 N);                                          //�ڴ�ʹ��״̬
void memory_data_add(u8 memx,void *_srcaddr,void *_address,u16 _size,u8 _user);
void memory_data_delete(u8 memx,struct _m_memory *_address);
struct _m_memory *memory_data_scanf(u8 memx,void *_address);
void Memory_Init(u8 memx);
u8 mem_perused(u8 memx);
void *memmory_malloc(u8 memx,u32 size,u8 _user,void *_srcaddr);
u8 memory_free(u8 memx,void *_address);
void *dsct_malloc(u8 memx,u32 size,u8 _user,void *_srcaddr);
void dsct_free(u8 memx,void *ptr);
void dsct_realloc(u8 memx,void *ptr,u32 Size);
void memory_defragment(u8 memx);


#endif  //__MALLOC_ENABLE

#endif  //__MALLOC_H_
