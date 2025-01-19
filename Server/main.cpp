//
// Created by Danyil Pozniakov on 1/17/2025.
//


#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <numeric>
#include <iostream>
#include <thread>

struct Result
{
    uint64_t totalWords;
    uint64_t uniqueWords;
    uint64_t sequenceOfUniqueWords;
};

/*
 * @brief This algorithm does not change the original text. It is only for analysis.
 */
bool isDelimiter(char c)
{
    return !std::isalpha(static_cast<unsigned char>(c)) && c != '\'';
}

std::pair<std::string::iterator, std::string::iterator> findNextToken(std::string::iterator current,
                                                                      const std::string::iterator& end)
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

std::string removePunctuation(std::string& token)
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

Result AnalyzeText(std::string& text)
{
    std::unordered_map<std::string, uint32_t> words = {};

    auto current = text.begin();
    auto end = text.end();
    uint32_t sequenceOfUniqueWords = 0;

    uint32_t counterOfUniqueWords = 0;
    while (true)
    {
        if (current == end)
        {
            break;
        }

        auto [tokenBegin, tokenEnd] = findNextToken(current, end);
        std::string rawToken(tokenBegin, tokenEnd);
        std::string word = removePunctuation(rawToken);
        if (!word.empty())
        {
            if(words.find(word) == words.end())
            {
                ++words[word];
                ++counterOfUniqueWords;
                current = tokenEnd;
            }
            else
            {
                sequenceOfUniqueWords = std::max(sequenceOfUniqueWords, counterOfUniqueWords);
                counterOfUniqueWords = 0;
            }
        }
        else
        {
            current = tokenEnd;
        }
    }

    //Make result
    Result result{};
    result.totalWords = std::accumulate(words.begin(), words.end(), 0, [](int sum, const auto& p)
    {
        return sum + p.second;
    });
    result.uniqueWords = words.size();
    result.sequenceOfUniqueWords = sequenceOfUniqueWords;

    return result;
}


// /**
//  * @brief test analysis with changes to the original test. Simple example
//  * @param text
//  * @return count of words
//  */
// int AnalyzeText(std::string text)
// {
//
//     std::unordered_map<std::string, int> words = {};
//
//     boost::replace_all(text, "--", " ");
//
//
//     auto pos = std::remove_if(text.begin(), text.end(), [](char c)
//     {
//         return !std::isalpha(c) && !std::isspace(c);
//     });
//
//     text.erase(pos, text.end());
//
//
//     std::cout << text << std::endl;
//
//     return 0;
//
//     std::this_thread::sleep_for(std::chrono::seconds(50000));
//     boost::algorithm::to_lower(text);
//
//     std::stringstream ss(text);
//     std::string word;
//     while (ss >> word)
//     {
//         words[word]++;
//     }
// }


///SERVER
int main()
{
    std::string text =
        R"(I do,' Alice hastily replied; 'at least--at least I mean what I say--that's the same thing, you know.)";
    //std::cout << text << std::endl;
    auto result = AnalyzeText(text);
    std::cout   << "Total words: " << result.totalWords
                << "\nUnique words: " << result.uniqueWords
                << "\nSequence of unique words: " << result.sequenceOfUniqueWords << std::endl;

    // using namespace boost::asio;
    // using namespace boost::asio::ip;
    //
    // std::vector<std::thread> threads;
    // threads.resize(std::thread::hardware_concurrency() - 2);
    // for (auto& thread : threads)
    // {
    //     thread = std::thread();
    // }
    //
    // io_context io_context;
    // ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 24245));
    // ip::tcp::socket socket(io_context);
    //
    // while (true)
    // {
    //     acceptor.accept(socket); //Blocking thread...
    //     std::cout << "Client connected!" << std::endl;
    //
    //     constexpr std::size_t bufferSize = 1024 * 16;
    //     char data[bufferSize] = {};
    //     try
    //     {
    //         while (true)
    //         {
    //             auto len = socket.receive(buffer(data));
    //             data[len] = '\0';
    //             std::cout << data << std::endl;
    //             // process(data);
    //         }
    //     }
    //     catch (boost::system::system_error& e)
    //     {
    //         if (true)
    //         {
    //             std::cerr << "Client was disconnected: " << e.what() << std::endl;
    //             continue;
    //         }
    //     }
    // }
    return 0;
}
