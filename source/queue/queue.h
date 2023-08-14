#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
struct Node {
    T data;
    Node<T>* next;
};

template <typename T>
class Queue {
public:
    Queue();
    ~Queue();

    void enqueue(const T& item);
    void dequeue();
    bool isEmpty();
    int size();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void clear();

private:
    // по гугл стилю в конце приватных полей ствят _, типа: front_
    Node<T>* front;
    Node<T>* rear;
    int count;
};

#endif // QUEUE_H
