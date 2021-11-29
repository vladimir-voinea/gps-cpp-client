#pragma once

#include <cstdint>

template<typename object_t>
class vector_container
{
public:
    using value_type = object_t;
    using index_type = std::size_t;
    using iterator = typename std::vector<object_t>::iterator;
    using const_iterator = typename std::vector<object_t>::const_iterator;

public:
    vector_container()
    {
        using T = vector_container<int>;
        static_assert(json_dto::details::meta::has_value_type<T>::value, "Must have value type");
        static_assert(!json_dto::details::meta::has_key_type<T>::value, "Must not have key type");
        static_assert(json_dto::details::meta::has_iterator_type<T>::value, "Must have iterator type");
        static_assert(json_dto::details::meta::has_const_iterator_type<T>::value, "Must have const iterator type");
        static_assert(json_dto::details::meta::has_begin<T>::value, "Must have begin");
        static_assert(json_dto::details::meta::has_end<T>::value, "Must have end");
        static_assert(json_dto::details::meta::has_emplace_back<T>::value 
            || (json_dto::details::meta::has_before_begin<T>::value && json_dto::details::meta::has_emplace_after<T>::value), "Must have emplace methods");

        static_assert(json_dto::details::meta::is_stl_like_sequence_container<T>::value, "Must be STL sequence like container");
    }

    vector_container(std::initializer_list<object_t> init)
        : m_vec(init)
        {}

    vector_container(std::vector<object_t> init)
        : m_vec(std::move(init))
        {}

    iterator begin()
    {
        return m_vec.begin();
    }

    iterator end()
    {
        return m_vec.end();
    }

    const_iterator begin() const
    {
        return m_vec.begin();
    }

    const_iterator end() const
    {
        return m_vec.end();
    }

    object_t& operator[](index_type index)
    {
        return m_vec[index];
    }

    const object_t& operator[](index_type index) const
    {
        return m_vec[index];
    }

    iterator find(index_type index)
    {
        return m_vec.begin() + index;
    }

    const_iterator find(index_type index) const
    {
        return m_vec.cbegin() + index;
    }

    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        return m_vec.emplace(pos, std::forward<Args>(args)...);
    }

    template<typename... Args>
    value_type& emplace_back(Args&&... args)
    {
        return m_vec.emplace_back(std::forward<Args>(args)...);
    }

    template<typename... Args>
    value_type& add(Args&&... args)
    {
        return emplace_back(std::forward<Args>(args)...);
    }

    iterator erase(iterator it)
    {
        return m_vec.erase(it);
    }

    const_iterator erase(const_iterator it)
    {
        return m_vec.erase(it);
    }

    iterator erase(iterator first, iterator last)
    {
        return m_vec.erase(first, last);
    }

    const_iterator erase(const_iterator first, const_iterator last)
    {
        return m_vec.erase(first, last);
    }

private:
    std::vector<object_t> m_vec;
};

