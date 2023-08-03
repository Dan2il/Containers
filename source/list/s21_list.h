
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

  Node() : data(), next_node(nullptr), previous_node(nullptr) { num_node = -1; }
  Node(const Type& t) : Node() { data = t; };
};

template <typename Type, typename Alloc = std::allocator<Node<Type>>>
class list {
 public:
  class list_iterator : public std::iterator<std::forward_iterator_tag, Type> {
   public:
    list_iterator();
    list_iterator(const list<Type>* list, Node<Type>* node);

    Type& operator*();

    bool operator!=(const list_iterator& other_it);
    bool operator==(const list_iterator& other_it);

    list_iterator operator++();
    list_iterator operator++(int);

    list_iterator operator--();
    list_iterator operator--(int);

   private:
    const list<Type>* link_list_;
    Node<Type>* link_node_;
  };

  typedef list_iterator iterator;
  typedef const list_iterator const_iterator;

  explicit list() = default;
  list(const size_t count, const Type& value);
  explicit list(size_t count);
  list(std::initializer_list<Type> const& items);
  list(const list<Type, Alloc>& other);
  list(list&& other) noexcept;

  ~list();

  // Capacity
  bool empty() const noexcept;
  size_t size() const noexcept;
  size_t max_size() const noexcept;

  Type& front();
  Type& front() const;

  Type& back();
  Type& back() const;

  // Iterators

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  // Modifiers
  void clear();

 private:
  Alloc alloc_;
  Node<Type>* null_node_ = nullptr;
  size_t stored_ = 0;

  Node<Type>* CreateNode();
  Node<Type>* CreateNode(const Type& data);

  void LinkPointer(Node<Type>* for_link_next, Node<Type>* for_link_previous);
};

template <typename Type, typename Alloc>
list<Type, Alloc>::list(const size_t count, const Type& value) {
  null_node_ = CreateNode();
  Node<Type>* buffer_address = null_node_;
  for (; stored_ < count;
       ++stored_, buffer_address = buffer_address->next_node) {
    buffer_address->next_node = CreateNode(value);
    LinkPointer(buffer_address, buffer_address->next_node);
  }
  null_node_->previous_node = buffer_address;
}

template <typename Type, typename Alloc>
list<Type, Alloc>::list(size_t count) {
  null_node_ = CreateNode();
  Node<Type>* buffer_address = null_node_;
  for (; stored_ < count;
       ++stored_, buffer_address = buffer_address->next_node) {
    buffer_address->next_node = CreateNode();
    LinkPointer(buffer_address, buffer_address->next_node);
  }
  null_node_->previous_node = buffer_address;
}

template <typename Type, typename Alloc>
s21::list<Type, Alloc>::list(std::initializer_list<Type> const& items) {
  null_node_ = CreateNode();
  Node<Type>* buffer_address = null_node_;
  for (auto it = items.begin(); it != items.end();
       it++, buffer_address = buffer_address->next_node) {
    buffer_address->next_node = CreateNode(*it);
    LinkPointer(buffer_address, buffer_address->next_node);
    ++stored_;
  }
  null_node_->previous_node = buffer_address;
}

template <typename Type, typename Alloc>
s21::list<Type, Alloc>::list(const list<Type, Alloc>& other) {
  null_node_ = CreateNode();
  Node<Type>* buffer_address = null_node_;
  for (const Node<Type>* node = other.null_node_->next_node; node != nullptr;
       node = node->next_node, buffer_address = buffer_address->next_node) {
    buffer_address->next_node = CreateNode(node->data);
    LinkPointer(buffer_address, buffer_address->next_node);
  }
  stored_ = other.stored_;
  null_node_->previous_node = buffer_address;
}

template <typename Type, typename Alloc>
s21::list<Type, Alloc>::list(list&& other) noexcept {
  null_node_ = other.null_node_;
  other.null_node_ = nullptr;
  stored_ = std::move(other.stored_);
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
size_t s21::list<Type, Alloc>::max_size() const noexcept {
  return alloc_.max_size();
  // return std::numeric_limits<Type>::max();
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
Node<Type>* s21::list<Type, Alloc>::CreateNode() {
  Node<Type>* for_create = alloc_.allocate(1);
  alloc_.construct(for_create);
  return for_create;
}

template <typename Type, typename Alloc>
Node<Type>* s21::list<Type, Alloc>::CreateNode(const Type& data) {
  Node<Type>* for_create = alloc_.allocate(1);
  alloc_.construct(for_create, data);
  return for_create;
}

template <typename Type, typename Alloc>
void s21::list<Type, Alloc>::LinkPointer(Node<Type>* for_link_next,
                                         Node<Type>* for_link_previous) {
  for_link_next->next_node = for_link_previous;
  for_link_previous->previous_node = for_link_next;
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::iterator s21::list<Type, Alloc>::begin() {
  return iterator(this, null_node_->next_node);
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::const_iterator s21::list<Type, Alloc>::begin()
    const {
  return const_iterator(this, null_node_->next_node);
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::iterator s21::list<Type, Alloc>::end() {
  return iterator(this, null_node_->previous_node);
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::const_iterator s21::list<Type, Alloc>::end() const {
  return const_iterator(this, null_node_->previous_node);
}

// Iterators

template <typename Type, typename Alloc>
s21::list<Type, Alloc>::iterator::list_iterator()
    : link_list_(nullptr), link_node_(nullptr) {}

template <typename Type, typename Alloc>
list<Type, Alloc>::iterator::list_iterator(const list<Type>* list,
                                           Node<Type>* node)
    : link_list_(list), link_node_(node) {}

template <typename Type, typename Alloc>
Type& list<Type, Alloc>::iterator::operator*() {
  return link_node_->data;
}

template <typename Type, typename Alloc>
bool list<Type, Alloc>::list_iterator::operator!=(
    const list_iterator& other_it) {
  return !(*this == other_it);
}

template <typename Type, typename Alloc>
bool list<Type, Alloc>::list_iterator::operator==(
    const list_iterator& other_it) {
  return link_list_ == other_it.link_list_ && link_node_ == other_it.link_node_;
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::list_iterator
list<Type, Alloc>::list_iterator::operator++() {
  if (link_node_ != nullptr) {
    link_node_ = link_node_->next_node;
  }
  return *this;
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::list_iterator
list<Type, Alloc>::list_iterator::operator++(int) {
  list<Type, Alloc>::iterator copy(link_list_, link_node_);
  if (link_node_) {
    link_node_ = link_node_->next_node;
  }
  return copy;
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::list_iterator
list<Type, Alloc>::list_iterator::operator--() {
  if (link_node_ != nullptr) {
    link_node_ = link_node_->previous_node;
  }
  return *this;
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::list_iterator
list<Type, Alloc>::list_iterator::operator--(int) {
  list<Type, Alloc>::iterator copy(link_list_, link_node_);
  if (link_node_) {
    link_node_ = link_node_->previous_node;
  }
  return copy;
}

}  // namespace s21