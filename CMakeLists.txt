cmake_minimum_required(VERSION 3.11)

project(MyMuduo VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(INSTALL_CMAKEDIR share/${project_name})
option(BUILD_EXAMPLES "Build examples" ON)

set(HEADER_FILES
  # include/Acceptor.h
  # include/Buffer.h
  # include/Callbacks.h
  # include/Channel.h
  # include/CurrentThread.h
  # include/EPollPoller.h
  # include/EventLoop.h
  # include/EventLoopThread.h
  # include/EventLoopThreadPool.h
  include/utils.h
  include/InetAddress.h
  include/Socket.h
  include/Epoll.h
  # include/Logger.h
  # include/noncopyable.h
  

  # include/TcpConnection.h
  # include/TcpServer.h
  # include/Thread.h
  # include/Timestamp.h
)

set(SOURCE_FILES
  # src/Acceptor.cc
  # src/Buffer.cc
  # src/Channel.cc
  # src/CurrentThread.cc
  # src/DefaultPoller.cc
  # src/EPollPoller.cc
  # src/EventLoop.cc
  # src/EventLoopThread.cc
  # src/EventLoopThreadPool.cc
  src/InetAddress.cpp
  src/Socket.cpp
  src/Epoll.cpp
  # src/Logger.cc
  

  # src/TcpConnection.cc
  # src/TcpServer.cc
  # src/Thread.cc
  # src/Timestamp.cc
)

add_library(${CMAKE_PROJECT_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
)


add_library(${CMAKE_PROJECT_NAME}::${CMAKE_PROJECT_NAME} ALIAS ${CMAKE_PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  #$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/MyMuduo>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  #$<INSTALL_INTERFACE:include/${project_name}>
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  pthread
)

set_target_properties(${CMAKE_PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

###########
# Examples
##

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

###############
# Installation
##








