#pragma once

/*
    This file is part of EJD.

    Copyright © 2019
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
#include <type_traits>
#include <tuple>
#include <functional>

namespace ejd
{
namespace detail {
    template <typename T, bool Expr =std::is_lvalue_reference_v<T>>
        struct choose_reference {
            using value_type = T&;
        };
    template <typename T>
        struct choose_reference<T,false> {
            using value_type = T;
        };

    template <typename T>
        using choose_reference_t = typename choose_reference<T>::value_type;

    template <typename... IteratorTypes>
        struct zip_iterator {
            public:

                using tuple_type = std::tuple<IteratorTypes...>;

                zip_iterator(IteratorTypes&&... its): m_its{std::forward<IteratorTypes>(its)...} {}

                bool operator==(const zip_iterator& o) const {return equal_to(_is(),o);}
                bool operator!=(const zip_iterator& o) const {return !equal_to(_is(),o);}
                bool operator<(const zip_iterator& o) const{return less_than(_is(),o);}

                zip_iterator& operator++() {return increment(_is());}

                auto operator*() {return dereference(_is());}

            private:
                tuple_type m_its;

            private:
                template <int... M>
                    using IS = typename std::integer_sequence<int,M...>;
                constexpr static auto _is() { return std::make_integer_sequence<int,sizeof...(IteratorTypes)>(); }
                template <int... M>
                    bool less_than(IS<M...>, zip_iterator& o) const {
                        return (std::less(std::get<M>(m_its),std::get<M>(o.m_its))  || ... );
                    }
                template <int... M>
                    auto dereference(IS<M...>) {
                        return std::make_tuple(std::get<M>(m_its).operator*()...);
                    }
                template <int... M>
                    zip_iterator& increment(IS<M...>) {
                        (++std::get<M>(m_its),...);
                        return *this;
                    }
                template <int... M>
                    bool equal_to(IS<M...>,const zip_iterator& o) const {
                        return (
                            std::equal_to<std::tuple_element_t<M,tuple_type>>()
                                (std::get<M>(m_its),std::get<M>(o.m_its))  || ...
                        );
                    }
        };

    template <typename... Types>
        struct zip_container {

            template <int... M>
                using IS = typename std::integer_sequence<int,M...>;
            constexpr static auto _is() { return std::make_integer_sequence<int,sizeof...(Types)>(); }

            zip_container() = delete;
            zip_container(zip_container&&) = default;
            zip_container(const zip_container&) = delete;
            zip_container& operator=(zip_container&&) = delete;
            zip_container& operator=(const zip_container&) = delete;

            using iterator = zip_iterator<typename std::remove_reference_t<Types>::iterator...>;

            template <int... M>
                iterator _begin(IS<M...>) const { return iterator(std::get<M>(m_containers).begin() ...); }

            iterator begin() const { return _begin(_is());}
            template <int... M>
                iterator _end(IS<M...>) const { return iterator(std::get<M>(m_containers).end() ...); }

            iterator end() const { return _end(_is());}

            zip_container(Types&&... t): m_containers{std::forward<Types>(t)...} {}

            private:
        
            std::tuple<choose_reference_t<Types>...> m_containers;
        };
}   // detail

template <typename... Types>
    auto zip(Types&&... t) {
        return detail::zip_container<Types...>(std::forward<Types>(t)...);
    }
}  
