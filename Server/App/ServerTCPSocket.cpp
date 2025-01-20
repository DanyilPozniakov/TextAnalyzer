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
}

void ServerTCPSocket::serverIOLoop()
{
    try
    {
        accept(); //blocking thread...

        while (isRunning.load())
        {
            // INPUT/OUTPUT LOOP

            if (client && client->is_open())
            {
                receive(client);
            }

            while (!outgoingMessages.empty())
            {
                send(outgoingMessages.front());
                std::lock_guard lock(outgoing_mtx);
                outgoingMessages.pop();
            }
        }
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
    if(ioThread.joinable())
    {
        ioThread.join();
    }
}

void ServerTCPSocket::accept()
{
    client = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    acceptor.accept(*client);
    std::cout << "Client connected!" << std::endl;
}

void ServerTCPSocket::receive(const std::shared_ptr<boost::asio::ip::tcp::socket>& client)
{
    try
    {
        char buffer[1024 * 16];
        boost::system::error_code error;
        std::size_t len = client->read_some(boost::asio::buffer(buffer), error);
        if (error == boost::asio::error::eof)
        {
            client->close();
            this->client.reset();
            std::cout << "Connection closed" << std::endl;
        }
        if (error)
        {
            throw boost::system::system_error(error);
        }

        {
            //Push the message to the incoming queue
            std::lock_guard lock(incoming_mtx);
            incomingMessages.push({client, std::string(buffer, len)});
            messageReceived_cv.notify_all();
        }
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
        boost::system::error_code error;
        boost::asio::write(*client, boost::asio::buffer(message.message), error);
        if (error)
        {
            std::cerr << "Send failed: " << error.message() << std::endl;
        }
    }
    else
    {
        std::cerr << "Client is disconnected" << std::endl;
    }
}
