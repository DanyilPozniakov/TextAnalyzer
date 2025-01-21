//
// Created by Pozniakov Danyil on 1/20/2025.
//

#ifndef SERVERTCPSOCKET_H
#define SERVERTCPSOCKET_H

#include <queue>
#include <boost/asio.hpp>
#include <memory>


struct Message
{
    std::weak_ptr<boost::asio::ip::tcp::socket> socket;
    std::string message;
};

class ServerTCPSocket
{
public:

    void start();
    void stop();

    explicit ServerTCPSocket(unsigned int port = 2525);
    virtual ~ServerTCPSocket();

    Message GetLastMessage();
    void Send(const Message& message);


private:
    void accept();
    void receive(const std::shared_ptr<boost::asio::ip::tcp::socket>& client);

    std::thread receiveThread;
    std::queue<Message> incomingMessages;
    std::mutex incomingMessagesMutex;
    std::condition_variable incomingMessagesCV;

    std::atomic<bool> isRunning = false;

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor;

    std::shared_ptr<boost::asio::ip::tcp::socket> client;
};


#endif //SERVERTCPSOCKET_H
