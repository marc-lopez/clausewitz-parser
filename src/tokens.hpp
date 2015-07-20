#ifndef RESERVED_TOKENS_HPP_
#define RESERVED_TOKENS_HPP_

#include <string>

namespace libparser
{
namespace tokens
{
    constexpr char kEmpty[] = "";
    constexpr char kComment[] = "#";
    constexpr char kEquals[] = "=";
    constexpr char kSentinel[] = "$";
    constexpr char kQuotes[] = "\"";

}
}
#endif // RESERVED_TOKENS_HPP_
