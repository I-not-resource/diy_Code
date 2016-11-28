#ifndef __MALLOC_H_
#define __MALLOC_H_
#include "Sys.h"
#if __MALLOC_ENABLE

//////////////////////////////////////////
//内存 情况
#define MEM_BLOCK_SIZE       32                                     //内存块大小
#define MEM1_MAX_SIZE        (100*1024)                             //开100K大小内部内存
#define MEM2_MAX_SIZE        (960*1024)                             //开960K大小内部内存
#define MEM3_MAX_SIZE        (60*1024)                              //开60K大小内部内存
#define MEM1_ALLOC_TABLE_IZE (MEM1_MAX_SIZE/MEM_BLOCK_SIZE/8)       //内存表1大小
#define MEM2_ALLOC_TABLE_IZE (MEM2_MAX_SIZE/MEM_BLOCK_SIZE/8)       //内存表2大小
#define MEM3_ALLOC_TABLE_IZE (MEM3_MAX_SIZE/MEM_BLOCK_SIZE/8)       //内存表3大小
#define MEM1_SIDE            (255 * 12 + (u32)&memory1)             //内部内存管理边界
#define MEM2_SIDE            (255 * 12 + 0X680F0F00)                //外部内存管理边界
//////////////////////////////////////////
//内存使用符号
#define MEM_NUB  3   //内存器件数量
#ifndef NULL
#define NULL     0   //空
#endif
#define MEM_USE  0   //内存自身使用
#define MEM_OFF  0   //未就绪
#define MEM_ON   1   //就绪
#define MEM_OK   1   //内存状态OK
#define MEM_NONE 2   //内存未被使用
#define MEM_FREE 3   //内存释放完成
#define MEM_OVER 4   //超出内存
#define MEM_END  10  //内存管理表已满
//////////////////////////////////////////
//内存错误代号
#define ERR_NONE     0XFF000000
#define ERR_UNNEED   0XFF000001    //不需要内存
#define ERR_SHORTAGE 0XFF000002    //内存不够
//////////////////////////////////////////
//定义三个内存池
#define SRAMIN	 0		//内部内存池
#define SRAMEX   1		//外部内存池
#define SRAMCCM  2		//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)
//////////////////////////////////////////
//内存使用情况记录 占12字节
struct _m_memory
{
  void **Address;         //地址
	u16 Size;               //需要内存块大小
	u8  User;               //内存使用编号
	struct _m_memory *next; //下一个内存使用情况
};
extern struct _m_memory memory1 __attribute__((aligned(4)));
extern struct _m_memory memory2 __attribute__((aligned(4))) __attribute__((at(0X68000000+0X000F0000+0X00000F00)));

//////////////////////////////////////////
//内存管理控制器
struct _m_memory_dev
{
  struct _m_memory *p_memory[MEM_NUB];  //内存使用情况表
	u8  (*tab)(u8,u32);                   //内存块使用状态
	u8  *mem_tab[MEM_NUB];                //内存使用情况表边界，低
	u32 MEM_SIDE[MEM_NUB];                //内存使用情况表边界，高
	u8  status[MEM_NUB];                  //内存器状态
};
extern struct _m_memory_dev memory_dev;
////////////////////////////////////////////////////////////////////////////////
//定义——函数
u8 men_tab(u8 memx,u32 N);                                          //内存使用状态
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
