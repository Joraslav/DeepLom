cmake_minimum_required(VERSION 3.22)
project(Xyu LANGUAGES CXX)
# project(Xyu VERSION ${CMAKE_VERSION} LANGUAGES CXX)

add_compile_options(-Wall -ggdb)
#add_compile_options(-ggdb)
#add_compile_options(-Ofast)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(HEADMat GenerateMass)
set(HDRS CXX/Headers)
set(SRCS CXX/Sources)

set(GLOB ${HDRS}/Glob.hpp ${SRCS}/Glob.cpp)
set(LEARNING ${HDRS}/Learning.hpp ${SRCS}/Learning.cpp)
set(MODELSETTINGS ${HDRS}/ModelSettings.hpp ${SRCS}/ModelSettings.cpp)
# file(GLOB GENERMASS "${HEADMat}/*.cpp" "${HEADMat}/*.h")

include_directories(${HDRS})
# include_directories(${HDRS} ${HEADMat})

add_library(LIB_Glob STATIC ${GLOB})
add_library(LIB_Learning STATIC ${LEARNING})
add_library(LIB_ModelSettings STATIC ${MODELSETTINGS})
# add_library(LIB_GenerMass STATIC ${GENERMASS})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Glob)
target_link_libraries(LIB_Learning PUBLIC LIB_Glob)
target_link_libraries(LIB_Learning PUBLIC LIB_ModelSettings)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Learning)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_ModelSettings)
# target_link_libraries(${PROJECT_NAME} PUBLIC LIB_GenerMass)