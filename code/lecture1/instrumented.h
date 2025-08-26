#pragma once

#ifndef INSTRUMENTED_H
#define INSTRUMENTED_H

#include <concepts>
#include <array>
#include <string>

template <std::totally_ordered T>
struct instrumented {
    static constexpr size_t number_ops = 9;
    static constexpr std::array<std::string, number_ops> counter_names = {
        "n",
        "copy",
        "move",
        "assign",
        "destruct",
        "default",
        "equal",
        "less",
        "construction"
    };
    struct operations {
        enum type {
            n,
            copy,
            move,
            assign,
            destruct,
            default_constructor,
            equal,
            less,
            construction
        };
    };

    inline static std::array<double, number_ops> counters = {};
    T value;

    instrumented(): value() {
        instrumented<T>::counters[operations::default_constructor]++;
    }

    instrumented(const T& value): value(value) {
        instrumented<T>::counters[operations::construction]++;
    }

    instrumented(const instrumented<T>& other): value(other.value) {
        instrumented<T>::counters[operations::copy]++;
    }

    instrumented(instrumented<T>&& other): value(std::move(other.value)) {
        instrumented<T>::counters[operations::move]++;
    }

    instrumented<T>& operator=(const instrumented<T>& other) {
        instrumented<T>::counters[operations::assign]++;
        value = other.value;
        return *this;
    }

    static void initialize(size_t n) {
        instrumented<T>::counters[0] = double(n);
        std::fill(instrumented<T>::counters.begin() + 1, instrumented<T>::counters.end(), 0.0);
    }

    ~instrumented() {
        counters[operations::destruct]++;
        value.~T();
    }

    friend bool operator==(const instrumented& lhs, const instrumented& rhs) {
        counters[operations::equal]++; // equality comparison
        return lhs.value == rhs.value;
    }

    friend bool operator!=(const instrumented& lhs, const instrumented& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const instrumented& lhs, const instrumented& rhs) {
        counters[operations::less]++; // less-than comparison
        return lhs.value < rhs.value;
    }

    friend bool operator<=(const instrumented& lhs, const instrumented& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const instrumented& lhs, const instrumented& rhs) {
        return rhs < lhs;
    }

    friend bool operator>=(const instrumented& lhs, const instrumented& rhs) {
        return !(lhs < rhs);
    }

};

#endif