//
// StdDeque.h
//
// Wrapper for std::deque container
//
// Authors : Gokcehan Kara <gokcehankara@gmail.com>
// License : This file is placed in the public domain.
//

#ifndef STD_DEQUE_H
#define STD_DEQUE_H

#include <cstdlib>
#include <deque>

template <typename T>
class StdDeque {
public:
    T& operator[](size_t ind) { return data[ind]; }

    void push_back(const T& val) { data.push_back(val); }
    void pop_back() { data.pop_back(); }
    void push_front(const T& val) { data.push_front(val); }
    void pop_front() { data.pop_front(); }

private:
    std::deque<T> data;
};

#endif // STD_DEQUE_H
