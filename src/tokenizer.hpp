#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <list>
#include <memory>
#include <string>
#include <queue>
#include "i_file_operations.hpp"

namespace libparser
{

class Tokenizer
{
public:
    Tokenizer(std::string, std::shared_ptr<IFileOperations>);
    std::string GetNext();

    static const std::list<std::string> kReservedTokens;

private:
    constexpr static char kEquals = '=';
    constexpr static char kQuotes = '"';

    void GetTokens();
    void StartDeferredActions();
    void FlushToTokens(std::string *);
    bool PartialTokenIsReserved(std::string);
    std::string PopFrontToken();
    std::string AccumulateLineCharacters(std::string, char);

    bool started_;
    std::string filename_;
    std::queue<std::string> tokens_;
    std::shared_ptr<std::istringstream> contents_;
    std::shared_ptr<IFileOperations> file_operations_;
};

}

#endif
