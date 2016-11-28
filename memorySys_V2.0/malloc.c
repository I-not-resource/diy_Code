/*-----------------------------------------------------
/
/File    :malloc.c
/By      :ZPW
/Version :v2.1更改内存申请方式，不再使用指向指针的指针
/        2、内存使用情况记录的结构体（_m_memory）的Address
/           使用方式改变，Address存将要申请的变量地址，
/                        *Address存放申请到的内存地址
/
-----------------------------------------------------*/ 

#include "malloc.h"
#if __MALLOC_ENABLE
__align(32) u8 mem1base[MEM1_MAX_SIZE];                                                //内部SRAM内存池
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));								 //外部SRAM内存池

u8 mem1table[MEM1_ALLOC_TABLE_IZE];                                                     //内部SRAM内存池表
u8 mem2table[MEM2_ALLOC_TABLE_IZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));       //外部SRAM内存池表


struct _m_memory memory1;                                                               //定义内存1的地址
struct _m_memory memory2;                                                               //定义内存2的地址
struct _m_memory_dev memory_dev=                                                        //定义内存管理器
{
 &memory1,&memory2,NULL,
 men_tab,
 mem1table,mem2table,NULL,
 MEM1_SIDE,MEM2_SIDE,NULL,
 MEM_OFF,MEM_OFF,NULL,
};

/***************************************************************************
函数名称：men_tab
函数功能：内存块使用状态，即记录内存已使用的位置
函数备注：memx,第x个内存
         N，内存块位置（对应二进制位）
***************************************************************************/
u8 men_tab(u8 memx,u32 N)
{
 u8 sta,bit;
 u8 *p;
 u16 group;                            //一组有8个内存块，对应一个数的8位二进制
 bit   = N % 8;                        //一组内第几个内存块
 group = N >> 3;                       //第几组组内存块
 p = memory_dev.mem_tab[memx];         //内存块的所属内存
 sta   = p[group] >> bit;              //得到内存的使用状态
 return sta & 1;                       //返回内存的使用状态
}

/***************************************************************************
函数名称：*memory_data_add
函数功能：内存使用情况信息添加
函数备注：memx,第x个内存
         *_address分配内存地址
				 _size，内存大小（内存块）
				 _user，内存使用标号
***************************************************************************/
void memory_data_add(u8 memx,void *_srcaddr,void *_address,u16 _size,u8 _user)
{
  struct _m_memory *p_mem;                       //临时内存管理
	p_mem = memory_dev.p_memory[memx];             //使用的内存地址
	while(p_mem->next != NULL)                     //检查是否有下一个使用内存
	{
    p_mem = p_mem->next;                         //有，则获得下个内存的使用情况
	}
	if((u32)p_mem >= memory_dev.MEM_SIDE[memx])    //检查内存情况表是否越界
		return;                                      //是则返回
	else
	{
		p_mem->next     = p_mem + 1;                  //分配下一个内存情况表的地址
		p_mem           = p_mem->next;
/*------------------------------*/
		p_mem->Address  = _srcaddr;                   //保存申请目标的原地址，可用于内存碎片管理
    *p_mem->Address =	_address;                   //添加新的分配内存地址
		p_mem->Size     = _size;                      //添加新的分配内存大小
		p_mem->User     = _user;                      //添加新的分配内存编号
		p_mem->next     = NULL;                       //下一个内存情况表为空
	}
}

/***************************************************************************
函数名称：memory_data_delete
函数功能：内存使用情况信息删除
函数备注：memx,第x个内存
         *_address，将要删除的已使用内存地址
***************************************************************************/
void memory_data_delete(u8 memx,struct _m_memory *_address)
{
	struct _m_memory *p0_mem,*p1_mem;              //定义临时内存情况表0、1
	p0_mem = memory_dev.p_memory[memx];            //获得内存情况表的第一个表
  while(*p0_mem->Address != _address)            //寻找要删除的内存情况表
		p0_mem = p0_mem->next;                       //获得下一个表
	if( p0_mem->next != NULL)
	{
		p1_mem          = p0_mem->next;              //定义临时内存情况表1为将删除的
		p1_mem->Address = NULL;                      //将要删除的内存地址清零
		p0_mem->next    = p1_mem->next;              //将要删除的上个内存情况表指向指向下一个内存情况表
	}
}

/***************************************************************************
函数名称：*memory_data_scanf
函数功能：内存情况信息扫描
函数备注：memx,第x个内存
         *_address，将要查找的已使用内存地址
***************************************************************************/
struct _m_memory *memory_data_scanf(u8 memx,void *_address)
{
  struct _m_memory *p_mem;                       //定义临时内存情况表
	p_mem = memory_dev.p_memory[memx];             //获得内存情况表的第一个表
	while(*p_mem->Address != _address)             //检测当前情况表是不是将要查找的
	{
		if(p_mem->next == NULL)                      //检查下一个内存情况表是否为空
			return NULL;                               //是则跳出
		p_mem = p_mem->next;                         //获得下一个表
	}
	return p_mem;                                  //返回查找到的内存情况表地址
}

/***************************************************************************
函数名称：Memory_Init
函数功能：内存管理初始化
函数备注：memx,第x个内存
***************************************************************************/
void Memory_Init(u8 memx)
{
	u8  *xx;
	u32 count;
	switch(memx)
	{
    case 0:	memory_dev.p_memory[0]->Address = (void*)mem1base;  //初始化内存1的内存地址
	          memory_dev.p_memory[0]->next    = NULL;      //初始化内存1的下一个为空
	          memory_dev.p_memory[0]->Size    = 3200;      //初始化内存1的内存块数量
	          memory_dev.p_memory[0]->User    = MEM_USE;   //初始化内存1的内存为自身使用
		break;
		case 1:memory_dev.p_memory[1]->Address = (void*)mem2base;   //初始化内存2的内存地址
	         memory_dev.p_memory[1]->next    = NULL;       //初始化内存2的下一个为空
	         memory_dev.p_memory[1]->Size    = 30720;      //初始化内存2的内存块数量
	         memory_dev.p_memory[1]->User    = MEM_USE;    //初始化内存2的内存为自身使用
		break;
	}
	count = memory_dev.p_memory[memx]->Size;               //获得将初始化的内存块数量
	xx = (u8*)memory_dev.p_memory[memx]->Address;               //获得的内存地址
	while(count--)
		*xx++ = NULL;                                        //清空内存内容
	
	count = memory_dev.p_memory[memx]->Size >> 3;          //获得将初始化的内存块组数
	xx = memory_dev.mem_tab[memx];                         //内存块情况地址
	while(count--)
		*xx++ = NULL;                                        //内存块全置零，即划分为未使用状态
	
	memory_dev.status[memx] = MEM_ON;                      //将内存标记为初始化完成
}

/***************************************************************************
函数名称：mem_perused
函数功能：内存使用率
函数备注：memx,第x个内存
***************************************************************************/
u8 mem_perused(u8 memx)
{
  u32 used=0,i=memory_dev.p_memory[memx]->Size;          //使用情况置零，获得内存块数量
	for(;i>0;i--)
	  used += (memory_dev.tab(memx,i) == 1);               //内存块被使用，used自加1
	return (used*100/memory_dev.p_memory[memx]->Size);     //返回使用百分比
}

/***************************************************************************
函数名称：*memmory_malloc
函数功能：申请内存，内用
函数备注：memx,第x个内存
         size,申请的内存大小（字节）
				 _user，内存使用标号
         *_srcaddr,申请对象地址
***************************************************************************/
void *memmory_malloc(u8 memx,u32 size,u8 _user,void *_srcaddr)
{
  long  num;                                                                      //剩余块
  u16 nblock,cblock,i;                                                            //定义需要内存，连续空余内存
	u8 *_address;                                                                   //偏移地址
	if(size == 0)                                                                   //如果需要的内存大小为0
		return (void *)ERR_UNNEED;                                                    //则返回不需要内存
	if(memory_dev.status[memx]!=MEM_ON)                                             //检查相应内寸是否初始化
		 Memory_Init(memx);                                                           //没有则初始化一遍
	nblock = (size + (MEM_BLOCK_SIZE - 1)) >> 5 ;                                   //得到需要的内存块数量
	
	for(num=memory_dev.p_memory[memx]->Size-1;num>=0;num--)                         //获得总内存块数量，并递减
	{
    if(!memory_dev.tab(memx,num))                                                 //检查内存块是否为空
			cblock++;                                                                   //是，连续内存块加1
		else
			cblock = 0;                                                                 //不是，连续内存块数清零
		if(cblock == nblock)                                                          //判断是否找到需要的连续内存大小
		{
	    for(i = 0;i<nblock;i++)                                                     //将要使用的内存块标记为已使用
			{
			  memory_dev.mem_tab[memx][((i + num) >> 3)] |= 1 << ((i + num) % 8);
			}
			_address  = (u8 *)((num << 5) + (long)memory_dev.p_memory[memx]->Address);  //获得将要分配的内存地址
			memory_data_add(memx,_srcaddr,_address,nblock,_user);                       //添加新的内存情况表
			return _address;                                                            //返回指向内存地址的地址
	  }
	}
	return (void *)ERR_SHORTAGE;                                                    //内存块不够，返回错误信息
}

/***************************************************************************
函数名称：memory_free
函数功能：内存释放，内用
函数备注：memx，第x个内存
         *_address，将要释放的内存地址
***************************************************************************/
u8 memory_free(u8 memx,void *_address)
{
  int i;
	long num;
	struct _m_memory *p_mem;
	if(memory_dev.status[memx] != MEM_ON)                                        //检查内存管理器初始化
	{
		Memory_Init(memx);                                                         //内存管理器未初始化，则初始化一遍
		return MEM_OFF;                                                            //并返回错误信息
	}
    p_mem =  memory_data_scanf(memx, _address);                                //获得将要释放的情况管理表
		num = ((u32)*p_mem->Address - (u32)memory_dev.p_memory[memx]) >> 5;        //获得内存块起始位置
		for(i = p_mem->Size - 1;i>=0;i--)
		  memory_dev.mem_tab[memx][((i + num) >> 3)] &= ~(1 <<((i + num) % 8));    //将释放的内存块清零
		memory_data_delete(memx,p_mem);                                            //删除释放内存的内存情况表 
		return MEM_FREE;                                                           //返回完成标志
}

/***************************************************************************
函数名称：*dsct_malloc
函数功能：内存申请，外用，返回值为指向指针的指针
函数备注：memx,第x个内存
         size,申请的内存大小（字节）
				 _user,内存使用标号
         *_srcaddr,申请对象地址
***************************************************************************/
void *dsct_malloc(u8 memx,u32 size,u8 _user,void *_srcaddr)
{
  void *offset;
	offset = memmory_malloc(memx,size, _user,_srcaddr);
	if(offset > (void *)ERR_NONE)
		return NULL;
	else return offset;
}

/***************************************************************************
函数名称：dsct_free
函数功能：内存释放，外用
函数备注：memx,第x个内存
         *ptr，将要释放内存的目标地址
***************************************************************************/
void dsct_free(u8 memx,void *ptr)
{
	if(ptr == NULL)
		return;
	memory_free(memx,ptr);
}

/***************************************************************************
函数名称：dsct_realloc
函数功能：单内存重新分配
函数备注：memx,第x个内存
         *ptr，将要释放内存的目标地址
				 Size，重新分配的大小
***************************************************************************/
void dsct_realloc(u8 memx,void *ptr,u32 Size)
{
  long num0,num1;
  u32 nblock,cblock,i;
	void *_address;
	u8 *address0,*address1;
	struct _m_memory *p_mem;
	
	if(memory_dev.status[memx] != MEM_ON)                                           //检查内存是否初始化
		Memory_Init(memx);                                                            //未初始化，初始化一遍
		
	if(Size == 0)                                                                   //检查重新分配的大小为否0
	{
		dsct_free(memx,ptr);                                                          //为0则释放内存
		return;
	}
		
	p_mem =  memory_data_scanf(memx,ptr);                                            //查找将要重新分配的内存情况表
		
	nblock = (Size + (MEM_BLOCK_SIZE - 1)) >> 5 ;	                                   //获得需要重新分配的内存块数量
	for(num0=memory_dev.p_memory[memx]->Size-1;num0>=0;num0--)                       //获得全部内存块数量，并递减
	{
    if(!memory_dev.tab(memx,num0))                                                 //检查内存块是否为空
			cblock++;                                                                    //是，连续内存块加1
		else
			cblock = 0;                                                                  //不是，连续内存块数清零
		if(cblock == nblock)                                                           //判断是否找到需要的连续内存大小
		{
	    for(i = 0;i<nblock;i++)
			{
			  memory_dev.mem_tab[memx][((i + num0) >> 3)] |= 1 << ((i + num0) % 8);      //标志重新分配的内存块
			}

			num1 = ((u32)*p_mem->Address - (u32)memory_dev.p_memory[memx]) >> 5;         //内存块重新分配的起始块数
	  	for(i = p_mem->Size;i>0;i--)
		  memory_dev.mem_tab[memx][((i + num1 -1) >> 3)] &= ~(1 <<((i + num1 -1) % 8));//将旧内存块清零
			
			p_mem->Size    = nblock;                                                     //更新重新分配的内存大小
			_address =  (u8 *)((num0 << 5) + (long)memory_dev.p_memory[memx]->Address);  //获得将要分配的内存地址
			
			address0 = (u8*)*p_mem->Address;                                             //旧内存地址
			address1 = _address;                                                         //新内存地址
			while(Size--)
			*address1++ = *address0++;                                                   //数据覆盖，数据搬移
			
			*p_mem->Address = _address;                                                  //更新重新分配的内存地址
			return;
	  }
	}
	
}

/***************************************************************************
函数名称：memory_defragment
函数功能：
函数备注：
***************************************************************************/
void memory_defragment(u8 memx)
{
    long num0,num1,Size;
  u32 nblock,cblock,i;
	void *_address;
	u8 *address0,*address1;
	struct _m_memory *p_mem;
	
	if(memory_dev.status[memx] != MEM_ON)                                           //检查内存是否初始化
		Memory_Init(memx);                                                            //未初始化，初始化一遍
		
	p_mem =  memory_dev.p_memory[memx];                                             //查找将要重新分配的内存情况表
	while(p_mem->next != NULL)
	{
	  p_mem = p_mem->next;
		Size = p_mem->Size;
		nblock = (Size + (MEM_BLOCK_SIZE - 1)) >> 5 ;	                                   //获得需要重新分配的内存块数量
		for(num0=memory_dev.p_memory[memx]->Size-1;num0>=0;num0--)                       //获得全部内存块数量，并递减
		{
			if(!memory_dev.tab(memx,num0))                                                 //检查内存块是否为空
			{
				for(i = Size;i>0;i--)
				  memory_dev.mem_tab[memx][((i + num1 -1) >> 3)] &= ~(1 <<((i + num1 -1) % 8));//将旧内存块清零
				
				for(i = 0;i<nblock;i++)
					memory_dev.mem_tab[memx][((i + num0) >> 3)] |= 1 << ((i + num0) % 8);      //标志重新分配的内存块

				num1 = ((u32)*p_mem->Address - (u32)memory_dev.p_memory[memx]) >> 5;         //内存块重新分配的起始块数
				
				p_mem->Size    = nblock;                                                     //更新重新分配的内存大小
				_address =  (u8 *)((num0 << 5) + (long)memory_dev.p_memory[memx]->Address);  //获得将要分配的内存地址
				
				address0 = (u8*)*p_mem->Address;                                             //旧内存地址
				address1 = _address;                                                         //新内存地址
				while(Size--)
				*address1++ = *address0++;                                                   //数据覆盖，数据搬移
				
				*p_mem->Address = _address;                                                  //更新重新分配的内存地址
				return;
			}
		}
	}
}






#endif  //__MALLOC_ENABLE
