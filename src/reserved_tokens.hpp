#ifndef RESERVED_TOKENS_HPP_
#define RESERVED_TOKENS_HPP_

#include <string>

namespace libparser
{
namespace tokens
{
    const std::string kEmpty("");
    const std::string kComment("#");
    const std::string kEquals("=");
    const std::string kSentinel("$");
    const std::string kQuotes(R"(")");

}
}
#endif // RESERVED_TOKENS_HPP_
