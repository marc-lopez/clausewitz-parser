#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include "tokens.hpp"
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

std::string Tokenizer::PopFrontToken()
{
    auto token_to_retrieve = tokens_.empty() ? tokens::kEmpty : tokens_.front();
    if (!tokens_.empty())
    {
        tokens_.pop();
    }

    return token_to_retrieve;
}

void Tokenizer::GetTokens()
{
    contents_ = file_operations_->Read(filename_);

    std::string segment;
    std::string partial_token;

    while (std::getline(*contents_, segment))
    {
        auto last_token_in_segment = std::accumulate(segment.begin(), segment.end(), std::string(),
                                                     [this](std::string partial_token, char ch) {
                                                        return this->AccumulateLineCharacters(partial_token, ch);});

        partial_token += (partial_token.empty() ? last_token_in_segment : " " + last_token_in_segment);
        TryFlushTokens(&partial_token);

        tokens_.push("\n");
    }
}

void Tokenizer::TryFlushTokens(std::string * partial_token)
{
    if(!XorOfEnds(partial_token, [this](const char &ch){return this->IsQuotes(ch);}) &&
        (!partial_token->empty()))
    {
        FlushToTokens(partial_token);
    }
}

void Tokenizer::FlushToTokens(std::string * partial_token)
{
    tokens_.push(*partial_token);
    partial_token->clear();
}

bool Tokenizer::IsQuotes(const char &ch)
{
    return IsToken(ch, tokens::kQuotes);
}

bool Tokenizer::IsReservedToken(std::string partial_token)
{
    return (std::find(
            Tokenizer::kReservedTokens.begin(),
            Tokenizer::kReservedTokens.end(),
            partial_token) !=
        Tokenizer::kReservedTokens.end());
}

inline bool Tokenizer::IsToken(const char& ch, const std::string& token)
{
    return (ch == token.front());
}

inline bool Tokenizer::XorOfEnds(std::string * str, std::function<bool(const char &)> predicate)
{
    return (predicate(str->front()) ^ predicate(str->back()));
}

std::string Tokenizer::AccumulateLineCharacters(std::string partial_token, char ch)
{
    PushTokenIfValid(partial_token, ch);

    if (IsToken(ch, tokens::kEmpty) ||
        ch == '\r' ||
        !IsPartOfQuotedToken(ch, partial_token))
    {
        return partial_token;
    }

    return partial_token + ch;
}

void Tokenizer::PushTokenIfValid(std::string& partial_token, const char& ch)
{
    if (IsToken(ch, tokens::kEquals) ||
        IsReservedToken(partial_token))
    {
        FlushToTokens(&partial_token);
    }
}

inline bool Tokenizer::IsPartOfQuotedToken(const char& ch, const std::string& partial_token)
{
    return ((ch != ' ') || IsQuotes(partial_token.front()));
}

}
