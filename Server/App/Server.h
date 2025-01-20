//
// Created by pozni on 1/20/2025.
//

#ifndef SERVER_H
#define SERVER_H

#include <boost/json.hpp>
#include <boost/json/serialize.hpp>
#include "ServerTCPSocket.h"

typedef std::pair<std::string::iterator, std::string::iterator> Token;

struct Result
{
    std::atomic<uint64_t> totalWords;
    std::atomic<uint64_t> uniqueWords;
    std::atomic<uint64_t> sequenceOfUniqueWords;

    [[nodiscard]]std::string toJson() const
    {
        boost::json::object obj;
        obj["totalWords"] = totalWords;
        obj["uniqueWords"] = uniqueWords;
        obj["sequenceOfUniqueWords"] = sequenceOfUniqueWords;
        return boost::json::serialize(obj);
    }

    Result& operator+=(const Result& other)
    {
        totalWords.fetch_add(other.totalWords.load());
        uniqueWords.fetch_add(other.uniqueWords.load());
        sequenceOfUniqueWords.fetch_add(other.sequenceOfUniqueWords.load());
        return *this;
    }
};

class Server
{
public:
    explicit Server();
    virtual ~Server() = default;
    void Run();
    void Stop();








private:
    std::queue<std::string> partsText;

    std::mutex partsTextMutex;
    std::condition_variable partsTextCV;

    ServerTCPSocket serverTCPSocket;
    std::vector<std::thread> threads;
    std::atomic<bool> isRunning = false;

    //Text analysis methods
    void splitText(const std::string& text, std::queue<std::string>& partsText, int parts);
    void workerThreadLoop();


    bool isDelimiter(char c);
    Token findNextToken(std::string::iterator current,const std::string::iterator& end);
    std::string removePunctuation(std::string& token);
    void AnalyzeText(std::string text);

};


#endif //SERVER_H
