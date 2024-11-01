#pragma once

#include <exception>
#include <stdexcept>
#include <vector>

template <typename T>
class Heap {
    std::vector<T> arr_;

    void Siftup(size_t v) {
        while (v > 1 && arr_[v] < arr_[v / 2]) {
            std::swap(arr_[v], arr_[v / 2]);
            v /= 2;
        }
    }

    void Siftdown(size_t v) {
        while (2 * v <= Size()) {
            size_t l = v * 2;
            size_t r = v * 2 + 1;
            size_t u = (r <= Size() && arr_[r] < arr_[l] ? r : l);
            if (arr_[v] <= arr_[u]) {
                break;
            }
            std::swap(arr_[v], arr_[u]);
            v = u;
        }
    }

public:
    Heap() : arr_(1) {
    }

    size_t Size() {
        return arr_.size() - 1;
    }

    void Push(T x) {
        arr_.push_back(x);
        Siftup(Size());
    }

    void Pop() {
        if (!Size()) {
            return;
        }
        std::swap(arr_[1], arr_[arr_.size() - 1]);
        arr_.pop_back();
        Siftdown(1);
    }

    T Top() {
        if (!Size()) {
            throw std::runtime_error("Get from empty queue");
        }
        return arr_[1];
    }
};
