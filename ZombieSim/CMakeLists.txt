# cmake . && make && ./ZombieSim to run.

cmake_minimum_required(VERSION 3.16)
project(ZombieSim)
add_executable(ZombieSim ZombieSim.cpp RunThreads.cpp Management.cpp CreateObjects.cpp CreatePlayField.cpp Initialize.cpp)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost 1.71 COMPONENTS system chrono thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
find_package(Threads REQUIRED)

target_link_libraries(ZombieSim PRIVATE ${Boost_LIBRARIES} Threads::Threads)