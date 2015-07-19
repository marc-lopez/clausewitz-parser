#ifndef I_TOKENIZER_MOCK_HPP
#define I_TOKENIZER_MOCK_HPP

#include "gmock/gmock.h"
#include "i_tokenizer.hpp"

class ITokenizerMock : public ITokenizer
{
public:
    MOCK_METHOD1(Read, void(std::string));
    MOCK_METHOD0(GetNext, std::string());
};

#endif
