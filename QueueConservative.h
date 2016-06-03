//
// QueueConservative.h
//
// Queue implementation with conservative memory management schema
//
// Authors : Gokcehan Kara <gokcehankara@gmail.com>
// License : This file is placed in the public domain.
//

#ifndef QUEUE_CONSERVATIVE_H
#define QUEUE_CONSERVATIVE_H

#include <algorithm>
#include <iostream>

template <typename T>
class QueueConservative {
public:
    QueueConservative()
        : capacity(1)
        , size(0)
        , offset(0)
    {
        data = new T[capacity];
    }

    ~QueueConservative() { delete[] data; }

    T& operator[](size_t ind) { return data[ind + offset]; }

    float load_factor() { return static_cast<float>(size) / capacity; }

    void reserve_back(size_t n)
    {
        if (n + offset > capacity) {
            offset = 0;
            capacity = n + offset;
            T* new_data = new T[capacity];
            std::copy(data + offset, data + offset + size, new_data + offset);
            delete[] data;
            data = new_data;
        }
    }

    void reserve_front(size_t n)
    {
        if (n > size + offset) {
            capacity = n;
            offset = n - size;
            T* new_data = new T[capacity];
            std::copy(data + offset, data + offset + size, new_data + offset);
            delete[] data;
            data = new_data;
        }
    }

    void push_back(const T& val)
    {
        if (size + offset >= capacity) {
            if (load_factor() >= 0.5) {
                capacity *= 2;
                T* new_data = new T[capacity];
                offset = 0;
                std::copy(data + offset, data + offset + size, new_data + offset);
                delete[] data;
                data = new_data;
            } else {
                std::move(data + offset, data + offset + size, data);
                offset = 0;
            }
        }
        data[size + offset] = val;
        size++;
    }

    void pop_back() { size--; }

    void push_front(const T& val)
    {
        if (offset > 0) {
            size++;
            offset--;
            data[offset] = val;
        } else {
            if (size >= capacity) {
                capacity *= 2;
                T* new_data = new T[capacity];
                std::copy(data, data + size, new_data + 1);
                delete[] data;
                data = new_data;
            } else {
                std::move_backward(data, data + size, data + size + 1);
            }
            data[0] = val;
            size++;
        }
    }

    void pop_front()
    {
        size--;
        offset++;
    }

    void draw()
    {
        std::cout << "|";
        for (size_t i = 0; i < capacity; ++i) {
            if (i < offset) {
                std::cout << " |";
            } else if (i < size + offset) {
                std::cout << "x|";
            } else {
                std::cout << " |";
            }
        }
        std::cout << std::endl;
    }

private:
    T* data;
    size_t capacity;
    size_t size;
    size_t offset;
};

#endif // QUEUE_CONSERVATIVE_H
