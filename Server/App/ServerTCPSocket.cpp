//
// Created by pozni on 1/20/2025.
//


#include <iostream>
#include "ServerTCPSocket.h"

ServerTCPSocket::ServerTCPSocket(unsigned int port)
    : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

ServerTCPSocket::~ServerTCPSocket()
{
    stop();
}

Message ServerTCPSocket::GetLastMessage()
{
    //This is a blocking call
    std::unique_lock<std::mutex> lock(incomingMessagesMutex);
    incomingMessagesCV.wait(lock, [this](){ return !incomingMessages.empty(); });
    auto message = incomingMessages.front();
    incomingMessages.pop();
    return message;
}


void ServerTCPSocket::start()
{
    isRunning.store(true);

    accept();

    receiveThread = std::thread([this]()
    {
        while(isRunning.load())
        {
            receive(client);
        }
    });
    receiveThread.detach();
}

void ServerTCPSocket::stop()
{
    isRunning.store(false);
    receiveThread.join();
    client->close();
}

void ServerTCPSocket::accept()
{
    boost::system::error_code error;
    client = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    acceptor.accept(*client,error);
    if(error)
    {
        std::cerr << "Error while accepting client: " << error.message() << std::endl;
        return;
    }
    std::cout << "Client connected!" << std::endl;
}

void ServerTCPSocket::receive(const std::shared_ptr<boost::asio::ip::tcp::socket>& client)
{
    try
    {
        if(!client->is_open())
        {
            std::cerr << "Client is not connected! in receive method" << std::endl;
            return;
        }
        char buffer[1024*100] = {0};
        boost::system::error_code error;
        std::size_t len = client->read_some(boost::asio::buffer(buffer), error);
        if(error)
        {
            if(error == boost::asio::error::eof)
            {
                std::cout << "Client disconnected!" << std::endl;
            }
            else
            {
                std::cerr << "Error while receiving message: " << error.message() << std::endl;
                return;
            }
        }

        {
            std::string message(buffer, len);
            std::lock_guard<std::mutex> lock(incomingMessagesMutex);
            incomingMessages.push({client, message});
            incomingMessagesCV.notify_one();
        }
    }
    catch (boost::system::system_error& e)
    {
        std::cerr << "Error while receiving message: " << e.what() << std::endl;

    }
}

void ServerTCPSocket::Send(const Message& message)
{
    if (auto client = message.socket.lock())
    {
        boost::system::error_code error;
        boost::asio::write(*client, boost::asio::buffer(message.message), error);
        if(error)
        {
            std::cerr << "Error while sending message: " << error.message() << std::endl;
        }
        else
        {
            std::cout << "Message sent!" << std::endl;
        }
    }
    else
    {
        std::cout << "Client disconnected!" << std::endl;
    }
}
