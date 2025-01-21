//
// Created by pozni on 1/20/2025.
//


#include <iostream>
#include "ServerTCPSocket.h"

ServerTCPSocket::ServerTCPSocket(unsigned int port)
    : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket(io_context)
{
}

ServerTCPSocket::~ServerTCPSocket()
{
    stop();
}

Message ServerTCPSocket::GetLastMessage()
{
    /*
     * This method blocks the thread when it is called until in the queue there is at least one message
     */

    std::unique_lock lock(incoming_mtx);
    messageReceived_cv.wait(lock, [this]()
    {
        return !incomingMessages.empty();
    });
    Message massage = incomingMessages.front();
    incomingMessages.pop();
    return massage;
}

void ServerTCPSocket::AddMessageToOutgoingQueue(const Message& message)
{
    std::lock_guard lock(outgoing_mtx);
    outgoingMessages.push(message);
    if(outgoingMessages.size() > 0)
    {
        send(message);
    }
}

void ServerTCPSocket::serverIOLoop()
{
    try
    {
        accept(); //blocking thread...
        io_context.run();
    }
    catch (boost::system::system_error& e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

void ServerTCPSocket::start()
{
    isRunning.store(true);
    ioThread = std::thread(&ServerTCPSocket::serverIOLoop, this);
}

void ServerTCPSocket::stop()
{
    isRunning.store(false);
    if (ioThread.joinable())
    {
        ioThread.join();
    }

    io_context.stop();

}

void ServerTCPSocket::accept()
{
    client = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    acceptor.accept(*client);
    receive(client);
    std::cout << "Client connected!" << std::endl;
}

void ServerTCPSocket::receive(const std::shared_ptr<boost::asio::ip::tcp::socket>& client)
{
    try
    {
        auto buffer = std::make_shared<std::vector<char>>(1024 * 16);
        boost::system::error_code error;
        client->async_read_some(boost::asio::buffer(*buffer), [this, client,buffer](boost::system::error_code error, std::size_t len)
        {
            if (!error)
            {
                std::lock_guard lock(incoming_mtx);
                incomingMessages.push(Message{client, std::string(buffer->data(), len)});
            }
            messageReceived_cv.notify_one();
            receive(client);
        });
    }
    catch (boost::system::system_error& e)
    {
        std::cerr << "Client was disconnected: " << e.what() << std::endl;
        client->close();
        this->client.reset();
    }
}

void ServerTCPSocket::send(const Message& message)
{
    if (auto client = message.socket.lock())
    {
        boost::asio::async_write(*client,boost::asio::buffer(message.message),
        [message](boost::system::error_code error, std::size_t len)
        {
            if (error)
            {
                std::cerr << "Error sending message: " << error.message() << std::endl;
            }
            else
            {
                std::cout << "Message sent!" << std::endl;
            }
        });

        outgoingMessages.pop();

        if(!outgoingMessages.empty())
        {
            send(outgoingMessages.front());
        }
    }
    else
    {
        std::cout << "Client disconnected!" << std::endl;
        std::lock_guard lock(outgoing_mtx);
        outgoingMessages.pop();
    }
}
