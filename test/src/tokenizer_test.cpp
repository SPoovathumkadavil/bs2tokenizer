#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "tokenizer/tokenizer.hpp"

TEST(LibTests, FileExistsTest)
{
  ASSERT_TRUE(std::filesystem::exists("resources/test.txt"));
}

TEST(TokenizerTests, CreateTokenizer)
{
  ASSERT_NO_THROW(tokenizer{});
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
