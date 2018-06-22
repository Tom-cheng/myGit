/**************************************************************************************
*  Copyright            chengxiangzhao 
*  Filename:            main_smartHome.h 
*  Description:         main子类实现
*  Author:              chengxiangzhao
*  Create:              2017-11-04
*  Modification history:
*
**************************************************************************************/
#ifndef _MAIN_SMART_HOME_H
#define _MAIN_SMART_HOME_H

#include"type.h"

extern "C"
{

class C_Main_SmartHome
{
public:
    C_Main_SmartHome();
    ~C_Main_SmartHome();
    int  main(void);
    int getDevBootTime(void) {return m_devBootTime;};

//    OSI_INT32 hwAdapterInit(OSI_VOID);
//    OSI_INT32 serviceInit(OSI_VOID);
    int businessInit(void);
    int communicationInit(void);
//    OSI_INT32 logInit(OSI_VOID);
//    OSI_VOID  writeDevStartLog(OSI_VOID);
//    OSI_INT32 devParamInit(OSI_VOID);
//    OSI_INT32 dataLayerInit(OSI_VOID);
//    OSI_INT32 netAppInit(OSI_VOID);
//    OSI_INT32 maintainInit(OSI_VOID);
protected:
private:
    unsigned int getSoftBuildDate(void);
    void signalIgnored(void); 
    time_t m_devBootTime;
};

}


#endif

