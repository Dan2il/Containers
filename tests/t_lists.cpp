#include <list>

#include "test.h"

TEST(test_list, default_constuctor) {
  s21::list<int> test;
  std::list<int> test_std;

  EXPECT_EQ(true, test.empty());
  EXPECT_EQ(test_std.empty(), test.empty());

  EXPECT_EQ(0, test.size());
  EXPECT_EQ(test_std.size(), test.size());

  EXPECT_EQ(sizeof(test), sizeof(test_std));
}
