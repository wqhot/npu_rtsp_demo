cmake_minimum_required(VERSION 3.5)

if (PLATFORM STREQUAL "ARM")

    SET(CMAKE_SYSTEM_NAME Linux)
    SET(CMAKE_SYSTEM_PROCESSOR arm) 

    SET(CMAKE_FIND_ROOT_PATH /home/wang/libs/cross/install)

    SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

    SET(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

else (PLATFORM STREQUAL "ARM")

    SET(CMAKE_FIND_ROOT_PATH /usr)

endif (PLATFORM STREQUAL "ARM")

project(gltest)

if (BUILD_TYPE STREQUAL "RELEASE")
set(CMAKE_BUILD_TYPE "RELEASE")
else (BUILD_TYPE STREQUAL "RELEASE")
set(CMAKE_BUILD_TYPE "DEBUG")
endif (BUILD_TYPE STREQUAL "RELEASE")

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -pthread -fuse-ld=gold")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pthread -fuse-ld=gold")
else(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O0 -g -Wall -pthread -pg -fuse-ld=gold")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -Wall -pthread -pg -fuse-ld=gold")
endif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)
find_package(OpenCV 4 REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/decode
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_FIND_ROOT_PATH}/include
        ${PROJECT_SOURCE_DIR}/decode/live/liveMedia/include
        ${PROJECT_SOURCE_DIR}/decode/live/BasicUsageEnvironment/include
        ${PROJECT_SOURCE_DIR}/decode/live/UsageEnvironment/include
        ${PROJECT_SOURCE_DIR}/decode/live/groupsock/include
        # ${CMAKE_FIND_ROOT_PATH}/include/liveMedia
        # ${CMAKE_FIND_ROOT_PATH}/include/BasicUsageEnvironment
        # ${CMAKE_FIND_ROOT_PATH}/include/UsageEnvironment
        # ${CMAKE_FIND_ROOT_PATH}/include/groupsock
)

link_directories(
    ${CMAKE_FIND_ROOT_PATH}/lib
    ${CMAKE_FIND_ROOT_PATH}/lib/aarch64-linux-gnu
)

add_library(glhelper
    glhelper.cpp
    fbhelper.cpp
)

target_link_libraries(glhelper
    GL
    GLU
    glut
)

add_library(decode
    decode/decode.c
    decode/rtsp_client.cpp
)

target_link_libraries(decode
    mwv206api
    mwv206dec
    jmgpumediacodec
    liveMedia
    groupsock
    BasicUsageEnvironment
    UsageEnvironment
    ssl
    crypto
)

add_executable(${PROJECT_NAME} main.cpp  
    pipeline.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glut
        gflags
        glog
        glhelper
        decode
        jssdetect
        ${OpenCV_LIBS} 
        ${CMAKE_THREAD_LIBS_INIT}
)