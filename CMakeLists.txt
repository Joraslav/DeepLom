cmake_minimum_required(VERSION 3.25.2)
project(Beng VERSION ${CMAKE_VERSION})

add_compile_options(-Wall -ggdb)
#add_compile_options(-ggdb)
#add_compile_options(-Ofast)

set(HDRS CXX/Headers)
set(SRCS CXX/Sources)

set(LIB_GLOB ${HDRS}/Glob.hpp)
set(LIB_Learning ${HDRS}/Learning.hpp ${SRCS}/Learning.cpp)

include_directories(${HDRS})

add_library(Glob STATIC ${LIB_GLOB})
add_library(Learning STATIC ${LIB_Learning})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Glob)
target_link_libraries(Learning PUBLIC Glob)
target_link_libraries(${PROJECT_NAME} PRIVATE Learning)