


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

if (WIN32)

    find_program(WINDEPLOYQT_EXECUTABLE windeployqt
            PATHS ${Qt6_DIR}/6.8.0/msvc2022_64/bin
            ENV PATH
    )

    if (WINDEPLOYQT_EXECUTABLE)
        message(STATUS "windeployqt find: ${WINDEPLOYQT_EXECUTABLE}")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(WINDEPLOYQT_FLAGS "--debug")
        else()
            set(WINDEPLOYQT_FLAGS "--release")
        endif()

        add_custom_command(TARGET TextAnalyzerClient POST_BUILD
                COMMAND ${WINDEPLOYQT_EXECUTABLE} ${WINDEPLOYQT_FLAGS}  "$<TARGET_FILE:TextAnalyzerClient>"
                COMMENT "windeployqt..."
        )
    else()
        message(FATAL_ERROR "windeployqt is not found")
    endif()

endif()

include(GNUInstallDirs)
install(TARGETS TextAnalyzerClient
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})