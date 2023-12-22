#pragma once
#include <iostream>
using namespace std;

template <typename T>  // template type queue
class Queue {
public:
    struct Node {
        T data;
        Node* next;
        Node(T value) : data(value), next(nullptr) {}
    };

    Node* front;   // it will store the data member at front
    Node* rear;    // it will store data member at last

public:
    Queue() : front(nullptr), rear(nullptr) {}

    ~Queue() {    // destructor
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(T value) {   // enqueue function
        Node* newNode = new Node(value);
        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void dequeue() {   // dequeue function
        if (isEmpty()) {
            std::cerr << "Queue is empty. Cannot dequeue." << std::endl;
            return;
        }

        Node* temp = front;
        front = front->next;
        delete temp;

        if (front == nullptr) {
            rear = nullptr; // If the last element is dequeued, update rear as well.
        }
    }

    T getFront() const {   // return the element at front 
        if (isEmpty()) {
            std::cerr << "Queue is empty." << std::endl;
            throw std::out_of_range("Queue is empty.");
        }

        return front->data;
    }

    bool isEmpty() const {
        return front == nullptr;
    }
};


// Find function 
template<typename T>
bool findObstacles(const T* begin, const T* end, const T& value) {  // function for finding obstacles 
    for (const T* current = begin; current != end; ++current) {
        if (*current == value) {
            return true;  // Value found
        }
    }
    return false;  // Value not found
}
