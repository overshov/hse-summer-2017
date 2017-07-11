#include <initializer_list>
#include <memory>

template <typename T,
        class Allocator = std::allocator<T>
> class my_vector {
private:

public:
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    // Member functions

    my_vector();

    ~ my_vector();

    my_vector& operator=(my_vector&& other);

    my_vector& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T& value);

    void assign(std::initializer_list<T> ilist);

    allocator_type get_allocator() const;

    // Element access

    reference at(size_type pos);

    const_reference at(size_type pos) const;

    reference operator[](size_type pos);

    const_reference operator[](size_type pos) const;

    reference front();

    const_reference front() const;

    reference back();

    const_reference back() const;

    T* data() noexcept;

    const T* data() const noexcept;

    // Iterators

    class iterator : public  std::iterator<std::random_access_iterator_tag, T> {
    };

    class const_iterator : public  std::iterator<std::random_access_iterator_tag, T> {
    };

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    iterator rbegin() noexcept;

    const_iterator rbegin() const noexcept;

    const_iterator crbegin() const noexcept;

    iterator rend() noexcept;

    const_iterator rend() const noexcept;

    const_iterator crend() const noexcept;

    // Capacity

    bool empty() const noexcept;

    size_type size() const noexcept;

    size_type max_size() const noexcept;

    void reserve(size_type new_cap);

    size_type capacity() const noexcept;

    void shrink_to_fit();

    // Modifiers

    void clear() noexcept;

    iterator insert(const_iterator pos, const T& value);

    iterator insert(const_iterator pos, T&& value);

    iterator insert(const_iterator pos, size_type count, const T& value);

    template <class InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);

    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    iterator erase(const_iterator pos);

    iterator erase(const_iterator first, const_iterator last);

    void push_back(T&& value);

    template <class... Args>
    reference emplace_back(Args&&... args);

    void pop_back();

    void resize(size_type count);

    void resize(size_type count, const value_type& value);

    void swap(my_vector& other) noexcept;

};

// Non-member functions

template<class T, class Alloc>
bool operator==(const my_vector<T, Alloc>& lhs,
                const my_vector<T, Alloc>& rhs);

template<class T, class Alloc>
bool operator!=(const my_vector<T, Alloc>& lhs,
                const my_vector<T, Alloc>& rhs);

template<class T, class Alloc>
bool operator<(const my_vector<T, Alloc>& lhs,
               const my_vector<T, Alloc>& rhs);

template<class T, class Alloc>
bool operator<=(const my_vector<T, Alloc>& lhs,
                const my_vector<T, Alloc>& rhs);

template<class T, class Alloc>
bool operator>(const my_vector<T, Alloc>& lhs,
               const my_vector<T, Alloc>& rhs);

template<class T, class Alloc>
bool operator>=(const my_vector<T, Alloc>& lhs,
                const my_vector<T, Alloc>& rhs);

template<class T, class Alloc>
void swap(my_vector<T,Alloc>& lhs,
          my_vector<T,Alloc>& rhs);