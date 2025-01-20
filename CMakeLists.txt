cmake_minimum_required(VERSION 3.29)
project(TextAnalyzer)

set(CMAKE_CXX_STANDARD 23)
add_definitions(-D_WIN32_WINNT=0x0601)

set(BOOST_ROOT          "C:/Lib/Boost/boost_1_87_0")
set(BOOST_LIBRARYDIR    "C:/Lib/Boost/boost_1_87_0/stage/lib")
set(BOOST_INCLUDEDIR    "C:/Lib/Boost/boost_1_87_0")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS json )

add_subdirectory(Server)
add_subdirectory(Client)


