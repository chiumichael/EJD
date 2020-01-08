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

#include <cstdlib>

namespace ejd {

namespace detail {
struct range_iterator {
    public:
        range_iterator(int val, int inc): m_val(val), m_inc(inc) {}
        bool operator==(const range_iterator& o) const {return m_val == o.m_val;}
        bool operator!=(const range_iterator& o) const {return m_val != o.m_val;}
        bool operator<(const range_iterator& o) const{return (m_inc>0)?(m_val < o.m_val):(m_val > o.m_val);}
        range_iterator& operator++() {m_val += m_inc; return *this;}
        auto operator*() {return m_val;}
    private:
        int m_val,m_inc;
};

template <typename... Types>
    struct range_container {
        range_container() = delete;
        range_container(range_container&&) = default;
        range_container(const range_container&) = default;
        range_container& operator=(range_container&&) = default;
        range_container& operator=(const range_container&) = default;

        using iterator = range_iterator;

        iterator begin() const { return iterator(m_start,m_inc);}
        iterator end() const { 
            int range = m_end - m_start;
            auto dr = std::lldiv(range,m_inc);
            if(dr.rem == 0) {
                return iterator(m_end,m_inc);
            } else {
                return iterator(m_start + m_inc * (dr.quot+1), m_inc);
            }
        }

        range_container(int a, int b, int c): m_start(a), m_end(b), m_inc(c) {}

        private:
        int m_start, m_end, m_inc;
    };
}   // namespace detail

auto range(int N = std::numeric_limits<int>::has_infinity ? std::numeric_limits<int>::infinity()
                                                        : std::numeric_limits<int>::max() 
){
    return detail::range_container(0,N,1);
}

auto range(int a, int b, int inc = 1) {
    return detail::range_container(a,b,inc);
}
// namespace ejd
}
