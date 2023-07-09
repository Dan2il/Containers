
#include <iostream>  // УДАЛИТЬ
#include <limits>
#include <memory>

namespace s21 {

template <typename Type>
struct Node {
  Type data;
  Node* next_node;
  long int num_node;

  Node() : next_node(nullptr) { num_node = -1; }
  Node(const Type& t) : Node() { data = t; };
};

template <typename Type, typename Alloc = std::allocator<Node<Type>>>
class list {
 public:
  explicit list() = default;
  list(const size_t count, const Type& value);
  // explicit list(size_type count);

  //   template <class InputIt>
  //   list(InputIt first, InputIt last);

  //   list(const list& other);
  //   list(list&& other);
  //   list(std::initializer_list<T> init);

  ~list();

  // Capacity
  bool empty() const noexcept;
  size_t size() const noexcept;

 private:
  Alloc alloc_;
  Node<Type>* null_node_ = nullptr;
  size_t stored_ = 0;
};

template <typename Type, typename Alloc>
list<Type, Alloc>::list(const size_t count, const Type& value) {
  null_node_ = alloc_.allocate(1);
  alloc_.construct(null_node_, value);

  Node<Type>* buffer_address = null_node_;
  for (size_t counter = 0; counter < count; ++counter) {
    Node<Type>* push = nullptr;
    push = alloc_.allocate(1);
    alloc_.construct(push, value);
    buffer_address->next_node = push;
    buffer_address = push;
    ++stored_;
  }
}

template <typename Type, typename Alloc>
list<Type, Alloc>::~list() {
  Node<Type>* buffer_address = null_node_;
  while (buffer_address) {
    Node<Type>* dealloc = buffer_address;
    buffer_address = dealloc->next_node;
    alloc_.destroy(dealloc);
    alloc_.deallocate(dealloc, 1);
  }
}

// Capacity
template <typename Type, typename Alloc>
bool s21::list<Type, Alloc>::empty() const noexcept {
  return !stored_;
}

template <typename Type, typename Alloc>
size_t s21::list<Type, Alloc>::size() const noexcept {
  return stored_;
}

}  // namespace s21