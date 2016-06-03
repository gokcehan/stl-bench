//
// StdVector.h
//
// Wrapper for std::vector container
//
// Authors : Gokcehan Kara <gokcehankara@gmail.com>
// License : This file is placed in the public domain.
//

#ifndef STD_VECTOR_H
#define STD_VECTOR_H

#include <cstdlib>
#include <vector>

template <typename T>
class StdVector {
public:
    T& operator[](size_t ind) { return data[ind]; }

    float load_factor() { return float(data.size()) / data.capacity(); }
    void reserve_back(size_t n) { data.reserve(n); }

    void push_back(const T& val) { data.push_back(val); }
    void pop_back() { data.pop_back(); }
    void push_front(const T& val) { data.insert(data.begin(), val); }
    void pop_front() { data.erase(data.begin()); }

private:
    std::vector<T> data;
};

#endif // STD_VECTOR_H
