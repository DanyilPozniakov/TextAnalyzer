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


    std::condition_variable messageReceived_cv;
    std::condition_variable outgoingMessage_cv;

    std::mutex incoming_mtx;
    std::mutex outgoing_mtx;

    void start();
    void stop();

    explicit ServerTCPSocket(unsigned int port = 24245);
    virtual ~ServerTCPSocket();

    Message GetLastMessage();
    void AddMessageToOutgoingQueue(const Message& message);


private:
    void serverIOLoop();
    void accept();
    void receive(const std::shared_ptr<boost::asio::ip::tcp::socket>& client);
    void send(const Message& message);

    std::queue<Message> incomingMessages;
    std::queue<Message> outgoingMessages;

    std::atomic<bool> isRunning = false;
    std::thread ioThread;

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;

    std::shared_ptr<boost::asio::ip::tcp::socket> client;
};


#endif //SERVERTCPSOCKET_H
