project(client)
cmake_minimum_required (VERSION 3.5)

MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})