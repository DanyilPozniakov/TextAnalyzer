//
// Created by Danyil Pozniakov on 1/17/2025.
//


#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <thread>



// int AnalyzeText(std::string text)
// {
//     std::unordered_map<std::string, int> words = {};
//
//     boost::replace_all(text, "--", " ");
//
//
//     auto pos = std::remove_if(text.begin(), text.end(), [](char c)
//     {
//         return !std::isalpha(c) && !std::isspace(c);
//     });
//
//     text.erase(pos, text.end());
//
//
//     std::cout << text << std::endl;
//
//     return 0;
//
//     std::this_thread::sleep_for(std::chrono::seconds(50000));
//     boost::algorithm::to_lower(text);
//
//     std::stringstream ss(text);
//     std::string word;
//     while(ss >> word)
//     {
//         words[word]++;
//     }
// }


///SERVER
int main()
{
    std::string text = "I do,' Alice hastily replied; 'at least--at least I mean what I say--that's the same thing, you know.";
    std::cout << text << std::endl;
    //AnalyzeText(text);






    // using namespace boost::asio;
    // using namespace boost::asio::ip;
    //
    // std::vector<std::thread> threads;
    // threads.resize(std::thread::hardware_concurrency() - 2);
    // for (auto& thread : threads)
    // {
    //     thread = std::thread();
    // }
    //
    // io_context io_context;
    // ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 24245));
    // ip::tcp::socket socket(io_context);
    //
    // while (true)
    // {
    //     acceptor.accept(socket); //Blocking thread...
    //     std::cout << "Client connected!" << std::endl;
    //
    //     constexpr std::size_t bufferSize = 1024 * 16;
    //     char data[bufferSize] = {};
    //     try
    //     {
    //         while (true)
    //         {
    //             auto len = socket.receive(buffer(data));
    //             data[len] = '\0';
    //             std::cout << data << std::endl;
    //             // process(data);
    //         }
    //     }
    //     catch (boost::system::system_error& e)
    //     {
    //         if (true)
    //         {
    //             std::cerr << "Client was disconnected: " << e.what() << std::endl;
    //             continue;
    //         }
    //     }
    // }
    return 0;
}
