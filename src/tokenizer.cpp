#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include "tokenizer.hpp"

using namespace std::placeholders;

namespace libparser
{

const std::list<std::string> Tokenizer::kReservedTokens
{
    "=",
    "#"
};

Tokenizer::Tokenizer(std::string filename, std::shared_ptr<IFileOperations> fileOperations) :
    started_(false), filename_(filename), tokens_(), contents_(), file_operations_(fileOperations)
{

}

std::string Tokenizer::GetNext()
{
    StartDeferredActions();

    return PopFrontToken();
}

void Tokenizer::GetTokens()
{
    contents_ = file_operations_->Read(filename_);

    std::string line;

    while (!(*contents_).eof())
    {
        *contents_ >> line;

        tokens_.push(
            std::accumulate(
                line.begin(),
                line.end(),
                std::string(),
                std::bind(&Tokenizer::AccumulateLineCharacters, this, _1, _2)));
    }
}

void Tokenizer::StartDeferredActions()
{
    if (!started_)
    {
        GetTokens();
    }
    started_ = true;
}

std::string Tokenizer::AccumulateLineCharacters(std::string constructed_token, char ch)
{
    if ((ch == '=') ||
        (std::find(
                Tokenizer::kReservedTokens.begin(),
                Tokenizer::kReservedTokens.end(),
                constructed_token) !=
            Tokenizer::kReservedTokens.end()))
    {
        tokens_.push(constructed_token);
        constructed_token.clear();
    }

    return constructed_token + ch;
}

std::string Tokenizer::PopFrontToken()
{
    auto token_to_retrieve = tokens_.empty() ? "" : tokens_.front();
    if (!tokens_.empty())
    {
        tokens_.pop();
    }

    return token_to_retrieve;
}

}
