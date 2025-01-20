//
// Created by pozni on 1/20/2025.
//


#include <boost/algorithm/string.hpp>
#include "Server.h"

Result globalResult;

Server::Server()
 : serverTCPSocket(2525)
{
    threads.reserve(std::thread::hardware_concurrency() - 2);

}

void Server::Run()
{
    serverTCPSocket.start();
    isRunning.store(true);

    for(auto& thread : threads)
    {
        thread = std::thread([this](){ });
    }


    while(isRunning.load())
    {

        auto message = serverTCPSocket.GetLastMessage();
        std::string text = message.message;

        splitText(text, partsText, threads.size());

    }

}

void Server::Stop()
{

}


void Server::splitText(const std::string& text, std::queue<std::string>& partsText, int parts)
{
    if(text.empty())
    {
        return;
    }
    std::size_t partSize = text.size() / parts;

    std::size_t start = 0;
    for(int i = 0; i < parts; ++i)
    {
        std::size_t end = start + partSize;

        while(end < text.size() && !std::isspace(static_cast<unsigned char>(text[end])))
        {
            ++end;
        }

        partsText.push(text.substr(start, end - start));
        start = end;

        if(start >= text.size())
        {
            break;
        }
    }
    if (start < text.size()) {
        partsText.push(text.substr(start));
    }
}

void Server::workerThreadLoop()
{
    std::string part;
    {
        std::unique_lock lock(partsTextMutex);
        partsTextCV.wait(lock, [this]()
        {
            return !partsText.empty();
        });

        std::string part = std::move(partsText.front());
        partsText.pop();
    }
    AnalyzeText(part);


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

void Server::AnalyzeText(std::string text)
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

    globalResult += result;
}

