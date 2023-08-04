#include <list>
#include <string>

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

  EXPECT_DOUBLE_EQ(567, test.front());
  EXPECT_DOUBLE_EQ(567, test.back());

  for (const int num : test) {
    EXPECT_EQ(567, num);
  }
}

TEST(test_list, constuctor_count) {
  s21::list<int> test(11);
  std::list<int> test_std(11);

  EXPECT_EQ(false, test.empty());
  EXPECT_EQ(test_std.empty(), test.empty());

  EXPECT_EQ(11, test.size());
  EXPECT_EQ(test_std.size(), test.size());

  EXPECT_EQ(sizeof(test), sizeof(test_std));

  EXPECT_DOUBLE_EQ(0, test.front());
  EXPECT_DOUBLE_EQ(0, test.back());

  for (const int& n : test) {
    EXPECT_EQ(0, n);
  }
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
  std::cout << "test.back() == " << test.back() << std::endl;
  EXPECT_DOUBLE_EQ(56.6542, test.back());

  EXPECT_DOUBLE_EQ(test_std.front(), test.front());
  EXPECT_DOUBLE_EQ(test_std.back(), test.back());

  std::list<double>::iterator it_std = test_std.begin();
  for (double& n : test) {
    EXPECT_DOUBLE_EQ(*it_std, n);
    ++it_std;
  }
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

  // EXPECT_DOUBLE_EQ(test_std_copy.front(), test_copy.front());
  // EXPECT_DOUBLE_EQ(test_std_copy.back(), test_copy.back());

  // std::list<double>::iterator it_std = test_std.begin();
  // for (double& n : test) {
  //   EXPECT_DOUBLE_EQ(*it_std, n);
  //   ++it_std;
  // }
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

  std::list<double>::iterator it_std = test_std_move.begin();
  for (double& n : test_move) {
    EXPECT_DOUBLE_EQ(*it_std, n);
    ++it_std;
  }
}

TEST(test_list, iterators) {
  {
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
  }
  {
    s21::list<double> test{1, 23, 53223, 543, 134234, 54, 56.6542};
    std::list<double> test_std{1, 23, 53223, 543, 134234, 54, 56.6542};

    std::list<double>::iterator it_std = test_std.end();
    s21::list<double>::iterator it_s21 = test.end();

    EXPECT_DOUBLE_EQ(56.6542, *(--it_std));
    EXPECT_DOUBLE_EQ(56.6542, *(--it_s21));
    // EXPECT_DOUBLE_EQ(*it_std, *it_s21);
  }
}

TEST(test_list, capacity) {
  {
    s21::list<double> test_d{1,     23,          53223,  543, 134234,
                             54,    56.6542,     583459, 0,   245,
                             23523, 23535.45244, 9.999};
    std::list<double> test_std_d{1,     23,          53223,  543, 134234,
                                 54,    56.6542,     583459, 0,   245,
                                 23523, 23535.45244, 9.999};

    EXPECT_FALSE(test_d.empty());
    EXPECT_EQ(test_std_d.empty(), test_d.empty());

    EXPECT_EQ(13, test_d.size());
    EXPECT_EQ(test_std_d.size(), test_d.size());

    EXPECT_EQ(test_std_d.max_size(), test_d.max_size());
  }
  {
    s21::list<int> test_i{1,      23, 53223, 543,   134234, 54, 56,
                          583459, 0,  245,   23523, 23535,  9};
    std::list<int> test_std_i{1,      23, 53223, 543,   134234, 54, 56,
                              583459, 0,  245,   23523, 23535,  9};

    EXPECT_FALSE(test_i.empty());
    EXPECT_EQ(test_std_i.empty(), test_i.empty());

    EXPECT_EQ(13, test_i.size());
    EXPECT_EQ(test_std_i.size(), test_i.size());

    EXPECT_EQ(test_std_i.max_size(), test_i.max_size());
  }
  {
    s21::list<unsigned long long int> test_ulli{
        1, 23, 53223, 543, 134234, 54, 56, 583459, 0, 245, 23523, 23535, 9};
    std::list<unsigned long long int> test_std_ulli{
        1, 23, 53223, 543, 134234, 54, 56, 583459, 0, 245, 23523, 23535, 9};

    EXPECT_FALSE(test_ulli.empty());
    EXPECT_EQ(test_std_ulli.empty(), test_ulli.empty());

    EXPECT_EQ(13, test_ulli.size());
    EXPECT_EQ(test_std_ulli.size(), test_ulli.size());

    EXPECT_EQ(test_std_ulli.max_size(), test_ulli.max_size());
  }
  {
    struct Test {
      int t1;
      double t2;
      size_t t3;
    };

    s21::list<Test> test_t{{1, 2.3, 5}, {1, 2.3, 5}, {1, 2.3, 5},
                           {1, 2.3, 5}, {1, 2.3, 5}, {1, 2.3, 5}};
    std::list<Test> test_std_t{{1, 2.3, 5}, {1, 2.3, 5}, {1, 2.3, 5},
                               {1, 2.3, 5}, {1, 2.3, 5}, {1, 2.3, 5}};

    EXPECT_FALSE(test_t.empty());
    EXPECT_EQ(test_std_t.empty(), test_t.empty());

    EXPECT_EQ(6, test_t.size());
    EXPECT_EQ(test_std_t.size(), test_t.size());

    EXPECT_EQ(test_std_t.max_size(), test_t.max_size());
  }
  {
    s21::list<std::string> test_s{"wfwefwe", "yjtynt", "sdfefwe"};
    std::list<std::string> test_std_s{"wfwefwe", "yjtynt", "sdfefwe"};

    EXPECT_FALSE(test_s.empty());
    EXPECT_EQ(test_std_s.empty(), test_s.empty());

    EXPECT_EQ(3, test_s.size());
    EXPECT_EQ(test_std_s.size(), test_s.size());

    EXPECT_EQ(test_std_s.max_size(), test_s.max_size());
  }
  {
    s21::list<char> test_ch;
    std::list<char> test_std_ch;

    EXPECT_TRUE(test_ch.empty());
    EXPECT_EQ(test_std_ch.empty(), test_ch.empty());

    EXPECT_EQ(0, test_ch.size());
    EXPECT_EQ(test_std_ch.size(), test_ch.size());

    EXPECT_EQ(test_std_ch.max_size(), test_ch.max_size());
  }
}

TEST(test_list, clear) {
  struct Test {
    int t1;
    double t2;
    size_t t3;
  };

  // s21::list<double> test{1, 23, 53223, 543, 134234, 54, 56.6542};
  std::list<double> test_std{1, 23, 53223, 543, 134234, 54, 56.6542};

  test_std.clear();
  double it = test_std.front();
  std::cout << it << std::endl;
}