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

TEST(test_list, constuctor_count_value) {
  s21::list<int> test(11, 567);
  std::list<int> test_std(11, 567);

  EXPECT_EQ(false, test.empty());
  EXPECT_EQ(test_std.empty(), test.empty());

  EXPECT_EQ(11, test.size());
  EXPECT_EQ(test_std.size(), test.size());

  EXPECT_EQ(sizeof(test), sizeof(test_std));

  // добавить проверку циклом
}

TEST(test_list, constuctor_count) {
  s21::list<int> test(11);
  std::list<int> test_std(11);

  EXPECT_EQ(false, test.empty());
  EXPECT_EQ(test_std.empty(), test.empty());

  EXPECT_EQ(11, test.size());
  EXPECT_EQ(test_std.size(), test.size());

  EXPECT_EQ(sizeof(test), sizeof(test_std));

  // добавить проверку циклом
}

TEST(test_list, constructor_range) {
  // std::vector<int> t = {1, 1, 11, 234, 12412321, 23};

  // s21::list<int> test(t.begin(), t.end());
  // std::list<int> test_std(t.begin(), t.end());

  // EXPECT_EQ(false, test.empty());
  // EXPECT_EQ(test_std.empty(), test.empty());

  // EXPECT_EQ(11, test.size());
  // EXPECT_EQ(test_std.size(), test.size());

  // EXPECT_EQ(sizeof(test), sizeof(test_std));
}