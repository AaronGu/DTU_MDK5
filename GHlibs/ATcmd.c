//
// Created by AG on 2018/6/18.
//
#include "ATcmd.h"



//#include "hal_usart.h"
//#include "delay.h"

//ATָ���

at_t at;                                                         /* ����at�ṹ�� */


/* ��ʼ��
**
*/
void at_init(void)
{
    memset(&at,0,sizeof(at));                                     /* ��ʼ���ṹ��at */
}

//void at_init(uint32_t baud)
//{
//		HAL_UART_Init(&huart3, 115200);
//    HalUSARTInit(AT_UART_PORT,baud,at_receive_handle);
//
//}


/* ���ò�����
** ���³�ʼ������
*/
void at_set_baud(uint32_t baud)
{
    //HalUSARTInit(AT_UART_PORT,baud,at_receive_handle);
}


/* ������е�ATָ��Ӧ��
**
*/
void at_clear_all(void)
{
    for(uint8_t i=0; i<AT_RSP_CMD_MAX_NUM; i++)                    /* ����ATָ����Ӧ���弶 */
        if(at.at_rsp[i].len)                                       /* ���ATָ����Ӧ���弶�������� */
        {
            Free(at.at_rsp[i].buf);                                /* �ͷ��ڴ� */
            memset(&at.at_rsp[i],0,sizeof(at_rsp_t));              /* ��ʼ�����弶�ṹ�� */
        }
}


/*** ATָ��� ***/
void at_processing(void)
{
    uint8_t i,index;                                               /* �ؼ��ֵĻ��漶�׵�ַ */

    for(i=0; i<AT_RSP_CMD_MAX_NUM; i++)                            /* ɾ����ʱ��Ӧ��ָ�� */
        if(at.at_rsp[i].timeout == RESET && at.at_rsp[i].len)      /* ����û��弶�������Ѿ���ʱ */
        {
            Free(at.at_rsp[i].buf);                                /* �ͷ��ڴ� */
            memset(&at.at_rsp[i], 0, sizeof(at_rsp_t));
//					  printf("Free:at.at_rsp[%d],size:[%d]\r\n", i, sizeof(at_rsp_t));
        }

    for(i=0; i<AT_WAIT_CMD_MAX_NUM; i++)                           /* �������еȴ���ATӦ�� */
        if(at.at_wait[i].sta == AT_HAND_WAIT)                      /* ����˻�������ATָ���ڵȴ�Ӧ�� */
        {
            if(at_search_cmd(at.at_wait[i].cmd, &index) == AT_DONE) /* ���AT��������ֵȴ���ATָ��Ӧ�� */
            {
                at.at_wait[i].index = index;                       /* ���Ƴ��ֹؼ��ֵĻ�������� */
                at.at_wait[i].sta = AT_HAND_DONE;                  /* ��־�ȴ���ָ���Ѿ�Ӧ�� �õȴ��Ľ���ȥ�������Ӧ��ָ�� */
//							printf("\r\nat.at_wait[%d].[%d]\r\n", i, index);
            }
            else if(at.at_wait[i].timeout == RESET)                /* ��εļ��û���ҵ��ȴ���Ӧ�� ��������ȴ���ָ���Ѿ���ʱ */
                at.at_wait[i].sta = AT_HAND_TIMEOUT;               /* �������ָ���Ѿ��ȴ���ʱ */
//						printf("\r\nat.at_wait[%d] timeout!\r\n", i);
        }
}


/*** ��Ҫ�ص���ATָ��� ***/
void at_cmd_cb_hand(void)
{
    uint8_t i,index;
AT_CB:
    for(i=0; i<AT_CB_CMD_MAX_NUM; i++)                                /* �����û�г�����Ҫ�ص���ATָ�� */
        if(at.cb[i].cb != RESET)                                      /* �������ص����� */
        {
            if(at_search_cmd(at.cb[i].cmd,&index) == AT_DONE)         /* ���AT�����������Ҫ�ص���ATָ�� */
            {
                at.cb[i].cb(at_buf_get(index),at_buf_len_get(index)); /* �ص� */
                at_free_buf(index);                                   /* ɾ������ATָ�� */
            }
        }


    for(i=0; i<AT_IGNORE_CMD_MAX_NUM; i++)                            /* �������Ե�ATָ���Ӧ�� */
        if(at.ignore[i] != NULL)                                      /* ����˻����е�ַ����Ҫ���� */
        {
            if(at_search_cmd(at.ignore[i],&index) == AT_DONE)         /* ���ATӦ�𻺳弶���б����Ե�ATָ����� */
            {
                for(i=0; i<AT_CB_CMD_MAX_NUM; i++)                    /* ɾ����ָ��֮ǰ�ȼ����Ҫɾ��֮�������û�г�����Ҫ�ص���ATָ�� */
                    if(at.cb[i].cb != RESET)                          /* �������ص����� */
                    {
                        if(strstr(at_buf_get(index),at.cb[i].cmd) != NULL)/* �����������������Ҫ�ص���ָ�� */
                            goto AT_CB;                               /* ��ȥ����ص� */
                    }
                at_free_buf(index);                                   /* ɾ������ATָ�� */
            }
        }
}


/*** ����ATָ�� ***/
void at_send_cmd(char *fmt,...)
{
    va_list ap;
    char *string;                                                            /* �����ַ�ָ�� */
    string = Malloc(512);                                                    /* �ڴ洫ֵ */
    if(string == NULL)                                                       /* ���û���ڴ�ռ� */
        return ;
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    //HalUSARTWriteData(AT_UART_PORT,(uint8_t *)string,strlen(string));
    HAL_UART_Transmit(AT_UART_PORT,(uint8_t *)string,strlen(string), 100);   /* ���ڷ������� */
    va_end(ap);
    Free(string);                                                            /* �ͷ��ڴ� */
}


/*** �������� ***/
void at_send_data(uint8_t *buf,uint16_t len)
{
    //HalUSARTWriteData(AT_UART_PORT,buf,len);
    HAL_UART_Transmit(AT_UART_PORT, buf, len, 100);                            /* ���ڷ������� */
}


/* ע��һ�������ӵ�ATָ��
** �յ���ע�����ָ���ֱ������������弶�ռ�
*/
at_error at_cmd_ignore_register(char *cmd)
{
    for(uint8_t i=0; i<AT_IGNORE_CMD_MAX_NUM; i++)
    {
        if(at.ignore[i] == NULL)                                             /* ����˻���û��ע�� */
        {
            at.ignore[i] = Malloc(strlen(cmd)+1);
            if(at.ignore[i] == NULL)return AT_ERROR;
            memset(at.ignore[i],0,strlen(cmd)+1);
            memcpy(at.ignore[i],cmd,strlen(cmd));
            return AT_DONE;
        }
    }
    return AT_UNFOUND;                                                       /* û���ҵ��յĻ��弶 */
}


/* ע����Ҫ�����Ե�ATָ��
**
*/
at_error at_cmd_ignore_cancel(char *cmd)
{
    for(uint8_t i=0; i<AT_IGNORE_CMD_MAX_NUM; i++)
    {
        if(at.ignore[i] != NULL)
        {
            if(memcmp(at.ignore[i],cmd,strlen(cmd)) == 0)                    /* �����������ATָ�� */
            {
                Free(at.ignore[i]);
                at.ignore[i] = NULL;
                return AT_DONE;
            }
        }
    }
    return AT_UNFOUND;
}


/* ATָ��ص�ע��
** cmd ��Ҫ���ص���ATָ�
** cb ��Ҫ�ص��ĺ�����ַ
*/
at_error at_cmd_cb_register(char *cmd,at_cmd_cb_t cb)
{
    uint8_t i;
    for(i=0; i<AT_CB_CMD_MAX_NUM; i++)                                    /* Ѱ��û�лص��Ļ��漶 */
    {
        if(at.cb[i].cb == NULL)                                           /* ���������漶û�лص� */
        {
            at.cb[i].cmd = Malloc(strlen(cmd)+1);                         /* Ϊ�˻��漶ָ���ַ�����ڴ� */
            if(at.cb[i].cmd == NULL)return AT_WAIT_FILL;                  /* ���û���ڴ�ռ� */
            memset(at.cb[i].cmd,0,strlen(cmd)+1);                         /* ����ڴ� */
            memcpy(at.cb[i].cmd,cmd,strlen(cmd));                         /* ������Ҫ�ص���ATָ�� */
            at.cb[i].cb = cb;                                             /* ���ƻص�������ַ */
            return AT_DONE;
        }
    }
    return AT_UNFOUND;                                                    /* û���ҵ��յĻ��漶 */
}


/* ATָ��ص�ע��
**
*/
at_error at_cmd_cb_cancel(at_cmd_cb_t cb)
{
    uint8_t i;
    for(i=0; i<AT_CB_CMD_MAX_NUM; i++)                                    /* Ѱ���Ѿ���ע��Ļص� */
    {
        if(at.cb[i].cb == cb)                                             /* ���������漶�Ļص�һ�� */
        {
            Free(at.cb[i].cmd);                                           /* �ջر���ָ����ڴ� */
            memset(&at.cb[i],0,sizeof(at_cb_t));                          /* ���������漶 */
            return AT_DONE;
        }
    }
    return AT_ERROR;
}


/* �ȴ�����ָ��
** �����������һ�������˳�������AT�����index  ���at_buf_get������ѯ�����׵�ַ
*/
at_error at_wait2_cmd(char *s,char *s2,uint16_t timeout,uint8_t *index)
{
    uint8_t one = 0xFF,two = 0xFF;                                           /* ��ȡ�����ȴ����弶 */
    at_error error;
    for(uint8_t i=0; i<AT_WAIT_CMD_MAX_NUM; i++)                             /* Ѱ��û���ڵȴ�Ӧ��Ļ��弶 */
    {
        if(at.at_wait[i].sta == AT_HAND_NOT)                                 /* ����˼�����û���ڵȴ�Ӧ�� */
        {
            if(one == 0xFF)
            {
                at.at_wait[i].cmd = Malloc(strlen(s)+1);                     /* �����ڴ� */
                if(at.at_wait[i].cmd == NULL)                                /* ���û�ڴ� */
                    return AT_WAIT_FILL;
                memset(at.at_wait[i].cmd,0,strlen(s)+1);                     /* ����ڴ� */
                memcpy(at.at_wait[i].cmd,s,strlen(s));                       /* ������Ҫ�ȴ���ATӦ�� ��esp taskȥ���� */
                at.at_wait[i].timeout = timeout;                             /* ��������ָ��ĵȴ���ʱ */
                at.at_wait[i].sta = AT_HAND_WAIT;                            /* ��־�ȴ�״̬ */
                one = i;
                break;
            }
        }
    }
    for(uint8_t i=0; i<AT_WAIT_CMD_MAX_NUM; i++)                             /* Ѱ��û���ڵȴ�Ӧ��Ļ��弶 */
    {
        if(at.at_wait[i].sta == AT_HAND_NOT)                                 /* ����˼�����û���ڵȴ�Ӧ�� */
        {
            if(two == 0xFF)
            {
                at.at_wait[i].cmd = Malloc(strlen(s)+1);                     /* �����ڴ� */
                if(at.at_wait[i].cmd == NULL)                                /* ���û�ڴ� */
                    return AT_WAIT_FILL;
                memset(at.at_wait[i].cmd,0,strlen(s)+1);                     /* ����ڴ� */
                memcpy(at.at_wait[i].cmd,s,strlen(s));                       /* ������Ҫ�ȴ���ATӦ�� ��esp taskȥ���� */
                at.at_wait[i].timeout = timeout;                             /* ��������ָ��ĵȴ���ʱ */
                at.at_wait[i].sta = AT_HAND_WAIT;                            /* ��־�ȴ�״̬ */
                two = i;
                break;
            }
        }
    }
    if(one != 0xFF && two != 0xFF)                                           /* ����������ȴ����弶 */
    {
        while(at.at_wait[one].sta == AT_HAND_WAIT && at.at_wait[two].sta == AT_HAND_WAIT)
            AT_DLY(1);                                                         /* �ȴ�ATӦ����߳�ʱ�˳� */
        if(at.at_wait[one].sta != AT_HAND_WAIT)                              /* ���one�ȴ��Ѿ����� */
        {
            if(at.at_wait[one].sta == AT_HAND_DONE)                          /* ���ATָ���Ѿ�Ӧ�� */
            {
                if(index == NULL)                                            /* �������Ҫ���� */
                    at_free_buf(at.at_wait[one].index);                      /* ɾ��������弶������ */
                else
                    *index = at.at_wait[one].index;                          /* ���ݳ���ATӦ��Ļ��弶���� */
                error = AT_DONE;
            }
            else if(at.at_wait[one].sta == AT_HAND_TIMEOUT)                  /* ���ATָ��ȴ���ʱ */
                error = AT_TIMEOUT;
        }
        else if(at.at_wait[two].sta != AT_HAND_WAIT)                         /* ���two�Ѿ����� */
        {
            if(at.at_wait[two].sta == AT_HAND_DONE)                          /* ���ATָ���Ѿ�Ӧ�� */
            {
                if(index == NULL)                                            /* �������Ҫ���� */
                    at_free_buf(at.at_wait[two].index);                      /* ɾ��������弶������ */
                else
                    *index = at.at_wait[two].index;                          /* ���ݳ���ATӦ��Ļ��弶���� */
                error = AT_DONE;
            }
            else if(at.at_wait[two].sta == AT_HAND_TIMEOUT)                  /* ���ATָ��ȴ���ʱ */
                error = AT_TIMEOUT;
        }
        Free(at.at_wait[one].cmd);                                           /* �ͷ��ڴ� */
        memset(&at.at_wait[one],0,sizeof(at_wait_t));                        /* �������ȴ����� */
        Free(at.at_wait[two].cmd);                                           /* �ͷ��ڴ� */
        memset(&at.at_wait[two],0,sizeof(at_wait_t));                        /* �������ȴ����� */
        return error;
    }
    else                                                                     /* û���������弶 */
    {   /* �����һ���������ڴ���Ҫ�ͷ� */
        if(one != 0xff)
        {
            Free(at.at_wait[one].cmd);                                       /* �ͷ��ڴ� */
            memset(&at.at_wait[one],0,sizeof(at_wait_t));                    /* �������ȴ����� */
        }
        if(two != 0xFF)
        {
            Free(at.at_wait[two].cmd);                                       /* �ͷ��ڴ� */
            memset(&at.at_wait[two],0,sizeof(at_wait_t));                    /* �������ȴ����� */
        }
    }
    return AT_WAIT_FILL;                                                     /* û��ʣ�µĵȴ����弶 */
}


/* �ȴ�ATָ��Ӧ��
** ����еȵ�Ӧ�� index����Ӧ���ATָ��弶������ ���index��NULL������ɾ��AT���弶���Ӧ��ָ��
*/
at_error at_wait_cmd(char *s,uint16_t timeout,uint8_t *index)
{
    uint8_t i;
    at_error error;
    for(i=0; i<AT_WAIT_CMD_MAX_NUM; i++)                                    /* Ѱ��û���ڵȴ�Ӧ��Ļ��弶 */
    {
        if(at.at_wait[i].sta == AT_HAND_NOT)                                /* ����˼�����û���ڵȴ�Ӧ�� */
        {
            at.at_wait[i].cmd = Malloc(strlen(s)+1);                        /* �����ڴ� */
            if(at.at_wait[i].cmd == NULL)                                   /* ���û�ڴ� */
                return AT_WAIT_FILL;
            memset(at.at_wait[i].cmd,0,strlen(s)+1);                        /* ����ڴ� */
            memcpy(at.at_wait[i].cmd,s,strlen(s));                          /* ������Ҫ�ȴ���ATӦ�� ��esp taskȥ���� */
            at.at_wait[i].timeout = timeout;                                /* ����ָ��ĵȴ���ʱ */
            at.at_wait[i].sta = AT_HAND_WAIT;                               /* ��־�ȴ�״̬ */
            while(at.at_wait[i].sta == AT_HAND_WAIT)AT_DLY(1);              /* �ȴ�ATӦ����߳�ʱ�˳� */
            if(at.at_wait[i].sta == AT_HAND_DONE)                           /* ���ATָ���Ѿ�Ӧ�� */
            {
                if(index == NULL)                                           /* �������Ҫ���� */
                    at_free_buf(at.at_wait[i].index);                       /* ɾ��������弶������ */
                else
                    *index = at.at_wait[i].index;                           /* ���ݳ���ATӦ��Ļ��弶���� */
                error = AT_DONE;
            }
            else if(at.at_wait[i].sta == AT_HAND_TIMEOUT)                   /* ���ATָ��ȴ���ʱ */
                error = AT_TIMEOUT;
            Free(at.at_wait[i].cmd);                                        /* �ͷ��ڴ� */
            memset(&at.at_wait[i],0,sizeof(at_wait_t));                     /* �������ȴ����� */
            return error;
        }
    }
    return AT_WAIT_FILL;                                                    /* û��ʣ�µĵȴ����弶 */
}


/*����AT�����������
* ����keyword��д����Ҫ�����Ĺؼ���
* ����index ����ҵ��ؼ����򷵻ش˹ؼ��ʳ��ֵ�ATָ��Ӧ��Ļ��弶����
*/
at_error at_search_cmd(char *keyword,uint8_t *index)
{
    uint8_t i;
    char *k_addr;
    for(i=0; i<AT_RSP_CMD_MAX_NUM; i++)                    /* ��ÿ���Ļ���ȥѰ�Ҷ�Ӧ��ATָ�� */
    {
        if(at.at_rsp[i].len != RESET)                      /* ���˼������Ƿ������� */
        {
            k_addr = strstr(at.at_rsp[i].buf,keyword);     /* ���Ҵ˼������Ƿ��еȴ�Ӧ���ATָ�� */
            if(k_addr != NULL)                             /* ����˼����������ڵȴ�ATӦ�� */
            {
                *index = i;                                /* �����ڵȴ�Ӧ���ATָ��Ļ��漶���� */
                return AT_DONE;
            }
        }
    }
    return AT_UNFOUND;//û����Ѱ�����ٵȴ���ATӦ��
}


//ESP��ʱ���� 1ms
void at_time_task(void)
{
    uint8_t i;                                              /* ���� */
    for(i=0; i<AT_WAIT_CMD_MAX_NUM; i++)                    /* ����ATָ��ȴ���Ӧ���弶 */
    {
        if(at.at_wait[i].timeout)                           /* �����ʱʱ��δ��0 */
				{
            at.at_wait[i].timeout--;                        /* ��ʱʱ���1 */
//					 printf("at.at_wait[%d].timeout:[%d]\r\n", i, at.at_wait[i].timeout);
				}
    }
    for(i=0; i<AT_RSP_CMD_MAX_NUM; i++)                     /* ����AT������Ӧ���弶 */
    {
        if(at.at_rsp[i].timeout)                            /* �����ʱʱ��δ��0 */
				{
            at.at_rsp[i].timeout--;                         /* ��ʱʱ���1 */
//					printf("at.at_rsp[%d].timeout:[%d]\r\n", i, at.at_rsp[i].timeout);
				}
    }
}


/*** �ͷ�AT���漶 ***/
void at_free_buf (uint8_t index)
{
    Free(at.at_rsp[index].buf);                              /* �ͷ�indexָ���Ļ��漶�ռ� */
    memset(&at.at_rsp[index],0,sizeof(at_rsp_t));            /* �ͷ�indexָ���Ļ��漶�ṹ�� */
}


/*** ��ѯ��Ӧ���弶�׵�ַ ***/
char *at_buf_get(uint8_t index)
{
    return at.at_rsp[index].buf;                             /* ����indexָ���Ļ��漶�׵�ַ */
}


/*** ��ѯ���漶�ĳ��� ***/
uint16_t at_buf_len_get(uint8_t index)
{
    return at.at_rsp[index].len;                             /* ����indexָ�����漶���ݳ��� */
}


/*** ATָ����մ����� ***/
void at_receive_handle(uint8_t *buf,uint16_t len)
{
    uint8_t i;                                               /* ���� */
    if(len < AT_BUF_LEN_MAX)                                 /* �����������Ƿ񳬳����弶�������� */
    {
        for(i=0; i<AT_RSP_CMD_MAX_NUM; i++)                  /* �������弶 */
        {
            if(at.at_rsp[i].len == RESET) 								   /* ���������弶û�б�ʹ�� */
            {
                at.at_rsp[i].buf = Malloc(len+1);            /* �����ڴ�ռ� */
                if(at.at_rsp[i].buf == NULL)                 /* ���û�ڴ� */
                {
                    printf("NO SPACE\r\n");
                    return ;
                }
                memset(at.at_rsp[i].buf,0,len+1);            /* ���ڴ�ռ䣬��ʼ�����ڴ�ռ� */
                memcpy(at.at_rsp[i].buf,buf,len);            /* �������ݵ��ڴ�ռ��� */
                at.at_rsp[i].len = len;                      /* �������ݳ��ȵ��ṹ���� */
                at.at_rsp[i].timeout = AT_RSP_TIMEOUT_TIME;  /* �������ݻ��峬ʱʱ�䵽�ṹ���� */
								HAL_UART_Transmit(&huart1, buf, len, 100);
               // printf("\r\nat_receive_handle\r\n");
                return ;
            }
        }
        printf("AT rece full\r\n");
        return ;
    }
    else {
        printf("data len out of AT_BUF_LEN_MAX");
        return ;
    }
}

