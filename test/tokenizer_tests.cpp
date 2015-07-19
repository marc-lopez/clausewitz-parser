#include <list>
#include <utility>
#include "gtest/gtest.h"
#include "test_case.hpp"
#include "mocks/i_file_operations_mock.hpp"
#include "tokenizer.hpp"

using namespace testing;

namespace libparser
{
namespace ut
{

class TokenizerShould : public TestCase, public TestWithParam<std::pair<std::string, std::list<std::string>>>
{
protected:

    virtual void SetUp()
    {
        file_operations_mock_ = std::make_shared<IFileOperationsMock>();
        tokenizer_ = std::make_shared<Tokenizer>(file_operations_mock_);
    }

    virtual void SetFileContents(std::string contents)
    {
        EXPECT_CALL(*file_operations_mock_, Read(kSampleFilename)).
            WillOnce(Return(std::make_shared<std::istringstream>(contents)));
    }

    virtual void ExpectTokenSequence(std::list<std::string> list)
    {
        for (auto token : list)
        {
            EXPECT_EQ(token, tokenizer_->GetNext());
        }
    }

    std::shared_ptr<IFileOperationsMock> file_operations_mock_;
    std::shared_ptr<Tokenizer> tokenizer_;
};

TEST_P(TokenizerShould, RecognizeTokens)
{
    auto param = GetParam();

    SetFileContents(param.first);

    tokenizer_->Read(kSampleFilename);

    ExpectTokenSequence(param.second);
}

INSTANTIATE_TEST_CASE_P(
    FromFileContents,
    TokenizerShould,
    Values(
        std::make_pair<std::string, std::list<std::string>>(
            R"(version="2.2")",
            {
                "version",
                "=",
                R"("2.2")"
            }
        ),
        std::make_pair<std::string, std::list<std::string>>(
R"(# Comment
version="2.2")",
            {
                "#",
                "Comment",
                "version",
                "=",
                R"("2.2")"
            }
        ),
        std::make_pair<std::string, std::list<std::string>>(
R"(name="of Constantinople")",
            {
                "name",
                "=",
                R"("of Constantinople")"
            }
        )
    ));

}
}
