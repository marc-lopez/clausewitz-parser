#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <map>
#include <memory>
#include <stack>
#include "i_tokenizer.hpp"

namespace libparser
{

class Parser
{
    public:
        Parser(std::shared_ptr<ITokenizer>);
        std::map<std::string, std::string> Parse(std::string);
    protected:
    private:
        std::string TakeMostRecentUnparsedToken();

        std::stack<std::string> unparsed_tokens_;
        std::shared_ptr<ITokenizer> tokenizer_;
};

}

#endif // PARSER_HPP_
