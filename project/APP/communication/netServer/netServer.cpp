/**************************************************************************************
*  Copyright            chengxiangzhao 
*  Filename:            netServer.pp
*  Description:         缃戠粶閫氬眰鐨勫疄鐜�
*  Author:              chengxiangzhao
*  Create:              2017-11-04
*  Modification history:
*
**************************************************************************************/
#include"netServer.h"
#include<iostream>

C_NetServer::C_NetServer()
{
    m_threadID = 0;
    m_index    = 0;
    memset(m_pool, 0, sizeof(m_pool));
}
C_NetServer::~C_NetServer()
{
    if(m_threadID != 0)
    {
        pthread_join(m_threadID,NULL);
    }
    
}
void C_NetServer::showText()
{
    std::cout << "This is a test"<< std::endl;
    return;
}
void C_NetServer::netServerTask()
{
    fd_set rset;
    struct timeval timeout;
    int socket_fd, connfd;
    int retVal = OSI_ERROR, opt = 1, ret = 0;
    struct sockaddr_in  servaddr, clientAddr;
    THREAD_STRUCT tmpData;
    pthread_t tmpID = 0;
reStartBNetServer:

    //鍒濆鍖朣ocket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

    //鍒濆鍖�
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP鍦板潃璁剧疆鎴怚NADDR_ANY,璁╃郴缁熻嚜鍔ㄨ幏鍙栨湰鏈虹殑IP鍦板潃銆�
    servaddr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
    servaddr.sin_port = htons(DEFAULT_PORT);//璁剧疆鐨勭鍙ｄ负DEFAULT_PORT

    //灏嗘湰鍦板湴鍧�缁戝畾鍒版墍鍒涘缓鐨勫鎺ュ瓧涓�
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }


    //寮�濮嬬洃鍚槸鍚︽湁瀹㈡埛绔繛鎺�
    if( listen(socket_fd, 1024) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }

    printf("======waiting for client's request======\n");

    for(;;)
    {
        FD_ZERO(&rset);//娓呯┖fdset涓庢墍鏈夋枃浠跺彞鏌勭殑鑱旂郴銆�
        FD_SET(socket_fd, &rset);//寤虹珛鏂囦欢鍙ユ焺fd涓巉dset鐨勮仈绯汇��
        //FD_CLR(int fd, fd_set *fdset)//锛氭竻闄ゆ枃浠跺彞鏌刦d涓巉dset鐨勮仈绯汇��
        //FD_ISSET(socket_fd, &rset);//妫�鏌dset鑱旂郴鐨勬枃浠跺彞鏌刦d鏄惁鍙鍐欙紝>0琛ㄧず鍙鍐欍��
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        retVal = select(socket_fd + 1, &rset, NULL, NULL, &timeout);

        if(retVal > 0)
        {
            printf("The netServerTask accept() succeed .\n");

            if(FD_ISSET(socket_fd, &rset))
            {
                memset(&clientAddr, 0, sizeof(clientAddr));
                if((connfd = accept(socket_fd, NULL, NULL)) < 0)
                {
                    if(errno == EMFILE)
                    {
                        printf("[%s:%d] accept() failed .\n", __FUNCTION__,__LINE__);
                        sleep(1);
                    }
                    continue;
                }
                tmpData.index = m_index;
                tmpData.confd = connfd;
                ret = pthread_create(&tmpID, NULL, processClientRequest,(void*)&tmpData);
                if(ret != 0)
                {
                    shutdown(connfd, SHUT_RDWR);
                    close(connfd);
                    printf ("Create pthread error!n");
                    return;
                }
                //鍥炴敹绾跨▼ID
                m_pool[m_index] = tmpID;
                m_index++;
            }
        }
        else
        {
            if(retVal == OSI_ERROR)
            {
                printf("[%s:%d] Select failed! restart netServer.\n",__FUNCTION__,__LINE__);
                shutdown(socket_fd, SHUT_RDWR);
                close(socket_fd);
                goto reStartBNetServer; 
            }
            usleep(10000);
            continue;
        }

        sleep(3);
    }
}
void *C_NetServer::netServerTaskWorker(void *pThis)
{
    C_NetServer *pNetServerObj = (C_NetServer*)pThis;
    pNetServerObj->netServerTask();
    return NULL;
}

int C_NetServer::netServerInit()
{
    int ret = 0;

    ret = pthread_create(&m_threadID, NULL,netServerTaskWorker,this);
    if(ret != 0)
    {
        printf ("Create pthread error!n");
        exit (1);
    }

    return OSI_OK;
}
/************************************
 * Function:      processClientRequest
 * Description:   澶勭悊瀹㈡埛绔姹傜殑涓诲嚱鏁�
 * Access Level:  public
 * Input:         connfd    --- 澶栭儴浼犲叆鐨剆ocket杩炴帴鎻忚堪绗�
 * Output:        N/A
 * Return:        OK/ERROR
 ************************************/
void *C_NetServer::processClientRequest(void *data)
{
    fd_set rset;
    char recvbuff[MAX_BOARD_SDK_BUFFSIZE], *precvbuff;
    int nread;
    struct timeval  select_timeout;
    NETCMD_HEADER netCmdHeader;
    int cmdLength = 0, leftLen = 0, retlen = 0;
    int retVal = OSI_OK;
    time_t sTime;

    THREAD_STRUCT *tmpFD = (THREAD_STRUCT *)data;
    int connfd = tmpFD->confd;

    struct sockaddr_in  clientSockAddr, *pClientSockAddr;
    socklen_t addrlen = sizeof(clientSockAddr);
    if (getpeername(connfd, (struct sockaddr *)&clientSockAddr,&addrlen) != 0)
    {
        printf("[%s:%d]getpeername failed!\n",__FUNCTION__,__LINE__);
    }
    pClientSockAddr = &clientSockAddr;
    memset(recvbuff, 0, MAX_BOARD_SDK_BUFFSIZE);

    select_timeout.tv_sec = 5;
    select_timeout.tv_usec = 0;
    FD_ZERO(&rset);
    FD_SET(connfd, &rset);

    if(select(connfd+1, &rset, NULL, NULL, &select_timeout) > 0)
    {
        if(FD_ISSET(connfd, &rset))
        {
            //receive length, 4bytes 
            //TODO:: should be recvn
            if((nread = recv(connfd, &cmdLength, sizeof(cmdLength), MSG_DONTWAIT)) != sizeof(cmdLength))
            {
                retVal = OSI_ERROR;
                goto errExit;
            }
            cmdLength = ntohl(cmdLength);
            printf("%s %d : Command length = 0x%0x\n", __FUNCTION__,__LINE__, cmdLength);

            if(cmdLength >= 24 && cmdLength <= MAX_BOARD_SDK_BUFFSIZE)
            {
                /* valid command length */
                leftLen = cmdLength - sizeof(cmdLength);

                if((nread=recv(connfd, recvbuff+sizeof(cmdLength), leftLen, MSG_DONTWAIT)) != leftLen)
                {
                    printf("[%s][%d]readn() length error: %d, %d\n",__FUNCTION__,__LINE__, nread, leftLen);
                    retVal = OSI_ERROR;
                    goto errExit;
                }
                precvbuff = recvbuff;
                *(int *)precvbuff = htonl(cmdLength);

                memcpy((char *)&netCmdHeader, recvbuff, sizeof(NETCMD_HEADER));
                netCmdHeader.netCmd = ntohl(netCmdHeader.netCmd);

                printf("cmd is %d\n", netCmdHeader.netCmd);

                switch(netCmdHeader.netCmd)
                {      
            	/* 鑾峰彇淇℃伅鍙戝竷鍖� */
                case NETCMD_GET_INFO:
                    printf("cmd is NETCMD_GET_INFO,buf=[%s]!\n", netCmdHeader.buf);
                    //retVal = SDK_GetInfoPublishPackage(connfd, recvbuff, pClientSockAddr);
                    retlen = send(connfd, &netCmdHeader.buf, sizeof(netCmdHeader.buf), 0); 
                    printf("The retlen is [%d]!\n", retlen);
                    break;

                default:
                printf("Unsupported command: 0x%x\n", netCmdHeader.netCmd);
                    //if (sendNetRetval(connfd, NETRET_NOT_SUPPORT) == ERROR)
                    //{
                    //    OSI_Debug(DLEVEL_ERROR, "[%s][%d]sendNetRetval NETRET_NOT_SUPPORT failed!\n",__FUNCTION__,__LINE__);
                    //}
                    retVal = OSI_ERROR;
                    usleep(50000);
                    break;
                }

            }
            else
            {
                retVal = OSI_ERROR;
            }
        }
    }
    else
    {
        printf("[%s][%d]select failed.\n",__FUNCTION__,__LINE__);
        retVal = OSI_ERROR;
    }

errExit:
    if(retVal != OSI_ERROR)
    {	/* read all datas from socket */
        //TODO:: time(NULL)闇�瑕佹洿鏀�
        //TODO: 涓轰粈涔堣鍔犱笅闈㈣繖娈典唬鐮�? 杩欐浠ｇ爜浼氬鑷磗ocket鑷冲皯5s涓嶄細鍙婃椂鍏虫帀, 绋嶅揩鑰岄绻佺殑澶栭儴SDK璇锋眰瀹规槗瀵艰嚧璁惧socket婧㈠嚭!!!
        sTime = time(NULL);
        while((nread = recv(connfd, recvbuff, 1, MSG_DONTWAIT)) > 0)
        {
            usleep(100000);
            if((time(NULL) - sTime) > 120)	/* wait up to 120 seconds */
            {
                nread = OSI_ERROR;
                break;
            }
        }
        if(nread == OSI_ERROR)
        {
            retVal = OSI_ERROR;
        }
    }

    //TODO::寮勬槑鐧絚onnfd 涓轰粈涔堜細澶辨晥
    if (connfd >= 0)
    {
        shutdown(connfd, SHUT_RDWR);
        close(connfd);
    }

    return NULL;
}
