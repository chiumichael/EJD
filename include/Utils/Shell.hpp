#pragma once
/*
    This file is part of EJD.

    Copyright © 2020
              Michael Chiu <chiu@cs.toronto.edu>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <tuple>
#include <utility>

namespace ejd {
namespace detail {

template <typename BeginIt, typename Endit>
struct shell_container;

template <typename T, bool Expr = std::is_lvalue_reference_v<T>>
struct choose_storage {
    using value_type = T&;
};

template <typename T>
struct choose_storage<T,false> {
    using value_type = T;
};

template <typename T, int D>
struct choose_storage<T[D],true> {
    using value_type = shell_container<T*, T*>;
};

template <typename T>
struct choose_storage<const T, false> {
    using value_type = const T;
};

template <typename T>
using choose_storage_t = typename choose_storage<T>::value_type;

template <typename BeginIt, typename Endit>
struct shell_container {
public:
    using iterator = BeginIt;
    shell_container(BeginIt&& b, Endit&& e) 
        : m_begin(b), m_end(e)
    {}

    auto begin() { return m_begin; }
    auto begin() const { return m_begin; }
    auto end() { return m_end; }
    auto end() const { return m_end; }
private:
    const BeginIt m_begin;
    const Endit m_end;

};

// namespace detail
}

template <typename BeginIt, typename EndIt>
auto shell(BeginIt&& b, EndIt&& e) {
    return detail::shell_container<BeginIt, EndIt>(std::forward<BeginIt>(b), std::forward<EndIt>(e));
}

template <typename Container>
auto shell(Container&& c) {
    return shell(c.begin(),c.end());
}

template <typename T, int D>
auto shell(const T c[D]) {
    return shell(c,c+D);
}

template <typename T, int D>
auto shell(T c[D]) {
    return shell(c,c+D);
}

// namespace ejd
}
