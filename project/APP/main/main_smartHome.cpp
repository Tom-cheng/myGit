/**************************************************************************************
*  Copyright            chengxiangzhao 
*  Filename:            main_smartHome.cpp
*  Description:         main子类实现
*  Author:              chengxiangzhao
*  Create:              2017-11-04
*  Modification history:
*
**************************************************************************************/

#include "main_smartHome.h"
#include "netServer.h"

C_Main_SmartHome::C_Main_SmartHome()
{
    m_devBootTime = time(NULL);
    struct tm* tmBlock;
    tmBlock = localtime(&m_devBootTime);
    printf("Current time is %s", asctime(tmBlock));

    getSoftBuildDate();
}

C_Main_SmartHome::~C_Main_SmartHome()
{
    m_devBootTime = 0;
}

int C_Main_SmartHome::main()
{
    if(OSI_OK != communicationInit())
    {
        fprintf(stderr, "[%s:%d]The communicationInit() is failed.\n", __FUNCTION__, __LINE__);
        return OSI_ERROR;
    }

    return OSI_OK;
}

void C_Main_SmartHome::signalIgnored()
{
    return ;
}

int C_Main_SmartHome::businessInit()
{
    return OSI_OK;
}
int C_Main_SmartHome::communicationInit()
{
    C_NetServer *netServerObjPtr = NULL;

    netServerObjPtr = new C_NetServer();
    assert(netServerObjPtr != NULL);

    if(OSI_OK != netServerObjPtr->netServerInit())
    {
        fprintf(stderr, "[%s:%d]The communicationInit is failed.\n", __FUNCTION__, __LINE__);
        return OSI_ERROR;
    }

    return OSI_OK;
}

unsigned int C_Main_SmartHome::getSoftBuildDate(void)
{
    int   y, m, d;
    char    ms[4];
    const char  *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    sscanf(__DATE__, "%s%d%d", ms, &d, &y);

    for (m = 0; m < 12; m++)
    {
        if(strcmp(ms, months[m]) == 0)
        {
            break;
        }
    }

    m++;
    y -= 2000;

    printf("y=%d,m=%d,d=%d\n", y, m, d);

    return OSI_OK;
}
