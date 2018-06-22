/**************************************************************************************
*  Copyright            chengxiangzhao 
*  Filename:            netServer.h
*  Description:         网络通层的头文件
*  Author:              chengxiangzhao
*  Create:              2017-11-04
*  Modification history:
*
**************************************************************************************/
#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "libOSI.h"

#define LOCAL_IP_ADDRESS   "192.168.0.123"
#define NETRET_NOT_SUPPORT  -1
#define NETCMD_GET_INFO    100

#define DEFAULT_PORT             8000
#define MAX_POOL                 1024
#define MAX_BOARD_SDK_BUFFSIZE   4096

typedef struct _NETCMD_HEADER
{
    int  length;
    int  netCmd;
    char buf[1024];
}NETCMD_HEADER,*NETCMD_HEADER_PTR;


typedef struct _THREAD_STRUCT
{
    int confd;
    int index;
}THREAD_STRUCT,*THREAD_STRUCT_PTR;

extern "C"
{

class C_NetServer
{
public:
     C_NetServer();
    ~C_NetServer();
    int  netServerInit();
    void showText();
private:
    static void *netServerTaskWorker(void *pThis);
    void netServerTask();
    static void *processClientRequest(void *data);
    int       m_index;
    pthread_t m_threadID;
    pthread_t m_pool[MAX_POOL];
};

}

#endif