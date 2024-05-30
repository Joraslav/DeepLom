cmake_minimum_required(VERSION 3.22.0)
project(DeepLom VERSION 0.1.0 LANGUAGES C CXX)

add_compile_options(-Wall)

set(CMAKE_CXX_STANDARD 23)

set(HDRS_MY MyLib/include)
set(SRCS_MY MyLib/src)

set(GLOB ${HDRS_MY}/Glob.hpp ${SRCS_MY}/Glob.cpp)
set(LEARNING ${HDRS_MY}/Learning.hpp ${SRCS_MY}/Learning.cpp)
set(MODEL ${HDRS_MY}/Model.hpp ${SRCS_MY}/Model.cpp)
set(MESH ${HDRS_MY}/Mesh.hpp ${SRCS_MY}/Mesh.cpp)
set(DATA ${HDRS_MY}/Data.hpp ${SRCS_MY}/Data.cpp)
# set(DQN ${HDRS_MY}/DQN.hpp ${SRCS_MY}/DQN.cpp)

include_directories(${HDRS_MY})

add_library(LIB_Glob STATIC ${GLOB})
add_library(LIB_Learning STATIC ${LEARNING})
add_library(LIB_Model STATIC ${MODEL})
add_library(LIB_Mesh STATIC ${MESH})
add_library(LIB_Data STATIC ${DATA})
# add_library(LIB_DQN STATIC ${DQN})

add_executable(DeepLom main.cpp)

# set(MLPACK_INCLUDE_DIRS "C:/msys64/mingw64/include/mlpack/mlpack.hpp")
# target_include_directories(${PROJECT_NAME} PRIVATE ${MLPACK_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Glob)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Learning)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Model)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Mesh)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_Data)
target_link_libraries(LIB_Learning PUBLIC LIB_Glob)
target_link_libraries(LIB_Learning PUBLIC LIB_Model)
target_link_libraries(LIB_Learning PUBLIC LIB_Mesh)
target_link_libraries(LIB_Data PUBLIC LIB_Glob)
target_link_libraries(LIB_Data PUBLIC LIB_Learning)