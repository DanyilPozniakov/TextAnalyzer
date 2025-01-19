//
// Created by Danyil Pozniakov on /18/2025.
//


#include <boost/asio.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>



boost::asio::io_context io_context;
boost::asio::ip::tcp::resolver resolver(io_context);
boost::asio::ip::tcp::socket clientSocket(io_context);


///CLIENT
int main()
{
    bool is_running = true;


    std::unordered_map<std::string_view, std::function<std::string(std::string)>> commands =
    {
        {
            "setText",
            [](std::string text)
            {
                std::cout << text << " executed." << std::endl;
                return std::string{};
            }
        },

        {
            "setFileText",
            [](std::string filePath)
            {
                return std::string{};
            }
        },

        {
            "connect",
            [&](std::string args)
            {
                std::vector<std::string> tokens;
                tokens.reserve(2);
                boost::split(tokens, args, boost::is_any_of(":"));
                if (tokens.size() != 2)
                {
                    return std::string{"Invalid arguments!"};
                }
                auto endpoint = resolver.resolve(tokens[0], tokens[1]);
                boost::system::error_code ec;
                auto result = boost::asio::connect(clientSocket, endpoint,ec);
                if(!ec)
                {
                    return std::string{"Connected to the server!"};
                }
                return std::string{ec.message()};
            }
        },

        {
            "help",
            [](std::string args)
            {
                return std::string{};
            }
        },

        {
            "disconnect",
            [&](std::string args)
            {
                if(clientSocket.is_open())
                {
                    clientSocket.close();
                    return std::string{"Disconnected from the server!"};
                }
                return std::string{"You are not connected to the server!"};
            }
        }


    };

    while (is_running)
    {
        std::string command;
        std::cout << ">";
        std::getline(std::cin, command);
        boost::algorithm::trim(command);
        if (!command.empty())
        {
            std::string commandName;
            std::string args;

            auto space = command.find(' ');
            if (space == std::string::npos)
            {
                commandName = command;
            }
            else
            {
                commandName = command.substr(0, space);
                args = command.substr(space + 1);
                boost::trim(args);
            }

            if (auto comm = commands.find(commandName); comm != commands.end())
            {
                auto result = comm->second(std::move(args));
                if(!result.empty())
                {
                    std::cout << result << std::endl;
                }
            }
            else
            {
                std::cout << "Command not found!" << std::endl;
            }
        }
    }


    return 0;


    std::cout << "Connected to the server!" << std::endl;

 //   std::string message = "Hello from the client!";
//    boost::asio::write(socket, boost::asio::buffer(message));

    return 0;
}
