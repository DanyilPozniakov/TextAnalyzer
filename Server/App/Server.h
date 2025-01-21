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
    uint64_t totalWords;
    uint64_t uniqueWords;
    uint64_t sequenceOfUniqueWords;

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
        totalWords += other.totalWords;
        uniqueWords += other.uniqueWords;
        sequenceOfUniqueWords += other.sequenceOfUniqueWords;
        return *this;
    }

    void reset()
    {
        totalWords = 0;
        uniqueWords = 0;
        sequenceOfUniqueWords = 0;
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
    std::atomic<bool> isRunning = false;

    ServerTCPSocket serverTCPSocket;

    //Text analysis methods
    void splitText(const std::string& text, std::queue<std::string>& partsText, int parts);
    bool isDelimiter(char c);
    Token findNextToken(std::string::iterator current,const std::string::iterator& end);
    std::string removePunctuation(std::string& token);
    Result AnalyzeText(std::string text);

};


#endif //SERVER_H
