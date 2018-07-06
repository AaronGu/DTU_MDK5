#ifndef _ATCMD_H_
#define _ATCMD_H_


#include "stm32f1xx_hal.h"
#include "usart.h"
#include "string.h"
#include "memalloc.h"
#include "stdarg.h"
#include "ATcmd.h"






#define AT_UART_PORT										&huart3
#define AT_WAIT_CMD_MAX_NUM							10                     /* 最多同时等待应答的AT指令 */
#define AT_RSP_CMD_MAX_NUM							10                     /* 最大的应答命令的缓冲数量 */
#define AT_CB_CMD_MAX_NUM								10                     /* 最大需要回调的AT指令数量 */
#define AT_IGNORE_CMD_MAX_NUM						10                     /* 最多被忽视的AT指令数量 */

#define AT_RSP_TIMEOUT_TIME							10                  /* 缓冲超时时间 */
#define AT_BUF_LEN_MAX									2061                   /* 缓冲最大长度 */
#define AT_DLY													HAL_Delay              /* AT等待时候的会不断回调这个函数 */


typedef void (*at_cmd_cb_t)(void *buf,uint16_t len);


typedef enum {
    AT_DONE = 0,                                               /* 完成 */
    AT_ERROR, 																								 /* 错误的 */
    AT_UNKNOWN,                                                /* 未知 */
    AT_TIMEOUT,                                                /* 超时*/
    AT_WAIT_FILL,                                              /* AT等待缓冲已满 */
    AT_UNFOUND,                                                /* 没有找到相关的AT指令 */
} at_error;

typedef enum {
    AT_HAND_NOT = 0,                                           /* 没有要等待的AT指令 */
    AT_HAND_WAIT = 1,                                          /* 正在等待应答 */
    AT_HAND_DONE = 2,                                          /* 等待的AT指令已经应答 */
    AT_HAND_TIMEOUT = 3,                                       /* 等待的AT指令已经超时*/
} at_sta;

typedef struct {
    uint16_t timeout;                                          /* 每个缓冲级都有个超时 未处理的则清除 */
    char *buf;                                                 /* AT指令缓冲 */
    uint16_t len;                                              /* AT指令长度 */
} at_rsp_t;                                                    /* 应答的AT指令 */

typedef struct {
    uint16_t timeout;                                          /* 超时倒计时 */
    char *cmd;                                                 /* 用来保存正在等待应答的AT指令(最多同时等待10条AT应答) */
    at_sta sta;                                                /* 此AT指令的状态 */
    uint8_t index;                                             /* 如果等待完成 这个结构体将成为出现应答的缓存级索引 */
} at_wait_t;                                                   /* 保存正在等待应答的指令 */

typedef struct {
    char *cmd;                                                 /* 需要被回调的指令 */
    at_cmd_cb_t cb;                                            /* AT指令回调 (例如出现+IPD时回调) */
} at_cb_t;

typedef struct {
    at_rsp_t at_rsp[AT_RSP_CMD_MAX_NUM];                       /* 保存接收到的AT应答 */
    at_wait_t at_wait[AT_WAIT_CMD_MAX_NUM];                    /* 正在等待应答的AT指令 */
    at_cb_t cb[AT_CB_CMD_MAX_NUM];                             /* AT指令回调 */
    char *ignore[AT_IGNORE_CMD_MAX_NUM];                       /* 被忽视的AT指令  (收到被忽视的指令后立即清除) */
} at_t;


void at_init(void);                																				/* at初始化 */
//void at_init(uint32_t baud);     																				/* at初始化 波特率设置 */
void at_set_baud(uint32_t baud);   																				/* at波特率设置 */
void at_processing(void);          																				/* at处理 */
void at_cmd_cb_hand(void);         																				/* at指令回调处理 */
void at_time_task(void);           																				/* at指令超时计时 */

void at_free_buf(uint8_t index);                                          /* 释放响应缓冲级内存 */
char *at_buf_get(uint8_t index);                                          /* 查询响应缓冲级首地址*/
uint16_t at_buf_len_get(uint8_t index);                                   /* 查询缓存级的长度 */
at_error at_search_cmd(char *keyword,uint8_t *index);                     /* 搜索AT缓冲里的数据 */

void at_clear_all(void);                                                  /* 清除所有的AT指令响应数据 */
void at_send_cmd(char *fmt,...);                                          /* at指令格式化发送 */
void at_send_data(uint8_t *buf,uint16_t len);                             /* at数据发送 */
at_error at_wait_cmd (char *s,uint16_t timeout,uint8_t *index);           /* 一个at指令等待 */
at_error at_wait2_cmd(char *s,char *s2,uint16_t timeout,uint8_t *index);  /* 两个at指令等待 */

at_error at_cmd_cb_register(char *cmd,at_cmd_cb_t cb);                    /* 注册at指令响应回调 */
at_error at_cmd_cb_cancel(at_cmd_cb_t cb);                                /* 注销at指令响应回调 */
at_error at_cmd_ignore_register(char *cmd);                               /* 注册at忽略指令 */
at_error at_cmd_ignore_cancel(char *cmd);                                 /* 注销at忽略指令 */

extern void at_receive_handle(uint8_t *buf,uint16_t len);                 /* at串口响应数据缓冲级接收处理 */


#endif // _ATCMD_H_
