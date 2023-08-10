#ifndef DQUEUE_H_
#define DQUEUE_H_
#include "../s21_containers.h"

template <typename T>
class Queue {
 public:
  class iterator;
  friend class iterator;

 private:
  class node;
  friend class node;
  class node {};
};

#endif