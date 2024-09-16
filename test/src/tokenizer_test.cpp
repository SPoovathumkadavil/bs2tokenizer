#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "tokenizer/tokenizer.hpp"

TEST(TokenizerTests, CreateTokenizer)
{
  tokenizer *t = new tokenizer;
  TModuleRec *r = new TModuleRec;
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
