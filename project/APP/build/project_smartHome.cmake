
cmake_minimum_required(VERSION 2.8)

# 指定工程名和对应的工具链
PROJECT(smartHome)
INCLUDE(${PROJECT_SOURCE_DIR}/build/toolchain-gcc.cmake)

#关联目录中的库编译
#ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/service/softwareBus)

# 指定工程需要的宏
SET(PROJECT_DEFINITIONS -D_REENTRANT -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 
				-D__linux__  -D_GNU_SOURCE -DOS_POSIX )

ADD_DEFINITIONS(${PROJECT_DEFINITIONS})
                 
INCLUDE(${PROJECT_SOURCE_DIR}/build/include_source.cmake)


LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/module)

# 编译输出可执行文件
ADD_EXECUTABLE(sc_smartHome ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../package/smartHome)

#SET(LIB_EXTERNDEP1 -lscNetIf -lcom_lib_init -lshellCmdSubSys -lbroker -lsbus -lUnionCom -lhkdpl -lqrencode -laestable -lbase64)

# 指定链接的库
TARGET_LINK_LIBRARIES(sc_smartHome
-lzmq -lDSI -lOSI
#-lxml -lSwBus -lAplSadp -lencrypt -lshellServer -lssl -lcrypto -lcurl
#-liconv -lsqlite3  
-lpthread -lstdc++ -lrt -lm  -ldl -lc)
#${LIB_EXTERNDEP1} ${LIB_EXTERNDEP2} }
