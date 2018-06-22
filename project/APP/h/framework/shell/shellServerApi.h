/**************************************************************************************
*  Copyright 2003-2014 Hangzhou Hikvision Digital Technology Co., Ltd. 
*  Filename:              shellServerApi.h
*  Description:           shellServer库API接口
*  Author:                chenwangxian
*  Create:                2014-07-24 
*  Modification history:
*
*
**************************************************************************************/ 
#ifndef _SHELLSERVER_API_H
#define _SHELLSERVER_API_H

#include "libOSI.h"
#include "libDSI.h"

#ifdef  __cplusplus
extern "C"
{
#endif

#define RECV_BUF_LEN                1024            //接收缓冲区,由shell命令和参数组成
#define SEND_BUF_LEN                (512*1024)      //发送缓冲区,由shell命令执行后的返回字符串组成
#define	SHELLCMD_NAME_LEN           32              //shell命令名称长度

#define LISTEN_QUENE_LEN            10	            //linux listen最大可处理的等待个数

/*shell管理节点数据结构*/
typedef struct
{
	NODE	           node;						/*shell server管理单元节点*/
	char	           cmdName[SHELLCMD_NAME_LEN];	/*shell server管理单元shell命令名字*/	
	OSI_INT32		   (*callbackFcn)(OSI_VOID*, const char* recvStr, char* sendStr);/*shell server管理单元回调函数*/
	OSI_VOID           *callbackParam;              /*回调函数参数*/
}SHELL_SERVER_UNIT, *SHELL_SERVER_UNIT_PTR;

/*************************************************
* Function:		  ShellServer_Init()
* Description:    启动shell Server模块
* Access Level:   public 
* Input:          N/A 
* Output:         N/A
* Return:         OSI_OK/OSI_ERROR
*************************************************/
OSI_INT32         ShellServer_Init(OSI_VOID);

/*************************************************
* Function:	      regShellCmd()
* Description:    注册shell命令 
*				     (每次注册一个shell命令，就动态分配一块内存连接在shell管理列表中)
* Access Level:   public 
* Input:    	  shellCmdName---shell命令名
*				  callbackFunc---shell命令回调函数
* Output:         N/A 
* Return:         OSI_Ok/OSI_ERROR 
*************************************************/
OSI_INT32         regShellCmd(const char* shellCmdName, OSI_INT32(*callbackFunc)(OSI_VOID*, const char *, char *), OSI_VOID* callbackParam);

/*************************************************
* Function:	      isShellCmdValid()
* Description:    判断字符串是否是shell命令
* Access Level:   public 
* Input:    	  shellCmdName---shell命令名
* Output:         N/A 
* Return:         OSI_TRUE/OSI_FALSE
*************************************************/
OSI_BOOL          isShellCmdValid(const char *shellCmdName);

/*************************************************
* Function:	      matchShellUnit()
* Description:    根据shell命令名寻找匹配的shell管理单元 
* Access Level:   public
* Input:         	
*				  shellCmdName---shell命令名
*				  pMatchShellUnit---匹配的shell管理单元
* Output:         N/A 
* Return:         OSI_OK/OSI_ERROR 
*************************************************/
OSI_INT32         matchShellUnit(const char* shellCmdName, SHELL_SERVER_UNIT_PTR pMatchShellUnit);


#ifdef  __cplusplus
}
#endif

#endif
