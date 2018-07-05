//
// Created by AG on 2018/6/19.
//

#ifndef DTU_WIFI_MEMALLOC_H
#define DTU_WIFI_MEMALLOC_H

//#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#define MEM_BLOCK_SIZE (32)
#define MAX_MEM_BASE_SIZE (1024*9+480)
#define MAX_ALLOC_TABLE_SIZE (MAX_MEM_BASE_SIZE/MEM_BLOCK_SIZE)
typedef struct
{
    uint8_t Mem_Base[MAX_MEM_BASE_SIZE];//�ڴ��
    uint8_t Mem_Map[MAX_ALLOC_TABLE_SIZE];//�ڴ��¼
    int LastDistAddr;//���һ�η����ڴ�ĵ�ַ
    uint8_t Mem_Flag;
}Memalloc;
void Mem_Init(void);//��ʼ���ڴ�
void *Malloc(uint32_t mem_size);//�ڴ����
void Free(void *ptr);//ɾ���ڴ�
float MemRatioGet(void);//�ڴ�ʹ���� �ٷֱ�


#endif //DTU_WIFI_MEMALLOC_H
