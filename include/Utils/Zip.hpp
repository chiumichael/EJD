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

#include "Utils/Shell.hpp"

#include <type_traits>
#include <tuple>
#include <functional>
#include <cxxabi.h>

namespace ejd {
namespace detail {

	template <typename T>
    struct is_initializer_list: public std::false_type {
    };

    template <typename T>
    struct is_initializer_list<std::initializer_list<T>>: public std::true_type {
    };

    template <typename T>
    constexpr static bool is_initializer_list_v = is_initializer_list<T>::value;

	template <typename T>
    using remove_cvref_t = std::remove_reference_t<std::remove_cv_t<T>>;

	template <typename T, bool Expr = is_initializer_list_v<remove_cvref_t<T>>>
    struct filter_initializer_list {
    	using value_type = std::vector<typename T::value_type>;
    };

	template <typename T>
	struct filter_initializer_list<T,false> {
		using value_type = T;
	};

	template <typename T>
	using filter_initializer_list_t = typename filter_initializer_list<T>::value_type;

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

        template <typename T>
        struct deref_type {
        	using type = decltype(*std::declval<T>());
        };

        template <typename T>
        using deref_t = typename deref_type<T>::type;

        template <int... M>
        auto dereference(IS<M...>){
        	using ret_types = std::tuple<deref_t<decltype((std::get<M>(m_its)))>...>;
        	using ret_type = std::tuple<choose_storage_t<std::tuple_element_t<M,ret_types>>...>;
        	return ret_type{*std::get<M>(m_its)...};
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

        //using iterator = zip_iterator<typename std::remove_reference_t<Types>::iterator...>;
        using iterator = zip_iterator<decltype(std::begin(std::declval<Types>()))...>;

        template <int... M>
            iterator _begin(IS<M...>) const { return iterator(std::begin(std::get<M>(m_containers)) ...); }

        template <int... M>
            iterator _end(IS<M...>) const { return iterator(std::end(std::get<M>(m_containers)) ...); }

        iterator end() { return _end(_is()); }
        iterator begin() const { return _begin(_is());}
        iterator end() const { return _end(_is());}

        zip_container(Types&&... t): m_containers{std::forward<Types>(t)...} {}

    private:
        std::tuple<choose_storage_t<Types>...> m_containers;
    };
}   // detail

template <typename... Types>
    auto zip(Types&&... t) {
        return detail::zip_container<Types...>(std::forward<Types>(t)...);
    }
}  
