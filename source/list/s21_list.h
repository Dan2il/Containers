
#include <iostream>  // УДАЛИТЬ

// #include "../s21_containers.h"

namespace s21 {

template <typename Type>
struct Node {
  Node* next_node;
  Node* previous_node;
  Type data;

  Node() : next_node(nullptr), previous_node(nullptr), data() {}
  explicit Node(const Type& t) : Node() { data = t; };
};

template <typename Type, typename Alloc = std::allocator<Node<Type>>>
class list {
 public:
  class list_iterator : public std::iterator<std::forward_iterator_tag, Type> {
    friend class list;

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

  list();
  explicit list(size_t count);
  list(const size_t count, const Type& value);
  explicit list(std::initializer_list<Type> const& items);
  list(const list<Type, Alloc>& other);
  list(list&& other) noexcept;

  ~list();

  // Capacity
  bool empty() const noexcept;
  size_t size() const noexcept;
  size_t max_size() const noexcept;

  Type& front();
  const Type& front() const;

  Type& back();
  const Type& back() const;

  // Iterators

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  // Modifiers
  void clear();

  iterator insert(iterator pos, const Type& value);

  void push_front(const Type& value);
  void push_back(const Type& value);

  void pop_front();
  void pop_back();

  // void swap(list& other);
  // void merge(list& other);
  // void splice(const_iterator pos, list& other);
  // void reverse();
  // void unique();
  // void sort();

 private:
  Alloc alloc_;
  Node<Type>* end_node_ = nullptr;
  size_t stored_ = 0;

  Node<Type>* CreateNode();
  Node<Type>* CreateNode(const Type& data);

  void FreeNode(Node<Type>* node);

  void LinkPointer(Node<Type>* for_link_next, Node<Type>* for_link_previous);
};

template <typename Type, typename Alloc>
list<Type, Alloc>::list() : end_node_(CreateNode()) {}

template <typename Type, typename Alloc>
list<Type, Alloc>::list(size_t count) : list() {
  Node<Type>* buffer_address = end_node_;
  for (; stored_ < count; ++stored_) {
    Node<Type>* push = CreateNode();
    buffer_address->next_node = push;
    push->previous_node = buffer_address;
    buffer_address = buffer_address->next_node;
  }
}

template <typename Type, typename Alloc>
list<Type, Alloc>::list(const size_t count, const Type& value) : list() {
  // Node<Type>* buffer_address = end_node_;
  for (; stored_ < count;) {
    push_back(value);
    // Node<Type>* push = CreateNode(value);
    // buffer_address->next_node = push;
    // push->previous_node = buffer_address;
    // buffer_address = buffer_address->next_node;
  }
}

template <typename Type, typename Alloc>
s21::list<Type, Alloc>::list(std::initializer_list<Type> const& items)
    : list() {
  Node<Type>* buffer_address = end_node_;
  for (auto it = items.begin(); it != items.end(); it++) {
    buffer_address->next_node = CreateNode(*it);
    buffer_address->next_node->previous_node = buffer_address;
    buffer_address = buffer_address->next_node;
    ++stored_;
  }
  buffer_address->next_node = CreateNode();
  buffer_address->next_node->previous_node = buffer_address;
  end_node_->previous_node = buffer_address->next_node;
}

template <typename Type, typename Alloc>
s21::list<Type, Alloc>::list(const list<Type, Alloc>& other) : list() {
  Node<Type>* buffer_address = end_node_;
  for (const Node<Type>* node = other.null_node_->next_node; node != nullptr;
       node = node->next_node) {
    buffer_address->next_node = CreateNode(node->data);
    buffer_address->next_node->previous_node = buffer_address;
    buffer_address = buffer_address->next_node;
  }
  stored_ = other.stored_;
  end_node_->previous_node = buffer_address;
}

template <typename Type, typename Alloc>
s21::list<Type, Alloc>::list(list&& other) noexcept {
  end_node_ = other.null_node_;
  other.null_node_ = nullptr;
  stored_ = std::move(other.stored_);
}

template <typename Type, typename Alloc>
list<Type, Alloc>::~list() {
  // Node<Type>* buffer_address = null_node_;
  if (end_node_) {
    Node<Type>* end = end_node_;
    for (Node<Type>* node = end_node_->next_node;
         node != nullptr && node != end;) {
      Node<Type>* dealloc = node;
      node = node->next_node;
      FreeNode(dealloc);
    }
    if (end) {
      FreeNode(end);
    }
  }
  stored_ = 0;
  end_node_ = nullptr;
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
  if (std::numeric_limits<typename Alloc::size_type>::max() /
          sizeof(Node<Type>) >
      alloc_.max_size()) {
    return alloc_.max_size();
  }
  return std::numeric_limits<typename Alloc::size_type>::max() /
         sizeof(Node<Type>);
}

template <typename Type, typename Alloc>
Type& s21::list<Type, Alloc>::front() {
  if (!end_node_->next_node) {
    return end_node_->data;
  }
  return end_node_->next_node->data;
}

template <typename Type, typename Alloc>
const Type& s21::list<Type, Alloc>::front() const {
  if (!end_node_->next_node) {
    return end_node_->data;
  }
  return end_node_->next_node->data;
}

template <typename Type, typename Alloc>
Type& s21::list<Type, Alloc>::back() {
  // assert(end_node_->previous_node != nullptr);
  if (!end_node_->previous_node) {
    return end_node_->data;
  }
  return end_node_->previous_node->data;
}

template <typename Type, typename Alloc>
const Type& s21::list<Type, Alloc>::back() const {
  if (!end_node_->previous_node) {
    return end_node_->data;
  }
  return end_node_->previous_node->data;
}

// Modifiers

template <typename Type, typename Alloc>
void s21::list<Type, Alloc>::clear() {
  if (stored_ > 0) {
    pop_front();
    clear();
  }
}

template <typename Type, typename Alloc>
typename s21::list<Type, Alloc>::iterator s21::list<Type, Alloc>::insert(
    iterator pos, const Type& value) {
  Node<Type>* new_node = CreateNode(value);
  Node<Type>* pos_node = pos.link_node_;
  Node<Type>* next_pos_node = pos_node->next_node;

  new_node->next_node = next_pos_node;
  new_node->previous_node = pos_node;

  pos_node->next_node = new_node;
  next_pos_node->previous_node = new_node;

  ++stored_;
  return iterator(this, new_node);
}

template <typename Type, typename Alloc>
void s21::list<Type, Alloc>::push_back(const Type& value) {
  assert(end_node_ != nullptr);
  Node<Type>* push = CreateNode(value);
  assert(push != nullptr);
  if (!stored_) {
    end_node_->previous_node = push;
    end_node_->next_node = push;

    push->next_node = end_node_;
    push->previous_node = end_node_;
  } else {
    Node<Type>* prev_end = end_node_->previous_node;

    prev_end->next_node = push;
    push->next_node = end_node_;

    end_node_->previous_node = push;
    push->previous_node = prev_end;
  }
  ++stored_;
}

template <typename Type, typename Alloc>
void s21::list<Type, Alloc>::pop_front() {
  if (end_node_->next_node != end_node_->previous_node) {
    Node<Type>* del_node = end_node_->next_node;

    end_node_->next_node = del_node->next_node;
    end_node_->next_node->previous_node = end_node_;

    FreeNode(del_node);

    --stored_;
  }
}

// мысль: сделать реверс и использовать pop_front
template <typename Type, typename Alloc>
void s21::list<Type, Alloc>::pop_back() {
  if (end_node_->next_node != end_node_->previous_node) {
    Node<Type>* end_node = end_node_->previous_node;
    Node<Type>* del_node = end_node->previous_node;
    end_node->previous_node = del_node->previous_node;
    end_node->previous_node->next_node = del_node->next_node;
    FreeNode(del_node);
    --stored_;
  }
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
void s21::list<Type, Alloc>::FreeNode(Node<Type>* node) {
  if (!node) {
    throw std::invalid_argument("destroy nullptr node");
  }
  alloc_.destroy(node);
  alloc_.deallocate(node, 1);
}

template <typename Type, typename Alloc>
void s21::list<Type, Alloc>::LinkPointer(Node<Type>* for_link_next,
                                         Node<Type>* for_link_previous) {
  for_link_next->next_node = for_link_previous;
  for_link_previous->previous_node = for_link_next;
}

// Iterators

template <typename Type, typename Alloc>
typename list<Type, Alloc>::iterator s21::list<Type, Alloc>::begin() {
  if (!stored_) {
    return iterator(this, end_node_);
  }
  return iterator(this, end_node_->next_node);
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::const_iterator s21::list<Type, Alloc>::begin()
    const {
  if (!stored_) {
    return iterator(this, end_node_);
  }
  return const_iterator(this, end_node_->next_node);
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::iterator s21::list<Type, Alloc>::end() {
  return iterator(this, end_node_->previous_node);
}

template <typename Type, typename Alloc>
typename list<Type, Alloc>::const_iterator s21::list<Type, Alloc>::end() const {
  return const_iterator(this, end_node_->previous_node);
}

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