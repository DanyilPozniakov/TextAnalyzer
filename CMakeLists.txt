cmake_minimum_required(VERSION 3.29)
project(TextAnalyzer)

set(CMAKE_CXX_STANDARD 23)

set(BOOST_ROOT          "C:/Lib/Boost/boost_1_87_0/boost")
set(BOOST_LIBRARYDIR    "C:/Lib/Boost/boost_1_87_0/stage/lib")
set(BOOST_INCLUDEDIR    "C:/Lib/Boost/boost_1_87_0")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)


add_subdirectory(Server)
add_subdirectory(Client)


