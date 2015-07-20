#include <list>
#include <memory>
#include <utility>
#include "gtest/gtest.h"
#include "test_case.hpp"
#include "mocks/i_tokenizer_mock.hpp"
#include "tokens.hpp"
#include "parser.hpp"

using namespace testing;

namespace libparser
{
namespace ut
{

class ParserShould : public TestCase, public Test
{
protected:
    virtual void SetUp()
    {
        tokens_sequence = Sequence();
        tokenizer_mock_ = std::make_shared<ITokenizerMock>();
        parser_ = std::make_shared<Parser>(tokenizer_mock_);

        EXPECT_CALL(*tokenizer_mock_, Read(kSampleFilename));
    }

    virtual void SetNextToken(const std::string& token)
    {
        EXPECT_CALL(*tokenizer_mock_, GetNext()).InSequence(tokens_sequence).WillOnce(Return(token));
    }

    virtual std::map<std::string, std::string> ParseFromTokens(std::list<std::string> tokens)
    {
        for (auto token : tokens)
        {
            SetNextToken(token);
        }
        SetNextToken(tokens::kEmpty);

        return parser_->Parse(kSampleFilename);
    }

    Sequence tokens_sequence;
    std::shared_ptr<ITokenizerMock> tokenizer_mock_;
    std::shared_ptr<Parser> parser_;
};

TEST_F(ParserShould, RecognizeKeyValuePair)
{
    auto data = ParseFromTokens({"version", tokens::kEquals, R"("2.2")"});

    EXPECT_EQ(R"("2.2")", data["version"]);
}

TEST_F(ParserShould, IgnoreComments)
{
    auto data = ParseFromTokens({
        "#", "version", tokens::kEquals, R"("2.2")", "\n",
        "date", tokens::kEquals, R"("1453.1.1")"});

    EXPECT_EQ(tokens::kEmpty, data["version"]);
    EXPECT_EQ(R"("1453.1.1")", data["date"]);
}

}
}
