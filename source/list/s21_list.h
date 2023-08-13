
#include <iostream>  // УДАЛИТЬ
#include <utility>

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

template <typename Type>
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
  list(const list<Type>& other);
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
  Node<Type>* end_node_ = nullptr;
  size_t stored_ = 0;

  Node<Type>* CreateNode();
  Node<Type>* CreateNode(const Type& data);

  void FreeNode(Node<Type>* node);

  void LinkPointer(Node<Type>* for_link_next, Node<Type>* for_link_previous);
};

template <typename Type>
list<Type>::list() : end_node_(CreateNode()) {}

template <typename Type>
list<Type>::list(size_t count) : list() {
  for (; stored_ < count;) {
    push_back(Type());
  }
}

template <typename Type>
list<Type>::list(const size_t count, const Type& value) : list() {
  for (; stored_ < count;) {
    push_back(value);
  }
}

template <typename Type>
s21::list<Type>::list(std::initializer_list<Type> const& items) : list() {
  for (const Type& it : items) {
    push_back(it);
  }
}

template <typename Type>
s21::list<Type>::list(const list<Type>& other) : list() {
  for (const Type& it : other) {
    push_back(it);
  }
}

template <typename Type>
s21::list<Type>::list(list&& other) noexcept {
  end_node_ = std::exchange(other.end_node_, nullptr);
  stored_ = std::exchange(other.stored_, 0);
}

template <typename Type>
list<Type>::~list() {
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
template <typename Type>
bool s21::list<Type>::empty() const noexcept {
  return !stored_;
}

template <typename Type>
size_t s21::list<Type>::size() const noexcept {
  return stored_;
}

template <typename Type>
size_t s21::list<Type>::max_size() const noexcept {
  // if (std::numeric_limits<typename Alloc::size_type>::max() /
  //         sizeof(Node<Type>) >
  //     alloc_.max_size()) {
  //   return alloc_.max_size();
  // }
  return std::numeric_limits<size_t>::max() / sizeof(Node<Type>);
}

template <typename Type>
Type& s21::list<Type>::front() {
  if (!end_node_->next_node) {
    return end_node_->data;
  }
  return end_node_->next_node->data;
}

template <typename Type>
const Type& s21::list<Type>::front() const {
  if (!end_node_->next_node) {
    return end_node_->data;
  }
  return end_node_->next_node->data;
}

template <typename Type>
Type& s21::list<Type>::back() {
  if (!end_node_->previous_node) {
    return end_node_->data;
  }
  return end_node_->previous_node->data;
}

template <typename Type>
const Type& s21::list<Type>::back() const {
  if (!end_node_->previous_node) {
    return end_node_->data;
  }
  return end_node_->previous_node->data;
}

// Modifiers

template <typename Type>
void s21::list<Type>::clear() {
  if (stored_ && end_node_) {
    pop_front();
    clear();
  }
}

template <typename Type>
typename s21::list<Type>::iterator s21::list<Type>::insert(iterator pos,
                                                           const Type& value) {
  // if (pos.link_list_ != this) {
  //   throw std::invalid_argument("Incorrect list");
  // }
  Node<Type>* new_node = CreateNode(value);

  if (!stored_) {
    end_node_->next_node = new_node;
    new_node->next_node = end_node_;

    end_node_->previous_node = new_node;
    new_node->previous_node = end_node_;
  } else {
    Node<Type>* pos_node = pos.link_node_;
    Node<Type>* prev_node = pos_node->previous_node;

    prev_node->next_node = new_node;
    new_node->next_node = pos_node;

    pos_node->previous_node = new_node;
    new_node->previous_node = prev_node;
  }
  ++stored_;
  return iterator(this, new_node);
}

template <typename Type>
void s21::list<Type>::push_back(const Type& value) {
  Node<Type>* push = CreateNode(value);
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

template <typename Type>
void s21::list<Type>::push_front(const Type& value) {
  Node<Type>* push = CreateNode(value);
  if (!stored_) {
    end_node_->next_node = push;
    end_node_->previous_node = push;

    push->previous_node = end_node_;
    push->next_node = end_node_;
  } else {
    Node<Type>* buf_first_node = end_node_->next_node;

    push->next_node = buf_first_node;
    push->previous_node = end_node_;

    buf_first_node->previous_node = push;
    end_node_->next_node = push;
  }
}

template <typename Type>
void s21::list<Type>::pop_front() {
  if (end_node_ && stored_) {
    if (stored_ > 1) {
      Node<Type>* del_node = end_node_->next_node;
      Node<Type>* buf_next_node = del_node->next_node;

      end_node_->next_node = buf_next_node;
      buf_next_node->previous_node = end_node_;

      FreeNode(del_node);
    } else {
      FreeNode(end_node_->next_node);
      end_node_->next_node = nullptr;
      end_node_->previous_node = nullptr;
    }
    --stored_;
  }
}

template <typename Type>
void s21::list<Type>::pop_back() {
  if (end_node_ && stored_) {
    if (stored_ > 1) {
      Node<Type>* del_node = end_node_->previous_node;
      Node<Type>* prev_node = del_node->previous_node;

      end_node_->previous_node = prev_node;
      prev_node->next_node = end_node_;

      FreeNode(del_node);
    } else {
      FreeNode(end_node_->next_node);
      end_node_->next_node = nullptr;
      end_node_->previous_node = nullptr;
    }
    --stored_;
  }
}

template <typename Type>
Node<Type>* s21::list<Type>::CreateNode() {
  Node<Type>* for_create = new Node<Type>;
  return for_create;
}

template <typename Type>
Node<Type>* s21::list<Type>::CreateNode(const Type& data) {
  Node<Type>* for_create = new Node<Type>(data);
  return for_create;
}

template <typename Type>
void s21::list<Type>::FreeNode(Node<Type>* node) {
  if (!node) {
    throw std::invalid_argument("destroy nullptr node");
  }
  delete node;
}

template <typename Type>
void s21::list<Type>::LinkPointer(Node<Type>* for_link_next,
                                  Node<Type>* for_link_previous) {
  for_link_next->next_node = for_link_previous;
  for_link_previous->previous_node = for_link_next;
}

// Iterators

template <typename Type>
typename list<Type>::iterator s21::list<Type>::begin() {
  if (!stored_) {
    return iterator(this, end_node_);
  }
  return iterator(this, end_node_->next_node);
}

template <typename Type>
typename list<Type>::const_iterator s21::list<Type>::begin() const {
  if (!stored_) {
    return iterator(this, end_node_);
  }
  return const_iterator(this, end_node_->next_node);
}

template <typename Type>
typename list<Type>::iterator s21::list<Type>::end() {
  return iterator(this, end_node_);
}

template <typename Type>
typename list<Type>::const_iterator s21::list<Type>::end() const {
  return const_iterator(this, end_node_);
}

template <typename Type>
s21::list<Type>::iterator::list_iterator()
    : link_list_(nullptr), link_node_(nullptr) {}

template <typename Type>
list<Type>::iterator::list_iterator(const list<Type>* list, Node<Type>* node)
    : link_list_(list), link_node_(node) {}

template <typename Type>
Type& list<Type>::iterator::operator*() {
  return link_node_->data;
}

template <typename Type>
bool list<Type>::list_iterator::operator!=(const list_iterator& other_it) {
  return !(*this == other_it);
}

template <typename Type>
bool list<Type>::list_iterator::operator==(const list_iterator& other_it) {
  return link_list_ == other_it.link_list_ && link_node_ == other_it.link_node_;
}

template <typename Type>
typename list<Type>::list_iterator list<Type>::list_iterator::operator++() {
  if (link_node_ != nullptr) {
    link_node_ = link_node_->next_node;
  }
  return *this;
}

template <typename Type>
typename list<Type>::list_iterator list<Type>::list_iterator::operator++(int) {
  list<Type>::iterator copy(link_list_, link_node_);
  if (link_node_) {
    link_node_ = link_node_->next_node;
  }
  return copy;
}

template <typename Type>
typename list<Type>::list_iterator list<Type>::list_iterator::operator--() {
  if (link_node_ != nullptr) {
    link_node_ = link_node_->previous_node;
  }
  return *this;
}

template <typename Type>
typename list<Type>::list_iterator list<Type>::list_iterator::operator--(int) {
  list<Type>::iterator copy(link_list_, link_node_);
  if (link_node_) {
    link_node_ = link_node_->previous_node;
  }
  return copy;
}

}  // namespace s21