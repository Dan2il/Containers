
#include <iostream>  // УДАЛИТЬ
#include <memory>

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
  //   explicit list(size_type count);

  //   template <class InputIt>
  //   list(InputIt first, InputIt last);

  //   list(const list& other);
  //   list(list&& other);
  //   list(std::initializer_list<T> init);

 private:
  Alloc alloc_;
  Node<Type>* null_node = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};

template <typename Type, typename Alloc>
list<Type, Alloc>::list(const size_t count, const Type& value) {
  null_node = alloc_.allocate(1);
  alloc_.construct(null_node, value);
  alloc_.destroy(null_node);
  alloc_.deallocate(null_node, 1);
  null_node = nullptr;
  //   std::cout << "null_node == " << null_node->num_node << std::endl;
}