/*-----------------------------------------------------
/
/File    :malloc.c
/By      :ZPW
/Version :v2.1�����ڴ����뷽ʽ������ʹ��ָ��ָ���ָ��
/        2���ڴ�ʹ�������¼�Ľṹ�壨_m_memory����Address
/           ʹ�÷�ʽ�ı䣬Address�潫Ҫ����ı�����ַ��
/                        *Address������뵽���ڴ��ַ
/
-----------------------------------------------------*/ 

#include "malloc.h"
#if __MALLOC_ENABLE
__align(32) u8 mem1base[MEM1_MAX_SIZE];                                                //�ڲ�SRAM�ڴ��
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));								 //�ⲿSRAM�ڴ��

u8 mem1table[MEM1_ALLOC_TABLE_IZE];                                                     //�ڲ�SRAM�ڴ�ر�
u8 mem2table[MEM2_ALLOC_TABLE_IZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));       //�ⲿSRAM�ڴ�ر�


struct _m_memory memory1;                                                               //�����ڴ�1�ĵ�ַ
struct _m_memory memory2;                                                               //�����ڴ�2�ĵ�ַ
struct _m_memory_dev memory_dev=                                                        //�����ڴ������
{
 &memory1,&memory2,NULL,
 men_tab,
 mem1table,mem2table,NULL,
 MEM1_SIDE,MEM2_SIDE,NULL,
 MEM_OFF,MEM_OFF,NULL,
};

/***************************************************************************
�������ƣ�men_tab
�������ܣ��ڴ��ʹ��״̬������¼�ڴ���ʹ�õ�λ��
������ע��memx,��x���ڴ�
         N���ڴ��λ�ã���Ӧ������λ��
***************************************************************************/
u8 men_tab(u8 memx,u32 N)
{
 u8 sta,bit;
 u8 *p;
 u16 group;                            //һ����8���ڴ�飬��Ӧһ������8λ������
 bit   = N % 8;                        //һ���ڵڼ����ڴ��
 group = N >> 3;                       //�ڼ������ڴ��
 p = memory_dev.mem_tab[memx];         //�ڴ��������ڴ�
 sta   = p[group] >> bit;              //�õ��ڴ��ʹ��״̬
 return sta & 1;                       //�����ڴ��ʹ��״̬
}

/***************************************************************************
�������ƣ�*memory_data_add
�������ܣ��ڴ�ʹ�������Ϣ���
������ע��memx,��x���ڴ�
         *_address�����ڴ��ַ
				 _size���ڴ��С���ڴ�飩
				 _user���ڴ�ʹ�ñ��
***************************************************************************/
void memory_data_add(u8 memx,void *_srcaddr,void *_address,u16 _size,u8 _user)
{
  struct _m_memory *p_mem;                       //��ʱ�ڴ����
	p_mem = memory_dev.p_memory[memx];             //ʹ�õ��ڴ��ַ
	while(p_mem->next != NULL)                     //����Ƿ�����һ��ʹ���ڴ�
	{
    p_mem = p_mem->next;                         //�У������¸��ڴ��ʹ�����
	}
	if((u32)p_mem >= memory_dev.MEM_SIDE[memx])    //����ڴ�������Ƿ�Խ��
		return;                                      //���򷵻�
	else
	{
		p_mem->next     = p_mem + 1;                  //������һ���ڴ������ĵ�ַ
		p_mem           = p_mem->next;
/*------------------------------*/
		p_mem->Address  = _srcaddr;                   //��������Ŀ���ԭ��ַ���������ڴ���Ƭ����
    *p_mem->Address =	_address;                   //����µķ����ڴ��ַ
		p_mem->Size     = _size;                      //����µķ����ڴ��С
		p_mem->User     = _user;                      //����µķ����ڴ���
		p_mem->next     = NULL;                       //��һ���ڴ������Ϊ��
	}
}

/***************************************************************************
�������ƣ�memory_data_delete
�������ܣ��ڴ�ʹ�������Ϣɾ��
������ע��memx,��x���ڴ�
         *_address����Ҫɾ������ʹ���ڴ��ַ
***************************************************************************/
void memory_data_delete(u8 memx,struct _m_memory *_address)
{
	struct _m_memory *p0_mem,*p1_mem;              //������ʱ�ڴ������0��1
	p0_mem = memory_dev.p_memory[memx];            //����ڴ������ĵ�һ����
  while(*p0_mem->Address != _address)            //Ѱ��Ҫɾ�����ڴ������
		p0_mem = p0_mem->next;                       //�����һ����
	if( p0_mem->next != NULL)
	{
		p1_mem          = p0_mem->next;              //������ʱ�ڴ������1Ϊ��ɾ����
		p1_mem->Address = NULL;                      //��Ҫɾ�����ڴ��ַ����
		p0_mem->next    = p1_mem->next;              //��Ҫɾ�����ϸ��ڴ������ָ��ָ����һ���ڴ������
	}
}

/***************************************************************************
�������ƣ�*memory_data_scanf
�������ܣ��ڴ������Ϣɨ��
������ע��memx,��x���ڴ�
         *_address����Ҫ���ҵ���ʹ���ڴ��ַ
***************************************************************************/
struct _m_memory *memory_data_scanf(u8 memx,void *_address)
{
  struct _m_memory *p_mem;                       //������ʱ�ڴ������
	p_mem = memory_dev.p_memory[memx];             //����ڴ������ĵ�һ����
	while(*p_mem->Address != _address)             //��⵱ǰ������ǲ��ǽ�Ҫ���ҵ�
	{
		if(p_mem->next == NULL)                      //�����һ���ڴ�������Ƿ�Ϊ��
			return NULL;                               //��������
		p_mem = p_mem->next;                         //�����һ����
	}
	return p_mem;                                  //���ز��ҵ����ڴ�������ַ
}

/***************************************************************************
�������ƣ�Memory_Init
�������ܣ��ڴ�����ʼ��
������ע��memx,��x���ڴ�
***************************************************************************/
void Memory_Init(u8 memx)
{
	u8  *xx;
	u32 count;
	switch(memx)
	{
    case 0:	memory_dev.p_memory[0]->Address = (void*)mem1base;  //��ʼ���ڴ�1���ڴ��ַ
	          memory_dev.p_memory[0]->next    = NULL;      //��ʼ���ڴ�1����һ��Ϊ��
	          memory_dev.p_memory[0]->Size    = 3200;      //��ʼ���ڴ�1���ڴ������
	          memory_dev.p_memory[0]->User    = MEM_USE;   //��ʼ���ڴ�1���ڴ�Ϊ����ʹ��
		break;
		case 1:memory_dev.p_memory[1]->Address = (void*)mem2base;   //��ʼ���ڴ�2���ڴ��ַ
	         memory_dev.p_memory[1]->next    = NULL;       //��ʼ���ڴ�2����һ��Ϊ��
	         memory_dev.p_memory[1]->Size    = 30720;      //��ʼ���ڴ�2���ڴ������
	         memory_dev.p_memory[1]->User    = MEM_USE;    //��ʼ���ڴ�2���ڴ�Ϊ����ʹ��
		break;
	}
	count = memory_dev.p_memory[memx]->Size;               //��ý���ʼ�����ڴ������
	xx = (u8*)memory_dev.p_memory[memx]->Address;               //��õ��ڴ��ַ
	while(count--)
		*xx++ = NULL;                                        //����ڴ�����
	
	count = memory_dev.p_memory[memx]->Size >> 3;          //��ý���ʼ�����ڴ������
	xx = memory_dev.mem_tab[memx];                         //�ڴ�������ַ
	while(count--)
		*xx++ = NULL;                                        //�ڴ��ȫ���㣬������Ϊδʹ��״̬
	
	memory_dev.status[memx] = MEM_ON;                      //���ڴ���Ϊ��ʼ�����
}

/***************************************************************************
�������ƣ�mem_perused
�������ܣ��ڴ�ʹ����
������ע��memx,��x���ڴ�
***************************************************************************/
u8 mem_perused(u8 memx)
{
  u32 used=0,i=memory_dev.p_memory[memx]->Size;          //ʹ��������㣬����ڴ������
	for(;i>0;i--)
	  used += (memory_dev.tab(memx,i) == 1);               //�ڴ�鱻ʹ�ã�used�Լ�1
	return (used*100/memory_dev.p_memory[memx]->Size);     //����ʹ�ðٷֱ�
}

/***************************************************************************
�������ƣ�*memmory_malloc
�������ܣ������ڴ棬����
������ע��memx,��x���ڴ�
         size,������ڴ��С���ֽڣ�
				 _user���ڴ�ʹ�ñ��
         *_srcaddr,��������ַ
***************************************************************************/
void *memmory_malloc(u8 memx,u32 size,u8 _user,void *_srcaddr)
{
  long  num;                                                                      //ʣ���
  u16 nblock,cblock,i;                                                            //������Ҫ�ڴ棬���������ڴ�
	u8 *_address;                                                                   //ƫ�Ƶ�ַ
	if(size == 0)                                                                   //�����Ҫ���ڴ��СΪ0
		return (void *)ERR_UNNEED;                                                    //�򷵻ز���Ҫ�ڴ�
	if(memory_dev.status[memx]!=MEM_ON)                                             //�����Ӧ�ڴ��Ƿ��ʼ��
		 Memory_Init(memx);                                                           //û�����ʼ��һ��
	nblock = (size + (MEM_BLOCK_SIZE - 1)) >> 5 ;                                   //�õ���Ҫ���ڴ������
	
	for(num=memory_dev.p_memory[memx]->Size-1;num>=0;num--)                         //������ڴ�����������ݼ�
	{
    if(!memory_dev.tab(memx,num))                                                 //����ڴ���Ƿ�Ϊ��
			cblock++;                                                                   //�ǣ������ڴ���1
		else
			cblock = 0;                                                                 //���ǣ������ڴ��������
		if(cblock == nblock)                                                          //�ж��Ƿ��ҵ���Ҫ�������ڴ��С
		{
	    for(i = 0;i<nblock;i++)                                                     //��Ҫʹ�õ��ڴ����Ϊ��ʹ��
			{
			  memory_dev.mem_tab[memx][((i + num) >> 3)] |= 1 << ((i + num) % 8);
			}
			_address  = (u8 *)((num << 5) + (long)memory_dev.p_memory[memx]->Address);  //��ý�Ҫ������ڴ��ַ
			memory_data_add(memx,_srcaddr,_address,nblock,_user);                       //����µ��ڴ������
			return _address;                                                            //����ָ���ڴ��ַ�ĵ�ַ
	  }
	}
	return (void *)ERR_SHORTAGE;                                                    //�ڴ�鲻�������ش�����Ϣ
}

/***************************************************************************
�������ƣ�memory_free
�������ܣ��ڴ��ͷţ�����
������ע��memx����x���ڴ�
         *_address����Ҫ�ͷŵ��ڴ��ַ
***************************************************************************/
u8 memory_free(u8 memx,void *_address)
{
  int i;
	long num;
	struct _m_memory *p_mem;
	if(memory_dev.status[memx] != MEM_ON)                                        //����ڴ��������ʼ��
	{
		Memory_Init(memx);                                                         //�ڴ������δ��ʼ�������ʼ��һ��
		return MEM_OFF;                                                            //�����ش�����Ϣ
	}
    p_mem =  memory_data_scanf(memx, _address);                                //��ý�Ҫ�ͷŵ���������
		num = ((u32)*p_mem->Address - (u32)memory_dev.p_memory[memx]) >> 5;        //����ڴ����ʼλ��
		for(i = p_mem->Size - 1;i>=0;i--)
		  memory_dev.mem_tab[memx][((i + num) >> 3)] &= ~(1 <<((i + num) % 8));    //���ͷŵ��ڴ������
		memory_data_delete(memx,p_mem);                                            //ɾ���ͷ��ڴ���ڴ������ 
		return MEM_FREE;                                                           //������ɱ�־
}

/***************************************************************************
�������ƣ�*dsct_malloc
�������ܣ��ڴ����룬���ã�����ֵΪָ��ָ���ָ��
������ע��memx,��x���ڴ�
         size,������ڴ��С���ֽڣ�
				 _user,�ڴ�ʹ�ñ��
         *_srcaddr,��������ַ
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
�������ƣ�dsct_free
�������ܣ��ڴ��ͷţ�����
������ע��memx,��x���ڴ�
         *ptr����Ҫ�ͷ��ڴ��Ŀ���ַ
***************************************************************************/
void dsct_free(u8 memx,void *ptr)
{
	if(ptr == NULL)
		return;
	memory_free(memx,ptr);
}

/***************************************************************************
�������ƣ�dsct_realloc
�������ܣ����ڴ����·���
������ע��memx,��x���ڴ�
         *ptr����Ҫ�ͷ��ڴ��Ŀ���ַ
				 Size�����·���Ĵ�С
***************************************************************************/
void dsct_realloc(u8 memx,void *ptr,u32 Size)
{
  long num0,num1;
  u32 nblock,cblock,i;
	void *_address;
	u8 *address0,*address1;
	struct _m_memory *p_mem;
	
	if(memory_dev.status[memx] != MEM_ON)                                           //����ڴ��Ƿ��ʼ��
		Memory_Init(memx);                                                            //δ��ʼ������ʼ��һ��
		
	if(Size == 0)                                                                   //������·���Ĵ�СΪ��0
	{
		dsct_free(memx,ptr);                                                          //Ϊ0���ͷ��ڴ�
		return;
	}
		
	p_mem =  memory_data_scanf(memx,ptr);                                            //���ҽ�Ҫ���·�����ڴ������
		
	nblock = (Size + (MEM_BLOCK_SIZE - 1)) >> 5 ;	                                   //�����Ҫ���·�����ڴ������
	for(num0=memory_dev.p_memory[memx]->Size-1;num0>=0;num0--)                       //���ȫ���ڴ�����������ݼ�
	{
    if(!memory_dev.tab(memx,num0))                                                 //����ڴ���Ƿ�Ϊ��
			cblock++;                                                                    //�ǣ������ڴ���1
		else
			cblock = 0;                                                                  //���ǣ������ڴ��������
		if(cblock == nblock)                                                           //�ж��Ƿ��ҵ���Ҫ�������ڴ��С
		{
	    for(i = 0;i<nblock;i++)
			{
			  memory_dev.mem_tab[memx][((i + num0) >> 3)] |= 1 << ((i + num0) % 8);      //��־���·�����ڴ��
			}

			num1 = ((u32)*p_mem->Address - (u32)memory_dev.p_memory[memx]) >> 5;         //�ڴ�����·������ʼ����
	  	for(i = p_mem->Size;i>0;i--)
		  memory_dev.mem_tab[memx][((i + num1 -1) >> 3)] &= ~(1 <<((i + num1 -1) % 8));//�����ڴ������
			
			p_mem->Size    = nblock;                                                     //�������·�����ڴ��С
			_address =  (u8 *)((num0 << 5) + (long)memory_dev.p_memory[memx]->Address);  //��ý�Ҫ������ڴ��ַ
			
			address0 = (u8*)*p_mem->Address;                                             //���ڴ��ַ
			address1 = _address;                                                         //���ڴ��ַ
			while(Size--)
			*address1++ = *address0++;                                                   //���ݸ��ǣ����ݰ���
			
			*p_mem->Address = _address;                                                  //�������·�����ڴ��ַ
			return;
	  }
	}
	
}

/***************************************************************************
�������ƣ�memory_defragment
�������ܣ�
������ע��
***************************************************************************/
void memory_defragment(u8 memx)
{
    long num0,num1,Size;
  u32 nblock,cblock,i;
	void *_address;
	u8 *address0,*address1;
	struct _m_memory *p_mem;
	
	if(memory_dev.status[memx] != MEM_ON)                                           //����ڴ��Ƿ��ʼ��
		Memory_Init(memx);                                                            //δ��ʼ������ʼ��һ��
		
	p_mem =  memory_dev.p_memory[memx];                                             //���ҽ�Ҫ���·�����ڴ������
	while(p_mem->next != NULL)
	{
	  p_mem = p_mem->next;
		Size = p_mem->Size;
		nblock = (Size + (MEM_BLOCK_SIZE - 1)) >> 5 ;	                                   //�����Ҫ���·�����ڴ������
		for(num0=memory_dev.p_memory[memx]->Size-1;num0>=0;num0--)                       //���ȫ���ڴ�����������ݼ�
		{
			if(!memory_dev.tab(memx,num0))                                                 //����ڴ���Ƿ�Ϊ��
			{
				for(i = Size;i>0;i--)
				  memory_dev.mem_tab[memx][((i + num1 -1) >> 3)] &= ~(1 <<((i + num1 -1) % 8));//�����ڴ������
				
				for(i = 0;i<nblock;i++)
					memory_dev.mem_tab[memx][((i + num0) >> 3)] |= 1 << ((i + num0) % 8);      //��־���·�����ڴ��

				num1 = ((u32)*p_mem->Address - (u32)memory_dev.p_memory[memx]) >> 5;         //�ڴ�����·������ʼ����
				
				p_mem->Size    = nblock;                                                     //�������·�����ڴ��С
				_address =  (u8 *)((num0 << 5) + (long)memory_dev.p_memory[memx]->Address);  //��ý�Ҫ������ڴ��ַ
				
				address0 = (u8*)*p_mem->Address;                                             //���ڴ��ַ
				address1 = _address;                                                         //���ڴ��ַ
				while(Size--)
				*address1++ = *address0++;                                                   //���ݸ��ǣ����ݰ���
				
				*p_mem->Address = _address;                                                  //�������·�����ڴ��ַ
				return;
			}
		}
	}
}






#endif  //__MALLOC_ENABLE
