#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <list>
#include <memory>
#include <string>
#include <queue>
#include "i_file_operations.hpp"
#include "i_tokenizer.hpp"
#include "reserved_tokens.hpp"

namespace libparser
{

class Tokenizer : public ITokenizer
{
public:
    Tokenizer(std::shared_ptr<IFileOperations>);
    void Read(std::string);
    std::string GetNext();

    static const std::list<std::string> kReservedTokens;

private:

    void GetTokens();
    void FlushToTokens(std::string *);
    void TryFlushTokens(std::string *);
    bool IsQuotes(const char&);
    bool IsReservedToken(std::string);
    bool XorOfEnds(std::string *, std::function<bool(const char &)>);
    std::string PopFrontToken();
    std::string AccumulateLineCharacters(std::string, char);

    std::string filename_;
    std::queue<std::string> tokens_;
    std::shared_ptr<std::istringstream> contents_;
    std::shared_ptr<IFileOperations> file_operations_;
};

}

#endif