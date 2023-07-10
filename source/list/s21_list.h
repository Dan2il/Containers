
#include <iostream>  // УДАЛИТЬ
#include <limits>
#include <memory>

namespace s21 {

template <typename Type>
struct Node {
  Type data;
  Node* next_node;
  Node* previous_node;
  long int num_node;

  Node() : next_node(nullptr), previous_node(nullptr) { num_node = -1; }
  Node(const Type& t) : Node() { data = t; };
};

template <typename Type, typename Alloc = std::allocator<Node<Type>>>
class list {
 public:
  explicit list() = default;
  list(const size_t count, const Type& value);
  explicit list(size_t count);

  template <class InputIt>
  list(InputIt first, InputIt last);

  //   list(const list& other);
  //   list(list&& other);
  //   list(std::initializer_list<T> init);

  ~list();

  // Capacity
  bool empty() const noexcept;
  size_t size() const noexcept;

  Type& front();
  Type& front() const;

  Type& back();
  Type& back() const;

 private:
  Alloc alloc_;
  Node<Type>* null_node_ = nullptr;
  size_t stored_ = 0;

  void LinkPointer(Node<Type>* for_link_next, Node<Type>* for_link_previous);
};

template <typename Type, typename Alloc>
list<Type, Alloc>::list(const size_t count, const Type& value) {
  null_node_ = alloc_.allocate(1);
  alloc_.construct(null_node_, value);

  Node<Type>* buffer_address = null_node_;
  for (; stored_ < count; ++stored_) {
    buffer_address->next_node = alloc_.allocate(1);
    alloc_.construct(buffer_address->next_node, value);
    LinkPointer(buffer_address, buffer_address->next_node);
    buffer_address = buffer_address->next_node;
    null_node_->previous_node = buffer_address;
  }
}

template <typename Type, typename Alloc>
list<Type, Alloc>::list(size_t count) {
  null_node_ = alloc_.allocate(1);
  alloc_.construct(null_node_);
  Node<Type>* buffer_address = null_node_;
  for (; stored_ < count; ++stored_) {
    buffer_address->next_node = alloc_.allocate(1);
    alloc_.construct(buffer_address->next_node);
    LinkPointer(buffer_address, buffer_address->next_node);
    buffer_address = buffer_address->next_node;
    null_node_->previous_node = buffer_address;
  }
}

template <typename Type, typename Alloc>
template <class InputIt>
s21::list<Type, Alloc>::list(InputIt first, InputIt last)
    : null_node_(alloc_.allocate(1)) {
  // null_node_ = alloc_.allocate(1);
  Node<Type>* buffer_address = null_node_;
  alloc_.construct(null_node_);
  for (; first != last; first++) {
    buffer_address->next_node = alloc_.allocate(1);
    alloc_.construct(buffer_address->next_node, *first);
    LinkPointer(buffer_address, buffer_address->next_node);
    buffer_address = buffer_address->next_node;
    null_node_->previous_node = buffer_address;
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

template <typename Type, typename Alloc>
Type& s21::list<Type, Alloc>::front() {
  return null_node_->next_node->data;
}

template <typename Type, typename Alloc>
Type& s21::list<Type, Alloc>::front() const {
  return null_node_->next_node->data;
}

template <typename Type, typename Alloc>
Type& s21::list<Type, Alloc>::back() {
  return null_node_->previous_node->data;
}

template <typename Type, typename Alloc>
Type& s21::list<Type, Alloc>::back() const {
  return null_node_->previous_node->data;
}

template <typename Type, typename Alloc>
void s21::list<Type, Alloc>::LinkPointer(Node<Type>* for_link_next,
                                         Node<Type>* for_link_previous) {
  for_link_next->next_node = for_link_previous;
  for_link_previous->previous_node = for_link_next;
}

}  // namespace s21