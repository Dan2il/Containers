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

TEST(test_list, constructor_initializer_list) {
  s21::list<double> test{1, 23, 53223, 543, 134234, 54, 56.6542};
  std::list<double> test_std{1, 23, 53223, 543, 134234, 54, 56.6542};

  EXPECT_EQ(false, test.empty());
  EXPECT_EQ(test_std.empty(), test.empty());

  EXPECT_EQ(7, test.size());
  EXPECT_EQ(test_std.size(), test.size());

  EXPECT_EQ(sizeof(test), sizeof(test_std));

  EXPECT_DOUBLE_EQ(1, test.front());
  EXPECT_DOUBLE_EQ(56.6542, test.back());

  EXPECT_DOUBLE_EQ(test_std.front(), test.front());
  EXPECT_DOUBLE_EQ(test_std.back(), test.back());

  // добавить проверку циклом
}

TEST(test_list, constructor_copy) {
  s21::list<double> test{1, 23, 53223, 543, 134234, 54, 56.6542};
  std::list<double> test_std{1, 23, 53223, 543, 134234, 54, 56.6542};

  s21::list<double> test_copy(test);
  std::list<double> test_std_copy(test_std);

  EXPECT_EQ(false, test_copy.empty());
  EXPECT_EQ(test_std_copy.empty(), test_copy.empty());

  EXPECT_EQ(7, test_copy.size());
  EXPECT_EQ(test_std_copy.size(), test_copy.size());

  EXPECT_DOUBLE_EQ(1, test_copy.front());
  EXPECT_DOUBLE_EQ(56.6542, test_copy.back());

  EXPECT_DOUBLE_EQ(test_std_copy.front(), test_copy.front());
  EXPECT_DOUBLE_EQ(test_std_copy.back(), test_copy.back());

  // добавить проверку циклом
}

TEST(test_list, constructor_move) {
  s21::list<double> test{1, 23, 53223, 543, 134234, 54, 56.6542};
  std::list<double> test_std{1, 23, 53223, 543, 134234, 54, 56.6542};

  s21::list<double> test_move(std::move(test));
  std::list<double> test_std_move(std::move(test_std));

  EXPECT_EQ(false, test_move.empty());
  EXPECT_EQ(test_std_move.empty(), test_move.empty());

  EXPECT_EQ(7, test_move.size());
  EXPECT_EQ(test_std_move.size(), test_move.size());

  EXPECT_DOUBLE_EQ(1, test_move.front());
  EXPECT_DOUBLE_EQ(56.6542, test_move.back());

  EXPECT_DOUBLE_EQ(test_std_move.front(), test_move.front());
  EXPECT_DOUBLE_EQ(test_std_move.back(), test_move.back());

  // добавить проверку циклом
}

TEST(test_list, iterators_begin) {
  s21::list<double> test{1, 23, 53223, 543, 134234, 54, 56.6542};
  std::list<double> test_std{1, 23, 53223, 543, 134234, 54, 56.6542};

  std::list<double>::iterator it_std = test_std.begin();
  s21::list<double>::iterator it_s21 = test.begin();

  EXPECT_DOUBLE_EQ(1, *it_std);
  EXPECT_DOUBLE_EQ(1, *it_s21);
  EXPECT_DOUBLE_EQ(*it_std, *it_s21);

  EXPECT_DOUBLE_EQ(23, *(++it_std));
  EXPECT_DOUBLE_EQ(23, *(++it_s21));
  EXPECT_DOUBLE_EQ(*it_std, *it_s21);

  EXPECT_DOUBLE_EQ(23, *(it_std++));
  EXPECT_DOUBLE_EQ(23, *(it_s21++));
  EXPECT_DOUBLE_EQ(*it_std, *it_s21);

  EXPECT_DOUBLE_EQ(53223, *(it_std));
  EXPECT_DOUBLE_EQ(53223, *(it_s21));
  EXPECT_DOUBLE_EQ(*it_std, *it_s21);

  EXPECT_DOUBLE_EQ(23, *(--it_std));
  EXPECT_DOUBLE_EQ(23, *(--it_s21));
  EXPECT_DOUBLE_EQ(*it_std, *it_s21);

  EXPECT_DOUBLE_EQ(23, *(it_std--));
  EXPECT_DOUBLE_EQ(23, *(it_s21--));
  EXPECT_DOUBLE_EQ(*it_std, *it_s21);

  EXPECT_DOUBLE_EQ(1, *(it_std));
  EXPECT_DOUBLE_EQ(1, *(it_s21));
  EXPECT_DOUBLE_EQ(*it_std, *it_s21);

  for (double it : test) {
    EXPECT_DOUBLE_EQ(it, *it_s21);
    ++it_s21;
  }

  it_s21 = test.begin();
  for (const double& it : test) {
    EXPECT_DOUBLE_EQ(it, *it_s21);
    ++it_s21;
  }

  // добавить проверку циклом
}