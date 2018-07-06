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
    while(Mem_Len--)Mem.Mem_Base[Mem_Len]=0;                                                         //�����̬�ڴ������
    Mem_Len=MAX_ALLOC_TABLE_SIZE;
    while(Mem_Len--)Mem.Mem_Map[Mem_Len] =0;                                                         //�����¼��
    Mem.Mem_Flag=1;
}


void *Malloc(uint32_t mem_size)
{
    uint32_t len,i;
    uint32_t mem_block = RESET;
    uint32_t Mem_Block = RESET;

    if(!Mem.Mem_Flag)Mem_Init();                                                                     //���δ��ʼ��
    if(mem_size==0)return ((void*)0);                                                                //�����Ҫ���ڴ�Ϊ��
    mem_block=mem_size/MEM_BLOCK_SIZE;                                                               //������Ҫ���ٸ���
    if(mem_size%MEM_BLOCK_SIZE)mem_block+=1;
    for(len=MAX_ALLOC_TABLE_SIZE;len>0;len--)
    {
        if(Mem.Mem_Map[len]==0)Mem_Block++;                                                          //������ڴ�δ������������ڴ�����
        else Mem_Block=0;                                                                            //�����δ�ﵽ�ڴ�������б��õ��Ŀ����ǰ��������
        if(Mem_Block >= mem_block)                                                                   //�������ﵽ
        {
            for(i=0;i<Mem_Block;i++)
                Mem.Mem_Map[i+len]=Mem_Block;                                                        //��¼�˶��ڴ��ѱ�����
            Mem.LastDistAddr = (len*MEM_BLOCK_SIZE)+(uint32_t)&Mem.Mem_Base;                         //�����ڴ��ַ
            return (void*)Mem.LastDistAddr;                                                          //�����ڴ��׵�ַ
        }
    }
    //debug("Malloc error\r\n");
    //debug("Use:%0.2f%",MemRatioGet());
    {
        //uint8_t wait = 1;
        //while(wait);//�ڴ治�����ֶ�����������Ϊ0�˳�ѭ��
    }

    return NULL;                                                                                     //����ڴ�����������ԭ��
}


float MemRatioGet(void)
{
    float ratio,per;
    ratio = MAX_MEM_BASE_SIZE - (Mem.LastDistAddr - (uint32_t)Mem.Mem_Base);                         //����ʹ��
    per = (float)((float)100/(float)MAX_MEM_BASE_SIZE) * (float)ratio;                               //����ٷֱ�
    return per;                                                                                      //���ذٷֱ�ʹ����
}


void Free(void *ptr)
{
    uint32_t Base_addr;
    uint32_t map_addr;
    uint32_t len;

    Base_addr=(uint32_t)ptr;                                                                         //��ȡ��ַ
    Base_addr-=(uint32_t)Mem.Mem_Base;                                                               //�ڴ�ƫ��
    if(Base_addr<MAX_MEM_BASE_SIZE)                                                                  //����ڴ�ƫ����С���ڴ��С
    {
        Base_addr/=MEM_BLOCK_SIZE;                                                                   //����˶��ڴ�ļ�¼��
        map_addr = Mem.Mem_Map[Base_addr];                                                           //��ȡ�˶��ڴ�ĳ���
        for(len=0;len<map_addr;len++)                                                                //����˶��ڴ��ڼ�¼������м�¼
            Mem.Mem_Map[len+Base_addr] = 0;
    }

}

