cmake_minimum_required(VERSION 3.26)
project(DeeP LANGUAGES CXX)

add_compile_options(-Wall -ggdb)
#add_compile_options(-ggdb)
#add_compile_options(-Ofast)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++23")

# set(HEADMat GenerateMass)
set(HDRS_CXX CXX/include)
set(SRCS_CXX CXX/src)

set(OpenNN_DIR C:/Lib/OpenNN/opennn)

set(GLOB ${HDRS_CXX}/Glob.hpp ${SRCS_CXX}/Glob.cpp)
set(LEARNING ${HDRS_CXX}/Learning.hpp ${SRCS_CXX}/Learning.cpp)
set(MODELSETTINGS ${HDRS_CXX}/ModelSettings.hpp ${SRCS_CXX}/ModelSettings.cpp)
set(MESH ${HDRS_CXX}/Mesh.hpp ${SRCS_CXX}/Mesh.cpp)
# file(GLOB GENERMASS "${HEADMat}/*.cpp" "${HEADMat}/*.h")

include_directories(${HDRS_CXX} ${OpenNN_DIR})
# include_directories(${HDRS} ${HEADMat})

add_library(LIB_Glob STATIC ${GLOB})
add_library(LIB_Learning STATIC ${LEARNING})
add_library(LIB_ModelSettings STATIC ${MODELSETTINGS})
add_library(LIB_Mesh STATIC ${MESH})
# add_library(LIB_GenerMass STATIC ${GENERMASS})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Glob)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Learning)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_ModelSettings)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Mesh)
target_link_libraries(LIB_Learning PUBLIC LIB_Glob)
target_link_libraries(LIB_Learning PUBLIC LIB_ModelSettings)
target_link_libraries(LIB_Learning PUBLIC LIB_Mesh)
# target_link_libraries(${PROJECT_NAME} PUBLIC LIB_GenerMass)