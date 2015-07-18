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

    std::string segment;
    std::string partial_token;

    while (!(*contents_).eof())
    {
        *contents_ >> segment;

        auto last_token_in_segment = std::accumulate(
                segment.begin(),
                segment.end(),
                std::string(),
                std::bind(&Tokenizer::AccumulateLineCharacters, this, _1, _2));

        partial_token += (partial_token.empty() ? last_token_in_segment : " " + last_token_in_segment);

        if(partial_token.front() != kQuotes || partial_token.back()== kQuotes)
        {
            FlushToTokens(&partial_token);
        }
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

void Tokenizer::FlushToTokens(std::string* partial_token)
{
    tokens_.push(*partial_token);
    partial_token->clear();
}

bool Tokenizer::PartialTokenIsReserved(std::string partial_token)
{
    return (std::find(
            Tokenizer::kReservedTokens.begin(),
            Tokenizer::kReservedTokens.end(),
            partial_token) !=
        Tokenizer::kReservedTokens.end());
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

std::string Tokenizer::AccumulateLineCharacters(std::string partial_token, char ch)
{
    if ((ch == kEquals) ||
        PartialTokenIsReserved(partial_token))
    {
        FlushToTokens(&partial_token);
    }

    return partial_token + ch;
}

}
