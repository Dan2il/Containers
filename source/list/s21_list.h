
#include <iostream>  // УДАЛИТЬ
#include <utility>

// #include "../s21_containers.h"

namespace s21 {

template <typename Type>
class list {
 public:
  struct Node {
    Node* next_node;
    Node* previous_node;
    Type data;

    Node() : next_node(nullptr), previous_node(nullptr), data() {}
    explicit Node(const Type& t) : Node() { data = t; };
  };

  class list_iterator : public std::iterator<std::forward_iterator_tag, Type> {
    friend class list;

   public:
    list_iterator();
    list_iterator(const list<Type>* list, Node* node);

    Type& operator*();
    const Type& operator*() const;

    bool operator!=(const list_iterator& other_it);
    bool operator==(const list_iterator& other_it);

    list_iterator operator++();
    list_iterator operator++(int);

    list_iterator operator--();
    list_iterator operator--(int);

   private:
    const list<Type>* link_list_;
    Node* link_node_;
  };

  using iterator = list_iterator;
  using const_iterator = const list_iterator;
  using pointer_node = Node*;

  // Constructors and destructor

  list();
  explicit list(size_t count);
  list(const size_t count, const Type& value);
  list(std::initializer_list<Type> const& items);
  list(const list<Type>& other);
  list(list&& other) noexcept;

  ~list();

  // Element access

  Type& front();
  const Type& front() const;

  Type& back();
  const Type& back() const;

  // Iterators

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  // Capacity

  bool empty() const noexcept;
  size_t size() const noexcept;
  size_t max_size() const noexcept;

  // Modifiers

  void clear();

  iterator insert(iterator pos, const Type& value);

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);

  void push_front(const Type& value);
  void push_back(const Type& value);

  void pop_front();
  void pop_back();

  void swap(list& other);
  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void splice(const_iterator pos, list&& other);
  void reverse();
  void unique();
  void sort();

  // Operators

  list& operator=(list&& other) noexcept;

  bool operator==(const list& other) const;
  bool operator!=(const list& other) const;

 private:
  pointer_node end_node_ = nullptr;
  size_t stored_ = 0;

  // Other

  void DeleteList();

  pointer_node CreateNode();
  pointer_node CreateNode(const Type& data);

  void FreeNode(pointer_node node);

  void LinkPointerNodeRange(pointer_node prev_pos, pointer_node pos,
                            pointer_node first, pointer_node last);
  void LinkPointerNode(pointer_node prev_node, pointer_node next_node);

  void DelLink(pointer_node node);

  pointer_node CreateTree();
  void PushNodeTree(pointer_node step, pointer_node head);
  void GetDataTree(pointer_node head);
};

// Constructors and destructor

template <typename Type>
list<Type>::list() : end_node_(CreateNode()) {}

template <typename Type>
list<Type>::list(size_t count) : list() {
  while (stored_ < count) {
    push_back(Type());
  }
}

template <typename Type>
list<Type>::list(const size_t count, const Type& value) : list() {
  while (stored_ < count) {
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
  *this = std::move(other);
}

template <typename Type>
list<Type>::~list() {
  DeleteList();
}

// Element access

template <typename Type>
Type& s21::list<Type>::front() {
  return !end_node_->next_node ? end_node_->data : end_node_->next_node->data;
}

template <typename Type>
const Type& s21::list<Type>::front() const {
  return !end_node_->next_node ? end_node_->data : end_node_->next_node->data;
}

template <typename Type>
Type& s21::list<Type>::back() {
  return !end_node_->previous_node ? end_node_->data
                                   : end_node_->previous_node->data;
}

template <typename Type>
const Type& s21::list<Type>::back() const {
  return !end_node_->previous_node ? end_node_->data
                                   : end_node_->previous_node->data;
}

// Iterators

template <typename Type>
typename list<Type>::iterator s21::list<Type>::begin() {
  return !stored_ ? end() : iterator(this, end_node_->next_node);
}

template <typename Type>
typename list<Type>::const_iterator s21::list<Type>::begin() const {
  return !stored_ ? end() : const_iterator(this, end_node_->next_node);
}

template <typename Type>
typename list<Type>::iterator s21::list<Type>::end() {
  return iterator(this, end_node_);
}

template <typename Type>
typename list<Type>::const_iterator s21::list<Type>::end() const {
  return const_iterator(this, end_node_);
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
  return std::numeric_limits<size_t>::max() / sizeof(Node);
}

// Modifiers

template <typename Type>
void s21::list<Type>::clear() {
  if (end_node_) {
    while (stored_) {
      pop_front();
    }
    DelLink(end_node_);
  }
}

template <typename Type>
typename s21::list<Type>::iterator s21::list<Type>::insert(iterator pos,
                                                           const Type& value) {
  if (pos.link_list_ != this) {
    throw std::invalid_argument("Incorrect list");
  }
  pointer_node push = CreateNode(value);
  !stored_ ? LinkPointerNodeRange(end_node_, end_node_, push, push)
           : LinkPointerNodeRange(pos.link_node_->previous_node, pos.link_node_,
                                  push, push);
  ++stored_;
  return iterator(this, push);
}

template <typename Type>
template <typename... Args>
typename s21::list<Type>::iterator s21::list<Type>::insert_many(
    const_iterator pos, Args&&... args) {
  pointer_node prev_push = pos.link_node_->previous_node;
  splice(pos, s21::list<Type>({args...}));
  return iterator(this, prev_push);
}

template <typename Type>
void s21::list<Type>::push_back(const Type& value) {
  pointer_node push = CreateNode(value);
  !stored_
      ? LinkPointerNodeRange(end_node_, end_node_, push, push)
      : LinkPointerNodeRange(end_node_->previous_node, end_node_, push, push);
  ++stored_;
}

template <typename Type>
void s21::list<Type>::push_front(const Type& value) {
  pointer_node push = CreateNode(value);
  !stored_ ? LinkPointerNodeRange(end_node_, end_node_, push, push)
           : LinkPointerNodeRange(push, push, end_node_->next_node, end_node_);
  ++stored_;
}

template <typename Type>
void s21::list<Type>::pop_front() {
  if (end_node_ && stored_) {
    pointer_node del_node = end_node_->next_node;
    stored_ > 1 ? LinkPointerNode(end_node_, del_node->next_node)
                : DelLink(end_node_);
    FreeNode(del_node);
    --stored_;
  }
}

template <typename Type>
void s21::list<Type>::pop_back() {
  if (end_node_ && stored_) {
    pointer_node del_node = end_node_->previous_node;
    stored_ > 1 ? LinkPointerNode(del_node->previous_node, end_node_)
                : DelLink(end_node_);
    FreeNode(del_node);
    --stored_;
  }
}

template <typename Type>
void s21::list<Type>::swap(list& other) {
  std::swap(end_node_, other.end_node_);
  std::swap(stored_, other.stored_);
}

template <typename Type>
void s21::list<Type>::merge(list& other) {
  if (*this != other && other.stored_) {
    if (!end_node_ || !stored_) {
      splice(begin(), other);
    } else {
      for (s21::list<Type>::iterator it_this = begin(); it_this != end();
           ++it_this) {
        if (*other.begin() < *it_this) {
          pointer_node other_link = other.end_node_->next_node->next_node;

          LinkPointerNodeRange(it_this.link_node_->previous_node,
                               it_this.link_node_, other.end_node_->next_node,
                               other.end_node_->next_node);

          stored_++;
          other.end_node_->next_node = other_link;
          other_link->previous_node = other.end_node_;
          other.stored_--;

          it_this = begin();
        }
        if (!other.stored_) {
          DelLink(other.end_node_);
          break;
        }
      }
      if (other.stored_) {
        splice(end(), other);
      }
    }
  }
}

template <typename Type>
void s21::list<Type>::splice(const_iterator pos, list& other) {
  if (*this != other && other.stored_) {
    if (!end_node_) {
      throw std::invalid_argument("Node is nullptr");
    }
    !stored_
        ? LinkPointerNodeRange(end_node_, end_node_, other.end_node_->next_node,
                               other.end_node_->previous_node)
        : LinkPointerNodeRange(pos.link_node_->previous_node, pos.link_node_,
                               other.end_node_->next_node,
                               other.end_node_->previous_node);
    stored_ += other.stored_;
    DelLink(other.end_node_);
    other.stored_ = 0;
  }
}

template <typename Type>
void s21::list<Type>::splice(const_iterator pos, list&& other) {
  if (*this != other && other.stored_) {
    if (!end_node_) {
      throw std::invalid_argument("Node is nullptr");
    }
    !stored_
        ? LinkPointerNodeRange(end_node_, end_node_, other.end_node_->next_node,
                               other.end_node_->previous_node)
        : LinkPointerNodeRange(pos.link_node_->previous_node, pos.link_node_,
                               other.end_node_->next_node,
                               other.end_node_->previous_node);
    stored_ += other.stored_;
    DelLink(other.end_node_);
    other.stored_ = 0;
  }
}

template <typename Type>
void s21::list<Type>::reverse() {
  if (stored_ > 1) {
    pointer_node next_node_end = end_node_->next_node;
    pointer_node prev_node_end = end_node_->previous_node;
    for (pointer_node step = end_node_->next_node; step != end_node_;) {
      pointer_node next_node = step->next_node;
      step->next_node = step->previous_node;
      step->previous_node = next_node;
      step = next_node;
    }
    end_node_->next_node = prev_node_end;
    end_node_->previous_node = next_node_end;
  }
}

template <typename Type>
void s21::list<Type>::unique() {
  if (stored_ > 1) {
    for (pointer_node step = end_node_->next_node;
         step->next_node != end_node_;) {
      if (step->data != step->next_node->data) {
        step = step->next_node;
      } else {
        pointer_node del_node = step->next_node;
        LinkPointerNode(step, del_node->next_node);
        FreeNode(del_node);
        --stored_;
      }
    }
  }
}

template <typename Type>
void s21::list<Type>::sort() {
  if (stored_ > 1) {
    GetDataTree(CreateTree());
  }
}

// Operators

template <typename Type>
list<Type>& s21::list<Type>::operator=(list&& other) noexcept {
  if (*this != other) {
    if (end_node_) {
      DeleteList();
    }
    end_node_ = std::exchange(other.end_node_, nullptr);
    stored_ = std::exchange(other.stored_, 0);
  }
  return *this;
}

// добавить проверку где не равны только последние элементы
template <typename Type>
bool s21::list<Type>::operator==(const list& other) const {
  if ((stored_ != other.stored_) || (!end_node_ && other.end_node_) ||
      (end_node_ && !other.end_node_)) {
    return false;
  }
  s21::list<Type>::iterator it_other = other.begin();
  for (const Type& data : *this) {
    if (data != *(it_other++)) {
      return false;
    }
  }
  return true;
}

template <typename Type>
bool s21::list<Type>::operator!=(const list& other) const {
  return !(*this == other);
}

// Other

template <typename Type>
typename s21::list<Type>::Node* s21::list<Type>::CreateTree() {
  pointer_node head = end_node_->next_node;
  pointer_node step = head->next_node;
  DelLink(end_node_);
  DelLink(head);
  while (step != end_node_) {
    pointer_node buf_next_step = step->next_node;
    DelLink(step);
    PushNodeTree(step, head);
    step = buf_next_step;
  }
  return head;
}

template <typename Type>
void s21::list<Type>::PushNodeTree(pointer_node step, pointer_node head) {
  for (pointer_node step_of_head = head;;) {
    if (step->data < step_of_head->data) {
      if (step_of_head->next_node != nullptr) {
        step_of_head = step_of_head->next_node;
      } else {
        step_of_head->next_node = step;
        break;
      }
    } else {
      if (step_of_head->previous_node != nullptr) {
        step_of_head = step_of_head->previous_node;
      } else {
        step_of_head->previous_node = step;
        break;
      }
    }
  }
}

template <typename Type>
void s21::list<Type>::GetDataTree(pointer_node head) {
  if (head->next_node) {
    GetDataTree(head->next_node);
  }
  pointer_node buf_prev_head = head->previous_node;
  !end_node_->next_node
      ? LinkPointerNodeRange(end_node_, end_node_, head, head)
      : LinkPointerNodeRange(end_node_->previous_node, end_node_, head, head);
  if (buf_prev_head) {
    GetDataTree(buf_prev_head);
  }
}

template <typename Type>
void s21::list<Type>::DeleteList() {
  if (end_node_) {
    if (end_node_->next_node) {
      clear();
    }
    FreeNode(end_node_);
    end_node_ = nullptr;
  }
}

template <typename Type>
typename s21::list<Type>::Node* s21::list<Type>::CreateNode() {
  pointer_node for_create = new Node;
  return for_create;
}

template <typename Type>
typename s21::list<Type>::Node* s21::list<Type>::CreateNode(const Type& data) {
  pointer_node for_create = new Node(data);
  return for_create;
}

template <typename Type>
void s21::list<Type>::FreeNode(pointer_node node) {
  if (!node) {
    throw std::invalid_argument("destroy nullptr node");
  }
  delete node;
}

template <typename Type>
void s21::list<Type>::LinkPointerNodeRange(pointer_node prev_pos,
                                           pointer_node pos, pointer_node first,
                                           pointer_node last) {
  LinkPointerNode(prev_pos, first);
  LinkPointerNode(last, pos);
}

template <typename Type>
void s21::list<Type>::LinkPointerNode(pointer_node prev_node,
                                      pointer_node next_node) {
  prev_node->next_node = next_node;
  next_node->previous_node = prev_node;
}

template <typename Type>
void s21::list<Type>::DelLink(pointer_node node) {
  node->next_node = nullptr;
  node->previous_node = nullptr;
}

// Iterators

template <typename Type>
s21::list<Type>::iterator::list_iterator()
    : link_list_(nullptr), link_node_(nullptr) {}

template <typename Type>
list<Type>::iterator::list_iterator(const list<Type>* list, Node* node)
    : link_list_(list), link_node_(node) {}

template <typename Type>
Type& list<Type>::iterator::operator*() {
  return link_node_->data;
}

template <typename Type>
const Type& list<Type>::const_iterator::operator*() const {
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