
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
    const Type& operator*() const;

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

  using pointer_node = Node<Type>*;

  list();
  explicit list(size_t count);
  list(const size_t count, const Type& value);
  list(std::initializer_list<Type> const& items);
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

  void swap(list& other);
  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void reverse();
  void unique();
  void sort();

  // Operators

  list& operator=(list&& other) noexcept;

  bool operator==(const list& other) const;
  bool operator!=(const list& other) const;

 private:
  Node<Type>* end_node_ = nullptr;
  size_t stored_ = 0;

  void DeleteList();

  Node<Type>* CreateNode();
  Node<Type>* CreateNode(const Type& data);

  void FreeNode(Node<Type>* node);

  void LinkPointerNodeRange(pointer_node prev_pos, pointer_node pos,
                            pointer_node first, pointer_node last);
  void LinkPointerNode(Node<Type>* prev_node, Node<Type>* next_node);

  Node<Type>* CreateTree();
  void PushNodeTree(Node<Type>* push, Node<Type>* head);
  void GetDataTree(Node<Type>* head);
  void PrintTree(Node<Type>* head);
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
  *this = std::move(other);
  // end_node_ = std::exchange(other.end_node_, nullptr);
  // stored_ = std::exchange(other.stored_, 0);
}

template <typename Type>
list<Type>::~list() {
  DeleteList();
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
  if (end_node_) {
    while (stored_) {
      pop_front();
    }
  }
}

template <typename Type>
typename s21::list<Type>::iterator s21::list<Type>::insert(iterator pos,
                                                           const Type& value) {
  if (pos.link_list_ != this) {
    throw std::invalid_argument("Incorrect list");
  }
  Node<Type>* new_node = CreateNode(value);

  if (!stored_) {
    LinkPointerNode(end_node_, new_node);
    LinkPointerNode(new_node, end_node_);
  } else {
    LinkPointerNode(pos.link_node_->previous_node, new_node);
    LinkPointerNode(new_node, pos.link_node_);
  }
  ++stored_;
  return iterator(this, new_node);
}

template <typename Type>
void s21::list<Type>::push_back(const Type& value) {
  Node<Type>* push = CreateNode(value);
  if (!stored_) {
    LinkPointerNode(end_node_, push);
    LinkPointerNode(push, end_node_);
  } else {
    LinkPointerNode(end_node_->previous_node, push);
    LinkPointerNode(push, end_node_);
  }
  ++stored_;
}

template <typename Type>
void s21::list<Type>::push_front(const Type& value) {
  Node<Type>* push = CreateNode(value);
  if (!stored_) {
    LinkPointerNode(end_node_, push);
    LinkPointerNode(push, end_node_);
  } else {
    LinkPointerNode(push, end_node_->next_node);
    LinkPointerNode(end_node_, push);
  }
  ++stored_;
}

template <typename Type>
void s21::list<Type>::pop_front() {
  if (end_node_ && stored_) {
    Node<Type>* del_node = end_node_->next_node;
    if (stored_ > 1) {
      LinkPointerNode(end_node_, del_node->next_node);
    } else {
      end_node_->next_node = nullptr;
      end_node_->previous_node = nullptr;
    }
    FreeNode(del_node);
    --stored_;
  }
}

template <typename Type>
void s21::list<Type>::pop_back() {
  if (end_node_ && stored_) {
    Node<Type>* del_node = end_node_->previous_node;
    if (stored_ > 1) {
      LinkPointerNode(del_node->previous_node, end_node_);
    } else {
      end_node_->next_node = nullptr;
      end_node_->previous_node = nullptr;
    }
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
          Node<Type>* other_link = other.end_node_->next_node->next_node;

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
          other.end_node_->next_node = nullptr;
          other.end_node_->previous_node = nullptr;
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
    if (!stored_) {
      LinkPointerNodeRange(end_node_, end_node_, other.end_node_->next_node,
                           other.end_node_->previous_node);
    } else {
      LinkPointerNodeRange(pos.link_node_->previous_node, pos.link_node_,
                           other.end_node_->next_node,
                           other.end_node_->previous_node);
    }
    stored_ += other.stored_;

    other.end_node_->next_node = nullptr;
    other.end_node_->previous_node = nullptr;
    other.stored_ = 0;
  }
}

template <typename Type>
void s21::list<Type>::reverse() {
  if (stored_ > 1) {
    Node<Type>* next_node_end = end_node_->next_node;
    Node<Type>* prev_node_end = end_node_->previous_node;
    for (Node<Type>* step = end_node_->next_node; step != end_node_;) {
      Node<Type>* next_node = step->next_node;
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
    for (Node<Type>* step = end_node_->next_node;
         step->next_node != end_node_;) {
      if (step->data != step->next_node->data) {
        step = step->next_node;
      } else {
        Node<Type>* del_node = step->next_node;
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
    Node<Type>* head = CreateTree();
    // PrintTree(head);
    GetDataTree(head);
  }
  // end_node_->next_node = nullptr;
  // end_node_->previous_node = nullptr;
  // stored_ = 0;
}

template <typename Type>
Node<Type>* s21::list<Type>::CreateTree() {
  Node<Type>* head = end_node_->next_node;
  Node<Type>* step = head->next_node;

  end_node_->next_node = nullptr;
  end_node_->previous_node = nullptr;

  head->next_node = nullptr;
  head->previous_node = nullptr;

  stored_ = 0;

  // std::cout << "stored_ = 0;" << std::endl;

  for (; step != end_node_;) {
    Node<Type>* buf_next_step = step->next_node;

    step->next_node = nullptr;
    step->previous_node = nullptr;
    // std::cout << "for " << step->data << std::endl;

    for (Node<Type>* step_of_head = head;;) {
      assert(step_of_head != nullptr);
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
          // std::cout << "step_of_head->previous_node = step;" << std::endl;
          step_of_head->previous_node = step;
          break;
        }
      }
    }
    step = buf_next_step;
  }
  // std::cout << "for end" << std::endl;
  return head;
}

template <typename Type>
void s21::list<Type>::PrintTree(Node<Type>* head) {
  assert(head != nullptr);
  if (head->next_node) {
    GetDataTree(head->next_node);
  }

  // std::cout << "data == " << head->data << std::endl;

  if (head->previous_node) {
    GetDataTree(head->previous_node);
  }
}

template <typename Type>
void s21::list<Type>::GetDataTree(Node<Type>* head) {
  if (head->next_node) {
    GetDataTree(head->next_node);
  }

  Node<Type>* buf_prev_head = head->previous_node;

  if (!end_node_->next_node) {
    end_node_->next_node = head;
    end_node_->previous_node = head;

    head->next_node = end_node_;
    head->previous_node = end_node_;
    ++stored_;
  } else {
    Node<Type>* buf_prev_list_node = end_node_->previous_node;

    buf_prev_list_node->next_node = head;
    head->next_node = end_node_;

    end_node_->previous_node = head;
    head->previous_node = buf_prev_head;
    ++stored_;
  }

  if (buf_prev_head) {
    GetDataTree(buf_prev_head);
  }
}

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

template <typename Type>
void s21::list<Type>::DeleteList() {
  if (end_node_) {
    Node<Type>* end = end_node_;
    if (end_node_->next_node) {
      for (Node<Type>* node = end_node_->next_node;
           node != nullptr && node != end;) {
        Node<Type>* dealloc = node;
        node = node->next_node;
        FreeNode(dealloc);
      }
    }
    if (end) {
      FreeNode(end);
    }
  }
  stored_ = 0;
  end_node_ = nullptr;
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
void s21::list<Type>::LinkPointerNodeRange(pointer_node prev_pos,
                                           pointer_node pos, pointer_node first,
                                           pointer_node last) {
  prev_pos->next_node = first;
  last->next_node = pos;

  pos->previous_node = last;
  first->previous_node = prev_pos;
}

template <typename Type>
void s21::list<Type>::LinkPointerNode(Node<Type>* prev_node,
                                      Node<Type>* next_node) {
  prev_node->next_node = next_node;
  next_node->previous_node = prev_node;
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