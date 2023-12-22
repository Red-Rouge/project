
message("linux")
cmake_minimum_required(VERSION 3.22.1)
project(test)


# 判断是否是Linux

#指定c版本
set(CMAKE_C_STANDARD 11)
# 连接数学库
# set(CMAKE_C_STANDARD_LIBRARIES "m")

file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.c)
include_directories(${PROJECT_SOURCE_DIR}/inc)
# 指定动态库路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
# include_directories(${PROJECT_SOURCE_DIR}/jpeg/include)


# 去除SRC_LIST里的test.c
list(REMOVE_ITEM SRC_LIST  ${PROJECT_SOURCE_DIR}/src/test.c)

# 指定执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c11 -Wall -fdiagnostics-color  -pthread ")




# 使用gcc命令
set(CMAKE_C_COMPILER arm-linux-gcc)

# 编译模式设置Release/Debug
set(CMAKE_BUILD_TYPE Debug)




add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} m)

# 链接
target_link_libraries(${PROJECT_NAME} libfont.a)


