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
    uint8_t Mem_Base[MAX_MEM_BASE_SIZE];//内存池
    uint8_t Mem_Map[MAX_ALLOC_TABLE_SIZE];//内存记录
    int LastDistAddr;//最后一次分配内存的地址
    uint8_t Mem_Flag;
}Memalloc;
void Mem_Init(void);//初始化内存
void *Malloc(uint32_t mem_size);//内存分配
void Free(void *ptr);//删除内存
float MemRatioGet(void);//内存使用率 百分比


#endif //DTU_WIFI_MEMALLOC_H
