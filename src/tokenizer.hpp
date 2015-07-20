#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <list>
#include <memory>
#include <string>
#include <queue>
#include "i_file_operations.hpp"
#include "i_tokenizer.hpp"
#include "tokens.hpp"

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
    void PushTokenIfValid(std::string&, const char&);
    void TryFlushTokens(std::string *);
    bool IsPartOfQuotedToken(const char&, const std::string&);
    bool IsQuotes(const char&);
    bool IsReservedToken(std::string);
    bool XorOfEnds(std::string *, std::function<bool(const char &)>);
    bool IsToken(const char& ch, const std::string& token);
    std::string PopFrontToken();
    std::string AccumulateLineCharacters(std::string, char);

    std::string filename_;
    std::queue<std::string> tokens_;
    std::shared_ptr<std::istringstream> contents_;
    std::shared_ptr<IFileOperations> file_operations_;
};

}

#endif
