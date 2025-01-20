
add_executable(Server main.cpp
        App/ServerTCPSocket.cpp
        App/ServerTCPSocket.h
        App/Server.cpp
        App/Server.h
)

target_link_libraries(Server PUBLIC Boost::boost)