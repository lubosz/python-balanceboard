CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(python-wiic)

#SET(CMAKE_CXX_FLAGS "-g -Wall")
include_directories(
/usr/include/wiic
/usr/include/python3.3m/
)

ADD_EXECUTABLE(balance wiic.c)
TARGET_LINK_LIBRARIES(balance wiic python3.3m)

ADD_EXECUTABLE(example example.c)
TARGET_LINK_LIBRARIES(example wiic)
