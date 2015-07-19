#include "reserved_tokens.hpp"
#include "parser.hpp"

namespace libparser
{
    Parser::Parser(std::shared_ptr<ITokenizer> tokenizer) : tokenizer_(tokenizer)
    {
        unparsed_tokens_.push(tokens::kSentinel);
    }

    std::map<std::string, std::string> Parser::Parse(std::string filename)
    {
        std::map<std::string, std::string> parsed_data;

        tokenizer_->Read(filename);

        for (
            auto current_token = tokenizer_->GetNext();
            current_token != tokens::kEmpty;
            current_token = tokenizer_->GetNext())
        {
            if (unparsed_tokens_.top().front() == tokens::kEquals)
            {
                unparsed_tokens_.pop();
                auto key = TakeMostRecentUnparsedToken();
                parsed_data[key] = current_token;
            }
            else
            {
                unparsed_tokens_.push(current_token);
            }
        }

        return parsed_data;
    }

    std::string Parser::TakeMostRecentUnparsedToken()
    {
        std::string result = unparsed_tokens_.top();

        unparsed_tokens_.pop();
        return result;
    }
}
