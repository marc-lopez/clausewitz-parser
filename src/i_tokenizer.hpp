#ifndef I_TOKENIZER_HPP_
#define I_TOKENIZER_HPP_

#include <string>

class ITokenizer
{
public:
    virtual void Read(std::string) = 0;
	virtual std::string GetNext() = 0;
};

#endif
