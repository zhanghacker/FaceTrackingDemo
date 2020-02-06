#cmake要求的最低版本
cmake_minimum_required(VERSION 3.4.1)

# 设置用debug还是release模式。debug允许断点，而release更快
#set( CMAKE_BUILD_TYPE Debug )
#set( CMAKE_BUILD_TYPE Release )
#去变量SRC_LIST_CPP 与SRC_LIST_C 指定生成libnative-lib 动态库   默认生成静态库  SHARED指定生成库类型为动态库
#ADD_LIBRARY(myprint SHARED ${SRC_LIST_CPP} ${SRC_LIST_C})
add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# 头文件目录
include_directories(include)

# 设置编译选项
# 允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
# 例子：set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )
# CMAKE_SOURCE_DIR CMakeLists.txt所在文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}")

target_link_libraries( # Specifies the target library.
        native-lib
        opencv_java4
        android
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})