//
// StdList.h
//
// Wrapper for std::list container
//
// Authors : Gokcehan Kara <gokcehankara@gmail.com>
// License : This file is placed in the public domain.
//

#ifndef STD_LIST_H
#define STD_LIST_H

#include <cstdlib>
#include <list>

template <typename T>
class StdList {
public:
    T& operator[](size_t ind) { return data[ind]; }

    void push_back(const T& val) { data.push_back(val); }
    void pop_back() { data.pop_back(); }
    void push_front(const T& val) { data.push_front(val); }
    void pop_front() { data.pop_front(); }

private:
    std::list<T> data;
};

#endif // STD_LIST_H
