#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include "reserved_tokens.hpp"
#include "tokenizer.hpp"

using namespace std::placeholders;

namespace libparser
{

const std::list<std::string> Tokenizer::kReservedTokens
{
    tokens::kEquals,
    tokens::kComment
};

Tokenizer::Tokenizer(std::shared_ptr<IFileOperations> fileOperations) :
    filename_(), tokens_(), contents_(), file_operations_(fileOperations)
{

}

void Tokenizer::Read(std::string filename)
{
    filename_ = filename;
    GetTokens();
}

std::string Tokenizer::GetNext()
{
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
        TryFlushTokens(&partial_token);
    }
}

void Tokenizer::FlushToTokens(std::string * partial_token)
{
    tokens_.push(*partial_token);
    partial_token->clear();
}

void Tokenizer::TryFlushTokens(std::string * partial_token)
{
    if(!XorOfEnds(partial_token, std::bind(&Tokenizer::IsQuotes, this, _1)))
    {
        FlushToTokens(partial_token);
    }

}

bool Tokenizer::IsReservedToken(std::string partial_token)
{
    return (std::find(
            Tokenizer::kReservedTokens.begin(),
            Tokenizer::kReservedTokens.end(),
            partial_token) !=
        Tokenizer::kReservedTokens.end());
}

bool Tokenizer::IsQuotes(const char &ch)
{
    return (ch == tokens::kQuotes.front());
}

bool Tokenizer::XorOfEnds(std::string * str, std::function<bool(const char &)> predicate)
{
    return (predicate(str->front()) ^ predicate(str->back()));
}

std::string Tokenizer::PopFrontToken()
{
    auto token_to_retrieve = tokens_.empty() ? tokens::kEmpty : tokens_.front();
    if (!tokens_.empty())
    {
        tokens_.pop();
    }

    return token_to_retrieve;
}

std::string Tokenizer::AccumulateLineCharacters(std::string partial_token, char ch)
{
    if ((ch == tokens::kEquals.front()) ||
        IsReservedToken(partial_token))
    {
        FlushToTokens(&partial_token);
    }

    return partial_token + ch;
}

}
