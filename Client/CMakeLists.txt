

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/msvc2022_64")

find_package(Qt6 6.8 COMPONENTS Core Quick qml QuickControls2 Gui REQUIRED)



add_executable(TextAnalyzerClient
        main.cpp
        MainWindow/mainwindow.cpp
        MainWindow/mainwindow.h
        MainWindow/mainwindow.ui


)

target_link_libraries(TextAnalyzerClient PUBLIC
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

