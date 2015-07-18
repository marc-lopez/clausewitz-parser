#include "gtest/gtest.h"
#include "mocks/i_file_operations_mock.hpp"
#include "tokenizer.hpp"

using namespace testing;

class LibparserShould : public Test
{


};

TEST_F(LibparserShould, RecognizeTokens)
{
    auto fileOperationsMock = std::make_shared<IFileOperationsMock>();
    std::string content(R"(version="2.2")");

    EXPECT_CALL(*fileOperationsMock, Read("save.ck2")).
        WillOnce(Return(std::make_shared<std::istringstream>(content)));

    Tokenizer tokenizer("save.ck2", fileOperationsMock);

    EXPECT_EQ("version", tokenizer.GetNext());
    EXPECT_EQ("=", tokenizer.GetNext());
    EXPECT_EQ(R"("2.2")", tokenizer.GetNext());
}
