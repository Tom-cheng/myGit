
cmake_minimum_required(VERSION 2.8)

# 指定工程名和对应的工具链
PROJECT(smartHome)

IF (DEFINED OPTIONS)
	ADD_DEFINITIONS(${OPTIONS})
ENDIF()

INCLUDE(${PROJECT_SOURCE_DIR}/build/${PROJECT_FILE})

# 设置不同编译等级对应的告警输出和优化等级
SET(DEBUG_ADD_DEFINE   "-O2 -Wall -fsigned-char -g")
SET(RELEASE_ADD_DEFINE "-O2 -Wall -fsigned-char")

# 设置屏蔽的告警类型
SET(EX_ADD_DEFINE "-Wno-unused -Wno-sign-compare -Wno-uninitialized -Wno-conversion -Wno-write-strings -Wno-return-type -Wno-format")

# 指定编译等级debug or release
SET(CMAKE_BUILD_TYPE "debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} ${DEBUG_ADD_DEFINE}")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} ${RELEASE_ADD_DEFINE} ${EX_ADD_DEFINE}")
SET(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} ${DEBUG_ADD_DEFINE}")
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} ${RELEASE_ADD_DEFINE} ${EX_ADD_DEFINE}")

# release版本strip hicore
if(${CMAKE_BUILD_TYPE} MATCHES "release")
    ADD_CUSTOM_COMMAND (
                     TARGET sc_infoPC 
                     POST_BUILD 
                     COMMAND ${CMAKE_STRIP} $@
                     COMMAND ${CMAKE_STRIP} -x -R .note -R .comment $@)
endif()



