#ifndef _ATCMD_H_
#define _ATCMD_H_


#include "stm32f1xx_hal.h"
#include "usart.h"
#include "string.h"
#include "memalloc.h"
#include "stdarg.h"
#include "ATcmd.h"






#define AT_UART_PORT										&huart3
#define AT_WAIT_CMD_MAX_NUM							10                     /* ���ͬʱ�ȴ�Ӧ���ATָ�� */
#define AT_RSP_CMD_MAX_NUM							10                     /* ����Ӧ������Ļ������� */
#define AT_CB_CMD_MAX_NUM								10                     /* �����Ҫ�ص���ATָ������ */
#define AT_IGNORE_CMD_MAX_NUM						10                     /* ��౻���ӵ�ATָ������ */

#define AT_RSP_TIMEOUT_TIME							10                  /* ���峬ʱʱ�� */
#define AT_BUF_LEN_MAX									2061                   /* ������󳤶� */
#define AT_DLY													HAL_Delay              /* AT�ȴ�ʱ��Ļ᲻�ϻص�������� */


typedef void (*at_cmd_cb_t)(void *buf,uint16_t len);


typedef enum {
    AT_DONE = 0,                                               /* ��� */
    AT_ERROR, 																								 /* ����� */
    AT_UNKNOWN,                                                /* δ֪ */
    AT_TIMEOUT,                                                /* ��ʱ*/
    AT_WAIT_FILL,                                              /* AT�ȴ��������� */
    AT_UNFOUND,                                                /* û���ҵ���ص�ATָ�� */
} at_error;

typedef enum {
    AT_HAND_NOT = 0,                                           /* û��Ҫ�ȴ���ATָ�� */
    AT_HAND_WAIT = 1,                                          /* ���ڵȴ�Ӧ�� */
    AT_HAND_DONE = 2,                                          /* �ȴ���ATָ���Ѿ�Ӧ�� */
    AT_HAND_TIMEOUT = 3,                                       /* �ȴ���ATָ���Ѿ���ʱ*/
} at_sta;

typedef struct {
    uint16_t timeout;                                          /* ÿ�����弶���и���ʱ δ���������� */
    char *buf;                                                 /* ATָ��� */
    uint16_t len;                                              /* ATָ��� */
} at_rsp_t;                                                    /* Ӧ���ATָ�� */

typedef struct {
    uint16_t timeout;                                          /* ��ʱ����ʱ */
    char *cmd;                                                 /* �����������ڵȴ�Ӧ���ATָ��(���ͬʱ�ȴ�10��ATӦ��) */
    at_sta sta;                                                /* ��ATָ���״̬ */
    uint8_t index;                                             /* ����ȴ���� ����ṹ�彫��Ϊ����Ӧ��Ļ��漶���� */
} at_wait_t;                                                   /* �������ڵȴ�Ӧ���ָ�� */

typedef struct {
    char *cmd;                                                 /* ��Ҫ���ص���ָ�� */
    at_cmd_cb_t cb;                                            /* ATָ��ص� (�������+IPDʱ�ص�) */
} at_cb_t;

typedef struct {
    at_rsp_t at_rsp[AT_RSP_CMD_MAX_NUM];                       /* ������յ���ATӦ�� */
    at_wait_t at_wait[AT_WAIT_CMD_MAX_NUM];                    /* ���ڵȴ�Ӧ���ATָ�� */
    at_cb_t cb[AT_CB_CMD_MAX_NUM];                             /* ATָ��ص� */
    char *ignore[AT_IGNORE_CMD_MAX_NUM];                       /* �����ӵ�ATָ��  (�յ������ӵ�ָ����������) */
} at_t;


void at_init(void);                																				/* at��ʼ�� */
//void at_init(uint32_t baud);     																				/* at��ʼ�� ���������� */
void at_set_baud(uint32_t baud);   																				/* at���������� */
void at_processing(void);          																				/* at���� */
void at_cmd_cb_hand(void);         																				/* atָ��ص����� */
void at_time_task(void);           																				/* atָ�ʱ��ʱ */

void at_free_buf(uint8_t index);                                          /* �ͷ���Ӧ���弶�ڴ� */
char *at_buf_get(uint8_t index);                                          /* ��ѯ��Ӧ���弶�׵�ַ*/
uint16_t at_buf_len_get(uint8_t index);                                   /* ��ѯ���漶�ĳ��� */
at_error at_search_cmd(char *keyword,uint8_t *index);                     /* ����AT����������� */

void at_clear_all(void);                                                  /* ������е�ATָ����Ӧ���� */
void at_send_cmd(char *fmt,...);                                          /* atָ���ʽ������ */
void at_send_data(uint8_t *buf,uint16_t len);                             /* at���ݷ��� */
at_error at_wait_cmd (char *s,uint16_t timeout,uint8_t *index);           /* һ��atָ��ȴ� */
at_error at_wait2_cmd(char *s,char *s2,uint16_t timeout,uint8_t *index);  /* ����atָ��ȴ� */

at_error at_cmd_cb_register(char *cmd,at_cmd_cb_t cb);                    /* ע��atָ����Ӧ�ص� */
at_error at_cmd_cb_cancel(at_cmd_cb_t cb);                                /* ע��atָ����Ӧ�ص� */
at_error at_cmd_ignore_register(char *cmd);                               /* ע��at����ָ�� */
at_error at_cmd_ignore_cancel(char *cmd);                                 /* ע��at����ָ�� */

extern void at_receive_handle(uint8_t *buf,uint16_t len);                 /* at������Ӧ���ݻ��弶���մ��� */


#endif // _ATCMD_H_
