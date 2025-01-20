//
// Created by pozni on 1/20/2025.
//


#include <boost/algorithm/string.hpp>
#include "Server.h"


Server::Server()
 : serverTCPSocket(2525)
{
    threads.reserve(std::thread::hardware_concurrency() - 2);

}

void Server::Run()
{
    serverTCPSocket.start();

    auto message = serverTCPSocket.incomingMessages.front();
}

void Server::Stop()
{
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
    boost::algorithm::to_lower(token);

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

Result Server::AnalyzeText(std::string text)
{
    std::unordered_map<std::string, uint32_t> words = {};
    auto current = text.begin();
    auto end = text.end();

    std::size_t totalWords = 0;
    std::size_t sequenceOfUniqueWords = 0;

    std::size_t startWindow = 0;
    std::size_t endWindow = 0;

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
    Result result;
    result.totalWords = totalWords;
    result.uniqueWords = words.size();
    result.sequenceOfUniqueWords = sequenceOfUniqueWords;
    return result;
}

