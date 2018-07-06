//
// Created by AG on 2018/6/19.
//

#include "memalloc.h"


Memalloc Mem={
        .Mem_Flag = 0,
};


void Mem_Init(void)
{
    uint32_t Mem_Len;
    Mem_Len=MAX_MEM_BASE_SIZE;
    while(Mem_Len--)Mem.Mem_Base[Mem_Len]=0;                                                         //清除动态内存的数据
    Mem_Len=MAX_ALLOC_TABLE_SIZE;
    while(Mem_Len--)Mem.Mem_Map[Mem_Len] =0;                                                         //清除记录表
    Mem.Mem_Flag=1;
}


void *Malloc(uint32_t mem_size)
{
    uint32_t len,i;
    uint32_t mem_block = RESET;
    uint32_t Mem_Block = RESET;

    if(!Mem.Mem_Flag)Mem_Init();                                                                     //如果未初始化
    if(mem_size==0)return ((void*)0);                                                                //如果需要的内存为零
    mem_block=mem_size/MEM_BLOCK_SIZE;                                                               //计算需要多少个块
    if(mem_size%MEM_BLOCK_SIZE)mem_block+=1;
    for(len=MAX_ALLOC_TABLE_SIZE;len>0;len--)
    {
        if(Mem.Mem_Map[len]==0)Mem_Block++;                                                          //如果此内存未被分配则递增内存需求
        else Mem_Block=0;                                                                            //如果还未达到内存需求就有被用到的块则此前递增清零
        if(Mem_Block >= mem_block)                                                                   //如果需求达到
        {
            for(i=0;i<Mem_Block;i++)
                Mem.Mem_Map[i+len]=Mem_Block;                                                        //记录此段内存已被分配
            Mem.LastDistAddr = (len*MEM_BLOCK_SIZE)+(uint32_t)&Mem.Mem_Base;                         //计算内存地址
            return (void*)Mem.LastDistAddr;                                                          //分配内存首地址
        }
    }
    //debug("Malloc error\r\n");
    //debug("Use:%0.2f%",MemRatioGet());
    {
        //uint8_t wait = 1;
        //while(wait);//内存不足请手动将变量设置为0退出循环
    }

    return NULL;                                                                                     //如果内存满或者特殊原因
}


float MemRatioGet(void)
{
    float ratio,per;
    ratio = MAX_MEM_BASE_SIZE - (Mem.LastDistAddr - (uint32_t)Mem.Mem_Base);                         //计算使用
    per = (float)((float)100/(float)MAX_MEM_BASE_SIZE) * (float)ratio;                               //计算百分比
    return per;                                                                                      //返回百分比使用率
}


void Free(void *ptr)
{
    uint32_t Base_addr;
    uint32_t map_addr;
    uint32_t len;

    Base_addr=(uint32_t)ptr;                                                                         //获取地址
    Base_addr-=(uint32_t)Mem.Mem_Base;                                                               //内存偏移
    if(Base_addr<MAX_MEM_BASE_SIZE)                                                                  //如果内存偏移量小于内存大小
    {
        Base_addr/=MEM_BLOCK_SIZE;                                                                   //计算此段内存的记录表
        map_addr = Mem.Mem_Map[Base_addr];                                                           //获取此段内存的长度
        for(len=0;len<map_addr;len++)                                                                //清除此段内存在记录表的所有记录
            Mem.Mem_Map[len+Base_addr] = 0;
    }

}

