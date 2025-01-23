cmake_minimum_required(VERSION 3.16)
project(TextAnalyzer)

set(CMAKE_CXX_STANDARD 23)
add_definitions(-D_WIN32_WINNT=0x0601)

#set(BOOST_ROOT          "PATH_TO_BOOST/boost_1_87_0")
#set(BOOST_LIBRARYDIR    "PATH_TO_BOOST/boost_1_87_0/lib")
#set(BOOST_INCLUDEDIR    "PATH_TO_BOOST/boos/boost_1_87_0")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_PREFIX_PATH "PATH_TO_QT/Qt/6.2.2/msvc2019_64")


set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS json )
find_package(Qt6 6.8 COMPONENTS Core Widgets Network REQUIRED)

add_subdirectory(Server)
add_subdirectory(Client)



