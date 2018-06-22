/**************************************************************************************
*  Copyright 2003-2014 Hangzhou Hikvision Digital Technology Co., Ltd. 
*  Filename:              shellServerApi.h
*  Description:           shellServer��API�ӿ�
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

#define RECV_BUF_LEN                1024            //���ջ�����,��shell����Ͳ������
#define SEND_BUF_LEN                (512*1024)      //���ͻ�����,��shell����ִ�к�ķ����ַ������
#define	SHELLCMD_NAME_LEN           32              //shell�������Ƴ���

#define LISTEN_QUENE_LEN            10	            //linux listen���ɴ���ĵȴ�����

/*shell����ڵ����ݽṹ*/
typedef struct
{
	NODE	           node;						/*shell server����Ԫ�ڵ�*/
	char	           cmdName[SHELLCMD_NAME_LEN];	/*shell server����Ԫshell��������*/	
	OSI_INT32		   (*callbackFcn)(OSI_VOID*, const char* recvStr, char* sendStr);/*shell server����Ԫ�ص�����*/
	OSI_VOID           *callbackParam;              /*�ص���������*/
}SHELL_SERVER_UNIT, *SHELL_SERVER_UNIT_PTR;

/*************************************************
* Function:		  ShellServer_Init()
* Description:    ����shell Serverģ��
* Access Level:   public 
* Input:          N/A 
* Output:         N/A
* Return:         OSI_OK/OSI_ERROR
*************************************************/
OSI_INT32         ShellServer_Init(OSI_VOID);

/*************************************************
* Function:	      regShellCmd()
* Description:    ע��shell���� 
*				     (ÿ��ע��һ��shell����Ͷ�̬����һ���ڴ�������shell�����б���)
* Access Level:   public 
* Input:    	  shellCmdName---shell������
*				  callbackFunc---shell����ص�����
* Output:         N/A 
* Return:         OSI_Ok/OSI_ERROR 
*************************************************/
OSI_INT32         regShellCmd(const char* shellCmdName, OSI_INT32(*callbackFunc)(OSI_VOID*, const char *, char *), OSI_VOID* callbackParam);

/*************************************************
* Function:	      isShellCmdValid()
* Description:    �ж��ַ����Ƿ���shell����
* Access Level:   public 
* Input:    	  shellCmdName---shell������
* Output:         N/A 
* Return:         OSI_TRUE/OSI_FALSE
*************************************************/
OSI_BOOL          isShellCmdValid(const char *shellCmdName);

/*************************************************
* Function:	      matchShellUnit()
* Description:    ����shell������Ѱ��ƥ���shell����Ԫ 
* Access Level:   public
* Input:         	
*				  shellCmdName---shell������
*				  pMatchShellUnit---ƥ���shell����Ԫ
* Output:         N/A 
* Return:         OSI_OK/OSI_ERROR 
*************************************************/
OSI_INT32         matchShellUnit(const char* shellCmdName, SHELL_SERVER_UNIT_PTR pMatchShellUnit);


#ifdef  __cplusplus
}
#endif

#endif
