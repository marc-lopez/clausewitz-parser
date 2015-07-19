#include <list>
#include <memory>
#include <utility>
#include "gtest/gtest.h"
#include "mocks/i_tokenizer_mock.hpp"
#include "parser.hpp"

using namespace testing;

namespace libparser
{
namespace ut
{


class ParserShould : public Test
{
protected:
    virtual void SetUp()
    {
        tokens_sequence = Sequence();
        tokenizer_mock_ = std::make_shared<ITokenizerMock>();
    }

    virtual void SetNextToken(std::string token)
    {
        EXPECT_CALL(*tokenizer_mock_, GetNext()).InSequence(tokens_sequence).WillOnce(Return(token));
    }

    Sequence tokens_sequence;
    std::shared_ptr<ITokenizerMock> tokenizer_mock_;
};

TEST_F(ParserShould, RecognizeKeyValuePair)
{
    auto parser = std::make_shared<Parser>(tokenizer_mock_);

    EXPECT_CALL(*tokenizer_mock_, Read("save.ck2"));

    for(std::string token : std::list<std::string> {"version", "=", R"("2.2")"})
    {
        SetNextToken(token);
    }
    SetNextToken("");

    auto data = parser->Parse("save.ck2");

    EXPECT_EQ(R"("2.2")", data["version"]);
}

}
}
