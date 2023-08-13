#include "queue.h"

template <typename T>
Queue<T>::Queue() : front(nullptr), rear(nullptr), count(0) {}

template <typename T>
Queue<T>::~Queue() {
    clear();
}

template <typename T>
void Queue<T>::enqueue(const T& item) {
    Node<T>* newNode = new Node<T>;
    newNode->data = item;
    newNode->next = nullptr;

    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }

    count++;
}

template <typename T>
void Queue<T>::dequeue() {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
    } else {
        Node<T>* temp = front;
        front = front->next;

        delete temp;
        count--;

        if (isEmpty()) {
            rear = nullptr;
        }
    }
}

template <typename T>
bool Queue<T>::isEmpty() {
    return count == 0;
}

template <typename T>
int Queue<T>::size() {
    return count;
}

template <typename T>
T& Queue<T>::front() {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        static T defaultValue;
        return defaultValue;
    } else {
        return front->data;
    }
}

template <typename T>
const T& Queue<T>::front() const {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        static T defaultValue;
        return defaultValue;
    } else {
        return front->data;
    }
}

template <typename T>
T& Queue<T>::back() {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        static T defaultValue;
        return defaultValue;
    } else {
        return rear->data;
    }
}

template <typename T>
const T& Queue<T>::back() const {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        static T defaultValue;
        return defaultValue;
    } else {
        return rear->data;
    }
}

template <typename T>
void Queue<T>::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}