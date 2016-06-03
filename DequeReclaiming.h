//
// DequeReclaiming.h
//
// Deque implementation with reclaiming memory management schema
//
// Authors : Gokcehan Kara <gokcehankara@gmail.com>
// License : This file is placed in the public domain.
//

#ifndef DEQUE_RECLAIMING_H
#define DEQUE_RECLAIMING_H

#include <algorithm>
#include <iostream>

template <typename T>
class DequeReclaiming {
public:
    DequeReclaiming()
        : back_capacity(1)
        , front_capacity(1)
        , back_size(0)
        , front_size(0)
        , back_offset(0)
        , front_offset(0)
    {
        back_data = new T[back_capacity];
        front_data = new T[front_capacity];
    }

    ~DequeReclaiming()
    {
        delete[] back_data;
        delete[] front_data;
    }

    T& operator[](size_t ind)
    {
        if (ind < front_size) {
            return front_data[((front_size - ind) + front_offset) - 1];
        } else {
            return back_data[(ind - front_size) + back_offset];
        }
    }

    float load_factor()
    {
        return static_cast<float>(front_size + back_size) / (front_capacity + back_capacity);
    }

    void reserve_back(size_t n)
    {
        if (n + back_offset > back_capacity) {
            back_offset = 0;
            back_capacity = n + back_offset;
            T* new_data = new T[back_capacity];
            std::copy(back_data + back_offset,
                      back_data + back_offset + back_size,
                      new_data + back_offset);
            delete[] back_data;
            back_data = new_data;
        }
    }

    void reserve_front(size_t n)
    {
        if (n + front_offset > front_capacity) {
            front_offset = 0;
            front_capacity = n + front_offset;
            T* new_data = new T[front_capacity];
            std::copy(front_data + front_offset,
                      front_data + front_offset + front_size,
                      new_data + front_offset);
            delete[] front_data;
            front_data = new_data;
        }
    }

    void push_back(const T& val)
    {
        if (back_size > 0) {
            if (back_size + back_offset >= back_capacity) {
                back_capacity *= 2;
                T* new_data = new T[back_capacity];
                back_offset = 0;
                std::copy(back_data + back_offset,
                          back_data + back_offset + back_size,
                          new_data + back_offset);
                delete[] back_data;
                back_data = new_data;
            }
            back_data[back_size + back_offset] = val;
            back_size++;
        } else if (front_offset > 0) {
            front_size++;
            front_offset--;
            front_data[front_offset] = val;
        } else {
            back_offset = 0;
            back_data[back_offset] = val;
            back_size++;
        }
    }

    void pop_back()
    {
        if (back_size > 0) {
            back_size--;
        } else {
            front_size--;
            front_offset++;
        }
    }

    void push_front(const T& val)
    {
        if (front_size > 0) {
            if (front_size + front_offset >= front_capacity) {
                front_capacity *= 2;
                T* new_data = new T[front_capacity];
                front_offset = 0;
                std::copy(front_data + front_offset,
                          front_data + front_offset + front_size,
                          new_data + front_offset);
                delete[] front_data;
                front_data = new_data;
                front_offset = 0;
            }
            front_data[front_size + front_offset] = val;
            front_size++;
        } else if (back_offset > 0) {
            back_size++;
            back_offset--;
            back_data[back_offset] = val;
        } else {
            front_offset = 0;
            front_data[front_offset] = val;
            front_size++;
        }
    }

    void pop_front()
    {
        if (front_size > 0) {
            front_size--;
        } else {
            back_size--;
            back_offset++;
        }
    }

    void draw()
    {
        for (size_t i = front_capacity; i > 0; --i) {
            if (i > front_size + front_offset) {
                std::cout << "| ";
            } else if (i > front_offset) {
                std::cout << "|x";
            } else {
                std::cout << "| ";
            }
        }
        std::cout << ":";

        for (size_t i = 0; i < back_capacity; ++i) {
            if (i < back_offset) {
                std::cout << " |";
            } else if (i < back_size + back_offset) {
                std::cout << "x|";
            } else {
                std::cout << " |";
            }
        }
        std::cout << std::endl;
    }

private:
    T* back_data;
    T* front_data;
    size_t back_capacity;
    size_t front_capacity;
    size_t back_size;
    size_t front_size;
    size_t back_offset;
    size_t front_offset;
};

#endif // DEQUE_RECLAIMING_H
