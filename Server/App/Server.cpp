//
// Created by pozni on 1/20/2025.
//

#include <iostream>
#include <boost/algorithm/string.hpp>
#include "Server.h"
#include <chrono>


Server::Server()
 : serverTCPSocket(2525)
{

}

void Server::Run()
{
    isRunning.store(true);
    serverTCPSocket.start();


    while(isRunning.load())
    {
        try
        {
            auto message = serverTCPSocket.GetLastMessage();
            std::cout << getCurrentTime() << " Server received a new request!\n";

            auto start = std::chrono::high_resolution_clock::now();
            std::string text = message.message;
            std::string response = analyzeText(text).toJson();

            Message responseMessage;
            responseMessage.socket = message.socket;
            responseMessage.message = response;
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            std::cout << getCurrentTime() << " Server processed the request in " << duration.count() << " ms\n";
            serverTCPSocket.Send(responseMessage);
        }
        catch (std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;

        }
    }
}

void Server::Stop()
{
    isRunning.store(false);
    serverTCPSocket.stop();
}

bool Server::isDelimiter(char c)
{
    return !std::isalpha(static_cast<unsigned char>(c)) && c != '\'';
}

Token Server::findNextToken(std::string::iterator current, const std::string::iterator& end)
{
    std::string::iterator tokenBegin = current;
    std::string::iterator tokenEnd;

    while (current != end && isDelimiter(*current))
    {
        ++current;
    }
    tokenBegin = current;

    while (current != end && !isDelimiter(*current))
    {
        ++current;
    }
    tokenEnd = current;

    return {tokenBegin, tokenEnd};
}

std::string Server::removePunctuation(std::string& token)
{
    //boost::algorithm::to_lower(token);
    std::transform(token.begin(), token.end(), token.begin(),
        [](unsigned char c){ return std::tolower(c); });

    while (!token.empty() && token.front() == '\'')
    {
        token.erase(token.begin());
    }

    while (!token.empty() && token.back() == '\'')
    {
        token.pop_back();
    }

    return token;
}

Result Server::analyzeText(std::string text)
{
    std::unordered_map<std::string, uint64_t> words = {};
    words.reserve(1000);
    auto current = text.begin();
    auto end = text.end();

    std::size_t totalWords = 0;
    std::size_t sequenceOfUniqueWords = 0;

    std::size_t startWindow = 0;

    std::size_t wordIndex = 0;
    while (current != end)
    {
        auto [tokenBegin, tokenEnd] = findNextToken(current, end);
        std::string rawToken(tokenBegin, tokenEnd);
        std::string word = removePunctuation(rawToken);

        if (!word.empty())
        {
            ++totalWords;
            if(auto it = words.find(word); it != words.end())
            {
                startWindow = std::max<std::size_t>(startWindow, it->second + 1);
                it->second = wordIndex;

            }
            else
            {
                words[word] = wordIndex;
            }
            ++wordIndex;

            sequenceOfUniqueWords = std::max(sequenceOfUniqueWords, wordIndex - startWindow);
        }
        current = tokenEnd;
    }

    return Result({totalWords, words.size(), sequenceOfUniqueWords});
}

std::string Server::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm buf;
#if defined(_MSC_VER) || defined(__MINGW32__)
    localtime_s(&buf, &in_time_t); // Windows
#else
    localtime_r(&in_time_t, &buf); // Unix macos
#endif

    std::ostringstream oss;
    oss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}



