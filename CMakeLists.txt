cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(PROJECT_EXAMPLE_NAME example)
set(EXAMPLE_C_FLAGS "-g")

add_executable(${PROJECT_EXAMPLE_NAME} example.cpp)

find_package(Cassandra REQUIRED)

include_directories(${CASSANDRA_INCLUDE_DIRS})
target_link_libraries(${PROJECT_EXAMPLE_NAME} ${CASSANDRA_LIBRARIES})

find_package(Libuv REQUIRED)
target_link_libraries(${PROJECT_EXAMPLE_NAME} ${LIBUV_LIBRARIES})

set_property(
  TARGET ${PROJECT_EXAMPLE_NAME}
  APPEND PROPERTY COMPILE_FLAGS ${EXAMPLE_C_FLAGS})
