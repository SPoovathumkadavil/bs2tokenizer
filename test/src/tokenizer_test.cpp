#include <gtest/gtest.h>
#include <filesystem>
#include <string>

TEST(LibTests, FileExistsTest)
{
  ASSERT_TRUE(std::filesystem::exists("resources/test.txt"));
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
