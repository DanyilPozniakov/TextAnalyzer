//
// Created by pozni on 1/20/2025.
//

#ifndef SERVER_H
#define SERVER_H

#include <boost/json.hpp>
#include "ServerTCPSocket.h"

typedef std::pair<std::string::iterator, std::string::iterator> Token;

struct Result
{
    uint64_t totalWords;
    uint64_t uniqueWords;
    uint64_t sequenceOfUniqueWords;

    std::string toJson()
    {
        boost::json::object obj;
        obj["totalWords"] = totalWords;
        obj["uniqueWords"] = uniqueWords;
        obj["sequenceOfUniqueWords"] = sequenceOfUniqueWords;
        return boost::json::serialize(obj);
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
    ServerTCPSocket serverTCPSocket;

    std::vector<std::thread> threads;

    //Text analysis methods
    bool isDelimiter(char c);
    Token findNextToken(std::string::iterator current,const std::string::iterator& end);
    std::string removePunctuation(std::string& token);
    Result AnalyzeText(std::string text);

};


#endif //SERVER_H
